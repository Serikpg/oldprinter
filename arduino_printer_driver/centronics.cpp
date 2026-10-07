#include "centronics.h"

void centronics_init() {
    // 1. Initialize data pins as OUTPUTs and default to LOW
    for (uint8_t i = 0; i < 8; i++) {
        pinMode(PIN_DATA[i], OUTPUT);
        digitalWrite(PIN_DATA[i], LOW);
    }

    // 2. Initialize control outputs
    pinMode(PIN_STROBE, OUTPUT);
    digitalWrite(PIN_STROBE, HIGH); // /STROBE is active LOW (idle HIGH)

    pinMode(PIN_INIT, OUTPUT);
    digitalWrite(PIN_INIT, HIGH);   // /INIT is active LOW (idle HIGH)

    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, LOW);

    // 3. Initialize status inputs
    pinMode(PIN_BUSY, INPUT);
    pinMode(PIN_ACK, INPUT_PULLUP);
    pinMode(PIN_FAULT, INPUT_PULLUP); // Active LOW on error/offline
    pinMode(PIN_PE, INPUT);           // Active HIGH when paper out
    pinMode(PIN_SELECT, INPUT);       // Active HIGH when online

    // 4. Perform initial hardware reset pulse on printer
    centronics_reset_printer();
}

void centronics_reset_printer() {
    // DMP3000 manual specifies minimum 100 µs active LOW pulse on /INPUT PRIME (Pin 31 / DB25 Pin 16)
    digitalWrite(PIN_INIT, LOW);
    delayMicroseconds(INIT_PULSE_US);
    digitalWrite(PIN_INIT, HIGH);
    
    // Give the printer 250 ms to finish internal microprocessor reset and home the printhead
    delay(250);
}

bool centronics_is_busy() {
    return (digitalRead(PIN_BUSY) == HIGH);
}

bool centronics_is_paper_out() {
    return (digitalRead(PIN_PE) == HIGH);
}

bool centronics_is_fault() {
    // /FAULT is active LOW on error condition
    return (digitalRead(PIN_FAULT) == LOW);
}

bool centronics_is_selected() {
    // SELECT is active HIGH when printer is online
    return (digitalRead(PIN_SELECT) == HIGH);
}

bool centronics_is_ready() {
    if (centronics_is_paper_out()) return false;
    if (centronics_is_fault())     return false;
    if (!centronics_is_selected()) return false;
    if (centronics_is_busy())      return false;
    return true;
}

PrinterStatus centronics_get_status() {
    PrinterStatus s;
    s.busy      = centronics_is_busy();
    s.paperOut  = centronics_is_paper_out();
    s.fault     = centronics_is_fault();
    s.selected  = centronics_is_selected();
    s.ready     = (!s.busy && !s.paperOut && !s.fault && s.selected);
    return s;
}

static inline void set_data_bus(uint8_t b) {
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
    // Direct port manipulation for ATmega328P (Uno/Nano):
    // Pins 2..7 correspond to PORTD bits 2..7 (leaving PD0/PD1 RX/TX intact)
    // Pins 8..9 correspond to PORTB bits 0..1 (leaving PB2..PB5 intact)
    PORTD = (PORTD & 0x03) | ((b & 0x3F) << 2);
    PORTB = (PORTB & 0xFC) | ((b >> 6) & 0x03);
#else
    // Generic fallback for any other architecture
    for (uint8_t i = 0; i < 8; i++) {
        digitalWrite(PIN_DATA[i], (b >> i) & 0x01);
    }
#endif
}

bool centronics_write_byte(uint8_t data) {
    // 1. Wait for BUSY to clear
    unsigned long startTime = millis();
    while (centronics_is_busy()) {
        if (millis() - startTime > BUSY_TIMEOUT_MS) {
            // Timed out waiting for printer to become ready
            return false;
        }
    }

    // 2. Check if printer is halted due to paper-out or fault
    if (centronics_is_paper_out() || centronics_is_fault()) {
        return false;
    }

    // 3. Put 8-bit data on the parallel bus
    set_data_bus(data);

    // 4. Data setup time (minimum 0.5 µs before /STROBE falling edge)
    delayMicroseconds(DATA_SETUP_US);

    // 5. Assert /STROBE LOW
    digitalWrite(PIN_STROBE, LOW);

    // 6. Hold /STROBE active LOW (minimum 0.5 µs per DMP3000 spec)
    delayMicroseconds(STROBE_PULSE_US);

    // 7. Deassert /STROBE HIGH
    digitalWrite(PIN_STROBE, HIGH);

    // 8. Data hold time
    delayMicroseconds(DATA_SETUP_US);

    // Small delay allowing printer logic to raise BUSY
    delayMicroseconds(2);

    return true;
}

bool centronics_write_buffer(const uint8_t *buffer, size_t length) {
    for (size_t i = 0; i < length; i++) {
        if (!centronics_write_byte(buffer[i])) {
            return false;
        }
    }
    return true;
}
