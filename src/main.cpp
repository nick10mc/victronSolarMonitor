/*
Solar Recorder background service for Victron MPPT Solar Chargers
Tested on a Raspberry Pi 4 with a Smartsolar 75/15

By Nicholas McCatherine, School of Earth Sciences, The Ohio State University
September 14th, 2024

This software decrypts the Bluetooth advertising data transmitted from the smart MPPT
controllers and transfers that data into a .csv which can then be read from the GUI or
exported into Excel.
*/

// Now, my includes:
#include "mfgRegisterDat.h"
#include "datFunc.h"
#include "BLE.h"
#include "AES.h"


// Library Includes
#include "common.h"


// Decrypt the ciphertext
/*
"key" is the AES-128-CTR cryptographic key, "iv" is the 'Initialization Vector' - acts as the nonce in AES-CTR, 
"cipher" is the encrypted input text, "cipLength" is the ciphertext length, "text" is the plaintext output,
textLength is the length of the plain text output - which should be the same length as the 
ciphertext.

OpenSSL/libcrypto functions will return a 1 or 0 indicating success or failure. All error codes should be checked
*/




// Main function
int main() {
    str devMAC;
    byteVector key;
    byteVector iv;
    str output_file;
    str intervalStr;
    std::chrono::days new_file_interval;
    ui8t scanInterval;

    // Read the config file at startup
    mapString config = readConfig("config.txt");

    // AES key and IV goes here
    key = hexStr2Bytes(config["key"]); // AES 128bit key
    scanInterval = static_cast<ui8t>(std::stoi(config["scan_interval"])); // Must be an integer between 1 and 255 seconds.
    devMAC = config["device_mac"]; // The MAC address of our desired device
    output_file = config["output_file"]; // The output file
    intervalStr = config["new_file_interval"];
    new_file_interval = std::chrono::days(std::stoi(intervalStr));


    try {
        //Create the AES decryptor
        AESdecrypt decryptor(key);
        std::cout << "\033[1;4;32mAES decryptor created.\033[0m" << std::endl;

        while (true) 
        {
            //Create the BLE scanner
            BLEscanner scanner(decryptor,devMAC,output_file,new_file_interval);
            std::cout << "\033[1;32mBLE scanner object created.\033[0m" << std::endl;

            //Start scanning and decryption...
            std::cout << "\033[1;33mStarting BLE scan... \033[0m" << std::endl;
            scanner.scan();

            //Add a delay
            std::cout << "\033[1;37mStarting next scan in: \033[33;4m" << std::to_string(scanInterval) << "\033[1;37m seconds...\033[0m" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(scanInterval));
        }
        

    } catch (const RuntimeError& e) {
        std::cerr << "\033[1;31mError: \033[4m" << e.what() << "\033[0m" << std::endl;
        restartBLEservice(); // Restart the BLE service to release BLE control on runtime errors
        return 1;
    }

    return 0;
}