#pragma once
#include <swc/swc.h>

constexpr int POT= 26;
constexpr int MOTOR_FORWARDS = 5;
constexpr int MOTOR_BACKWARDS = 4;
constexpr int MAX_POT_VALUE = 4095;
constexpr uint32_t SIGNATURE = 0xB1A5B1A5;

typedef enum {
    PLUS = SWC_BUTTONS_BIAS_DIRECTION_PLUS_CHOICE,
    MINUS = SWC_BUTTONS_BIAS_DIRECTION_MINUS_CHOICE
} BiasDirection;
