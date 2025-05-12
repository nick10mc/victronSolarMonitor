/*
General data manipulation functions for SolarRecorder background program.
By Nicholas McCatherine, September 22nd, 2024
Ohio State University, School of Earth Sciences, Undergraduate Research
*/

#include "datFunc.h"
#include <algorithm> // specific for readConfig - needed for std::remove_if
#include "bleParse.h"
#include <filesystem>
#include <bit>
#include <chrono>
#include <optional>
#include <ctime>


//Function to extract bits... used for our CSV as Victron uses fractional bytes to describe certain information.
unsigned int extractBits(const byteVector& data, size_t startBit, size_t numBits) {
    size_t byteIndex = startBit / 8;
    size_t bitOffset = startBit % 8;
    unsigned int value = 0;

    while (numBits > 0)
    {
        size_t bitsLeft = std::min(numBits, 8-bitOffset);
        unsigned char byte = data[byteIndex];
        unsigned char mask = ((1 << bitsLeft)-1) << bitOffset;
        unsigned int bits = (byte & mask) >> bitOffset;

        value |= bits << (numBits - bitsLeft);
        numBits -= bitsLeft;
        bitOffset += bitsLeft;

        if (bitOffset >= 8) 
        {
            bitOffset = 0;
            ++byteIndex;
        }
    }
    
    return value;
}



// Function to create and swap endianess of a 16 bit integer, assign them to new variable.
ui16t byteSwaparoo(const ui8t byte1, const ui8t byte2)
{
    ui16t Swapped = __builtin_bswap16(static_cast<ui16t>((byte1 << 8) | byte2));
    return Swapped;
}




// Function to convert ui16t Init Vectors to byteVectors
byteVector ui16t_2_byteVector(ui16t value) {
    byteVector vector;
    vector.push_back(static_cast<ui8t>((value >> 8) & 0xFF));
    vector.push_back(static_cast<ui8t>(value & 0xFF));
    return vector;
}




// Function to trim whitespace and quotes for readConfig
str trim(const str& s) {
    auto start = s.find_first_not_of(" \t\r\n\"");
    auto end = s.find_last_not_of(" \t\r\n\"");
    return (start == str::npos) ? "" : s.substr(start, end - start+1);
}



// Helper function for readConfig, throws an error if the config has the wrong key length
void validateHexString(const str& key, const str& value) {
    if (key == "key" && value.length() != 32) {
        throw RuntimeError("\033[31mInvalid length for key: expected 32 characters.\033[0m");
    }
}


// Function to read config file
/*
config.txt expected to be formatted like so:
device_mac=d19a0beb9982
key=3b3baa4986a89b47e4b709ab117a4a2d
scan_interval=5
output_file="output.csv"
new_file_interval=14
*/
mapString readConfig(const str& filename) {
    ifStream file(filename);
    mapString config;

    if (!file.is_open()) {
        throw RuntimeError("\033[1;31mFailed to open config file!\033[0m");
    }

    str line;
    while (std::getline(file,line))
    {
        std::istringstream iss(line);
        str key, value; // The line is expected to be in the format of "key=value;"
        if (std::getline(iss, key, '=') && std::getline(iss, value))
        {
            key = trim(key);
            value = trim(value);
            // This should get rid of the whitespaces and fix our parser errors

            if (key == "output_file") {
                if (value.size() < 4 || value.substr(value.size()-1) == "/") {
                    // DDG: the logic was adding .csv twice
                    // value += ".csv";
                    value += "logfile";
                }
            }
            if (!key.empty() && !value.empty())
            {
                config[key] = value;
            }
            
        }

        // Print parsed configuration data
        for (const auto& [key, value] : config) {
            std::cout << key << " = " << value << std::endl;
        }
    }

    return config;
}




