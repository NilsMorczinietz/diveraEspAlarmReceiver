#include "WebSocketHandler.h"
// #include "UDPHandler.h"
#include "settings.h"
#include "secrets.h"
#include "MessageProcessor.h"

WebsocketsClient client;

void onMessageCallback(WebsocketsMessage message) {
    Serial.print("Message received: ");
    Serial.println(message.data());

    // Send UDP message to all workers
    // sendUDPMessageToAll("on");
    processMessage(message.data());
}

void onEventsCallback(WebsocketsEvent event, String data) {
    switch (event) {
        case WebsocketsEvent::ConnectionOpened:
            Serial.println("WebSocket connection established.");
            break;
        case WebsocketsEvent::ConnectionClosed:
            Serial.println("WebSocket connection closed. Attempting to reconnect...");
            setupWebSocket();
            break;
        case WebsocketsEvent::GotPing:
            Serial.println("Ping received.");
            break;
        case WebsocketsEvent::GotPong:
            Serial.println("Pong received.");
            break;
    }
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

void handleWebSocket() {
    if (WiFi.status() == WL_CONNECTED) {
        if (client.available()) {
            client.poll();
        } else {
            Serial.println("WebSocket not available. Attempting to reconnect...");
            setupWebSocket();
        }
    }
}

void keepWebSocketAlive() {
    static unsigned long lastPingTime = 0;
    if (millis() - lastPingTime >= 60000 && client.available()) {
        client.ping();
        Serial.println("Ping sent.");
        lastPingTime = millis();
    }
}
