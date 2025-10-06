#include <Arduino.h>
#include <CAN.h>
#include <map>
#include <EEPROM.h>
#include <r3.h>

struct SavedData {
    uint32_t signature;
    float position;
    //uint32_t crc;
};

constexpr int CAN_RECEIVE = 20;
constexpr int CAN_TRANSMIT = 21;
constexpr int POT= 26;
constexpr int MOTOR_FORWARDS = 5;
constexpr int MOTOR_BACKWARDS = 4;
constexpr int MAX_POT_VALUE = 4095; 

constexpr uint32_t SIGNATURE = 0xB1A5B1A5;
constexpr int EEPROM_SIZE = sizeof(savedData);
constexpr int EEPROM_ADDRESS = 0;

SavedData savedData;

int potValue;

//TODO: add CRC and front-end indicator of success or failure
void loadsavedData() {
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.get(EEPROM_ADDRESS, savedData);

    if (savedData.signature == SIGNATURE) {
        potValue = savedData.position;
    };
};

//TODO: add CRC
void savePosition() {
    savedData.signature = SIGNATURE;
    savedData.position = readPotValue();
    //savedData.crc = 

    EEPROM.put(EEPROM_ADDRESS, savedData);
};


int readPotValue() {
    return analogRead(POT);
};



/* 
 * for brake pressure: map all values (pairing front and back) over a period of time 
 * and then select the highest value pair, since the ratio is most consistent when max
 * pressure is applied
 */

//0x36B 20hz TX 0-1 Unsigned kPa y = x - 101.3
double readFrontBrakePressure() {
    int packetSize = CAN.parsePacket();
    if (packetSize == R3_GROUP08_LENGTH && CAN.packetId() == R3_GROUP08_FRAME_ID && !CAN.packetExtended()) {
        uint8_t buf[8];
        for (int i = 0; i < 8; i++) buf[i] = CAN.read;

        r3_group08_t message;
        if (r3_group08_unpack(message, buf, 8) == 0) {
            double kpa = r3_group08_brake_pressure_front_decode(message.brake_pressure_front);
            return kpa;
        };
    };
    return -1.0;
};

//0x476 20hz TX 0-1 Unsigned kPa y = x-101.3
double readRearBrakePressure() {
    int packetSize = CAN.parsePacket();
    if (packetSize == R3_GROUP45_LENGTH && CAN.packetId == R3_GROUP45_FRAME_ID && !CAN.packetExtended) {
        uint8_t buf[8];
        for (int i = 0, i < 8, i++) buf[i] = CAN.read();

        r3_group45_t message;
        if (r3_group45_unpack(message, buf, 8) == 0) {
            double kpa = r3_group45_brake_pressure_rear_decode(message);
            return kpa;
        };
    };
    return -1.0;
};

std::map mapPressures() {

}

int findMaxValues() {

};

double calculateRatio() {

};

void increaseFront() {

};

void increaseRear() {

};

void setup() {
    loadsavedData();
};

void loop() {

};

