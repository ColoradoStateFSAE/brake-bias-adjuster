#include <Arduino.h>
#include <CAN.h>
#include <map>
#include <EEPROM.h>
#include <optional>
#include "CanController/CanController.h"
#include "Signals/Signals.h"

Signals signals;

#ifdef PIO_UNIT_TESTING

using namespace fakeit;

Mock<CanController> mockCanController;
CanController& canController = mockCanController.get();

#else

std::optional<MCP2515> mcp;
CanController canController(*mcp, signals);

#endif

struct SavedData {
    uint32_t signature;
    float position;
    //uint32_t crc;
};

SavedData savedData;

int potValue;

int readPotValue() {
    return analogRead(POT);
};

// TODO: add CRC and front-end indicator of success or failure
void loadsavedData() {
    #ifdef ARDUINO_ARCH_RP2040
    EEPROM.begin(EEPROM.length());
    EEPROM.get(0, savedData);
    #endif

    if (savedData.signature == SIGNATURE) {
        potValue = savedData.position;
    };
};

void increaseFront() {

};

void increaseRear() {

};

void setup() {
    loadsavedData();
    pinMode(POT, INPUT);

    #ifdef ARDUINO_ARCH_RP2040
    mcp.emplace(MCP2515(PIN_CAN_CS));
    #endif

    canController.begin();

    signals.onBiasAdjust = [](BiasDirection direction) {
        if(direction == BiasDirection::PLUS) {
            
        } else if(direction == BiasDirection::MINUS) {

        }
    };
};

void loop() {
    canController.update();
};

