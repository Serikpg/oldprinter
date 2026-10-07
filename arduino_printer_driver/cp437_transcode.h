#ifndef CP437_TRANSCODE_H
#define CP437_TRANSCODE_H

#include <Arduino.h>

// Stream-based UTF-8 to IBM CP437 transcoder.
// Returns:
//   > 0: A decoded CP437 byte ready to be sent to the printer.
//   0:   Intermediate byte absorbed (more UTF-8 bytes required).
//   -1:  Invalid byte or unmapped symbol (fallback already handled or dropped).
int16_t cp437_process_byte(uint8_t incoming);

// Resets transcoder state (call on new print job or timeout)
void cp437_reset();

#endif // CP437_TRANSCODE_H
