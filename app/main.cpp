
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <rfid_injector.h>
#include <rfid_controller.h>
int main(int argc, char* argv[]) {

    std::vector<std::string> args(argv, argv + argc);
    
    std::string address = "tcp://127.0.0.1:1883";
    std::string clientId = "rfid_simulator";
    std::string topic = "rfid/events";
    
    for(size_t i = 1; i < args.size(); ++i) {
        if(args[i] == "--address" && i + 1 < args.size()) {
            address = args[++i];
        } else if(args[i] == "--client-id" && i + 1 < args.size()) {
            clientId = args[++i];
        } else if(args[i] == "--topic" && i + 1 < args.size()) {
            topic = args[++i];
        } else if(args[i] == "--help") {
            std::cout << "Usage: rfid_simulator [options]\n"
                      << "  --address <url>      MQTT broker address\n"
                      << "  --client-id <id>     MQTT client ID\n"
                      << "  --topic <topic>      MQTT topic\n";
            return 0;
        }
    }

    try {
        using Controller = std::shared_ptr<RFIDController>;
        Controller controller = std::make_shared<RFIDController>(address, clientId, topic);
        auto injector = std::make_shared<RfidInjector>(controller);

        std::cout << "Connecting to " << address << " ..." << std::endl;
        controller->Connect();
        std::cout << "Starting injector ..." << std::endl;
        injector->Start<std::chrono::seconds>(10);

    } catch(const mqtt::exception& e) {
        std::cerr << "MQTT Error: " << e.what() << std::endl;
        return 1;
    } catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
