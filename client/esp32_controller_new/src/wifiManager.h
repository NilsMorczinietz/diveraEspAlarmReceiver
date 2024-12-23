#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>
#include <WiFi.h>
#include "secrets.h"

/**
 * @brief Connects to the specified Wi-Fi network.
 *
 * This function attempts to connect to a Wi-Fi network using the provided SSID and password.
 * It will block until the connection is established.
 *
 * @param ssid The SSID (name) of the Wi-Fi network (default: Config::WIFI_SSID).
 * @param password The password of the Wi-Fi network (default: Config::WIFI_PASSWORD).
 */
void connectToWifi(const char *ssid = Config::WIFI_SSID, const char *password = Config::WIFI_PASSWORD);

/**
 * @brief Handles Wi-Fi reconnection if the connection is lost.
 *
 * This function checks the Wi-Fi connection status, and if the connection is lost,
 * it will attempt to reconnect using the connectToWifi function.
 */
void handleWifiReconnection();

#endif // WIFI_MANAGER_H