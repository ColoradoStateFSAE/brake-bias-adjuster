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

struct BiasRatio {
    float percentFront;
    float percentRear;
    bool valid;
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

int readPotValue() {
    return analogRead(POT);
};

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


/* 
 * for brake pressure: map all values (pairing front and back) over a period of time 
 * and then select the highest value pair, since the ratio is most consistent when max
 * pressure is applied
 */

//0x36B 20hz TX 0-1 Unsigned kPa y = x - 101.3
float readFrontBrakePressure() {
    int packetSize = CAN.parsePacket();
    if (packetSize == R3_GROUP08_LENGTH && CAN.packetId() == R3_GROUP08_FRAME_ID && !CAN.packetExtended()) {
        uint8_t buf[8];
        for (int i = 0; i < 8; i++) buf[i] = CAN.read;

        r3_group08_t message;
        if (r3_group08_unpack(message, buf, 8) == 0) {
            float kpa = r3_group08_brake_pressure_front_decode(message.brake_pressure_front);
            return kpa;
        };
    };
    return -1.0;
};

//0x476 20hz TX 0-1 Unsigned kPa y = x-101.3
float readRearBrakePressure() {
    int packetSize = CAN.parsePacket();
    if (packetSize == R3_GROUP45_LENGTH && CAN.packetId == R3_GROUP45_FRAME_ID && !CAN.packetExtended) {
        uint8_t buf[8];
        for (int i = 0, i < 8, i++) buf[i] = CAN.read();

        r3_group45_t message;
        if (r3_group45_unpack(message, buf, 8) == 0) {
            float kpa = r3_group45_brake_pressure_rear_decode(message);
            return kpa;
        };
    };
    return -1.0;
};


BiasRatio calculateRatio() {
    BiasRatio bias;

    float pressureFront = readFrontBrakePressure();
    float pressureRear = readRearBrakePressure();

    if (pressureFront > 0 && pressureRear > 0) {
        float total = pressureFront + pressureRear;
        bias.percentFront = (pressureFront / total) * 100f;
        bias.percentRear = (pressureRear / total) * 100f;
        bias.valid = true;
    } else {
        bias.percentFront = 0;
        bias.percentRear = 0;
        bias.valid = false;
    }
    return bias;
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

