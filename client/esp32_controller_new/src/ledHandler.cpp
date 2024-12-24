#include "ledHandler.h"

std::bitset<MAX_PINS> initializedPins; // Tracks which pins are initialized

void setupPin(unsigned int pin)
{
    if (!initializedPins.test(pin))
    {
        pinMode(pin, OUTPUT);
        initializedPins.set(pin);
    }
}

void ledOn(unsigned int pin)
{
    setupPin(pin);
    digitalWrite(pin, HIGH);
}

void ledOff(unsigned int pin)
{
    setupPin(pin);
    digitalWrite(pin, LOW);
}
