/**
 * ============================================================================
 * Arduino Parallel Printer Driver for Amstrad DMP3000 / IBM Proprinter
 * ============================================================================
 * Interfaces an Arduino Uno or Nano (5V ATmega328P) to a 1980s dot-matrix
 * printer via DB25 / Centronics 36-pin parallel port and bridges to ESP8266
 * over Serial with full XON/XOFF backpressure and UTF-8 -> CP437 transcoding.
 * ============================================================================
 */

#include <Arduino.h>
#include "config.h"
#include "centronics.h"
#include "cp437_transcode.h"

// Ring buffer for incoming serial stream
static uint8_t  rx_buffer[RING_BUFFER_SIZE];
static uint16_t rx_head = 0;
static uint16_t rx_tail = 0;
static bool     flow_paused = false;

// Tracking state for auto-CR insertion and command parsing
static uint8_t  last_char_sent = 0;
static String   command_buffer = "";
static bool     in_command_mode = false;

// Status change monitoring
static bool     last_pe_state = false;
static bool     last_fault_state = false;
static unsigned long last_status_check = 0;

static inline uint16_t buffer_count() {
    return (rx_head - rx_tail + RING_BUFFER_SIZE) % RING_BUFFER_SIZE;
}

static inline bool buffer_is_full() {
    return ((rx_head + 1) % RING_BUFFER_SIZE) == rx_tail;
}

static void send_status_report() {
    PrinterStatus s = centronics_get_status();
    Serial.print(F("STATUS: "));
    if (s.paperOut) {
        Serial.print(F("PAPER_OUT"));
    } else if (s.fault) {
        Serial.print(F("FAULT"));
    } else if (s.busy) {
        Serial.print(F("BUSY"));
    } else {
        Serial.print(F("READY"));
    }
    Serial.print(F(" [PE="));
    Serial.print(s.paperOut ? F("1") : F("0"));
    Serial.print(F(" FAULT="));
    Serial.print(s.fault ? F("1") : F("0"));
    Serial.print(F(" BUSY="));
    Serial.print(s.busy ? F("1") : F("0"));
    Serial.print(F(" BUF="));
    Serial.print(buffer_count());
    Serial.println(F("]"));
}

void setup() {
    // 1. Initialize hardware serial to ESP8266
    Serial.begin(SERIAL_BAUD_RATE);
    while (!Serial) { ; }

    // 2. Initialize Centronics parallel interface pins and pulse printer reset
    centronics_init();
    cp437_reset();

    // 3. Clear buffers
    rx_head = 0;
    rx_tail = 0;
    flow_paused = false;

    // 4. Send initial XON to ensure ESP starts streaming immediately
    Serial.write(FLOW_XON);

    // 5. Initial status report
    send_status_report();

    // 6. Flush any UART startup noise/framing fragments from serial buffer
    // Wait 1500ms so ESP8266 finishes its 74880-baud ROM bootloader message before settling
    delay(1500);
    while (Serial.available()) {
        Serial.read();
    }
}

static bool is_esp_diagnostic(const String &s) {
    String lower = s;
    lower.toLowerCase();
    if (lower.indexOf("[esp]") >= 0) return true;
    if (lower.indexOf("init:") >= 0) return true;
    if (lower.indexOf("status:") >= 0) return true;
    if (lower.indexOf("[filtered") >= 0) return true;
    if (lower.indexOf("[print") >= 0) return true;
    if (lower.indexOf("ok:") >= 0) return true;
    if (lower.indexOf("print portal") >= 0) return true;
    if (lower.indexOf("web ui") >= 0) return true;
    if (lower.indexOf("portal") >= 0) return true;
    if (lower.indexOf("ready") >= 0) return true;
    if (lower.indexOf("server ready") >= 0) return true;
    if (lower.indexOf("mdns") >= 0 || lower.indexOf("responder") >= 0) return true;
    if (lower.indexOf("jetdirect") >= 0 || lower.indexOf("appsocket") >= 0) return true;
    if (lower.indexOf("controller ready") >= 0) return true;
    if (lower.indexOf("parallel controller") >= 0) return true;
    if (lower.indexOf("heartbeat") >= 0) return true;
    if (lower.indexOf("free heap") >= 0) return true;
    if (lower.indexOf("wi-fi clients") >= 0 || lower.indexOf("wifi") >= 0) return true;
    if (lower.indexOf("softap") >= 0) return true;
    if (lower.indexOf("chip uid") >= 0) return true;
    if (lower.indexOf("target ssid") >= 0) return true;
    if (lower.indexOf("station:") >= 0) return true;
    if (lower.indexOf("aid ") >= 0) return true;
    if (lower.indexOf("add 1") >= 0 || lower.indexOf("rm 1") >= 0) return true;
    if (lower.startsWith("add ") || lower.startsWith("rm ")) return true;
    if (lower.indexOf("bcn ") >= 0) return true;
    if (lower.indexOf("scandone") >= 0) return true;
    if (lower.indexOf("mode :") >= 0) return true;
    if (lower.indexOf("add if") >= 0 || lower.indexOf("del if") >= 0) return true;
    if (lower.indexOf("dhcp server") >= 0) return true;
    if (lower.indexOf("fpm close") >= 0) return true;
    if (lower.indexOf("port 9100") >= 0 || lower.indexOf("port 515") >= 0 || lower.indexOf("port 80") >= 0) return true;
    if (lower.indexOf("requesting printer status") >= 0) return true;
    if (lower.indexOf("access point") >= 0 || lower.indexOf("firmware") >= 0 || lower.indexOf("bridge") >= 0) return true;
    if (lower.indexOf("rf scan") >= 0 || lower.indexOf("networks:") >= 0 || lower.indexOf("captive dns") >= 0) return true;
    if (lower.indexOf("dbm") >= 0 || lower.indexOf("ets jan") >= 0 || lower.indexOf("rst cause") >= 0 || lower.indexOf("boot mode") >= 0) return true;
    if (lower.indexOf("load 0x") >= 0 || lower.indexOf("chksum") >= 0 || lower.indexOf("csum") >= 0 || lower.indexOf("~ld") >= 0) return true;
    if (lower.indexOf("tail ") >= 0 || lower.startsWith("tail") || lower.indexOf("v000") >= 0 || lower.indexOf("@cp:") >= 0) return true;
    if (lower == "ld" || lower.endsWith("ld") || lower.indexOf("room ") >= 0 || lower.indexOf("len ") >= 0 || lower.indexOf("ets") >= 0) return true;
    if (lower.indexOf("wdt reset") >= 0 || lower.indexOf("fatal exception") >= 0) return true;
    return false;
}

