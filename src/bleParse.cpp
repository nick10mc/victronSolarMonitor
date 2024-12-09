/*
C++ file for BLE Advertisement data packet parsing
Nicholas McCatherine, October 13th, 2024
Ohio State University, School of Earth Sciences, Undergraduate Research

This parser places the Advertiser data bytes into a map whereby the key, defined
by the preceding variable name, provides access to the data.
*/

#include "bleParse.h"
#include <typeinfo> // for typeid
#include "AES.h"

// Constructor - Initialize the object
parsePacket::parsePacket(const byteVector& buffer) {
    parse(buffer);
}

void parsePacket::parse(const byteVector& buffer) {
    // check to make sure that "buffer" actually contains data before we access it
    if (buffer.size() < 20) {
        std::cout << "\033[31mWarning: Input packet for parsePacket class is too small! Size = \033[0m" << buffer.size() << "\033[31m - See bleParse.cpp \033[0m" << std::endl;
        return;
    }

    // Parse the header
    ui8t j = 0; // Offset for Manufacturer's Name code, 0x02e1
    ui8t PacketPrefix = buffer[j]; // MFG data Code, 0x10 for Victron
    ui16t model = byteSwaparoo(buffer[j+2],buffer[j+3]); // Model Code
    ui8t unknown = buffer[j+3]; // Unknown data, no documentation
    ui8t recordType = buffer[j+4]; // Record Type, 0x01 is solar MPPT charger
    ui16t InitVector = (static_cast<ui16t>(buffer[j+5]) << 8) | static_cast<ui16t>(buffer[j+6]); // This is the Initialization Vector for decryption
    ui8t first_Byte_of_key = buffer[j+7];
    
    // Assign the encrypted data to it's own variable
    byteVector encryptedData;
    encryptedData.assign(buffer.begin() + j+8, buffer.end());

    // Create our map to hold the data
    parsedData["Packet Prefix"] = PacketPrefix;
    parsedData["Model"] = model;
    parsedData["Unknown"] = unknown;
    parsedData["Record Type"] = recordType;
    parsedData["Init Vector"] = InitVector;
    parsedData["First Byte Of Key"] = first_Byte_of_key;
    parsedData["Encrypted Data"] = encryptedData;

}

void parsePacket::parseDecryptedData(const ParsedValue& decryptedBuffer) {
    const auto& buffer = std::get<std::vector<unsigned char>>(decryptedBuffer);

    ui8t i = 0;
    ui8t DeviceState = buffer[i];
    ui8t chargerError = buffer[i+1];
    ui16t batteryVoltage = byteSwaparoo(buffer[i+2],buffer[i+3]);
    ui16t batteryCurrent = byteSwaparoo(buffer[i+4],buffer[i+5]);
    ui16t yieldToday = byteSwaparoo(buffer[i+6],buffer[i+7]);
    ui16t PVpower = byteSwaparoo(buffer[i+8],buffer[i+9]);
    ui8t loadCurrentMSbit = buffer[i+11];
    loadCurrentMSbit &= 0x01; // Bitmask-clear all but the LSbit of loadCurrent MSByte
    ui16t loadCurrent = byteSwaparoo(buffer[i+10],loadCurrentMSbit);

    // Edit our map to hold the decrypted data
    parsedData["Device State"] = DeviceState;
    parsedData["Charger Error"] = chargerError;
    parsedData["Battery Voltage"] = batteryVoltage;
    parsedData["Battery Current"] = batteryCurrent;
    parsedData["Yield Today"] = yieldToday;
    parsedData["PV Power"] = PVpower;
    parsedData["Load Current"] = loadCurrent;
}

// Overload operator[] to access the data
ParsedValue parsePacket::operator[](const str& key) const
{
    auto data = parsedData.find(key); // Directly use the std::map's find method
    if (data != parsedData.end()) {
        return data->second; // Return the corresponding value if found
    }
    std::ostringstream ErrorMsg;
    ErrorMsg << "\033[1;31mKey not found in parsed data: \033[0m'" << key << "'\033[1;31m of type \033[0m" << typeid(key).name() << "\033[1;31m - in bleParse.cpp - parsePacket::operator[]. \033[0m";
    throw std::out_of_range(ErrorMsg.str());
}







void parsePacket::printData() {
    //Print the encrypted data
    if (auto encryptedData = std::get_if<byteVector>(&parsedData["Encrypted Data"])) {
        std::cout << "\033[37;1mEncrypted Data: \033[0m";
        for (unsigned char byte : *encryptedData) {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)byte << " ";
        }
        std::cout << std::dec << std::endl;
    } else {
        std::cout << "\033[1;31mEncrypted Data is not of type byteVector\033[0m" << std::endl;
    }

    //Print out the parsedData map for debugging purposes
    std::cout << "\033[1;35mParsed Decrypted Data:\033[1;37m" << std::endl;
    for (const auto& pair : parsedData) {
        std::cout << "    " << pair.first << ": ";
        std::visit([](const auto& value) {
            if constexpr (std::is_same_v<std::decay_t<decltype(value)>, ui8t>) {
                std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)value << std::dec;
            } else if constexpr (std::is_same_v<std::decay_t<decltype(value)>, ui16t>) {
                std::cout << std::hex << std::setw(4) << std::setfill('0') << value << std::dec;
            } else if constexpr (std::is_same_v<std::decay_t<decltype(value)>, byteVector>) {
                std::cout << "Vector of size " << value.size();
            } else {
                std::cout << value;
            }
        }, pair.second);
        std::cout << "\033[0m" << std::endl;
    }
}