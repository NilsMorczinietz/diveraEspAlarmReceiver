#include "udpHandler.h"

static WiFiUDP udp;

const int numWorkers = sizeof(Config::WORKER_IPS) / sizeof(Config::WORKER_IPS[0]);

void sendUDPMessageToAll(const String &message)
{
    for (int i = 0; i < numWorkers; i++)
    {
        udp.beginPacket(Config::WORKER_IPS[i], Config::UDP_PORT);
        udp.print(message);
        udp.endPacket();
        Serial.println("UDP message sent to " + String(Config::WORKER_IPS[i]) + ": " + message);
    }
}

void sendUDPMessageToSpecific(const String &message, const char *targetIP)
{
    udp.beginPacket(targetIP, Config::UDP_PORT);
    udp.print(message);
    udp.endPacket();
    Serial.println("UDP message sent to " + String(targetIP) + ": " + message);
}