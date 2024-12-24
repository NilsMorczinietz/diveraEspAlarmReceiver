#ifndef MESSAGE_PROCESSOR_H
#define MESSAGE_PROCESSOR_H

#include <Arduino.h>
#include "udpHandler.h"

// Define a structure for keyword-action pairs
struct KeywordAction {
    String keyword;
    void (*action)(); // Function pointer to the action
};


void processMessage(const String& message);

#endif // MESSAGE_PROCESSOR_H