#include <WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>
#include "secrets.h" // Contains WIFI_SSID and WIFI_PASSWORD
#include "led_control.h"
#include "settings.h"

#define DEBUG true

// UDP configuration
WiFiUDP udp;
const int udpPort = 4210;
char incomingPacket[255]; // Buffer for incoming messages

// LED strip configuration
Adafruit_NeoPixel strip(NUM_MAX_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Prints debug messages if DEBUG is enabled
void debugPrint(const char *message)
{
  if (DEBUG)
  {
    Serial.println(message);
  }
}

// Connects to the Wi-Fi network
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

// Reconnects to Wi-Fi if disconnected
void checkWiFiConnection()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Wi-Fi disconnected. Attempting reconnection...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(1000);
      Serial.println("Reconnecting to Wi-Fi...");
    }
    Serial.println("Reconnected to Wi-Fi.");
  }
}

// Handles incoming UDP packets
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

    // Handle specific commands
    if (message.equalsIgnoreCase("on"))
    {
      turnOnLEDsWhite(strip);
      Serial.println("LEDS AN");
    }
    else if (message.equalsIgnoreCase("off"))
    {
      turnOffAllLEDs(strip);
      Serial.println("LEDS AUS");
    }
    else
    {
      Serial.println("Unknown command");
    }

    // Send response to the client
    // udp.beginPacket(udp.remoteIP(), udp.remotePort());
    // udp.print("Message received!");
    // udp.endPacket();
    // Serial.println("Response sent.");
  }
}

void setup()
{
  Serial.begin(9600);
  setupWiFi();

  // Initialize the LED strip
  strip.begin();
  strip.show();

  // Start UDP server
  if (udp.begin(udpPort))
  {
    Serial.println("UDP-Port 4210 erfolgreich geöffnet.");
  }
  else
  {
    Serial.println("Fehler beim Öffnen des UDP-Ports.");
  }
}

void loop()
{
  checkWiFiConnection(); // Ensure Wi-Fi connection is active
  handleIncomingPacket();
  delay(10); // Small delay to reduce CPU usage
}

/**
 * Test commands:
 * echo "off" | ncat -u 192.168.178.105 4210
 * echo "on" | ncat -u 192.168.178.105 4210
 */