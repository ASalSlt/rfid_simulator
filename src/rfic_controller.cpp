#include "rfid_controller.h"
#include <iostream>


RFIDController::RFIDController(const std::string& address, const std::string& clientID, const std::string& topic):
    client(new mqtt::async_client(address, clientID), MqttDeleter()),
    address(address), clientID(clientID), topic(topic) {}

void RFIDController::Connect()
{
    const auto connOpts = LoadConnectionOptions();
    client->connect(connOpts)->wait();
    std::cout << "Connected to MQTT broker!" << std::endl;
}

void RFIDController::Disconnect()
{
    client->disconnect()->wait();
    std::cout << "Disconnected to MQTT broker!" << std::endl;
}

void RFIDController::OnNewEvent(Event ev, std::any payload) const
{
    switch(ev)
    {
        case Event::RFID: SendRFID(payload); break;
    }
}

void RFIDController::SendRFID(std::any payload) const
{
    if(const std::string* rfidTag = std::any_cast<std::string>(&payload))
    {
        mqtt::message_ptr pubmsg = mqtt::make_message(topic, *rfidTag);
        pubmsg->set_qos(1);
        std::cout << "Trying to publish.." << std::endl << std::flush;
        client->publish(pubmsg);
        std::cout << "RFIDController::Published: " << *rfidTag << std::endl;
    }
    else
    {
        std::cerr << "Internal Error: payload does not a string!" << std::endl;
    }
}
mqtt::connect_options RFIDController::LoadConnectionOptions()
{
    mqtt::connect_options connOpts;
    connOpts.set_clean_session(true);
    connOpts.set_keep_alive_interval(5);  
    return connOpts;
}