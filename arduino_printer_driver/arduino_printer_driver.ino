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

    // 5. Initial announcement
    Serial.println();
    Serial.println(F("INIT: Amstrad DMP3000 Parallel Controller Ready"));
    send_status_report();

    // 6. Flush any UART startup noise/framing fragments from serial buffer
    delay(100);
    while (Serial.available()) {
        Serial.read();
    }
}

static bool is_esp_diagnostic(const String &s) {
    if (s.indexOf("[ESP]") >= 0) return true;
    if (s.indexOf("INIT:") >= 0) return true;
    if (s.indexOf("STATUS:") >= 0) return true;
    if (s.indexOf("[FILTERED") >= 0) return true;
    if (s.indexOf("[PRINT") >= 0) return true;
    if (s.indexOf("OK:") >= 0) return true;
    if (s.indexOf("Print Portal") >= 0 || s.indexOf("Print portal") >= 0 || s.indexOf("print portal") >= 0) return true;
    if (s.indexOf("Web UI") >= 0 || s.indexOf("web ui") >= 0 || s.indexOf("Portal") >= 0 || s.indexOf("portal") >= 0) return true;
    if (s.indexOf("ready") >= 0 || s.indexOf("READY") >= 0) return true;
    if (s.indexOf("server ready") >= 0) return true;
    if (s.indexOf("mDNS") >= 0 || s.indexOf("responder") >= 0) return true;
    if (s.indexOf("JetDirect") >= 0 || s.indexOf("AppSocket") >= 0) return true;
    if (s.indexOf("Controller Ready") >= 0) return true;
    if (s.indexOf("Parallel Controller") >= 0) return true;
    if (s.indexOf("Heartbeat") >= 0) return true;
    if (s.indexOf("Free heap") >= 0) return true;
    if (s.indexOf("Wi-Fi Clients") >= 0) return true;
    if (s.indexOf("SoftAP") >= 0) return true;
    if (s.indexOf("Chip UID") >= 0) return true;
    if (s.indexOf("Target SSID") >= 0) return true;
    if (s.indexOf("station:") >= 0) return true;
    if (s.indexOf("aid ") >= 0) return true;
    if (s.indexOf("add 1") >= 0) return true;
    if (s.indexOf("rm 1") >= 0) return true;
    if (s.startsWith("add ")) return true;
    if (s.startsWith("rm ")) return true;
    if (s.indexOf("bcn ") >= 0) return true;
    if (s.indexOf("scandone") >= 0) return true;
    if (s.indexOf("mode :") >= 0) return true;
    if (s.indexOf("add if") >= 0) return true;
    if (s.indexOf("del if") >= 0) return true;
    if (s.indexOf("dhcp server") >= 0) return true;
    if (s.indexOf("fpm close") >= 0) return true;
    if (s.indexOf("Port 9100") >= 0) return true;
    if (s.indexOf("Port 515") >= 0) return true;
    if (s.indexOf("Port 80") >= 0) return true;
    if (s.indexOf("Requesting printer status") >= 0) return true;
    if (s.indexOf("Access Point") >= 0 || s.indexOf("Firmware") >= 0 || s.indexOf("Bridge") >= 0) return true;
    if (s.indexOf("RF Scan") >= 0 || s.indexOf("networks:") >= 0 || s.indexOf("Captive DNS") >= 0) return true;
    if (s.indexOf("dBm") >= 0 || s.indexOf("ets Jan") >= 0 || s.indexOf("rst cause") >= 0 || s.indexOf("boot mode") >= 0) return true;
    return false;
}

void loop() {
    // ------------------------------------------------------------------------
    // 1. Read incoming bytes from ESP8266 Serial with strict diagnostic filter
    // ------------------------------------------------------------------------
    static String line_buffer = "";

    while (Serial.available()) {
        uint8_t c = Serial.read();

        // Control characters / Escape sequences (except \r, \n, \t) bypass line buffering
        if (c < 32 && c != '\r' && c != '\n' && c != '\t') {
            if (line_buffer.length() > 0) {
                if (!is_esp_diagnostic(line_buffer)) {
                    for (unsigned int i = 0; i < line_buffer.length(); i++) {
                        if (!buffer_is_full()) {
                            rx_buffer[rx_head] = line_buffer[i];
                            rx_head = (rx_head + 1) % RING_BUFFER_SIZE;
                        }
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
                        // Diagnostic message from ESP: forward to PC USB monitor, NEVER to printer!
                        // Drop loopback/status lines silently to prevent serial echo loops
                        if (!cmd.startsWith("INIT:") && 
                            !cmd.startsWith("STATUS:") && 
                            !cmd.startsWith("[FILTERED") && 
                            !cmd.startsWith("[PRINT") &&
                            !cmd.startsWith("OK:")) {
                            Serial.print(F("[FILTERED ESP LOG] "));
                            Serial.println(cmd);
                        }
                    } else if (cmd == "!STATUS") {
                        send_status_report();
                    } else if (cmd == "!RESET") {
                        centronics_reset_printer();
                        cp437_reset();
                        Serial.println(F("OK: Printer Reset"));
                    } else if (cmd == "!EJECT" || cmd == "!FF") {
                        centronics_write_byte(0x0C);
                        Serial.println(F("OK: Form Feed Dispatched"));
                    } else {
                        // Valid print text: push full line preserving all leading and trailing spaces!
                        for (unsigned int i = 0; i < line_buffer.length(); i++) {
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
