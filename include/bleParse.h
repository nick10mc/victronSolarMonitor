/*
Header file for BLE Advertisement data packet parsing
Nicholas McCatherine, October 13th, 2024
Ohio State University, School of Earth Sciences, Undergraduate Research

This parser places the Advertiser data bytes into a map whereby the key, defined
by the preceding variable name, provides access to the data.
*/

#ifndef BLE_PARSE_H
#define BLE_PARSE_H

    // Common Library Includes...
    #include "common.h"
    #include "datFunc.h"

    typedef std::variant<ui8t,char,ui16t,int,byteVector> ParsedValue; // So we can used multiple data types in the map

    // Function Declarations
    class parsePacket {
        public:

            parsePacket(const byteVector& buffer);
            void parse(const byteVector& buffer);
            void parseDecryptedData(const ParsedValue& decryptedBuffer);
            ParsedValue operator[](const str& key) const;
            void printData();

        private:
            std::map<str, ParsedValue> parsedData;
    };

#endif // BLE_PARSE_H