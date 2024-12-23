#ifndef STATUS_MANAGER_H
#define STATUS_MANAGER_H

#include "Arduino.h"

// Enum for the system status
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

class StatusManager
{
public:
    /**
     * @brief Set the current system status.
     *
     * @param newStatus The new status to set.
     */
    static void setStatus(Status newStatus)
    {
        status = newStatus;
    }

    /**
     * @brief Get the current system status.
     *
     * @return The current status.
     */
    static Status getStatus()
    {
        return status;
    }

    /**
     * @brief Convert the current status to a human-readable string.
     *
     * @return The status as a string.
     */
    static const char *statusToString()
    {
        switch (status)
        {
        case Status::active:
            return "active";
        case Status::processing:
            return "processing";
        case Status::alarmOn:
            return "alarmOn";
        case Status::probeOn:
            return "probeOn";
        case Status::waiting:
            return "waiting";
        case Status::error:
            return "error";
        case Status::off:
            return "off";
        default:
            return "unknown";
        }
    }

private:
    static Status status;
};

// Initialize the static member variable
Status StatusManager::status = Status::off;

#endif // STATUS_MANAGER_H