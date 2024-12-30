#include <Arduino.h>
#include "wifiManager.h"
#include "websocketHandler.h"
#include "ledHandler.h"
#include "AlarmSystem.h"

AlarmSystem alarmSystem;

void setup()
{
  Serial.begin(9600);
  delay(1000);

  ledOff(Led::esp);

  connectToWifi();

  connectWebSocket();

  alarmSystem = AlarmSystem();

  ledOn(Led::esp);
  Serial.println("System initialized.");

  sendUDPMessageToAll("off");
}

void loop()
{
  delay(10);

  handleWifiReconnection();

  boolean result = checkWebSocket();

  if (!result)
  {
    return;
  }

  alarmSystem.checkAlarm();

  pollWebSocket();

  String message = getLatestMessage();

  if (message == "")
  {
    return;
  }

  Serial.println(message);

  alarmSystem.handleNewMessage(message);
}