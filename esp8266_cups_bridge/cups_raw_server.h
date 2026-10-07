#ifndef CUPS_RAW_SERVER_H
#define CUPS_RAW_SERVER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

void raw_server_init();
void raw_server_loop();

// Enqueues data to be sent to the Arduino/printer
bool raw_server_push_byte(uint8_t b);
bool raw_server_push_buffer(const uint8_t *data, size_t len);

// Status queries for Web UI and diagnostics
String   raw_server_get_printer_status();
bool     raw_server_is_paper_out();
bool     raw_server_is_busy();
bool     raw_server_is_fault();
uint32_t raw_server_get_bytes_printed();
uint16_t raw_server_get_buffer_fill();
bool     raw_server_is_flow_allowed();

// Sends a control command to Arduino (e.g. "!STATUS", "!RESET", "!EJECT")
void raw_server_send_command(const String &cmd);

#endif // CUPS_RAW_SERVER_H
