#ifndef MESSAGEPROCESSOR_H
#define MESSAGEPROCESSOR_H

#include <Arduino.h>

// Define a structure for keyword-action pairs
struct KeywordAction {
    String keyword;
    void (*action)(); // Function pointer to the action
};

// Function prototypes
void processMessage(const String& message);

// Example actions
void actionTestProbe();
void actionCritical();
void actionNonCritical();

#endif // MESSAGEPROCESSOR_H