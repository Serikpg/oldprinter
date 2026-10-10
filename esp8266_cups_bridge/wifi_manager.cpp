#include "wifi_manager.h"

static DNSServer dnsServer;
static String full_ssid = "";
static String chip_uid  = "";
static IPAddress apIP(AP_STATIC_IP);
static IPAddress netMsk(AP_SUBNET_MASK);

void wifi_manager_init() {
    // 1. Generate unique 6-character uppercase hardware UID from chip ID
    uint32_t id = ESP.getChipId();
    char buf[12];
    snprintf(buf, sizeof(buf), "%06X", (unsigned int)(id & 0xFFFFFF));
    chip_uid = String(buf);

    // 2. Build network SSID including the unique hardware UID
    full_ssid = String(AP_SSID_PREFIX) + chip_uid;

    Serial.println(F("[ESP] Starting Wi-Fi Access Point initialization..."));
    Serial.print(F("[ESP] Chip UID: "));
    Serial.println(chip_uid);
    Serial.print(F("[ESP] Target SSID: "));
    Serial.println(full_ssid);

    Serial.println(F("[ESP] Testing 2.4 GHz RF radio by scanning for local APs..."));
    WiFi.mode(WIFI_STA);
    int n = WiFi.scanNetworks();
    Serial.print(F("[ESP] RF Scan complete! Detected "));
    Serial.print(n);
    Serial.println(F(" networks:"));
    for (int i = 0; i < n && i < 6; i++) {
        Serial.print(F("[ESP]   * "));
        Serial.print(WiFi.SSID(i));
        Serial.print(F(" ("));
        Serial.print(WiFi.RSSI(i));
        Serial.println(F(" dBm)"));
    }

    // Now switch to SoftAP mode
    WiFi.persistent(false);
    WiFi.disconnect(true);
    delay(50);
    WiFi.mode(WIFI_AP);

    bool ok = WiFi.softAP(full_ssid.c_str());
    if (ok) {
        Serial.print(F("[ESP] SoftAP active! SSID: "));
        Serial.print(full_ssid);
        Serial.print(F(" | IP: "));
        Serial.println(WiFi.softAPIP());
    } else {
        Serial.println(F("[ESP] SoftAP ERROR: Failed to start!"));
    }

    // 4. Start Captive DNS server on port 53 (redirects all domains to 192.168.4.1)
    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(DNS_PORT, "*", apIP);
    Serial.println(F("[ESP] Captive DNS active on port 53"));
}

void wifi_manager_loop() {
    dnsServer.processNextRequest();
}

String wifi_manager_get_ssid() {
    return full_ssid;
}

String wifi_manager_get_uid() {
    return chip_uid;
}

String wifi_manager_get_ip() {
    return apIP.toString();
}

IPAddress wifi_manager_get_ip_addr() {
    return apIP;
}

uint8_t wifi_manager_get_station_count() {
    return WiFi.softAPgetStationNum();
}
