#ifndef UDPHANDLER_H
#define UDPHANDLER_H

#include <WiFiUdp.h>
#include <Arduino.h>

void sendUDPMessageToAll(const String& message);
void sendUDPMessageToSpecific(const String& message, const char* targetIP);

#endif // UDPHANDLER_H
