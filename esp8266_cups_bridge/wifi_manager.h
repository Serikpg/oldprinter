#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include "config.h"

// Initializes standalone direct Wi-Fi Access Point broadcasting SSID with unique hardware UID
void wifi_manager_init();

// Services DNS requests (redirects all traffic directly to printer IP)
void wifi_manager_loop();

// Network status queries
String    wifi_manager_get_ssid();
String    wifi_manager_get_uid();
String    wifi_manager_get_ip();
IPAddress wifi_manager_get_ip_addr();
uint8_t   wifi_manager_get_station_count();

#endif // WIFI_MANAGER_H
