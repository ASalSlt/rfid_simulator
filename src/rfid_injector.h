#pragma once
#include <memory>
#include <random>
#include <chrono>
#include <thread>
#include <string>
#include "rfid_listener.h"
class RfidInjector
{
    public:
    RfidInjector(const std::shared_ptr<RFIDListener>& controller): controller(controller){}

    template <typename DurationType = std::chrono::seconds>
    void Start(int interval = 10)
    {
        while(true) {
            std::string tag = GenerateTag<tagLength>(tagPrefix);
            auto now = std::chrono::system_clock::now();
            std::time_t timestamp = std::chrono::system_clock::to_time_t(now);

            std::string payload = "{\"tag\":\"" + tag + "\", \"timestamp\":" + std::to_string(timestamp) + "}";
            controller->OnNewEvent(RFIDListener::Event::RFID, payload);
            std::this_thread::sleep_for(DurationType(interval));
        }
    }

    private:
    template<size_t LENGTH>
    std::string GenerateTag(std::string tag) {
        static const char hex[] = "0123456789ABCDEF";
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 15);
        for(int i = 0; i < LENGTH; ++i) tag += hex[dis(gen)];
        return tag;
    }

    void LoadInjectorParams(){/*TODO*/};

    std::shared_ptr<RFIDListener> controller;
    static auto constexpr tagPrefix{"E2"};
    static auto constexpr tagLength{15}; 
    static auto constexpr waitTime{200};
};