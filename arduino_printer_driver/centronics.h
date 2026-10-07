#ifndef CENTRONICS_H
#define CENTRONICS_H

#include <Arduino.h>
#include "config.h"

struct PrinterStatus {
    bool busy;
    bool paperOut;
    bool fault;
    bool selected;
    bool ready;
};

// Initializes GPIO pins and triggers hardware reset pulse
void centronics_init();

// Hardware reset via /INIT (INPUT PRIME) pin
void centronics_reset_printer();

// Reads individual hardware status lines
bool centronics_is_busy();
bool centronics_is_paper_out();
bool centronics_is_fault();
bool centronics_is_selected();
bool centronics_is_ready();

// Returns full status structure
PrinterStatus centronics_get_status();

// Writes a single byte to the 8-bit parallel bus with full Centronics handshake
// Returns true on success, false on timeout or error
bool centronics_write_byte(uint8_t data);

// Sends a raw string or byte buffer
bool centronics_write_buffer(const uint8_t *buffer, size_t length);

#endif // CENTRONICS_H
