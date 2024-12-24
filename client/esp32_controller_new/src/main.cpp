#include <Arduino.h>
#include "wifiManager.h"
#include "statusManager.h"
#include "websocketHandler.h"
#include "ledHandler.h"

void setup()
{
  Serial.begin(9600);
  delay(1000);
  
  ledOff(Led::esp);

  connectToWifi();

  connectWebSocket();

  StatusManager::setStatus(Status::active);

  ledOn(Led::esp);
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