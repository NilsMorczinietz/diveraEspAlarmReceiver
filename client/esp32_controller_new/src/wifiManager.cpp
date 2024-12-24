#include "wifiManager.h"

void connectToWifi(const char *ssid, const char *password)
{
    Serial.print("Connecting to ");
    Serial.println(ssid);
    Serial.print("Please wait...");

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(".");
    }

    ledOn(Led::esp);

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void handleWifiReconnection()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        ledOff(Led::esp);
        Serial.println("WiFi connection lost. Reconnecting...");
        connectToWifi();
    }
}