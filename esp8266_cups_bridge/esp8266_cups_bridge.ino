/**
 * ============================================================================
 * ESP8266 CUPS & Network Print Bridge (ESP-01 8-Pin Firmware)
 * ============================================================================
 * Bridges standard CUPS network printing protocols (RAW AppSocket/JetDirect
 * on port 9100, LPD on port 515, mDNS/Bonjour discovery, and an embedded
 * Web Management & Print Portal on port 80) to an Arduino Uno/Nano parallel
 * printer driver over hardware Serial (115200 baud) with XON/XOFF flow control.
 *
 * Supports automatic Wi-Fi connection with non-volatile EEPROM storage and
 * hardware router UID (BSSID) network scanning & locking.
 * ============================================================================
 */

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "config.h"
#include "wifi_manager.h"
#include "cups_raw_server.h"
#include "web_portal.h"

void setup() {
    // 1. Initialize Hardware UART (GPIO1 TX, GPIO3 RX)
    Serial.begin(SERIAL_BAUD_RATE);
    delay(100);

    // 2. Initialize EEPROM and connect to Wi-Fi (loads saved credentials or AP fallback)
    wifi_manager_init();
    wifi_manager_connect();

    // 3. Initialize mDNS / Bonjour Advertising for automatic CUPS & macOS discovery
    if (MDNS.begin(MDNS_HOSTNAME)) {
        // Port 9100 RAW JetDirect / AppSocket (primary CUPS / macOS backend)
        MDNS.addService("pdl-datastream", "tcp", RAW_JETDIRECT_PORT);
        MDNS.addServiceTxt("pdl-datastream", "tcp", "ty", PRINTER_MODEL_NAME);
        MDNS.addServiceTxt("pdl-datastream", "tcp", "product", "(Amstrad DMP3000)");
        MDNS.addServiceTxt("pdl-datastream", "tcp", "pdl", "application/octet-stream,text/plain");

        // Port 515 Line Printer Daemon (LPD)
        MDNS.addService("printer", "tcp", LPD_PORT);

        // Port 80 Web Portal
        MDNS.addService("http", "tcp", HTTP_PORT);
    }

    // 4. Initialize TCP print servers and Web Portal
    raw_server_init();
    web_portal_init();

    // 5. Query initial status from Arduino
    raw_server_send_command("!STATUS");
}

void loop() {
    // Service mDNS
    MDNS.update();

    // Service RAW TCP & LPD streaming pipeline with XON/XOFF flow control
    raw_server_loop();

    // Service Web Portal requests
    web_portal_loop();
}
