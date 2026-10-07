/**
 * ============================================================================
 * ESP8266 CUPS & Network Print Bridge (ESP-01 8-Pin Firmware)
 * ============================================================================
 * Standalone direct Wi-Fi print server (NO ROUTER REQUIRED).
 *
 * Broadcasts an independent Wi-Fi network named with the ESP's unique hardware
 * Chip UID (e.g. "Amstrad-DMP3000-A1B2C3") allowing direct connection from
 * Mac, Linux, or Android devices without needing any home router.
 *
 * Bridges standard CUPS network printing protocols (RAW AppSocket/JetDirect
 * on port 9100, LPD on port 515, mDNS/Bonjour discovery, and an embedded
 * Web Management & Print Portal on port 80) to an Arduino Uno/Nano parallel
 * printer driver over hardware Serial (115200 baud) with XON/XOFF flow control.
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

    // 2. Start Standalone Direct Wi-Fi Access Point (SSID contains unique Chip UID)
    wifi_manager_init();

    // 3. Initialize mDNS / Bonjour Advertising on 192.168.4.1 for automatic CUPS & macOS discovery
    if (MDNS.begin(MDNS_HOSTNAME, wifi_manager_get_ip_addr())) {
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
    // Service Captive DNS
    wifi_manager_loop();

    // Service mDNS
    MDNS.update();

    // Service RAW TCP & LPD streaming pipeline with XON/XOFF flow control
    raw_server_loop();

    // Service Web Portal requests
    web_portal_loop();
}
