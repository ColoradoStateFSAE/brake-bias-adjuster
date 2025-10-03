#include <Arduino.h>
#include <CAN.h>
#include <map>
#include <EEPROM.h>

struct LastPosition {
    uint32_t signature;
    float position;
    uint32_t crc;
};

constexpr int CAN_RECEIVE = 20;
constexpr int CAN_TRANSMIT = 21;
constexpr int POT= 26;
constexpr int MOTOR_FORWARDS = 5;
constexpr int MOTOR_BACKWARDS = 4;
constexpr int MAX_POT_VALUE = 4095; 

constexpr uint32_t SIGNATURE = 0xB1A5B1A5;
constexpr int EEPROM_SIZE = sizeof(LastPosition);
constexpr int EEPROM_ADDRESS = 0;


int potValue;

LastPosition lastPosition;

//TODO: add CRC
void loadLastPosition() {
    EEPROM.begin(EEPROM_SIZE);
    EEPROM.get(EEPROM_ADDRESS, lastPosition);

    if (lastPosition.signature == SIGNATURE) {
        potValue = lastPosition.position;
    };
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
    loadLastPosition();
};

void loop() {

};

