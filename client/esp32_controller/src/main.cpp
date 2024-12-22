#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <Adafruit_NeoPixel.h>

#include "secrets.h"
#include "settings.h"

using namespace websockets;

WebsocketsClient client;

// Function prototypes
void setupWiFi();
void setupWebSocket();

// WebSocket message callback
void onMessageCallback(WebsocketsMessage message) {
    Serial.print("Got Message: ");
    Serial.println(message.data());
}

// WebSocket event callback
void onEventsCallback(WebsocketsEvent event, String data) {
    if (event == WebsocketsEvent::ConnectionOpened) {
        Serial.println("Connection Opened");
    } else if (event == WebsocketsEvent::ConnectionClosed) {
        Serial.println("Connection Closed");
    } else if (event == WebsocketsEvent::GotPing) {
        Serial.println("Got a Ping!");
    } else if (event == WebsocketsEvent::GotPong) {
        Serial.println("Got a Pong!");
    }
}

void setup() {
    Serial.begin(9600);
    delay(1000);

    // Setup Wi-Fi and WebSocket
    setupWiFi();
    setupWebSocket();
}

void loop() {
    client.poll();  // Continuously check for incoming messages and events

    // Send a Ping every 60 seconds to keep the WebSocket connection alive
    static unsigned long lastPingTime = 0;
    if (millis() - lastPingTime >= 60000) {
        client.ping();
        Serial.println("Ping sent");
        lastPingTime = millis();
    }
}

void setupWiFi() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to Wi-Fi...");
    }
    Serial.println("Wi-Fi connected!");
}

void setupWebSocket() {
    client.onMessage(onMessageCallback);
    client.onEvent(onEventsCallback);
    client.connect(WEBSOCKETS_SERVER_HOST);

    client.send("Hello Server");
    client.ping();
}