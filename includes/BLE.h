/*
Header file for BLE Operations relevant to the SolarRecorder background program
Nicholas McCatherine, September 22nd, 2024
Ohio State University, School of Earth Sciences, Undergraduate Research
*/

#ifndef BLE_H
#define BLE_H

    #include "AES.h"
    // Common Library Includes...
    #include "common.h"
    #include "datFunc.h"
    #include "bleParse.h"

    // Function Declarations
    class BLEscanner {
        public:
            //Constructor Declaration
            BLEscanner(const AESdecrypt& decryptor, const str mac, const str& outputFile, const std::chrono::days& fileInterval);
            void scan();

        private:
            const AESdecrypt& decryptor_;
            const str devMAC;
            const str output_file;
            const std::chrono::days new_file_interval;
            int device_id;
            int socket;
            SimpleBLE::Adapter adapter;
    };
    void signalHandler(int sig);
    void restartBLEservice();

#endif //BLE_H