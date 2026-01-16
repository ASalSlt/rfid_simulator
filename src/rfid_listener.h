#pragma once
#include <any>
class RFIDListener
{
    public:
    enum class Event
    {
        RFID
    };

    virtual void OnNewEvent(Event ev, std::any payload) const = 0;
};