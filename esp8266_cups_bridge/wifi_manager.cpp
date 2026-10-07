#include "wifi_manager.h"
#include <EEPROM.h>

static bool is_connected = false;

String format_bssid(const uint8_t *b) {
    char buf[20];
    snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X",
             b[0], b[1], b[2], b[3], b[4], b[5]);
    return String(buf);
}

static bool parse_bssid(const String &str, uint8_t b[6]) {
    if (str.length() < 17) return false;
    unsigned int v[6];
    if (sscanf(str.c_str(), "%x:%x:%x:%x:%x:%x",
               &v[0], &v[1], &v[2], &v[3], &v[4], &v[5]) == 6) {
        for (int i = 0; i < 6; i++) b[i] = (uint8_t)v[i];
        return true;
    }
    return false;
}

void wifi_manager_init() {
    EEPROM.begin(EEPROM_CONFIG_SIZE);
}

bool wifi_manager_load(SavedWiFiConfig &cfg) {
    EEPROM.get(0, cfg);
    return (cfg.magic == EEPROM_MAGIC && cfg.ssid[0] != '\0');
}

bool wifi_manager_save(const String &ssid, const String &pass, const String &bssid_str, bool lock_bssid) {
    SavedWiFiConfig cfg;
    cfg.magic = EEPROM_MAGIC;
    memset(cfg.ssid, 0, sizeof(cfg.ssid));
    memset(cfg.password, 0, sizeof(cfg.password));
    memset(cfg.bssid, 0, sizeof(cfg.bssid));

    strncpy(cfg.ssid, ssid.c_str(), sizeof(cfg.ssid) - 1);
    strncpy(cfg.password, pass.c_str(), sizeof(cfg.password) - 1);
    cfg.lock_bssid = lock_bssid && parse_bssid(bssid_str, cfg.bssid);

    EEPROM.put(0, cfg);
    return EEPROM.commit();
}

bool wifi_manager_connect() {
    SavedWiFiConfig cfg;
    bool has_saved = wifi_manager_load(cfg);

    WiFi.mode(WIFI_STA);
    is_connected = false;

    // 1. Try saved credentials from EEPROM
    if (has_saved) {
        if (cfg.lock_bssid) {
            WiFi.begin(cfg.ssid, cfg.password, 0, cfg.bssid);
        } else {
            WiFi.begin(cfg.ssid, cfg.password);
        }

        unsigned long startAttempt = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 12000) {
            delay(250);
        }
        if (WiFi.status() == WL_CONNECTED) {
            is_connected = true;
            return true;
        }
    }

    // 2. Try default hardcoded credentials if present
    if (String(DEFAULT_WIFI_SSID).length() > 0) {
        WiFi.begin(DEFAULT_WIFI_SSID, DEFAULT_WIFI_PASSWORD);
        unsigned long startAttempt = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - startAttempt < 10000) {
            delay(250);
        }
        if (WiFi.status() == WL_CONNECTED) {
            is_connected = true;
            return true;
        }
    }

    // 3. Fallback to Access Point mode for on-the-fly network discovery & setup
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(AP_FALLBACK_SSID, AP_FALLBACK_PASS);
    return false;
}

String wifi_manager_scan_json() {
    int n = WiFi.scanNetworks(false, true); // Active async-free scan including hidden
    String json = "[";
    for (int i = 0; i < n; i++) {
        if (i > 0) json += ",";
        json += "{";
        json += "\"ssid\":\"" + WiFi.SSID(i) + "\",";
        json += "\"bssid\":\"" + WiFi.BSSIDstr(i) + "\",";
        json += "\"rssi\":" + String(WiFi.RSSI(i)) + ",";
        json += "\"channel\":" + String(WiFi.channel(i)) + ",";
        json += "\"secure\":" + String(WiFi.encryptionType(i) != ENC_TYPE_NONE ? "true" : "false");
        json += "}";
    }
    json += "]";
    return json;
}

bool wifi_manager_is_connected() {
    return (WiFi.status() == WL_CONNECTED);
}

String wifi_manager_get_active_ssid() {
    if (WiFi.status() == WL_CONNECTED) {
        return WiFi.SSID();
    }
    return String(AP_FALLBACK_SSID) + " (Setup AP)";
}

String wifi_manager_get_active_bssid() {
    if (WiFi.status() == WL_CONNECTED) {
        return WiFi.BSSIDstr();
    }
    return WiFi.softAPmacAddress();
}
