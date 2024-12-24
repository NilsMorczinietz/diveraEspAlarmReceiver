#include "messageProcessor.h"

void demo() {
    Serial.println("Action: Alarm");
    sendUDPMessageToAll("on");

    long waitingTime = 2 * 60 * 1000;
    delay(waitingTime);

    sendUDPMessageToAll("off");
    
    waitingTime = 20 * 60 * 1000;
    delay(waitingTime);
}

// Array of keyword-action pairs
KeywordAction keywordActions[] = {
    {"Test", demo},
    {"Probe", demo},
    {"P01", demo},
    {"Probealarm", demo},
    {"Infoalarm", demo},

    {"Arbeit", demo},
    {"H20", demo},
    {"Kraftstoff", demo},
    
    {"F04", demo},
    {"BMA", demo},

    {"F10", demo},
    {"KZW", demo},
    {"AM03", demo},
    {"Vollalarm", demo},
    {"Stadtalarm", demo},
};

const int numActions = sizeof(keywordActions) / sizeof(keywordActions[0]);

void processMessage(const String& message) {
    // Extract the "text" field from the received message
    int textStart = message.indexOf("text:{") + 6;
    int textEnd = message.indexOf("}", textStart);
    if (textStart < 0 || textEnd < 0) {
        Serial.println("Error: Could not extract 'text' from the message.");
        return;
    }
    String notificationText = message.substring(textStart, textEnd);

    // Debugging: Print the extracted text
    Serial.println("Extracted text: " + notificationText);

    // Check each keyword and execute its associated action
    bool matched = false;
    for (int i = 0; i < numActions; i++) {
        if (notificationText.indexOf(keywordActions[i].keyword) >= 0) {
            Serial.println("Matched keyword: " + keywordActions[i].keyword);
            keywordActions[i].action();
            matched = true;
            break;
        }
    }

    // Execute the default action if no keywords matched
    if (!matched) {
        demo();
    }
}