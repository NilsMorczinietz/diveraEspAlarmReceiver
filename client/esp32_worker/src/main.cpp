#include <WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>
#include "secrets.h" // Contains WIFI_SSID and WIFI_PASSWORD
#include "led_control.h"
#include "settings.h"

// UDP configuration
WiFiUDP udp;
const int udpPort = 4210;
char incomingPacket[255]; // Buffer for incoming messages

Adafruit_NeoPixel strip(NUM_MAX_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

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

void handleIncomingPacket()
{
  int packetSize = udp.parsePacket();
  if (packetSize)
  {
    int len = udp.read(incomingPacket, sizeof(incomingPacket) - 1);
    if (len > 0)
    {
      incomingPacket[len] = '\0'; // Null-terminate the string
    }

    // Remove unwanted characters (e.g., quotes or spaces)
    String message = String(incomingPacket);
    message.trim();            // Removes leading/trailing whitespace
    message.replace("\"", ""); // Removes quotes

    Serial.printf("Received message: %s\n", incomingPacket);

    Serial.printf("Received message: \"%s\"\n", message.c_str());

    // Handle specific commands
    if (message == "on")
    {
      turnOnLEDsWhite(strip);
      Serial.println("LEDS AN");
    }
    else if (message == "off")
    {
      turnOffAllLEDs(strip);
      Serial.println("LEDS AUS");
    }
    else
    {
      Serial.println("Unknown command");
    }

    // Send a response
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.print("Message received!");
    udp.endPacket();
    Serial.println("Response sent.");
  }
}

void setup()
{
  Serial.begin(9600);
  setupWiFi();

  // Initialize LED strip
  strip.begin();
  strip.show();

  // Start UDP
  if (udp.begin(udpPort)) {
  Serial.println("UDP-Port 4210 erfolgreich geöffnet.");
} else {
  Serial.println("Fehler beim Öffnen des UDP-Ports.");
}

}

void loop()
{
  handleIncomingPacket();
  delay(10); // Small delay to reduce CPU usage
}

/**
 * echo "off" | ncat -u 192.168.178.105 4210
 * echo "on" | ncat -u 192.168.178.105 4210
 */