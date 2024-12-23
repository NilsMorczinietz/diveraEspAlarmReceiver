#include "MessageProcessor.h"
#include "UDPHandler.h"
#include "settings.h"

enum class AlarmStatus {
    Active,          // No alarm is active
    Triggert,        // Alarm has been triggered
    Waiting,         // Waiting for the 2-minute timeout
    Break,           // min. beak between alarms 
};
static AlarmStatus status = AlarmStatus::Active; // Initial status

// Define the actions
void actionProbe() {
    Serial.println("Action: Probe alarm");
    sendUDPMessageToAll("probe");
}

void actionAbort() {
    Serial.println("Action: Abort alarm");
    // sendUDPMessageToAll("on");
    // TODO: Implement specific behavior
}

void actionAlarm() {
    if(status != AlarmStatus::Active) {
        return;
    }
    status = AlarmStatus::Triggert;

    Serial.println("Action: Alarm");
    sendUDPMessageToAll("on");

    status = AlarmStatus::Waiting;

    long waitingTime = WAITING_TIME_ALARM * 60 * 1000;
    delay(waitingTime);
    sendUDPMessageToAll("off");

    status = AlarmStatus::Break;
    waitingTime = 20 * 60 * 1000;
    delay(waitingTime);
}

void actionDefault() {
    if(status != AlarmStatus::Active) {
        return;
    }
    status = AlarmStatus::Triggert;

    Serial.println("Action: Default");
    sendUDPMessageToAll("on");

    status = AlarmStatus::Waiting;

    long waitingTime = WAITING_TIME_ALARM * 60 * 1000;
    delay(waitingTime);
    sendUDPMessageToAll("off");

    status = AlarmStatus::Break;
    waitingTime = 20 * 60 * 1000;
    delay(waitingTime);
}

// Array of keyword-action pairs
KeywordAction keywordActions[] = {
    {"Einsatzabruch", actionAbort},
    {"Einsatzabbruch", actionAbort},
    {"Rueckalarm", actionAbort},
    {"RUECKALARM", actionAbort},

    {"Test", actionProbe},
    {"Probe", actionProbe},
    {"P01", actionProbe},
    {"Probealarm", actionProbe},
    {"Infoalarm", actionProbe},

    {"Arbeit", actionAlarm},
    {"H20", actionAlarm},
    {"Kraftstoff", actionAlarm},
    
    {"F04", actionAlarm},
    {"BMA", actionAlarm},

    {"F10", actionAlarm},
    {"KZW", actionAlarm},
    {"AM03", actionAlarm},
    {"Vollalarm", actionAlarm},
    {"Stadtalarm", actionAlarm},
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
        actionDefault();
    }
}