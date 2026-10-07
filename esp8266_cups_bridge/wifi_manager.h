#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "config.h"

// Initializes EEPROM and attempts connection
void wifi_manager_init();

// Attempts to connect to stored network, fallback credentials, or starts Access Point
bool wifi_manager_connect();

// Saves Wi-Fi credentials and optional target BSSID (UID) to EEPROM
bool wifi_manager_save(const String &ssid, const String &pass, const String &bssid_str, bool lock_bssid);

// Loads stored config from EEPROM
bool wifi_manager_load(SavedWiFiConfig &cfg);

// Performs active Wi-Fi scan and returns JSON array of visible networks
String wifi_manager_scan_json();

// Status queries
bool   wifi_manager_is_connected();
String wifi_manager_get_active_ssid();
String wifi_manager_get_active_bssid();
String format_bssid(const uint8_t *b);

#endif // WIFI_MANAGER_H
