#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <Adafruit_NeoPixel.h>
#include "settings.h"

/**
 * @brief Turns on all LEDs in white color.
 * 
 * This function iterates over all available LEDs on the strip and sets their color to white
 * (RGB value: 255, 255, 255). It then updates the strip to display the changes.
 * 
 * @param strip Reference to the Adafruit_NeoPixel object controlling the LED strip.
 */
void turnOnLEDsWhite(Adafruit_NeoPixel& strip) {
    for (int i = 0; i < NUM_MAX_LEDS; i++) {
        strip.setPixelColor(i, strip.Color(255, 255, 255));
    }
    strip.show();
}

/**
 * @brief Turns off all LEDs.
 * 
 * This function sets the color of all LEDs on the strip to black (RGB value: 0, 0, 0),
 * effectively turning them off. It then updates the strip to apply the changes.
 * 
 * @param strip Reference to the Adafruit_NeoPixel object controlling the LED strip.
 */
void turnOffAllLEDs(Adafruit_NeoPixel& strip) {
    for (int i = 0; i < NUM_MAX_LEDS; i++) {
        strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    strip.show();
}

/**
 * @brief Turns on a specific number of LEDs in white color.
 * 
 * This function iterates over the specified number of LEDs and sets their color to white
 * (RGB value: 255, 255, 255). If the count exceeds the maximum number of LEDs, it only affects
 * the available LEDs. The strip is then updated to reflect the changes.
 * 
 * @param strip Reference to the Adafruit_NeoPixel object controlling the LED strip.
 * @param count Number of LEDs to turn on in white color.
 */
void turnOnLEDsCount(Adafruit_NeoPixel& strip, int count) {
    for (int i = 0; i < count && i < NUM_MAX_LEDS; i++) {
        strip.setPixelColor(i, strip.Color(255, 255, 255));
    }
    strip.show();
}

#endif // LED_CONTROL_H