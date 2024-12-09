#include "mfgRegisterDat.h"

dataTables victronDataTables;

dataTables::dataTables() {
    ModelTable();
    StatusTable();
    ErrorTable();
    RecordType();
}

///////////////////////// Model Table /////////////////////////

void dataTables::ModelTable() {
    // Initialize the model table with the given values - Seems to be wrong incorrect for BLE packets though.
    modelTable[0x0300] = "BlueSolar MPPT 70|15";
    modelTable[0xa040] = "BlueSolar MPPT 75|50";
    modelTable[0xa041] = "BlueSolar MPPT 150|35";
    modelTable[0xa042] = "BlueSolar MPPT 75|15";
    modelTable[0xa043] = "BlueSolar MPPT 100|15";
    modelTable[0xa044] = "BlueSolar MPPT 100|30";
    modelTable[0xa045] = "BlueSolar MPPT 100|50";
    modelTable[0xa046] = "BlueSolar MPPT 150|70";
    modelTable[0xa047] = "BlueSolar MPPT 150|100";
    modelTable[0xa048] = "BlueSolar MPPT 75|50 rev2";
    modelTable[0xa049] = "BlueSolar MPPT 100|50 rev2";
    modelTable[0xa04a] = "BlueSolar MPPT 100|30 rev2";
    modelTable[0xa04b] = "BlueSolar MPPT 150|35 rev2";
    modelTable[0xa04c] = "BlueSolar MPPT 75|10";
    modelTable[0xa04d] = "BlueSolar MPPT 150|45";
    modelTable[0xa04e] = "BlueSolar MPPT 150|60";
    modelTable[0xa04f] = "BlueSolar MPPT 150|85";
    modelTable[0xa050] = "SmartSolar MPPT 250|100";
    modelTable[0xa051] = "SmartSolar MPPT 150|100";
    modelTable[0xa052] = "SmartSolar MPPT 150|85";
    modelTable[0xa053] = "SmartSolar MPPT 75|15";
    modelTable[0xa054] = "SmartSolar MPPT 75|10";
    modelTable[0xa055] = "SmartSolar MPPT 100|15";
    modelTable[0xa056] = "SmartSolar MPPT 100|30";
    modelTable[0xa057] = "SmartSolar MPPT 100|50";
    modelTable[0xa058] = "SmartSolar MPPT 150|35";
    modelTable[0xa059] = "SmartSolar MPPT 150|100 rev2";
    modelTable[0xa05a] = "SmartSolar MPPT 150|85 rev2";
    modelTable[0xa05b] = "SmartSolar MPPT 250|70";
    modelTable[0xa05c] = "SmartSolar MPPT 250|85";
    modelTable[0xa05d] = "SmartSolar MPPT 250|60";
    modelTable[0xa05e] = "SmartSolar MPPT 250|45";
    modelTable[0xa05f] = "SmartSolar MPPT 100|20";
    modelTable[0xa060] = "SmartSolar MPPT 100|20 48V";
    modelTable[0xa061] = "SmartSolar MPPT 150|45";
    modelTable[0xa062] = "SmartSolar MPPT 150|60";
    modelTable[0xa063] = "SmartSolar MPPT 150|70";
    modelTable[0xa064] = "SmartSolar MPPT 250|85 rev2";
    modelTable[0xa065] = "SmartSolar MPPT 250|100 rev2";
    modelTable[0xa066] = "BlueSolar MPPT 100|20";
    modelTable[0xa067] = "BlueSolar MPPT 100|20 48V";
    modelTable[0xa068] = "SmartSolar MPPT 250|60 rev2";
    modelTable[0xa069] = "SmartSolar MPPT 250|70 rev2";
    modelTable[0xa06a] = "SmartSolar MPPT 150|45 rev2";
    modelTable[0xa06b] = "SmartSolar MPPT 150|60 rev2";
    modelTable[0xa06c] = "SmartSolar MPPT 150|70 rev2";
    modelTable[0xa06d] = "SmartSolar MPPT 150|85 rev2";
    modelTable[0xa06e] = "SmartSolar MPPT 150|100 rev3";
    modelTable[0xa06f] = "BlueSolar MPPT 150|45 rev2";
    modelTable[0xa070] = "BlueSolar MPPT 150|60 rev2";
    modelTable[0xa071] = "BlueSolar MPPT 150|70 rev2";
    modelTable[0xa072] = "BlueSolar MPPT 150|45 rev3";
    modelTable[0xa073] = "SmartSolar MPPT 150|45 rev3";
    modelTable[0xa074] = "SmartSolar MPPT 75|10 rev2";
    modelTable[0xa075] = "SmartSolar MPPT 75|15 rev2";
    modelTable[0xa076] = "BlueSolar MPPT 100|30 rev3";
    modelTable[0xa077] = "BlueSolar MPPT 100|50 rev3";
    modelTable[0xa078] = "BlueSolar MPPT 150|35 rev2";
    modelTable[0xa079] = "BlueSolar MPPT 75|10 rev2";
    modelTable[0xa07a] = "BlueSolar MPPT 75|15 rev2";
    modelTable[0xa07b] = "BlueSolar MPPT 100|15 rev2";
    modelTable[0xa07c] = "BlueSolar MPPT 75/10 rev3";
    modelTable[0xa07d] = "BlueSolar MPPT 75/15 rev3";
    modelTable[0xa07e] = "SmartSolar Charger MPPT 100/30";
    modelTable[0xa102] = "SmartSolar MPPT VE.Can 150|70";
    modelTable[0xa103] = "SmartSolar MPPT VE.Can 150|45";
    modelTable[0xa104] = "SmartSolar MPPT VE.Can 150|60";
    modelTable[0xa105] = "SmartSolar MPPT VE.Can 150|85";
    modelTable[0xa106] = "SmartSolar MPPT VE.Can 150|100";
    modelTable[0xa107] = "SmartSolar MPPT VE.Can 250|45";
    modelTable[0xa108] = "SmartSolar MPPT VE.Can 250|60";
    modelTable[0xa109] = "SmartSolar MPPT VE.Can 250|70";
    modelTable[0xa10a] = "SmartSolar MPPT VE.Can 250|85";
    modelTable[0xa10b] = "SmartSolar MPPT VE.Can 250|100";
    modelTable[0xa10c] = "SmartSolar MPPT VE.Can 150|70 rev2";
    modelTable[0xa10d] = "SmartSolar MPPT VE.Can 150|85 rev2";
    modelTable[0xa10e] = "SmartSolar MPPT VE.Can 150|100 rev2";
    modelTable[0xa10f] = "BlueSolar MPPT VE.Can 150|100";
    modelTable[0xa110] = "SmartSolar MPPT RS 450/100";
    modelTable[0xa111] = "SmartSolar MPPT RS 450/200";
    modelTable[0xa112] = "BlueSolar MPPT VE.Can 250|70";
    modelTable[0xa113] = "BlueSolar MPPT VE.Can 250|100";
    modelTable[0xa114] = "SmartSolar MPPT VE.Can 250|70 rev2";
    modelTable[0xa115] = "SmartSolar MPPT VE.Can 250|100 rev2";
    modelTable[0xa116] = "SmartSolar MPPT VE.Can 250|85 rev2";
    modelTable[0xa117] = "BlueSolar MPPT VE.Can 150|100 rev2";
    std::cout << "ModelTable initialized with " << modelTable.size() << " entries." << std::endl;
}


