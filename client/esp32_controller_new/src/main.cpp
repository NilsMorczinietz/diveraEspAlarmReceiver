#include <Arduino.h>
#include "wifiManager.h"
#include "statusManager.h"
#include "websocketHandler.h"

const int ESP_LED_PIN = 2;

void setup()
{
  pinMode(ESP_LED_PIN, OUTPUT);
  digitalWrite(ESP_LED_PIN, LOW);

  Serial.begin(9600);
  delay(1000);

  connectToWifi();

  connectWebSocket();

  StatusManager::setStatus(Status::active);

  digitalWrite(ESP_LED_PIN, HIGH);
  Serial.println("System initialized.");
}

void loop()
{
  delay(10);

  handleWifiReconnection();

  checkWebSocket();

  pollWebSocket();

  String message = getLatestMessage();

  if (message == "")
  {
    return;
  }

  Serial.println(message);
}