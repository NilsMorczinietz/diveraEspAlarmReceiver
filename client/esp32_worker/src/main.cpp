#include <WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>
#include "secrets.h" // Contains WIFI_SSID and WIFI_PASSWORD
#include "led_control.h"
#include "settings.h"
#include "ledHandler.h"

#define DEBUG true

// UDP Configuration
WiFiUDP udp;
const int udpPort = 4210;
char incomingPacket[255]; // Buffer for incoming messages

// LED Strip Configuration
Adafruit_NeoPixel strip(NUM_MAX_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Global variable for the message accessed by Task2
String lastMessage = "off";

// Print debug messages if DEBUG mode is enabled
void debugPrint(const char *message)
{
    if (DEBUG)
    {
        Serial.println(message);
    }
}

// Connect to the Wi-Fi network
void setupWiFi()
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to Wi-Fi...");
    }
    Serial.println("Connected to Wi-Fi.");
    Serial.print("Device IP Address: ");
    Serial.println(WiFi.localIP());
}

// Reconnect to Wi-Fi if disconnected
void checkWiFiConnection()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        ledOff(Led::esp);
        Serial.println("Wi-Fi disconnected. Attempting reconnection...");
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(1000);
            Serial.println("Reconnecting to Wi-Fi...");
        }
        Serial.println("Reconnected to Wi-Fi.");
        ledOn(Led::esp);
    }
}

// Handle incoming UDP packets
void handleIncomingPacket()
{
    int packetSize = udp.parsePacket();

    // Ignore packets that are too large
    if (packetSize >= sizeof(incomingPacket))
    {
        Serial.println("Packet too large to process.");
        udp.flush();
        return;
    }

    // Process incoming packets
    if (packetSize > 0)
    {
        int len = udp.read(incomingPacket, sizeof(incomingPacket) - 1);
        if (len <= 0 || len >= sizeof(incomingPacket))
        {
            Serial.println("Invalid packet size.");
            return;
        }

        incomingPacket[len] = '\0'; // Null-terminate the string
        String message = String(incomingPacket);
        message.trim();
        message.replace("\"", "");

        Serial.printf("Received message: \"%s\"\n", message.c_str());

        message.toLowerCase();
        lastMessage = message;
    }
}

// Task1: Handles Wi-Fi reconnection and incoming UDP packets
void Task1(void *pvParameters)
{
    while (true)
    {
        checkWiFiConnection(); // Ensure Wi-Fi connection is active
        handleIncomingPacket();
        delay(500);
    }
}

// Task2: Executes actions based on the last received message
void Task2(void *pvParameters)
{
    while (true)
    {
        String message = lastMessage;

        // Handle specific commands
        if (message.equalsIgnoreCase("on"))
        {
            turnOnLEDsWhite(strip);
            Serial.println("LEDs ON");
        }
        else if (message.equalsIgnoreCase("off"))
        {
            turnOffAllLEDs(strip);
            Serial.println("LEDs OFF");
        }
        else if (message.equalsIgnoreCase("probe"))
        {
            blinkAllColors(strip, 5, 200);
            Serial.println("LEDs BLINKING");
            testEachLED(strip, 200, 2);
            Serial.println("LEDs TESTING");
        }
        else if (message.equalsIgnoreCase("blink:red"))
        {
            uint32_t blinkColor = strip.Color(255, 0, 0);
            uint32_t constantColor = strip.Color(255, 255, 255);
            blinkFirstLedsRestConstant(strip, blinkColor, constantColor, 100, 300);
        }
        else if (message.equalsIgnoreCase("blink:green"))
        {
            uint32_t blinkColor = strip.Color(0, 255, 0);
            uint32_t constantColor = strip.Color(255, 255, 255);
            blinkFirstLedsRestConstant(strip, blinkColor, constantColor, 100, 300);
        }
        else if (message.equalsIgnoreCase("blink:blue"))
        {
            uint32_t blinkColor = strip.Color(0, 0, 255);
            uint32_t constantColor = strip.Color(255, 255, 255);
            blinkFirstLedsRestConstant(strip, blinkColor, constantColor, 100, 300);
        }
        else if (message.equalsIgnoreCase("blink:white"))
        {
            uint32_t blinkColor = strip.Color(255, 255, 255);
            uint32_t constantColor = strip.Color(255, 255, 255);
            blinkFirstLedsRestConstant(strip, blinkColor, constantColor, 100, 300);
        }
        else
        {
            Serial.println("Unknown command");
        }
    }
}

void setup()
{
    ledOff(Led::esp);

    Serial.begin(9600);
    setupWiFi();

    // Initialize the LED strip
    strip.begin();
    strip.show();

    // Start UDP server
    if (udp.begin(udpPort))
    {
        Serial.println("UDP port 4210 opened successfully.");
        ledOn(Led::esp);
    }
    else
    {
        Serial.println("Failed to open UDP port.");
    }

    // Create Task1 (Receive UDP messages)
    xTaskCreate(Task1, "Task1", 10000, NULL, 1, NULL);

    // Create Task2 (Execute actions based on received messages)
    xTaskCreate(Task2, "Task2", 10000, NULL, 1, NULL);
}

void loop()
{
    // No code required in the loop as tasks handle operations
}

/**
 * Test commands:
 * echo "off" | ncat -u 192.168.178.105 4210
 * echo "on" | ncat -u 192.168.178.105 4210
 */