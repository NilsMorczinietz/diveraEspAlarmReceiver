#pragma once

namespace Config
{
    constexpr const char* WORKER_IPS[] = {"192.168.178.105", "192.168.178.158", "192.168.178.35"};
    constexpr const unsigned int UDP_PORT = 4210;
    constexpr const unsigned int ALARM_COOLDOWN_DURATION = 20; //In minutes
    constexpr const unsigned int ALARM_ON_DURATION = 2; //In minutes
}