// Function to Convert MAC address to string.
str macToString(const ui8t* mac) {
    char macStr[18];
    std::sprintf(macStr, "%02x:%02x:%02x:%02x:%02x:%02x", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
    return str(macStr);
}

// Function to convert config device_mac string into a ui8t array
void strMACtoINT(const str& macStr, ui8t* mac) {
    if (macStr.length() != 12) //Checks if the string is valid
    {
        throw std::invalid_argument("\033[1;31mInvalid MAC String!\033[0m");
    }

    // Convert each character pair to a byte
    for (size_t i=0; i<6; i++) 
    {
        str byteStr = macStr.substr((5-i)*2,2);
        mac[i] = static_cast<ui8t>(std::stoi(byteStr, nullptr,16)); // convert String to Integer - stoi
    }

}


// Function to convert hexadecimal string to bytes
byteVector hexStr2Bytes(const str& hexStr) {
    byteVector bytes;
    str byteString;

    // DEBUGGING - Check if the hex string has an even length - IE: must be divisible by 2
    if (hexStr.length() % 2 != 0) {
        std::cerr << "Hex string has an odd length: " << hexStr << std::endl;
        return bytes; // Early exit or handle the error as needed
    } else {
        std::cout << "Parsing key value from config.txt..." << std::endl;
    }

    for (size_t i = 0; i < hexStr.length(); i += 2)
    {
        byteString = hexStr.substr(i,2);

        // DEBUGGING - Check for valid byteString
        if (byteString.length() != 2) {
            std::cerr << "\033[1;31mInvalid byte string length: \033[0m" << byteString << std::endl;
            continue;
        }

        std::cout << "Parsing value: " << byteString << std::endl; // Aids in debugging

        try {             
            unsigned char byte = static_cast<unsigned char>(std::stoi(byteString, nullptr, 16));
            bytes.push_back(byte);
        } catch (const std::invalid_argument& e) {
            std::cerr << "\033[1;31mInvalid argument for stoi: \033[0m" << byteString << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "\033[1;31mValue out of range for stoi: \033[0m" << byteString << std::endl;
        }
        
    }

    return bytes;
}


auto getStartOfDay(std::chrono::system_clock::time_point tp) {
    std::time_t now_t = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::localtime(&now_t);
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

sysClk::time_point CSV::lastTimeWritten = sysClk::now();

// Write plaintext data to a .csv file to be imported into the GUI or excel..
void CSV::write(std::vector<dataVariant>& data, const str& filename, const std::chrono::days& interval)
{
    // Check if the time interval between separate files has passed, and update
    static std::optional<sysClk::time_point> lastTimeWritten;
    auto now = sysClk::now();
    static str currentName;

    if (!lastTimeWritten || (getStartOfDay(now) - getStartOfDay(*lastTimeWritten) >= interval)) {
        currentName = generateFileName(filename);
        lastTimeWritten = now;
    }

    // Make sure that the directory exists...
    std::filesystem::path filePath(currentName);
    std::filesystem::path dir = filePath.parent_path();
    if (!dir.empty() && !std::filesystem::exists(dir)) {
        std::filesystem::create_directories(dir); // Create the directory
    }

    // Open the output file
    std::ofstream file(currentName, std::ios::out | std::ios::app);
    std::cout << "Attempting to open file: '" << currentName << "'" << std::endl;
    if (!file.is_open())
    {
        std::cerr << "\033[31;1mFailed to open CSV file \033[0m" << currentName 
                  << ": " << std::strerror(errno) << std::endl;
        return;
    }

    // Get the current time and date
    auto now_time = sysClk::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time);
    //Formatting the timestamp
    char timestamp[20];
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", now_tm);
    // Write the timestamp to the csv
    file << timestamp << ",";

     // Write each data field with logging
    for (const auto& field : data) {
        std::visit([&file](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::vector<unsigned char>>) {
                // Convert byte vector to hex string for CSV output
                std::ostringstream hexString;
                for (unsigned char byte : arg) {
                    hexString << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
                }
                file << hexString.str();
                std::cout << "Writing data field (vector<unsigned char>) with hex value: " << hexString.str() << std::endl;
            } else {
                file << arg;
                std::cout << "Writing data field type: " << typeid(T).name() << " with value: " << arg << std::endl;
            }
        }, field);
        file << ",";
    }
    file << std::endl;
}




// Helper function for CSV writing method, generates a new file name using the starting date
std::string generateFileName(const str& name) {
    auto now = sysClk::now();
    std::time_t now_time = sysClk::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time);

    std::ostringstream dateSuffix;
    dateSuffix << name << "_" << (now_tm->tm_year + 1900) << "-" << std::setw(2) << std::setfill('0') << (now_tm->tm_mon + 1) << "-"
               << std::setw(2) << std::setfill('0') << now_tm->tm_mday << ".csv";
    return dateSuffix.str();
}