///////////////////////// Status Register /////////////////////////

void dataTables::StatusTable()
{
    statusTable[0x00] = "NOT CHARGING";
    statusTable[0x02] = "FAULT";
    statusTable[0x03] = "BULK"; // FULL CURRENT CHARGE
    statusTable[0x04] = "ABSORPTION"; // VOLTAGE CONTROLLED WITH ABSORPTION VOLTAGE SETPOINT
    statusTable[0x05] = "FLOAT"; // VOLTAGE CONTROLLED WITH FLOAT VOLTAGE SET-POINT
    statusTable[0x06] = "STORAGE"; // VOLTAGE CONTROLLED WITH STORAGE VOLTAGE SETPOINT
    statusTable[0x07] = "MANUAL EQUALISE"; // VOLTAGE CONTROLLED W/ EQUALISATION VOLTAGE SETPOINT
    statusTable[0xf5] = "WAKE-UP"; // THE DEVICE IS ABOUT TO START
    statusTable[0xf7] = "AUTO EQUALISE"; // VOLTAGE CONTROLLED W/ EQUALISATION VOLTAGE SETPOINT
    statusTable[0xfa] = "BLOCKED"; // UNIT BEING UPDATED, NOT AVAILABLE
    statusTable[0xfc] = "EXTERNAL CONTROL"; // VOLTAGE CONTROLLED W/ REMOTE VOLTAGE SETPOINT
    statusTable[0xff] = "UNAVAILABLE"; // NO INFORMATION AVAIALABLE
    std::cout << "StatusTable initialized with " << statusTable.size() << " entries." << std::endl;
}


