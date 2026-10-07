#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ============================================================================
// Direct Standalone Access Point Mode (NO ROUTER REQUIRED!)
// ============================================================================
// The ESP broadcasts its own independent Wi-Fi network directly.
// Your Mac, Linux PC, or Android phone connects directly to this Wi-Fi.
#define AP_SSID_PREFIX        "Amstrad-DMP3000-"  // Appends unique hardware Chip UID
#define AP_PASSWORD           ""                  // Empty string = Open Wi-Fi (no password needed)
                                                  // Or set to "12345678" if you want a password

// Direct Network Addressing
#define AP_STATIC_IP          192, 168, 4, 1      // Direct printer IP
#define AP_SUBNET_MASK        255, 255, 255, 0

// Hostname for mDNS (accessible as http://oldprinter.local or 192.168.4.1)
#define MDNS_HOSTNAME         "oldprinter"
#define PRINTER_MODEL_NAME    "Amstrad DMP3000 (IBM Mode)"

// ============================================================================
// Network Ports
// ============================================================================
#define RAW_JETDIRECT_PORT    9100  // Standard CUPS / macOS / AppSocket RAW port
#define LPD_PORT              515   // Line Printer Daemon port
#define HTTP_PORT             80    // Web UI & REST API port
#define DNS_PORT              53    // Captive DNS server port

// ============================================================================
// Serial Interface (ESP8266 <-> Arduino Uno / Nano)
// ============================================================================
// ESP-01 uses hardware UART: GPIO1 (TX) and GPIO3 (RX)
#define SERIAL_BAUD_RATE      115200

// Software Flow Control (XON/XOFF)
#define FLOW_XON              0x11  // DC1: Resume sending
#define FLOW_XOFF             0x13  // DC3: Pause sending

// Buffer configuration
#define STREAM_BUFFER_SIZE    4096  // In-memory FIFO queue for incoming print stream

#endif // CONFIG_H
