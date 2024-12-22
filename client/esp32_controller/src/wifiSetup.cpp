#include "WiFiSetup.h"

void setupWiFi() {
    Serial.println("Connecting to Wi-Fi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi connected!");
}

void handleWiFiReconnect() {
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.disconnect();
        delay(1000);
        setupWiFi();
    }
}