#pragma once
#include <any>
#include <memory>
#include <mqtt/async_client.h>

#include "rfid_listener.h"

class RFIDController: public RFIDListener
{
public:
    RFIDController(const std::string& address, const std::string& clientID, const std::string& topic);
    
    void Connect();
    void Disconnect();
    void OnNewEvent(Event ev, std::any payload) const override;
private:
    void SendRFID(std::any payload) const;
    mqtt::connect_options LoadConnectionOptions();

    struct MqttDeleter {
        void operator()(mqtt::async_client* obj) const {
            if(obj) {
                if(obj->is_connected())
                    obj->disconnect()->wait();
                
                delete obj;
            }
        }
    };    
    std::unique_ptr<mqtt::async_client, MqttDeleter> client;

    const std::string address;
    const std::string clientID;
    const std::string topic;
};
