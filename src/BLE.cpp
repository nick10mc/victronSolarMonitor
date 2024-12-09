/*
BLE functions for SolarRecorder background program.
By Nicholas McCatherine, September 22nd, 2024
Ohio State University, School of Earth Sciences, Undergraduate Research

Note - mfgRegisterDat currently bugged, current workaround is to print the raw data
and process it in MATLAB later
Lines relevant are commented out and labeled with // Bugged
*/

#include "BLE.h"
#include "mfgRegisterDat.h"
#include <cerrno>
#include <csignal>


// Read the Bluetooth Advertising Data...
// Initialize the adapter & socket
BLEscanner::BLEscanner(const AESdecrypt& decryptor, const str mac, const str& outputFile, const std::chrono::days& fileInterval) 
    : decryptor_(decryptor), devMAC(mac), output_file(outputFile), new_file_interval(fileInterval) 
{
    // First, restart the bluetooth system service (to fix the blocking problem 9/30/2024)
    restartBLEservice();
    std::cout << "Using SimpleBLE version: " << SimpleBLE::get_simpleble_version() << std::endl;

    // Next, get the adapters via SimpleBLE - 10/5/2024
    std::vector<SimpleBLE::Adapter> adapters = SimpleBLE::Adapter::get_adapters();
    // Now, check if there actually is an adapter
    if (adapters.empty()) {
        std::cerr << "\033[1;31mNo BLE Adapter Found!\033[0m" << std::endl;
        return;
    }
    // Get the first adapter from the adapters vector -- is equivalent to device_id
    adapter = adapters[0];

    // Initialize our data tables for parsing
    dataTables victronDataTables;
}


// Helper function to restart the bluetooth service
void restartBLEservice() {
    // Restart the service
    int restart = std::system("sudo systemctl restart bluetooth");
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Sleep to allow for restart - fix hci_get_route "No Such Device!" 9/30/2024
    
    // Error checking: 
    if (restart == -1) {
        std::cerr << "\033[1;31mError: Failed to restart BLE service!\033[0m" << std::endl;
    } else {
        std::cout << "\033[1;34mBLE service restarted.\033[0m" << std::endl;
    }
}

// signalHandler - handle interrupts that order a stop to the service - and stop scanning through BlueZ to fix the BlueZ Blocking problem 9/30/2024
// Might not be as useful with SimpleBLE being used 10/5/2024
bool scanning = true;
void signalHandler(int sig) {
    scanning = false;
    // Stop the scan
    int device_id = hci_get_route(NULL);
    int socket = hci_open_dev(device_id);

    if (socket < 0) {
        perror("\033[1;31mError opening the Bluetooth Socket! - In signalHandler\033[0m");
    }

    // scan STOP
    if ((hci_le_set_scan_enable(socket, 0x00, 1, 1000)) < 0) {
        perror("\033[1;31mError stopping the Bluetooth scan! Requires manual reset: sudo systemctl restart bluetooth\033[0m");
    }
    
    close(socket);
    exit(sig);
}

