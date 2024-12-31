#ifndef ALARM_SYSTEM_H
#define ALARM_SYSTEM_H

#include <Arduino.h>
#include "config.h"
#include "udpHandler.h"
#include <ArduinoJson.h>

enum class AlarmType
{
    probe,
    alarm,
    alarm_red,
    alarm_green,
    alarm_blue,
    alarm_white,
    standard,
    error
};

struct KeywordAction
{
    String keyword;
    AlarmType alarmType;
};

enum class Status
{
    active,
    processing,
    alarmOn,
    probeOn,
    waiting,
    error,
    off
};

class AlarmSystem
{
private:
    unsigned long lastTimestamp = 0;
    Status status = Status::off;

    void alarm(AlarmType alarmType = AlarmType::alarm, String blinkColor = "");
    void probe(String blinkColor = "");
    void error();
    AlarmType processMessage(const String &message);

public:
    AlarmSystem();

    void handleNewMessage(const String &message);
    void checkAlarm();
};

#endif // ALARM_SYSTEM_H