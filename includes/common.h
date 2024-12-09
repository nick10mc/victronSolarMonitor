/*
This file defines the common libraries for the SolarRecorder background program.
By Nicholas McCatherine, September 22nd, 2024.
Ohio State University, School of Earth Sciences, Undergraduate Research
*/

#ifndef common_h
#define common_h

    // External Library Includes...
    #include <openssl/conf.h>
    #include <openssl/evp.h>
    #include <openssl/err.h>
    #include <bluetooth/bluetooth.h> // BlueZ libraries retained due to issues with access being blocked - Needed for the restart function
    #include <bluetooth/hci.h>
    #include <bluetooth/hci_lib.h>
    #include <simpleble/SimpleBLE.h> // Additional library to make it easier to access BLE peripheral

    // Standard Includes - those which are standard to C++
    #include <iostream>
    #include <fstream>
    #include <sstream>
    #include <iomanip>
    #include <cstring>
    #include <unistd.h>
    #include <array>
    #include <vector>
    #include <map>
    #include <thread>
    #include <chrono>
    #include <ctime>
    #include <variant>
    #include <functional>
    #include <fcntl.h>

    // Type Aliases
    typedef std::vector<unsigned char> byteVector;
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
    typedef std::variant<float, ui8t, str, byteVector> dataVariant;

    // Function Declarations

#endif //common_h