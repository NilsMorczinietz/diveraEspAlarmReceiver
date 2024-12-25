#pragma once

namespace Config
{
    constexpr const char* WORKER_IPS[] = {"192.168.178.105", "192.168.178.106", "192.168.178.107"};
    constexpr const unsigned int UDP_PORT = 4210;
    constexpr const unsigned int ALARM_COOLDOWN_DURATION = 30; //In minutes
    constexpr const unsigned int ALARM_ON_DURATION = 2; //In minutes
}