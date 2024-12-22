#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>

#include "secrets.h"  // Contains WIFI_SSID and WIFI_PASSWORD
#include "settings.h" // Contains WEBSOCKETS_SERVER_HOST

using namespace websockets;

WebsocketsClient client;
WiFiUDP udp;

// List of worker/slave IP addresses
const char* workerIPs[] = {"192.168.178.105", "192.168.178.106", "192.168.178.107"};
const unsigned int udpPort = 4210; // Target port
const int numWorkers = sizeof(workerIPs) / sizeof(workerIPs[0]);

// Function prototypes
void setupWiFi();
void setupWebSocket();
void reconnectWiFi();
void reconnectWebSocket();
void sendUDPMessageToAll(const String& message);
void sendUDPMessageToSpecific(const String& message, const char* targetIP);

// WebSocket message callback
void onMessageCallback(WebsocketsMessage message) {
    Serial.print("Message received: ");
    Serial.println(message.data());

    // Send UDP message to all workers
    sendUDPMessageToAll("on");
}

// WebSocket event callback
void onEventsCallback(WebsocketsEvent event, String data) {
    switch (event) {
        case WebsocketsEvent::ConnectionOpened:
            Serial.println("WebSocket connection established.");
            break;
        case WebsocketsEvent::ConnectionClosed:
            Serial.println("WebSocket connection closed. Attempting to reconnect...");
            reconnectWebSocket();
            break;
        case WebsocketsEvent::GotPing:
            Serial.println("Ping received.");
            break;
        case WebsocketsEvent::GotPong:
            Serial.println("Pong received.");
            break;
    }
}

void setup() {
    Serial.begin(9600);
    delay(1000);

    setupWiFi();
    setupWebSocket();
}

void loop() {
    // Check and poll WebSocket messages/events
    if (WiFi.status() == WL_CONNECTED) {
        if (client.available()) {
            client.poll();
        } else {
            Serial.println("WebSocket not available. Attempting to reconnect...");
            reconnectWebSocket();
        }
    } else {
        Serial.println("Wi-Fi disconnected. Attempting to reconnect...");
        reconnectWiFi();
    }

    // Keep WebSocket connection alive
    static unsigned long lastPingTime = 0;
    if (millis() - lastPingTime >= 60000 && client.available()) {
        client.ping();
        Serial.println("Ping sent.");
        lastPingTime = millis();
    }
}

void setupWiFi() {
    Serial.println("Connecting to Wi-Fi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi connected!");
}

void setupWebSocket() {
    Serial.println("Connecting to WebSocket...");
    client.onMessage(onMessageCallback);
    client.onEvent(onEventsCallback);

    if (client.connect(WEBSOCKETS_SERVER_HOST)) {
        Serial.println("WebSocket connected!");
        client.send("Hello Server");
        client.ping();
    } else {
        Serial.println("WebSocket connection failed.");
    }
}

void reconnectWiFi() {
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.disconnect();
        delay(1000);
        setupWiFi();
    }
}

void reconnectWebSocket() {
    if (!client.available()) {
        client.close();
        delay(1000);
        setupWebSocket();
    }
}

void sendUDPMessageToAll(const String& message) {
    for (int i = 0; i < numWorkers; i++) {
        udp.beginPacket(workerIPs[i], udpPort);
        udp.print(message);
        udp.endPacket();
        Serial.println("UDP message sent to " + String(workerIPs[i]) + ": " + message);
    }
}

void sendUDPMessageToSpecific(const String& message, const char* targetIP) {
    udp.beginPacket(targetIP, udpPort);
    udp.print(message);
    udp.endPacket();
    Serial.println("UDP message sent to " + String(targetIP) + ": " + message);
}