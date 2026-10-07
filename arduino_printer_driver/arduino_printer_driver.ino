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

    // 4. Initial announcement
    Serial.println();
    Serial.println(F("INIT: Amstrad DMP3000 Parallel Controller Ready"));
    send_status_report();
}

void loop() {
    // ------------------------------------------------------------------------
    // 1. Read incoming bytes from ESP8266 Serial into ring buffer
    // ------------------------------------------------------------------------
    while (Serial.available()) {
        uint8_t c = Serial.read();

        // Support command protocol prefixed with '!' (e.g. !STATUS, !RESET, !EJECT)
        if (c == '!' && buffer_count() == 0 && !in_command_mode) {
            in_command_mode = true;
            command_buffer = "!";
            continue;
        }

        if (in_command_mode) {
            if (c == '\n' || c == '\r') {
                in_command_mode = false;
                command_buffer.trim();
                if (command_buffer == "!STATUS") {
                    send_status_report();
                } else if (command_buffer == "!RESET") {
                    centronics_reset_printer();
                    cp437_reset();
                    Serial.println(F("OK: Printer Reset"));
                } else if (command_buffer == "!EJECT" || command_buffer == "!FF") {
                    centronics_write_byte(0x0C); // Form feed
                    Serial.println(F("OK: Form Feed Dispatched"));
                } else {
                    Serial.println(F("ERR: Unknown Command"));
                }
                command_buffer = "";
            } else {
                command_buffer += (char)c;
                if (command_buffer.length() > 32) {
                    in_command_mode = false;
                    command_buffer = "";
                }
            }
            continue;
        }

        // Put byte into buffer
        if (!buffer_is_full()) {
            rx_buffer[rx_head] = c;
            rx_head = (rx_head + 1) % RING_BUFFER_SIZE;
        }

        // Software flow control: send XOFF when reaching high-water mark
        if (!flow_paused && (buffer_count() >= BUFFER_HIGH_WATER)) {
            Serial.write(FLOW_XOFF);
            flow_paused = true;
        }
    }

    // ------------------------------------------------------------------------
    // 2. Dispatch buffered bytes to parallel printer port
    // ------------------------------------------------------------------------
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

                // Handle auto carriage-return before line feed to prevent staircase effect
#if AUTO_CR_ON_LF
                if (char_to_send == '\n' && last_char_sent != '\r') {
                    centronics_write_byte('\r');
                }
#endif
                centronics_write_byte(char_to_send);
                last_char_sent = char_to_send;
            }

            digitalWrite(PIN_LED, LOW);
        }
    } else {
        // Buffer is empty - turn off LED
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
