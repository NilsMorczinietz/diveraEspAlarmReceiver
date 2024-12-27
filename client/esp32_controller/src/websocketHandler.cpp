#include "websocketHandler.h"

static bool webSocketConnected = false;
static String latestMessage = "";

static WebsocketsClient client;

void pingWebSocket()
{
    if (webSocketConnected)
    {
        client.ping();
    }
}

void pongWebSocket()
{
    if (webSocketConnected)
    {
        client.pong();
    }
}

void pollWebSocket()
{
    if (webSocketConnected)
    {
        client.poll();
    }
}

void onMessageCallback(WebsocketsMessage message)
{
    latestMessage = message.data();
}

void onEventsCallback(WebsocketsEvent event, String data)
{
    switch (event)
    {
    case WebsocketsEvent::ConnectionOpened:
        Serial.println("WebSocket connection established.");
        ledOn(Led::esp);
        webSocketConnected = true;
        break;
    case WebsocketsEvent::ConnectionClosed:
        Serial.println("WebSocket connection closed. Attempting to reconnect...");
        ledOff(Led::esp);
        webSocketConnected = false;
        break;
    case WebsocketsEvent::GotPing:
        Serial.println("Ping received.");
        webSocketConnected = true;
        break;
    case WebsocketsEvent::GotPong:
        Serial.println("Pong received.");
        pongWebSocket();
        webSocketConnected = true;
        break;
    }
}

void connectWebSocket(const String serverHost)
{
    Serial.println("Connecting to WebSocket...");

    client.onMessage(onMessageCallback);
    client.onEvent(onEventsCallback);

    while (!webSocketConnected)
    {
        if (client.connect(serverHost))
        {
            Serial.println("WebSocket connected!");
            client.send("Hello Server");
            client.ping();

            webSocketConnected = true;
        }
        else
        {
            Serial.println("WebSocket connection failed. Retrying...");
            delay(1000); // Wait 1 second before retrying
        }
    }
}

boolean checkWebSocket(unsigned long interval, const int maxAttempts)
{
    static unsigned long lastCheckTime = 0;
    static int connectionAttempts = 0;

    if (millis() - lastCheckTime < interval)
    {
        return true;
    }

    lastCheckTime = millis();

    if (!webSocketConnected || !client.available())
    {
        if (connectionAttempts >= maxAttempts)
        {
            return false;
        }

        connectionAttempts++;

        if (client.connect(Config::WEBSOCKETS_SERVER_HOST))
        {
            connectionAttempts = 0;
        }
    }
    else
    {
        connectionAttempts = 0;
        pingWebSocket();
        pollWebSocket();
    }

    return true;
}

String getLatestMessage()
{
    String message = latestMessage;
    latestMessage = "";
    return message;
}