/*
Header file for data functions relevant to the SolarRecorder background program
Nicholas McCatherine, September 22nd, 2024
Ohio State University, School of Earth Sciences, Undergraduate Research
*/

#ifndef DATA_FUNC_H
#define DATA_FUNC_H

    // Common Library Includes...
    #include "common.h"

    // datFunc specific library includes
    #include <bit>
    #include <cstdint>

    // Function Declarations
    unsigned int extractBits(const byteVector& data, size_t startBit, size_t numBits); //Forward Declaration of extractBits
    recordVector parseDat(const byteVector& data); // Forward declartion of parseDat - parses the extracted BLE advert data
    mapString readConfig(const str& filename); // Forward Declaration of readConfig - reads the config file
    str macToString(const ui8t* mac); // Forward Declaration of macToString - MAC address input to string output
    byteVector hexStr2Bytes(const str& hexStr); // Forward Declaration of hexStr2Bytes - Convert hexadecimal string to bytes
    void strMACtoINT(const str& macStr, ui8t* mac); // Converts the string MAC address in config.txt to a byte (int) array
    ui16t byteSwaparoo(const ui8t byte1, const ui8t byte2); // Forward declaration of byteSwaparoo - just constructs and flips the endianess of a 16bit int and assigns it to a new variable
    byteVector ui16t_2_byteVector(ui16t value); // Forward declaration of ui16t to byteVector - Just converts the ui16t IV to a byteVector, just a patch to get this to work
    std::string generateFileName(const str& name); // Generates the file name depending on the date

    // CSV writing function
    class CSV {
        public:
            static void write(std::vector<dataVariant>& data, const str& filename, const std::chrono::days& interval);
            static sysClk::time_point lastTimeWritten;
        private:
    };

#endif // DATA_FUNC_H
