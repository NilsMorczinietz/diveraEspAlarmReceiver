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

/**
 * @brief Blinks all LEDs quickly through multiple colors for testing.
 * 
 * This function displays a sequence of colors and tests the LED strip.
 * It quickly switches between different colors and simulates a blinking pattern.
 * 
 * @param strip Reference to the Adafruit_NeoPixel object controlling the LED strip.
 * @param blinkCount Number of blinks per color.
 * @param delayMs Duration (in milliseconds) per blink.
 */
void blinkAllColors(Adafruit_NeoPixel& strip, int blinkCount, int delayMs) {
    uint32_t colors[] = {
        strip.Color(255, 0, 0),   // Red
        strip.Color(0, 255, 0),   // Green
        strip.Color(0, 0, 255),   // Blue
        strip.Color(255, 255, 0), // Yellow
        strip.Color(0, 255, 255), // Cyan
        strip.Color(255, 0, 255), // Magenta
        strip.Color(255, 255, 255) // White
    };

    int numColors = sizeof(colors) / sizeof(colors[0]);

    for (int b = 0; b < blinkCount; b++) {
        for (int c = 0; c < numColors; c++) {
            // Set all LEDs to the current color
            for (int i = 0; i < NUM_MAX_LEDS; i++) {
                strip.setPixelColor(i, colors[c]);
            }
            strip.show();
            delay(delayMs);

            // Turn off all LEDs
            for (int i = 0; i < NUM_MAX_LEDS; i++) {
                strip.setPixelColor(i, strip.Color(0, 0, 0));
            }
            strip.show();
            delay(delayMs);
        }
    }
}

/**
 * @brief Tests each LED on the strip individually.
 * 
 * This function lights up each LED on the strip sequentially in white color,
 * keeps it on for a short duration, and then turns it off before moving to the next LED.
 * The entire process can be repeated multiple times.
 * 
 * @param strip Reference to the Adafruit_NeoPixel object controlling the LED strip.
 * @param delayMs Duration (in milliseconds) each LED stays lit.
 * @param repeatCount Number of times to repeat the test sequence.
 */
void testEachLED(Adafruit_NeoPixel& strip, int delayMs, int repeatCount) {
    for (int r = 0; r < repeatCount; r++) {
        for (int i = 0; i < NUM_MAX_LEDS; i++) {
            // Turn on the current LED
            strip.setPixelColor(i, strip.Color(255, 255, 255));
            strip.show();
            delay(delayMs);

            // Turn off the current LED
            strip.setPixelColor(i, strip.Color(0, 0, 0));
            strip.show();
        }
    }
}

#endif // LED_CONTROL_H