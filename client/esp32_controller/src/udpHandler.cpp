#include "UDPHandler.h"

WiFiUDP udp;
const char* workerIPs[] = {"192.168.178.105", "192.168.178.106", "192.168.178.107"};
const unsigned int udpPort = 4210;
const int numWorkers = sizeof(workerIPs) / sizeof(workerIPs[0]);

void sendUDPMessageToAll(const String& message) {
    for (int i = 0; i < numWorkers; i++) {
        udp.beginPacket(workerIPs[i], udpPort);
        udp.print(message);
        udp.endPacket();
        Serial.println("UDP message sent to " + String(workerIPs[i]) + ": " + message);
    }
}

void sendUDPMessageToSpecific(const String& message, const char* targetIP) {
    udp.beginPacket(targetIP, udpPort);
    udp.print(message);
    udp.endPacket();
    Serial.println("UDP message sent to " + String(targetIP) + ": " + message);
}