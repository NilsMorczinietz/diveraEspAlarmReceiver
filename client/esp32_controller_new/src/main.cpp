#include <Arduino.h>
#include "wifiManager.h"
#include "statusManager.h"
#include "websocketHandler.h"

void setup()
{
  Serial.begin(9600);
  delay(1000);

  connectToWifi();

  connectWebSocket();

  StatusManager::setStatus(Status::active);

  Serial.println("System initialized.");
}

void loop()
{
  pollWebSocket();

  handleWifiReconnection();

  checkWebSocket();

  delay(100);
}