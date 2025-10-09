#pragma once
#ifdef PIO_UNIT_TESTING

#include <gtest/gtest.h>
#include <Arduino.h>
#include <mcp2515.h>
#include "CanController/CanController.h"

using namespace fakeit;
using namespace std;

class CanControllerTest : public ::testing::Test {
  protected:
    struct can_frame readFrame;

    Signals signals;
    
    // MCP2515
    Mock<MCP2515> mockMcp;
    MCP2515& mcp = mockMcp.get();

    CanController canController = CanController(mcp, signals);

    unsigned long time = 0;

    void SetUp() override {
        ArduinoFakeReset();

        // MCP2515
        When(Method(mockMcp, reset)).AlwaysReturn();
        When(Method(mockMcp, setBitrate)).AlwaysReturn();
        When(Method(mockMcp, setNormalMode)).AlwaysReturn();
        When(Method(mockMcp, sendMessage)).AlwaysReturn(MCP2515::ERROR_OK);
        When(Method(mockMcp, readMessage)).AlwaysDo([&](struct can_frame *frame) -> MCP2515::ERROR {
            *frame = readFrame;
            return MCP2515::ERROR_OK;
        });

        When(Method(ArduinoFake(), millis)).AlwaysDo([&]() -> unsigned long { return time; });

        canController.begin();
    }
};

TEST_F(CanControllerTest, Begin) {
    Verify(
        Method(mockMcp, reset),
        Method(mockMcp, setBitrate).Using(CAN_1000KBPS),
        Method(mockMcp, setNormalMode)
    );
}

TEST_F(CanControllerTest, haltech_group08) {
    INIT_MESSAGE(haltech_group08);
    ENCODE_SIGNAL(haltech_group08, brake_pressure_front, 1000);

    INIT_FRAME(haltech_group08);
    PACK_MESSAGE(haltech_group08, haltech_group08_msg.data);
    readFrame = haltech_group08_msg;
    
    canController.update();

    ASSERT_NEAR(signals.brakePressureFront, 145.038, 1);
}

TEST_F(CanControllerTest, haltech_group45) {
    INIT_MESSAGE(haltech_group45);
    ENCODE_SIGNAL(haltech_group45, brake_pressure_rear, 1000);
    ENCODE_SIGNAL(haltech_group45, brake_pressure_front_ratio, 60);

    INIT_FRAME(haltech_group45);
    PACK_MESSAGE(haltech_group45, haltech_group45_msg.data);
    readFrame = haltech_group45_msg;
    
    canController.update();

    ASSERT_NEAR(signals.brakePressureRear, 145.038, 1);
    ASSERT_EQ(signals.brakePressureFrontRatio, 60);
}

TEST_F(CanControllerTest, swc_buttons) {
    const vector<BiasDirection> expected = {PLUS, MINUS};
    vector<BiasDirection> actual;

    signals.onBiasAdjust = [&](BiasDirection direction) {
        actual.push_back(direction);
    };

    for(auto direction : {PLUS, MINUS}) {
        INIT_MESSAGE(swc_buttons);
        ENCODE_SIGNAL(swc_buttons, bias_direction, direction);

        INIT_FRAME(swc_buttons);
        PACK_MESSAGE(swc_buttons, swc_buttons_msg.data);
        readFrame = swc_buttons_msg;

        canController.update();
    }

    ASSERT_EQ(actual, expected);
}

#endif
