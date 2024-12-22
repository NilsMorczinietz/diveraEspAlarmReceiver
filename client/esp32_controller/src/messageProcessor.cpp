#include "MessageProcessor.h"
#include "UDPHandler.h"

// Define the actions
void actionTestProbe() {
    Serial.println("Action: Test or probe detected.");
    sendUDPMessageToAll("on");
    // TODO: Implement specific behavior
}

void actionCritical() {
    Serial.println("Action: Critical keywords detected (e.g., danger, smoke).");
    sendUDPMessageToAll("on");
    // TODO: Implement specific behavior
}

void actionNonCritical() {
    Serial.println("Action: Non-critical alert (e.g., F04, BMA).");
    sendUDPMessageToAll("on");
    // TODO: Implement specific behavior
}

void actionDefault() {
    Serial.println("Action: Default case. No specific keywords detected.");
    sendUDPMessageToAll("on");
    // TODO: Implement default behavior
}

// Array of keyword-action pairs
KeywordAction keywordActions[] = {
    {"Test", actionTestProbe},
    {"probe", actionTestProbe},
    {"F10", actionCritical},
    {"KZW", actionCritical},
    {"Leben", actionCritical},
    {"Menschen", actionCritical},
    {"Gefahr", actionCritical},
    {"Rauch", actionCritical},
    {"Geruch", actionCritical},
    {"F04", actionNonCritical},
    {"BMA", actionNonCritical},
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
            keywordActions[i].action(); // Call the associated action
            matched = true;
            break; // Exit the loop once a match is found
        }
    }

    // Execute the default action if no keywords matched
    if (!matched) {
        actionDefault();
    }
}