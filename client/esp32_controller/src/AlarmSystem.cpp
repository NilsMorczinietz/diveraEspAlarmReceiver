#include "AlarmSystem.h"

static KeywordAction keywordActions[] = {
    {"Test", AlarmType::probe},
    {"Probe", AlarmType::probe},
    {"P01", AlarmType::probe},
    {"Probealarm", AlarmType::probe},
    {"Infoalarm", AlarmType::probe},

    {"Arbeit", AlarmType::alarm},
    {"H20", AlarmType::alarm},
    {"Kraftstoff", AlarmType::alarm},

    {"F04", AlarmType::alarm},
    {"BMA", AlarmType::alarm},

    {"F10", AlarmType::alarm},
    {"KZW", AlarmType::alarm},
    {"AM03", AlarmType::alarm},
    {"Vollalarm", AlarmType::alarm},
    {"Stadtalarm", AlarmType::alarm},
};

AlarmSystem::AlarmSystem()
{
    status = Status::active;
    lastTimestamp = 0;
}

void AlarmSystem::alarm(AlarmType alarmType)
{
    if (status != Status::processing)
    {
        return;
    }
    lastTimestamp = millis();

    status = Status::alarmOn;

    sendUDPMessageToAll("on");
    Serial.println("Action: Alarm:  Alarm is on");
}

void AlarmSystem::probe()
{
    Serial.println("PROBE");

    if (status != Status::processing)
    {
        return;
    }
    lastTimestamp = millis();

    Serial.println("PROBE2");

    status = Status::probeOn;

    sendUDPMessageToAll("on");
    Serial.println("Action: Probealarm");
}

void AlarmSystem::error()
{
    status = Status::active;
}

void AlarmSystem::handleNewMessage(const String &message)
{
    if (status != Status::active && status != Status::probeOn)
    {
        return;
    }

    status = Status::processing;

    AlarmType alarmTypeResult = processMessage(message);

    switch (alarmTypeResult)
    {
    case AlarmType::alarm:
        Serial.println("1");
        alarm(alarmTypeResult);
        status = Status::alarmOn;
        break;
    case AlarmType::probe:
        Serial.println("2");
        probe();
        status = Status::probeOn;
        break;
    case AlarmType::standard:
        Serial.println("3");
        alarm();
        break;
    case AlarmType::error:
        error();
        break;
    default:
        Serial.println("No valid action for the message.");
        break;
    }
}

void AlarmSystem::checkAlarm()
{
    if (status == Status::alarmOn)
    {
        long waitingTime = Config::ALARM_ON_DURATION * 60 * 1000;
        if (millis() - lastTimestamp < waitingTime)
        {
            return;
        }

        sendUDPMessageToAll("off");
        status = Status::waiting;
    }
    else if (status == Status::probeOn)
    {
        long waitingTime = Config::ALARM_ON_DURATION * 60 * 1000;
        if (millis() - lastTimestamp < waitingTime)
        {
            return;
        }

        sendUDPMessageToAll("off");
        status = Status::active;
    }
    else if (status == Status::waiting)
    {
        long waitingTime = Config::ALARM_COOLDOWN_DURATION * 60 * 1000;
        if (millis() - lastTimestamp < waitingTime)
        {
            return;
        }

        Serial.println("Action: Alarm: Alarm is off");

        status = Status::active;
    }
    else
    {
        return;
    }
}

AlarmType AlarmSystem::processMessage(const String &message)
{
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, message);

    if (error)
    {
        Serial.println("JSON Parse Error: " + String(error.c_str()));
        return AlarmType::error;
    }

    // Extrahiere das 'text'-Feld
    const char* notificationText = doc["text"];
    if (notificationText == nullptr)
    {
        Serial.println("Error: 'text' field missing.");
        return AlarmType::error;
    }

    Serial.println("Extracted text: " + String(notificationText));

    const int numActions = sizeof(keywordActions) / sizeof(keywordActions[0]);
    for (int i = 0; i < numActions; i++)
    {
        if (String(notificationText).indexOf(keywordActions[i].keyword) >= 0)
        {
            Serial.println("Matched keyword: " + String(keywordActions[i].keyword));
            return keywordActions[i].alarmType;
        }
    }

    Serial.println("No keyword matched. Returning default AlarmType.");
    return AlarmType::standard;
}