#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
//#include <string.h>
#include <cstring>
#include <unistd.h>
#include <array>
#include <vector>
#include <map>
#include <thread>
#include <chrono>
#include <ctime>
#include <bit>
#include <cstdint>
#include <fcntl.h>


// Type Aliases
typedef std::vector<unsigned char> byteV;
typedef std::vector<std::array<unsigned char, 2>> recordVector;
typedef std::runtime_error RuntimeError;
typedef std::string str;
typedef uint8_t ui8t;
typedef uint16_t ui16t;
typedef int8_t i8t;
typedef int16_t i16t;
typedef std::map<std::string, std::string> mapString;
typedef std::ifstream ifStream;
typedef std::chrono::system_clock sysClk;


// Function to convert hexadecimal string to bytes
byteV hexStr2Bytes(const str& hexStr) {
    byteV bytes;
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
            std::cerr << "Invalid byte string length: " << byteString << std::endl;
            continue;
        }

        std::cout << "Parsing value: " << byteString << std::endl; // Aids in debugging

        try {             
            unsigned char byte = static_cast<unsigned char>(std::stoi(byteString, nullptr, 16));
            bytes.push_back(byte);
        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument for stoi: " << byteString << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "Value out of range for stoi: " << byteString << std::endl;
        }
        
    }

    return bytes;
}


#include <byteswap.h> 


int main() {

    //hexStr2Bytes("3b3baa4986a89b47e4b709ab117a4a2d")
    const byteV& key = {0x3b,0x3b,0xaa,0x49,0x86,0xa8,0x9b,0x47,0xe4,0xb7,0x09,0xab,0x11,0x7a,0x4a,0x2d};
    const byteV& iv = {0x21,0xf4,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    //const byteV& cipher = {0xbf,0x19,0x9c,0xb8,0xcd,0x70,0x30,0xf5,0x17,0xdf,0x4b,0xaf}; //without 0xfe padding
    const byteV& cipher = {0x6d,0x17,0x0f,0x51,0xc7,0x6d,0x61,0xc5,0xae,0x07,0x63,0x8c,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe}; //with 0xfe padding

    // create EVP_CIPHER_CTX
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if(!ctx)
    {
        throw RuntimeError("Failed to create EVP_CIPHER_CTX!"); // Throw an error
    }

    // Set algorithm as AES-128-CTR and initialize
    const EVP_CIPHER* cipherType = EVP_aes_128_ctr(); 
    if (!EVP_DecryptInit_ex(ctx, cipherType, NULL, key.data(), iv.data()))
    {
        EVP_CIPHER_CTX_free(ctx); // Free up the memory used
        throw RuntimeError("Initialization failed! (EVP_DecryptInit_ex)"); // Throw an error
    }

    byteV text(cipher.size());
    int length;
    if (!EVP_DecryptUpdate(ctx, text.data(), &length, cipher.data(), cipher.size()))
    {
        EVP_CIPHER_CTX_free(ctx);
        throw RuntimeError("Failed to update! (EVP_DecryptUpdate)");
    }

    int textLength = length;
    if (!EVP_DecryptFinal_ex(ctx, text.data() + length, &length))
    {
        EVP_CIPHER_CTX_free(ctx);
        throw RuntimeError("Failed to finalize decryption! (EVP_DecryptFinal_ex)");
    }
    textLength += length;

    text.resize(textLength);
    EVP_CIPHER_CTX_free(ctx);

    for (const auto& byte : text) 
    {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    std::cout << std::endl;
    std::cout << "DEVSTATE text[0]: " << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(text[0]) << std::endl;
    std::cout << "CHRGERR text[1]: " << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(text[1]) << std::endl;
    std::cout << "BVOLT LSB text[2]: " << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(text[2]) << std::endl;
    std::cout << "BVOLT MSB text[3]: " << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(text[3]) << std::endl;
    std::cout << "BAMP LSB text[4]: " << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(text[4]) << std::endl;
    std::cout << "BAMP MSB text[5]: " << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(text[5]) << std::endl;
    std::cout << "YIELD LSB text[6]: " << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(text[6]) << std::endl;
    std::cout << "YIELD MSB text[7]: " << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(text[7]) << std::endl;
    std::cout << "POW LSB text[8]: " << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(text[8]) << std::endl;
    std::cout << "POW MSB text[9]: " << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(text[9]) << std::endl;
    float floatVolts = static_cast<float>(std::byteswap(static_cast<ui16t>((text[2] << 8) | text[3]))) * 0.01;
    float floatCurrent = static_cast<float>(std::byteswap(static_cast<ui16t>((text[4] << 8) | text[5]))) * 0.1;
    float floatYieldToday = static_cast<float>(std::byteswap(static_cast<ui16t>((text[6] << 8) | text[7]))) * 0.01;
    float floatPVpow = static_cast<float>(std::byteswap(static_cast<ui16t>((text[8] << 8) | text[9])));
    std::cout << std::dec << "voltFloat: " << floatVolts << std::endl;
    std::cout << std::dec << "currentFloat: " << floatCurrent << std::endl;
    std::cout << std::dec << "YieldToday_kWh: " << floatYieldToday << std::endl;
    std::cout << std::dec << "PVpower_W: " << floatPVpow << std::endl;
}