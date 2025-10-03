#include <Arduino.h>
#include <CAN.h>
#include <map>
#include <EEPROM.h>

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


int potValue;

SavedData savedData;

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
    savedData.position = potValue;
    //savedData.crc = 

    EEPROM.put(EEPROM_ADDRESS, savedData);
};

int readPotValue() {
    return analogRead(POT);
};

int readFrontBrakePressure() {

};

int readRearBrakePressure() {

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

