#include "AlarmSystem.h"

static KeywordAction keywordActions[] = {
    {"Test", AlarmType::alarm_white},
    {"Probe", AlarmType::alarm_white},
    {"P01", AlarmType::alarm_white}, // Probealarm
    {"Probealarm", AlarmType::alarm_white},
    {"Infoalarm", AlarmType::alarm_white},

    {"Arbeit", AlarmType::alarm_green},
    {"H20", AlarmType::alarm_green}, // Kraftstoff
    {"Kraftstoff", AlarmType::alarm_green},

    {"F04", AlarmType::alarm_blue}, // BMA
    {"BMA", AlarmType::alarm_blue},

    {"F10", AlarmType::alarm_red}, // KZW
    {"KZW", AlarmType::alarm_red},
    {"F27", AlarmType::alarm_red}, // Dach
    {"Dach", AlarmType::alarm_red},
    {"AM03", AlarmType::alarm_red}, // Nachalarmierung Vollalarm
    {"Vollalarm", AlarmType::alarm_red},
    {"Stadtalarm", AlarmType::alarm_red},
};

AlarmSystem::AlarmSystem()
{
    status = Status::active;
    lastTimestamp = 0;
}

void AlarmSystem::alarm(AlarmType alarmType, String blinkColor )
{
    if (status != Status::processing)
    {
        return;
    }
    lastTimestamp = millis();

    status = Status::alarmOn;

    if (blinkColor == "")
    {
        sendUDPMessageToAll("on");
    }
    else
    {
        sendUDPMessageToAll("blink:" + blinkColor);
    }
    Serial.println("Action: Alarm:  Alarm is on");
}

void AlarmSystem::probe(String blinkColor)
{
    Serial.println("PROBE");

    if (status != Status::processing)
    {
        return;
    }
    lastTimestamp = millis();

    Serial.println("PROBE2");

    status = Status::probeOn;

    if (blinkColor == "")
    {
        sendUDPMessageToAll("on");
    }
    else
    {
        sendUDPMessageToAll("blink:" + blinkColor);
    }
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
    case AlarmType::alarm_red:
        alarm(alarmTypeResult, "red");
        status = Status::alarmOn;
        break;
    case AlarmType::alarm_green:
        alarm(alarmTypeResult, "green");
        status = Status::alarmOn;
        break;
    case AlarmType::alarm_blue:
        alarm(alarmTypeResult, "blue");
        status = Status::alarmOn;
        break;
    case AlarmType::alarm_white:
        alarm(alarmTypeResult, "white");
        status = Status::alarmOn;
        break;
    case AlarmType::alarm:
        alarm(alarmTypeResult);
        status = Status::alarmOn;
        break;
    case AlarmType::probe:
        probe("white");
        // probe();
        status = Status::probeOn;
        break;
    case AlarmType::standard:
        alarm(alarmTypeResult, "red");
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
    static unsigned long lastActiveTimestamp = 0;

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
        long waitingTime = Config::PROBE_ON_DURATION * 60 * 1000;
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
    else if (status == Status::active)
    {
        long interval = Config::UDP_TIMEOUT * 60 * 1000;
        if (millis() - lastActiveTimestamp >= interval)
        {
            sendUDPMessageToAll("off");
            lastActiveTimestamp = millis();
        }
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
    const char *notificationText = doc["text"];
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