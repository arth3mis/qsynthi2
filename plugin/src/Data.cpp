//
// Created by art on 15.05.24.
//
#include "QSynthi2/Data.h"

SimFrame Data::getSimulationDisplayFrame() {
    std::lock_guard lock(frameAccessMutex);
    return simulationDisplayFrame;
}

void Data::setSimulationDisplayFrame(const SimFrame& f) {
    std::lock_guard lock(frameAccessMutex);
    simulationDisplayFrame = f;
}