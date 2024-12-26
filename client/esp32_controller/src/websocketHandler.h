#ifndef WEBSOCKET_HANDLER_H
#define WEBSOCKET_HANDLER_H

#include <ArduinoWebsockets.h>
#include "secrets.h"
#include "ledHandler.h"

using namespace websockets;

/**
 * @brief Sends a pong response to the WebSocket server.
 *
 * This function responds to a received ping message from the WebSocket server.
 * It is typically used to acknowledge the server's ping and maintain the
 * WebSocket connection's state.
 */
void pongWebSocket();

/**
 * @brief Sends a ping message to the WebSocket server.
 *
 * This function ensures the WebSocket connection is active and sends a
 * ping message to verify connectivity and keep the connection alive.
 */
void pingWebSocket();

/**
 * @brief Polls the WebSocket for incoming messages or events.
 *
 * This function checks for and processes any incoming data or connection events.
 * It should be called periodically to ensure the WebSocket client remains responsive.
 */
void pollWebSocket();

/**
 * @brief Callback function to handle incoming WebSocket messages.
 *
 * @param message The WebSocket message received from the server.
 *
 * This function is triggered when the WebSocket client receives a message.
 * It processes and outputs the message data.
 */
void onMessageCallback(WebsocketsMessage message);

/**
 * @brief Callback function to handle WebSocket connection events.
 *
 * @param event The type of WebSocket event (e.g., connection opened, connection closed, etc.).
 * @param data Additional data associated with the event.
 *
 * This function is triggered on connection-related events. It updates the
 * connection status and handles ping/pong messages.
 */
void onEventsCallback(WebsocketsEvent event, String data);

/**
 * @brief Establishes a connection to the WebSocket server.
 *
 * @param serverHost The URL or IP address of the WebSocket server.
 *
 * This function attempts to connect to the specified WebSocket server and
 * initializes the connection if successful. It also registers callbacks for
 * handling messages and events.
 */
void connectWebSocket(const String serverHost = Config::WEBSOCKETS_SERVER_HOST);

/**
 * @brief Periodically checks the WebSocket connection and keeps it alive.
 *
 * @param interval The interval (in milliseconds) between successive checks.
 *
 * This function checks if the WebSocket connection is active and operational.
 * If the connection is lost, it attempts to reconnect. It also sends ping
 * messages and polls for data to maintain the connection.
 */
void checkWebSocket(unsigned long interval = 60000);

/**
 * @brief Retrieves the latest message received via WebSocket.
 *
 * This function returns the latest message received from the WebSocket server.
 * If no message has been received since the last retrieval, it returns an empty string.
 * After calling this function, the stored message is cleared to avoid processing the same message multiple times.
 *
 * @return String - The latest message received, or an empty string if no new message is available.
 */
String getLatestMessage();

#endif // WEBSOCKET_HANDLER_H