///////////////////////// Error Register /////////////////////////

void dataTables::ErrorTable() // REGISTER 0xEDDA
{
    errorTable[0x00] = "NOT CHARGING";
    errorTable[0x02] = "FAULT";
    errorTable[0x03] = "BAT. TEMP SENSOR ISSUE"; // BATTERY TEMPERATURE SENSOR ISSUE
    errorTable[0x04] = "BAT. TEMP SENSOR ISSUE"; // BATTERY TEMPERATURE SENSOR ISSUE
    errorTable[0x05] = "BAT. TEMP SENSOR ISSUE"; // BATTERY TEMPERATURE SENSOR ISSUE
    errorTable[0x06] = "BAT. VOLT SENSOR ISSUE"; // BATTERY VOLTAGE SENSOR ISSUE
    errorTable[0x07] = "BAT. VOLT SENSOR ISSUE"; // BATTERY VOLTAGE SENSOR ISSUE
    errorTable[0x08] = "BAT. VOLT SENSOR ISSUE"; // BATTERY VOLTAGE SENSOR ISSUE
    errorTable[0x0e] = "BAT. TEMP. TOO LOW"; // BATTERY TEMP TOO LOW, NO CHARGING
    errorTable[0x11] = "CHARGER TEMP. TOO HIGH"; // CHARGER TEMP TOO HIGH
    errorTable[0x12] = "CHARGER- EXCESSIVE OUTPUT AMPS"; // CHARGER OUTPUT OVERCURRENT
    errorTable[0x13] = "CHARGER CURRENT REVERSED"; // CHARGER CURRENT POLARITY REVERSED
    errorTable[0x14] = "CHARGER BULK TIME EXPIRED"; // APPLIES WHEN 10 HOUR BULK TIME PROTECTION IS ACTIVE
    errorTable[0x15] = "CHARGER CURRENT SENSOR ISSUE"; // BIAS NOT WITHIN EXPECTED LIMITS DURING OFF STATE
    errorTable[0x16] = "CHARGER INTERNAL TEMP. SENSOR ISSUE"; // CHARGER INTERNAL TEMPERATURE SENSOR ISSUE
    errorTable[0x17] = "CHARGER INTERNAL TEMP. SENSOR ISSUE"; // CHARGER INTERNAL TEMPERATURE SENSOR ISSUE
    errorTable[0x1a] = "CHARGER TERMINALS OVERHEAT"; // CHARGER TERMINALS OVERHEATED
    errorTable[0x1b] = "CHARGER SHORT CIRCUIT"; // CHARGER SHORT CIRCUIT
    errorTable[0x1c] = "CONVERTER ISSUE"; // DUAL CONVERTER MODELS, ONE OF THE CONVERTERS IS NOT WORKING
    errorTable[0x1d] = "BATTERY OVER-CHARGE PROTECTION"; // BATTERY OVER-CHARGE PROTECTION
    errorTable[0x21] = "INPUT VOLTAGE TOO HIGH"; 
    errorTable[0x22] = "INPUT EXCESSIVE CURRENT"; 
    errorTable[0x26] = "INPUT SHUTDOWN - BAT. HIGH VOLTS"; // DUE TO EXCESSIVE BATTERY VOLTAGE
    errorTable[0x27] = "INPUT SHUTDOWN - CURRENT FLOW"; // CURRENT FLOWING WHILE THE CONVERTER IS OFF
    errorTable[0x42] = "INCOMPATIBLE DEVICE IN NETWORK"; // FOR SYNCHRONIZED CHARGING
    errorTable[0x43] = "BMS CONNECTION LOST";
    errorTable[0x44] = "NETWORK MISCONFIGURED"; // E.G. COMBINING ESS WITH VE.SMART NETWORKING
    errorTable[0x74] = "CALIBRATION DATA LOST";
    errorTable[0x75] = "INCOMPATIBLE FIRMWARE";
    errorTable[0x77] = "SETTINGS DATA INVALID/CORRUPTED"; // USE RESTORE TO DEFAULTS AND RESET TO RECOVER
    std::cout << "ErrorTable initialized with " << errorTable.size() << " entries." << std::endl;
}


