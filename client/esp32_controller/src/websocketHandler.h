#ifndef WEBSOCKETHANDLER_H
#define WEBSOCKETHANDLER_H

#include <ArduinoWebsockets.h>
using namespace websockets;

void setupWebSocket();
void handleWebSocket();
void keepWebSocketAlive();

#endif // WEBSOCKETHANDLER_H