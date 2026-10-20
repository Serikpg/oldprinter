#include <ESP8266WiFi.h>

void setup() {
    delay(1000);
    WiFi.persistent(false);
    WiFi.mode(WIFI_AP);
    WiFi.softAP("ESP_TEST_NETWORK");
}

void loop() {
    delay(1000);
}
