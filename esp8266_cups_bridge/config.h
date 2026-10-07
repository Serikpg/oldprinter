#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ============================================================================
// Wi-Fi Configuration
// ============================================================================
// Optional fallback hardcoded credentials (if EEPROM is empty)
#define DEFAULT_WIFI_SSID     ""
#define DEFAULT_WIFI_PASSWORD ""

// Access Point fallback if no network is saved or connection fails
#define AP_FALLBACK_SSID      "OldPrinter-Setup"
#define AP_FALLBACK_PASS      "12345678"

// Hostname for mDNS (accessible as http://oldprinter.local)
#define MDNS_HOSTNAME         "oldprinter"
#define PRINTER_MODEL_NAME    "Amstrad DMP3000 (IBM Mode)"

// ============================================================================
// EEPROM Persistent Wi-Fi Storage
// ============================================================================
#define EEPROM_CONFIG_SIZE    256
#define EEPROM_MAGIC          0x50524E31  // "PRN1"

struct SavedWiFiConfig {
    uint32_t magic;
    char     ssid[33];
    char     password[65];
    uint8_t  bssid[6];      // Hardware MAC / UID of target router/AP
    bool     lock_bssid;    // If true, connects strictly to this specific AP UID
};

// ============================================================================
// Network Ports
// ============================================================================
#define RAW_JETDIRECT_PORT    9100  // Standard CUPS / macOS / AppSocket RAW port
#define LPD_PORT              515   // Line Printer Daemon port
#define HTTP_PORT             80    // Web UI & REST API port

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
