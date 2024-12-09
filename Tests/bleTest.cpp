#include <simpleble/SimpleBLE.h>
#include <iostream>
#include <iomanip>

int main() {
    std::vector<SimpleBLE::Adapter> adapters = SimpleBLE::Adapter::get_adapters();

    //get the first adapter
    SimpleBLE::Adapter adapter = adapters[0];
    std::cout << "Using SimpleBLE version: " << SimpleBLE::get_simpleble_version() << std::endl;
    std::cout << "Bluetooth enabled: " << SimpleBLE::Adapter::bluetooth_enabled() << std::endl;

    if (adapters.empty()) {
        std::cout << "No adapter found" << std::endl;
        return EXIT_FAILURE;
    }

    while (true) {
        adapter.scan_for(1000);  // Scan for 5 seconds
        std::vector<SimpleBLE::Peripheral> results = adapter.scan_get_results();


        for (auto device : results) {
            std::cout << "Found device: " << device.identifier() << " (" << device.address() << ")" << std::endl;
            
            // Get Mfg. Data
            std::map<uint16_t, SimpleBLE::ByteArray> manufacturer_data = device.manufacturer_data();
            if (!manufacturer_data.empty()) {
                std::cout << "Mfg. Data: " << std::endl;

                for (const auto& [key,value] : manufacturer_data) {
                    std::cout << "    Mfg. ID: 0x" << std::hex << std::setw(4) << std::setfill('0') << key << std::dec << std::endl;
                    std::cout << "    Data: ";

                    //Print each value
                    for (unsigned char byte : value) {
                        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte << " ";
                    }
                    std::cout << std::dec << std::endl;
                }
            } else {
                std::cout << " No Mfg. data available." << std::endl;
            }
            
        }
    }


}