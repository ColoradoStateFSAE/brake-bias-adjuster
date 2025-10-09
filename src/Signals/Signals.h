#pragma once
#include <functional>
#include "constants.h"

struct Signals {
    bool offline = true;

    float brakePressureFront = 0; // PSI
    float brakePressureRear = 0; // PSI
    float brakePressureFrontRatio = 0;

    std::function<void(BiasDirection)> onBiasAdjust = [](BiasDirection) {};
};
