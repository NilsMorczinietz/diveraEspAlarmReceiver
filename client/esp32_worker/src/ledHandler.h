#pragma once

#include <bitset>
#include <Arduino.h>

namespace Led
{
    constexpr const unsigned int esp = 2;
}

// Maximum number of pins supported
constexpr size_t MAX_PINS = 40;

/**
 * @brief Set up the pin if not already initialized.
 *
 * @param pin The GPIO pin to configure.
 */
void setupPin(unsigned int pin);

/**
 * @brief Turns the LED on.
 *
 * @param pin The GPIO pin connected to the LED.
 */
void ledOn(unsigned int pin);

/**
 * @brief Turns the LED off.
 *
 * @param pin The GPIO pin connected to the LED.
 */
void ledOff(unsigned int pin);