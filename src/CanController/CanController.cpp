#include "CanController.h"

void CanController::begin() {
    mcp.reset();
    mcp.setBitrate(CAN_1000KBPS);
    mcp.setNormalMode();
}

void CanController::update() {
    can_frame msg;
    if(mcp.readMessage(&msg) == MCP2515::ERROR_OK) {
        lastUpdateTime = millis();

        int id = msg.can_id & (~CAN_EFF_FLAG);
        switch(id) {
            READ_MESSAGE_CASE(haltech_group08);
            READ_MESSAGE_CASE(haltech_group45);
            READ_MESSAGE_CASE(swc_buttons);
        }
    }

    signals.offline = millis() - lastUpdateTime >= 100;
}

void CanController::DECODE_MESSAGE(haltech_group08) {
    signals.brakePressureFront = DECODE_SIGNAL(haltech_group08, brake_pressure_front) / 6.895;
}

void CanController::DECODE_MESSAGE(haltech_group45) {
    signals.brakePressureRear = DECODE_SIGNAL(haltech_group45, brake_pressure_rear) / 6.895;
    signals.brakePressureFrontRatio = DECODE_SIGNAL(haltech_group45, brake_pressure_front_ratio);
}

void CanController::DECODE_MESSAGE(swc_buttons) {
    BiasDirection direction = static_cast<BiasDirection>(DECODE_SIGNAL(swc_buttons, bias_direction));
    signals.onBiasAdjust(direction);
}