void BLEscanner::scan() 
{
    // Signal Handler: Whenever the following interrupt signals are received, execute signalHandler as a callback function:
    std::signal(SIGINT, signalHandler); // Signal - Interrupt - Ctrl+C
    std::signal(SIGTSTP, signalHandler); // Signal - Terminal Stop - Ctrl+Z
    std::signal(SIGQUIT, signalHandler); // Signal - Terminal Quit - Core Dump - Ctrl+"\"
    std::signal(SIGTERM, signalHandler); // Signal - Termination Signal
    std::signal(SIGHUP, signalHandler); // Signal - Terminal Signal Hangup - serial line dropped
    std::signal(SIGBUS, signalHandler); // Signal - Access to an Undefined portion of memory
    std::signal(SIGSEGV, signalHandler); // Signal - Invalid memory reference
    // KILL signals cannot be caught. STOP can not be caught.    

    // Convert devMAC - which is the unformated config input, into the correct formatted form
    ui8t mac[6];
    strMACtoINT(devMAC,mac);
    str formMAC = macToString(mac);
    std::cout << "\033[1;36mSearching for address: \033[0m" << formMAC << std::endl;

    // Now for the actual scanning method:
    adapter.scan_for(2000); // Scan for 2 seconds
    std::vector<SimpleBLE::Peripheral> results = adapter.scan_get_results();
    for (auto& device : results) {
        // Asign device variables
        auto deviceID = device.identifier();
        auto deviceAddr = device.address();
        auto deviceRSSI = device.rssi();

        std::cout << "\033[1;32mFound device: \033[0m" << deviceID;
        std::cout << "\033[1;32m. With Address of: \033[0m" << deviceAddr << std::endl;
        std::cout << "\033[1;32mRSSI: \033[0m" << deviceRSSI << std::endl;

        // Get advert data
        std::map<ui16t, SimpleBLE::ByteArray> adverts = device.manufacturer_data();

        // Make sure that the addresses are both of the same case in order to compare below
        std::transform(deviceAddr.begin(), deviceAddr.end(), deviceAddr.begin(), ::toupper);
        std::transform(formMAC.begin(), formMAC.end(), formMAC.begin(), ::toupper);

        // If the detected device has the desired MAC address, continue...
        if (deviceAddr != formMAC) {
            std::cout << "\033[1;31m    Address mismatch: \033[0m" << deviceAddr << "\033[1;31m of type \033[0m" << typeid(deviceAddr).name() << " != " << formMAC << "\033[1;31m of type \033[0m" << typeid(formMAC).name() << std::endl;
            continue;
        }

        // 
        for (const auto& [key,value] : adverts) {
            std::cout << "\033[37;1m    Mfg. ID:\033[0m 0x" << std::hex << std::setw(4) << std::setfill('0') << key << std::dec << std::endl;
            std::cout << "\033[37;1m    Data: \033[0m";

            //Print each value
            for (unsigned char byte : value) {
                std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte << " ";
            }
            std::cout << std::dec << std::endl;
        }

        // first, convert advert to byteVector
        byteVector advertBytes;  // Define a byteVector
        for (const auto& [key, value] : adverts) {
            advertBytes.insert(advertBytes.end(), value.begin(), value.end());  // Concatenate values to advertBytes
        }
        
        parsePacket packet(advertBytes);
        auto packetPrefix = std::get<ui8t>(packet["Packet Prefix"]);
        ui8t desiredPrefix = 0x10;
        if (packetPrefix == 0) {
            std::cerr << "\033[31;1mPacket Prefix is uninitialized or incorrect! - in BLEscanner::scan() \033[0m" << std::endl;
        } else if (packetPrefix != desiredPrefix) {
            std::cerr << "\033[31;1mPacket Prefix is not a Victron product, \033[0m0x10\033[1;31m. Prefix entered is of type \033[0m'" << typeid(packetPrefix).name();
            std::cerr << "'\033[31;1m and the data in it is \033[0m0x" << std::hex << std::setw(2) << std::setfill('0') << packetPrefix << std::dec;
            std::cerr << "\033[1;31m - in BLEscanner::scan()\033[0m" << std::endl;
            return;
        }



        // Extract the encrypted values
        auto encryptedText = std::get<byteVector>(packet["Encrypted Data"]);
        // Extract the Initialization Vector (IV)
        ui16t iv = std::get<ui16t>(packet["Init Vector"]);
        byteVector iv_ = ui16t_2_byteVector(iv);

        // Decrypt the packet
        byteVector decryptedValues = decryptor_.decrypt(encryptedText,iv_); // Encrypted text is always a byteVector
        packet.parseDecryptedData(decryptedValues);

        packet.printData();

        std::cout << "\033[37;1m " << std::endl;
        std::cout << "Extracted Header Data: " << std::endl;

        str mtype_ = "Model";
        ui16t model_ = std::get<ui16t>(packet[mtype_]);
        //str modelName = victronDataTables.lookup("model",model_); //Bugged, Comment out until it can be fixed
        auto modelName = model_;
        std::cout << "    Model Name: " << modelName << std::endl;

        str rtype_ = "Record Type";
        ui8t record_ = std::get<ui8t>(packet[rtype_]);
        //str recordType = victronDataTables.lookup("record",record_); //Bugged, Comment out until it can be fixed
        auto recordType = record_;
        std::cout << "    Record Type: " << recordType << std::endl;

        // Organize the data into individual values for writing to CSV
        ui8t state_ = std::get<ui8t>(packet["Device State"]);
        //auto DeviceState = victronDataTables.lookup("status",state_); //Bugged, Comment out until it can be fixed
        auto DeviceState = state_;
        ui8t error_ = std::get<ui8t>(packet["Charger Error"]);
        //auto ChargerError = victronDataTables.lookup("error",error_); //Bugged, Comment out until it can be fixed
        auto ChargerError = error_;
        float batVoltage = static_cast<int16_t>(std::get<ui16t>(packet["Battery Voltage"])) * 0.01;
        float batCurrent = static_cast<int16_t>(std::get<ui16t>(packet["Battery Current"])) * 0.1;
        float yieldToday = std::get<ui16t>(packet["Yield Today"]) * 0.01;
        float PVpower = std::get<ui16t>(packet["PV Power"]);
        float loadCurrent = std::get<ui16t>(packet["Load Current"]) * 0.1;
        
        // Print victronDataTables content for debugging
        //victronDataTables.printModelTable();

        // Assign data
        std::vector<dataVariant> data;
        data.push_back(DeviceState);
        data.push_back(ChargerError);
        data.push_back(batVoltage);
        data.push_back(batCurrent);
        data.push_back(yieldToday);
        data.push_back(PVpower);
        data.push_back(loadCurrent);

        // Now, print to the CSV file
        CSV csvWriter;
        csvWriter.write(data,output_file,new_file_interval);
    }
    
}

///////// Packet format, see https://docs.silabs.com/bluetooth/2.13/bluetooth-fundamentals-advertising-scanning/bluetooth-adv-data-basics
// https://docs.silabs.com/bluetooth/2.13/bluetooth-code-examples-stack-features-adv-and-scanning/adv-manufacturer-specific-data
// 