#ifndef UDPHANDLER_H
#define UDPHANDLER_H

#include <Arduino.h>
#include <WiFiUdp.h>
#include "secrets.h"
#include "config.h"

/**
 * @brief Sends a UDP message to all worker devices.
 *
 * This function iterates through all the worker IP addresses specified in the configuration
 * and sends the provided message via UDP to each of them. The function logs each transmission
 * to the serial monitor.
 *
 * @param message The message to be sent to all workers.
 */
void sendUDPMessageToAll(const String &message);

/**
 * @brief Sends a UDP message to a specific worker device.
 *
 * This function sends the provided message via UDP to a specific target device, identified
 * by its IP address. The function logs the transmission to the serial monitor.
 *
 * @param message The message to be sent to the target device.
 * @param targetIP The IP address of the target device as a C-style string (const char*).
 */
void sendUDPMessageToSpecific(const String &message, const char *targetIP);

#endif // UDPHANDLER_H