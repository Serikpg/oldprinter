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

    // 3. Set Wi-Fi to Access Point mode only (direct device-to-printer link)
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, netMsk);

    // If AP_PASSWORD is empty, launch an Open network for instant connection
    const char *pass = (strlen(AP_PASSWORD) >= 8) ? AP_PASSWORD : NULL;
    WiFi.softAP(full_ssid.c_str(), pass);

    // 4. Start Captive DNS server on port 53 (redirects all domains to 192.168.4.1)
    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(DNS_PORT, "*", apIP);
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