void loop() {
    // ------------------------------------------------------------------------
    // 1. Read incoming bytes from ESP8266 Serial with strict diagnostic filter
    // ------------------------------------------------------------------------
    static String line_buffer = "";

    while (Serial.available()) {
        uint8_t c = Serial.read();

        // Drop NUL (0x00) bytes immediately (framing errors and line noise produce NUL)
        if (c == 0) {
            continue;
        }

        // Control characters / Escape sequences (except \r, \n, \t) bypass line buffering
        if (c < 32 && c != '\r' && c != '\n' && c != '\t') {
            if (line_buffer.length() > 0) {
                if (is_esp_diagnostic(line_buffer)) {
                    line_buffer = "";
                    continue; // Drop the diagnostic line AND the trailing control char!
                }
                for (unsigned int i = 0; i < line_buffer.length(); i++) {
                    if (!buffer_is_full()) {
                        rx_buffer[rx_head] = line_buffer[i];
                        rx_head = (rx_head + 1) % RING_BUFFER_SIZE;
                    }
                }
                line_buffer = "";
            }
            if (!buffer_is_full()) {
                rx_buffer[rx_head] = c;
                rx_head = (rx_head + 1) % RING_BUFFER_SIZE;
            }
            continue;
        }

        // Handle line endings
        if (c == '\n' || c == '\r') {
            if (line_buffer.length() > 0) {
                String cmd = line_buffer;
                cmd.trim();
                if (cmd.length() > 0) {
                    if (is_esp_diagnostic(cmd)) {
                        // Diagnostic message from ESP: drop silently to avoid polluting serial link
                    } else if (cmd == "!STATUS") {
                        send_status_report();
                    } else if (cmd == "!RESET") {
                        centronics_reset_printer();
                        cp437_reset();
                        Serial.println(F("OK: Printer Reset"));
                    } else if (cmd == "!CLEAR" || cmd == "!FLUSH" || cmd == "!CANCEL") {
                        rx_head = 0;
                        rx_tail = 0;
                        flow_paused = false;
                        Serial.write(FLOW_XON);
                        // Send CAN (0x18) to DMP3000 to erase any partially buffered line in the printer
                        centronics_write_byte(0x18);
                        Serial.println(F("OK: Buffer Cleared"));
                    } else if (cmd == "!EJECT" || cmd == "!FF") {
                        centronics_write_byte(0x0C);
                        Serial.println(F("OK: Form Feed Dispatched"));
                    } else {
                        // Valid print text: strip trailing whitespace only (preserves all leading indentation)
                        int end_idx = line_buffer.length();
                        while (end_idx > 0 && line_buffer[end_idx - 1] == ' ') {
                            end_idx--;
                        }
                        for (int i = 0; i < end_idx; i++) {
                            if (!buffer_is_full()) {
                                rx_buffer[rx_head] = line_buffer[i];
                                rx_head = (rx_head + 1) % RING_BUFFER_SIZE;
                            }
                        }
                        if (!buffer_is_full()) {
                            rx_buffer[rx_head] = '\n';
                            rx_head = (rx_head + 1) % RING_BUFFER_SIZE;
                        }
                    }
                }
                line_buffer = "";
            }
            continue;
        }

        // Printable text character
        line_buffer += (char)c;
        if (line_buffer.length() > 96) {
            if (is_esp_diagnostic(line_buffer)) {
                line_buffer = "";
            } else {
                for (unsigned int i = 0; i < line_buffer.length(); i++) {
                    if (!buffer_is_full()) {
                        rx_buffer[rx_head] = line_buffer[i];
                        rx_head = (rx_head + 1) % RING_BUFFER_SIZE;
                    }
                }
                line_buffer = "";
            }
        }

        // Software flow control: send XOFF when reaching high-water mark
        if (!flow_paused && (buffer_count() >= BUFFER_HIGH_WATER)) {
            Serial.write(FLOW_XOFF);
            flow_paused = true;
        }
    }

    // ------------------------------------------------------------------------
    // 2. Dispatch buffered bytes to parallel printer port & live console echo
    // ------------------------------------------------------------------------
    static char echo_buf[80];
    static uint8_t echo_len = 0;

    if (rx_head != rx_tail) {
        // If printer has an error or is out of paper, wait and indicate on LED
        bool pe = centronics_is_paper_out();
        bool fault = centronics_is_fault();

        if (pe || fault) {
            // Rapid blink for paper out / fault
            digitalWrite(PIN_LED, (millis() / 200) % 2);
        } else if (!centronics_is_busy()) {
            // Printer is ready to receive next byte!
            digitalWrite(PIN_LED, HIGH);

            uint8_t b = rx_buffer[rx_tail];
            rx_tail = (rx_tail + 1) % RING_BUFFER_SIZE;

            // Optional UTF-8 to IBM CP437 transcoding
            int16_t to_print = b;
#if ENABLE_CP437_TRANSCODE
            to_print = cp437_process_byte(b);
#endif

            if (to_print > 0) {
                uint8_t char_to_send = (uint8_t)to_print;

                static uint8_t prev_raw_char = 0;

#if AUTOFD_PIN14_GROUNDED
                // DB25 Pin 14 (/AUTOFD) is tied to GND: printer hardware automatically performs LF on every CR (\r).
                // Sending \n after \r causes double line spacing. Convert lone \n to \r, and drop \n if preceded by \r.
                if (char_to_send == '\n') {
                    if (prev_raw_char != '\r') {
                        centronics_write_byte('\r');
                    }
                    // \n itself is dropped since \r triggered CR + LF in hardware
                } else {
                    centronics_write_byte(char_to_send);
                }
                prev_raw_char = char_to_send;
#else
                // Handle auto carriage-return before line feed to prevent staircase effect
#if AUTO_CR_ON_LF
                if (char_to_send == '\n' && last_char_sent != '\r') {
                    centronics_write_byte('\r');
                }
#endif
                centronics_write_byte(char_to_send);
                last_char_sent = char_to_send;
#endif

                // Live print monitor echo to PC USB serial console
                if (char_to_send == '\n' || char_to_send == '\r') {
                    if (echo_len > 0) {
                        echo_buf[echo_len] = '\0';
                        Serial.print(F("[PRINT DATA] "));
                        Serial.println(echo_buf);
                        echo_len = 0;
                    }
                } else if (char_to_send >= 32 && char_to_send <= 126) {
                    if (echo_len < sizeof(echo_buf) - 1) {
                        echo_buf[echo_len++] = (char)char_to_send;
                    } else {
                        echo_buf[echo_len] = '\0';
                        Serial.print(F("[PRINT DATA] "));
                        Serial.println(echo_buf);
                        echo_len = 0;
                        echo_buf[echo_len++] = (char)char_to_send;
                    }
                } else if (char_to_send == 0x0C) {
                    Serial.println(F("[PRINT DATA] <FORM FEED (Page Eject)>"));
                }
            }

            digitalWrite(PIN_LED, LOW);
        }
    } else {
        // Buffer is empty - only flush echo buffer if idle for > 150 ms
        static unsigned long last_rx_empty = 0;
        if (echo_len > 0) {
            if (last_rx_empty == 0) last_rx_empty = millis();
            if (millis() - last_rx_empty > 150) {
                echo_buf[echo_len] = '\0';
                Serial.print(F("[PRINT DATA] "));
                Serial.println(echo_buf);
                echo_len = 0;
                last_rx_empty = 0;
            }
        } else {
            last_rx_empty = 0;
        }

        if (!centronics_is_paper_out() && !centronics_is_fault()) {
            digitalWrite(PIN_LED, LOW);
        }
    }

    // ------------------------------------------------------------------------
    // 3. Flow control recovery: send XON when buffer drops below low-water mark
    // ------------------------------------------------------------------------
    if (flow_paused && (buffer_count() <= BUFFER_LOW_WATER)) {
        Serial.write(FLOW_XON);
        flow_paused = false;
    }

    // Periodic XON assertion when buffer is empty to ensure flow remains unblocked
    static unsigned long last_idle_xon = 0;
    if (!flow_paused && buffer_count() == 0 && (millis() - last_idle_xon > 2000)) {
        last_idle_xon = millis();
        Serial.write(FLOW_XON);
    }

    // ------------------------------------------------------------------------
    // 4. Periodic status & change notification
    // ------------------------------------------------------------------------
    unsigned long now = millis();
    if (now - last_status_check >= 1000) {
        last_status_check = now;
        bool pe = centronics_is_paper_out();
        bool fault = centronics_is_fault();

        if (pe != last_pe_state || fault != last_fault_state) {
            last_pe_state = pe;
            last_fault_state = fault;
            send_status_report();
        }
    }
}
