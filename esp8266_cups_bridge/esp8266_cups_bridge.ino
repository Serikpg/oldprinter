/**
 * ============================================================================
 * ESP8266 CUPS & Network Print Bridge (ESP-01 8-Pin Firmware)
 * ============================================================================
 * Bridges standard CUPS network printing protocols (RAW AppSocket/JetDirect
 * on port 9100, LPD on port 515, mDNS/Bonjour discovery, and an embedded
 * Web Management & Print Portal on port 80) to an Arduino Uno/Nano parallel
 * printer driver over hardware Serial (115200 baud) with XON/XOFF flow control.
 * ============================================================================
 */

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "config.h"
#include "cups_raw_server.h"
#include "web_portal.h"

void setup() {
    // 1. Initialize Hardware UART (GPIO1 TX, GPIO3 RX)
    Serial.begin(SERIAL_BAUD_RATE);
    delay(100);

    // 2. Connect to Wi-Fi
    WiFi.mode(WIFI_STA);
    bool connected = false;

    if (String(DEFAULT_WIFI_SSID) != "YOUR_WIFI_SSID" && String(DEFAULT_WIFI_SSID).length() > 0) {
        WiFi.begin(DEFAULT_WIFI_SSID, DEFAULT_WIFI_PASSWORD);

        unsigned long startAttempt = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 15000) {
            delay(250);
        }
        if (WiFi.status() == WL_CONNECTED) {
            connected = true;
        }
    }

    // 3. Fallback to Access Point mode if Wi-Fi connection fails
    if (!connected) {
        WiFi.mode(WIFI_AP_STA);
        WiFi.softAP(AP_FALLBACK_SSID, AP_FALLBACK_PASS);
    }

    // 4. Initialize mDNS / Bonjour Advertising for automatic CUPS & macOS discovery
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

    // 5. Initialize TCP print servers and Web Portal
    raw_server_init();
    web_portal_init();

    // 6. Query initial status from Arduino
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
