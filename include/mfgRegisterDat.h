#ifndef PRODUCT_INFO_H
#define PRODUCT_INFO_H

#include <string>
#include <iostream>
#include <unordered_map>
//Include common library
#include "common.h"

typedef std::variant<ui8t, char, ui16t, int, byteVector> keyType;

// Functions and classes
class dataTables {
    public:
        dataTables();
        void ModelTable();
        void StatusTable();
        void ErrorTable();
        void RecordType();
        str lookup(const str& tableType, const keyType& key) const;
        void printModelTable();
    private:
        std::map<keyType, str> modelTable;
        std::map<keyType, str> statusTable;
        std::map<keyType, str> errorTable;
        std::map<keyType, str> recordType;
        struct VariantPrinter;
        keyType Variant;
};

extern dataTables victronDataTables;

#endif //PRODUCT_INFO_H