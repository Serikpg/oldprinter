#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ============================================================================
// Arduino Uno / Nano Pin Mapping to DB25 / Centronics 36-Pin Parallel Port
// ============================================================================

// 8-Bit Parallel Data Lines:
// Pin 2 -> DB25 Pin 2 (Centronics Pin 2) -> DATA 0 (LSB)
// Pin 3 -> DB25 Pin 3 (Centronics Pin 3) -> DATA 1
// Pin 4 -> DB25 Pin 4 (Centronics Pin 4) -> DATA 2
// Pin 5 -> DB25 Pin 5 (Centronics Pin 5) -> DATA 3
// Pin 6 -> DB25 Pin 6 (Centronics Pin 6) -> DATA 4
// Pin 7 -> DB25 Pin 7 (Centronics Pin 7) -> DATA 5
// Pin 8 -> DB25 Pin 8 (Centronics Pin 8) -> DATA 6
// Pin 9 -> DB25 Pin 9 (Centronics Pin 9) -> DATA 7 (MSB)
const uint8_t PIN_DATA[8] = { 2, 3, 4, 5, 6, 7, 8, 9 };

// Parallel Handshake & Control Lines:
const uint8_t PIN_STROBE = 10;   // Output: /STROBE (DB25 Pin 1, Centronics Pin 1) - Active LOW
const uint8_t PIN_BUSY   = 11;   // Input:  BUSY    (DB25 Pin 11, Centronics Pin 11) - Active HIGH
const uint8_t PIN_ACK    = 12;   // Input:  /ACK    (DB25 Pin 10, Centronics Pin 10) - Active LOW
const uint8_t PIN_LED    = 13;   // Output: Onboard LED (Status & Activity indicator)

const uint8_t PIN_INIT   = A0;   // Output: /INIT   (DB25 Pin 16, Centronics Pin 31) - Active LOW (Reset)
const uint8_t PIN_FAULT  = A1;   // Input:  /FAULT  (DB25 Pin 15, Centronics Pin 32) - Active LOW (Error/Offline)
const uint8_t PIN_PE     = A2;   // Input:  PE      (DB25 Pin 12, Centronics Pin 12) - Active HIGH (Paper End)
const uint8_t PIN_SELECT = A3;   // Input:  SELECT  (DB25 Pin 13, Centronics Pin 13) - Active HIGH (Online)

// ============================================================================
// Communication Settings
// ============================================================================
// Serial link between Arduino and ESP8266
// Hardware Serial pins: Pin 0 (RX), Pin 1 (TX)
#define SERIAL_BAUD_RATE 115200

// Software Flow Control (XON/XOFF)
#define FLOW_XON  0x11   // DC1: Resume transmission
#define FLOW_XOFF 0x13   // DC3: Pause transmission

// Serial Incoming Ring Buffer Size (in bytes)
// ATmega328P has 2048 bytes of SRAM; 512 bytes gives ample buffering while leaving ~1.2KB for stack/globals.
#define RING_BUFFER_SIZE 512
#define BUFFER_HIGH_WATER (RING_BUFFER_SIZE * 3 / 4) // 384 bytes -> send XOFF
#define BUFFER_LOW_WATER  (RING_BUFFER_SIZE / 4)     // 128 bytes -> send XON

// Parallel Interface Timing Constants (Amstrad DMP3000 / Centronics specification)
#define STROBE_PULSE_US   2      // Min 0.5 us per DMP3000 spec (using 2 us for noise immunity)
#define DATA_SETUP_US     1      // Min 0.5 us data setup time
#define BUSY_TIMEOUT_MS   10000  // 10 second timeout waiting for printer ready before warning
#define INIT_PULSE_US     200    // Min 100 us /INPUT PRIME pulse to reset printer

// Text Processing Flags
#define AUTO_CR_ON_LF          true  // When true, translates single \n to \r\n to prevent staircase effect
#define ENABLE_CP437_TRANSCODE true  // When true, converts multi-byte UTF-8 into IBM CP437 character codes
#define ENABLE_STATUS_REPLY    true  // When true, responds to ESP query commands (!STATUS)

#endif // CONFIG_H
