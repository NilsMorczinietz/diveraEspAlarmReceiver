#include "WiFiSetup.h"
#include "WebSocketHandler.h"
#include "UDPHandler.h"

void setup() {
    Serial.begin(9600);
    delay(1000);

    setupWiFi();
    setupWebSocket();
}

void loop() {
    handleWebSocket();
    handleWiFiReconnect();
    keepWebSocketAlive();
}