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

    // Don't load/store SDK Wi-Fi config in flash; start the radio from a clean state
    WiFi.persistent(false);
    WiFi.mode(WIFI_OFF);
    delay(100);
    WiFi.mode(WIFI_AP);

    // Lower TX power reduces current peaks on marginal 3.3V supplies (prevents brownout)
    WiFi.setOutputPower(17.0);

    const char *pass = (strlen(AP_PASSWORD) >= 8) ? AP_PASSWORD : NULL;
    bool ok = WiFi.softAPConfig(apIP, apIP, netMsk) &&
              WiFi.softAP(full_ssid.c_str(), pass, 6, 0, 4);
    if (!ok) {
        Serial.println(F("[ESP] Custom SoftAP config failed, trying minimal fallback..."));
        WiFi.softAP(full_ssid.c_str(), pass);
    } else {
        Serial.print(F("[ESP] SoftAP active! SSID: "));
        Serial.print(full_ssid);
        Serial.print(F(" | IP: "));
        Serial.println(apIP);
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
