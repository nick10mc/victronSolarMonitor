#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <iostream>
#include <vector>
#include <array>
#include <cstring>
#include <cstdint>
#include <map>

typedef std::vector<unsigned char> byteV;
typedef std::runtime_error RuntimeError;

struct victronManufacturerData {
    uint16_t vendorID;
    uint8_t victronRecordType;
    uint8_t encryptKeyMatch;
    uint16_t nonceDataCounter;
    uint8_t victronEncryptedData[16]; // Adjust size as needed
};

struct victronPanelData {
    uint8_t deviceState;
    uint8_t errorCode;
    uint16_t batteryVoltage;
    uint16_t batteryCurrent;
    uint16_t todayYield;
    uint16_t inputPower;
    uint8_t outputCurrentHi;
    uint8_t outputCurrentLo;
};

class BLEAdvertisedDevice {
public:
    bool haveManufacturerData() {
        return true; // Simulating that we always have manufacturer data
    }
    
    byteV getManufacturerData() {
        // Simulating some dummy manufacturer data for testing
        victronManufacturerData data = {
            0x02E1,  // Vendor ID (Victron)
            0x01,    // Record Type (Solar Charger)
            0x3b,    // Encryption Key Match (for test purposes)
            0x0255,  // Nonce Data Counter
            {0xbf, 0x19, 0x9c, 0xb8, 0xcd, 0x70, 0x30, 0xf5,
             0x17, 0xdf, 0x4b, 0xaf} // Encrypted Data
        };
        return byteV(reinterpret_cast<unsigned char*>(&data), reinterpret_cast<unsigned char*>(&data) + sizeof(data));
    }
    
    std::string getName() {
        return "Victron Device"; // Placeholder
    }

    bool haveName() {
        return true; // Simulating that we always have a name
    }
};

// Function to decrypt data
void decryptData(const byteV& key, const byteV& iv, const byteV& cipher, byteV& decrypted) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw RuntimeError("Failed to create EVP_CIPHER_CTX!"); // Throw an error
    }

    const EVP_CIPHER* cipherType = EVP_aes_128_ctr();
    if (!EVP_DecryptInit_ex(ctx, cipherType, NULL, key.data(), iv.data())) {
        EVP_CIPHER_CTX_free(ctx);
        throw RuntimeError("Initialization failed! (EVP_DecryptInit_ex)"); // Throw an error
    }

    int length;
    decrypted.resize(cipher.size()); // Resize decrypted vector to the cipher size
    if (!EVP_DecryptUpdate(ctx, decrypted.data(), &length, cipher.data(), cipher.size())) {
        EVP_CIPHER_CTX_free(ctx);
        throw RuntimeError("Failed to update! (EVP_DecryptUpdate)");
    }

    int textLength = length;
    if (!EVP_DecryptFinal_ex(ctx, decrypted.data() + length, &length)) {
        EVP_CIPHER_CTX_free(ctx);
        throw RuntimeError("Failed to finalize decryption! (EVP_DecryptFinal_ex)");
    }
    textLength += length;

    decrypted.resize(textLength); // Resize decrypted vector to actual decrypted size
    EVP_CIPHER_CTX_free(ctx); // Clean up
}

// Callbacks for handling advertised devices
class MyAdvertisedDeviceCallbacks {
public:
    void onResult(BLEAdvertisedDevice advertisedDevice) {
        if (advertisedDevice.haveManufacturerData()) {
            byteV manData = advertisedDevice.getManufacturerData();
            uint8_t manCharBuf[32] = {}; // Buffer to store manufacturer data
            std::memcpy(manCharBuf, manData.data(), std::min(manData.size(), size_t(31)));

            victronManufacturerData* vicData = (victronManufacturerData*)manCharBuf;

            // Ignore if the Vendor ID isn't Victron
            if (vicData->vendorID != 0x02E1) {
                return;
            }

            // Ignore if it isn't type 0x01 (Solar Charger)
            if (vicData->victronRecordType != 0x01) {
                return;
            }

            if (advertisedDevice.haveName()) {
                std::string savedDeviceName = advertisedDevice.getName();
            }

            const byteV key = {0x3b, 0x3b, 0xaa, 0x49, 0x86, 0xa8, 0x9b, 0x47, 0xe4, 0xb7, 0x09, 0xab, 0x11, 0x7a, 0x4a, 0x2d};
            const byteV iv = {0x02, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

            byteV inputData(vicData->victronEncryptedData, vicData->victronEncryptedData + sizeof(vicData->victronEncryptedData));
            byteV outputData(16); // Adjust size as needed

            decryptData(key, iv, inputData, outputData);

            victronPanelData* victronData = (victronPanelData*)outputData.data();

            // Extract values from decrypted data
            uint8_t deviceState = victronData->deviceState;
            float batteryVoltage = float(victronData->batteryVoltage) * 0.01f;
            float batteryCurrent = float(victronData->batteryCurrent) * 0.1f;

            // Output data (replace with actual logging)
            std::cout << "Battery: " << batteryVoltage << " Volts, Current: " << batteryCurrent << " Amps" << std::endl;
        }
    }
};

int main() {
    // Example of how to use MyAdvertisedDeviceCallbacks
    MyAdvertisedDeviceCallbacks callbacks;
    BLEAdvertisedDevice device;

    // Simulating a result from the BLE scan
    callbacks.onResult(device);

    return 0;
}