///////////////////////// Record Type /////////////////////////

void dataTables::RecordType() // Record Type
{
    recordType[0x00] = "Test record";
    recordType[0x01] = "Solar Charger";
    recordType[0x02] = "Battery Monitor";
    recordType[0x03] = "Inverter";
    recordType[0x04] = "DC/DC Converter";
    recordType[0x05] = "SmartLithium";
    recordType[0x06] = "Inverter RS";
    recordType[0x07] = "GX-Device";
    recordType[0x08] = "AC Charger";
    recordType[0x09] = "Smart Battery Protect";
    recordType[0x0a] = "(Lynx Smart) BMS";
    recordType[0x0b] = "Multi RS";
    recordType[0x0c] = "VE.Bus";
    recordType[0x0d] = "DC Energy Meter";
    std::cout << "RecordType initialized with " << recordType.size() << " entries." << std::endl;
}

//// Debugging - Print all of the tables ////
// Visitor to handle printing the variant types
struct VariantPrinter {
    std::ostream& os;

    void operator()(unsigned char value) const {
        os << static_cast<int>(value); // cast to int for proper display
    }

    void operator()(unsigned short value) const {
        os << value;
    }

    void operator()(int value) const {
        os << value;
    }

    void operator()(const std::vector<unsigned char>& value) const {
        os << "[";
        for (size_t i = 0; i < value.size(); ++i) {
            os << "0x" << std::hex << static_cast<int>(value[i]);
            if (i < value.size() - 1) {
                os << ", ";
            }
        }
        os << "]";
    }
};


void dataTables::printModelTable() {
    for (const auto& pair : modelTable) {
        // Handle the key (pair.first)
        // Create a buffer to hold the string representation of the key
        char keyBuffer[256]; // Ensure enough space for complex keys
        keyBuffer[0] = '\0';  // Initialize buffer to be empty

        // Print the key using std::visit
        std::visit([&](const auto& value) {
            if constexpr (std::is_same_v<decltype(value), ui8t>) {
                snprintf(keyBuffer, sizeof(keyBuffer), "%02x", value); // Print as hex
            } else if constexpr (std::is_same_v<decltype(value), ui16t>) {
                snprintf(keyBuffer, sizeof(keyBuffer), "%04x", value); // Print as hex
            }
        }, pair.first);

        // Print the key
        std::cout << "Key: " << keyBuffer;
        
        std::visit([](const auto& value) {
            if constexpr (std::is_same_v<decltype(value), unsigned char> ||
                        std::is_same_v<decltype(value), unsigned short> ||
                        std::is_same_v<decltype(value), int>) {
                std::cout << std::hex << static_cast<int>(value) << ' ';
            } else if constexpr (std::is_same_v<decltype(value), std::vector<unsigned char>>) {
                std::cout << "Vector: ";
                for (const auto& byte : value) {
                    std::cout << std::hex << static_cast<int>(byte) << ' ';
                }
            }
        }, Variant);
        
        std::cout << ", Value: " << pair.second << std::endl;
    }
}

///////////////////////// Lookup function /////////////////////////
str dataTables::lookup(const str& tableType, const keyType& key) const {
    if (tableType == "model") {
        auto data = modelTable.find(key);
        if (data != modelTable.end()) {
            return data->second;
        }
    } else if (tableType == "status") {
        auto data = statusTable.find(key);
        if (data != statusTable.end()) {
            return data->second;
        }
    } else if (tableType == "error") {
        auto data = errorTable.find(key);
        if (data != errorTable.end()) {
            return data->second;
        }
    } else if (tableType == "record") {
        auto data = recordType.find(key);
        if (data != recordType.end()) {
            return data->second;
        }
    } else {
        throw std::out_of_range("\033[1;31mInvalid table type in dataTables::lookup\033[0m");
    }
    
    // str keyTypeName = typeid(key).name();
    // std::cout << "Key not found in parsed data - dataTables::operator[]. Key used was of type " << keyTypeName << " with a value of " << key << std::endl;
    return "";
}
