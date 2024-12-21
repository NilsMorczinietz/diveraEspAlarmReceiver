#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include "secrets.h"  // Include the file with sensitive information like Wi-Fi credentials

using namespace websockets;

void onMessageCallback(WebsocketsMessage message) {
    // Handle incoming WebSocket messages
    Serial.print("Got Message: ");
    Serial.println(message.data());
}

void onEventsCallback(WebsocketsEvent event, String data) {
    // Handle WebSocket events like connection opened/closed, ping/pong
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

WebsocketsClient client;

void setup() {
    Serial.begin(9600);
    delay(1000);

    // Connect to Wi-Fi using credentials from secrets.h
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to Wi-Fi...");
    }
    Serial.println("Wi-Fi connected!");

    // Set up WebSocket client and callbacks
    client.onMessage(onMessageCallback);
    client.onEvent(onEventsCallback);
    client.connect(WEBSOCKETS_SERVER_HOST);

    // Initial message and ping to the server
    client.send("Hello Server");
    client.ping();
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