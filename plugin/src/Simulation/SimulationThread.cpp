#include "QSynthi2/Simulation/SimulationThread.h"
#include <QSynthi2/Parameter/ParameterCollection.h>
#include <chrono>

SimulationThread::SimulationThread(const std::shared_ptr<Simulation> &s) {
    simulation = s;
    newestFrame = -1;
    started = false;
    terminate = false;
    t = std::thread(&SimulationThread::simulationLoop, this);
}

SimulationThread::~SimulationThread() {
    t.join();
}

void SimulationThread::simulationLoop() {
    // parameters
    Decimal timestep = 0;
    newParameters = true;

    while (!terminate) {
        // update parameters
        if (newParameters) {
            std::lock_guard lock(parameterMutex);
            timestep = this->timestep;
            newParameters = false;
        }

        if (started && frameBuffer.size() < bufferTargetSize) {
            if (juce::approximatelyEqual(timestep, static_cast<Decimal>(0))) appendFrame(std::make_shared<SimulationFrame>(*frameBuffer.back()));
            appendFrame(std::make_shared<SimulationFrame>(simulation->getNextFrame(timestep, {})));
        } else {
            // todo try both busy waiting and sleep with new buffer method -> busy waiting seems to work
            // ++sleepCounter;
            // std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

void SimulationThread::updateParameters(const ParameterCollection* parameterCollection, const List<ModulationData*> &modulationData) {
    std::lock_guard lock(parameterMutex);
    newParameters = true;

    Decimal simulationStepsPerSecond = parameterCollection->simulationStepsPerSecond->getModulated(modulationData, 1)[0];
    Decimal simulationSpeedFactor = parameterCollection->simulationSpeedFactor->getModulated(modulationData, 1)[0];
    Decimal simulationBufferSeconds = parameterCollection->simulationBufferSeconds->getModulated(modulationData, 1)[0];

    // TODO read from parameter
    bufferTargetSize = std::max(static_cast<size_t>(round(simulationBufferSeconds * simulationStepsPerSecond)), static_cast<size_t>(2));

    timestep = simulationSpeedFactor / simulationStepsPerSecond;
}

void SimulationThread::appendFrame(const std::shared_ptr<SimulationFrame>& f) {
    std::lock_guard lock(frameMutex);
    frameBuffer.append(f);
    ++newestFrame;
}

FrameList SimulationThread::getFrames(const size_t n) {
    std::lock_guard lock(frameMutex);
    jassert(bufferTargetSize >= 2 * n); // buffer must at least double the size of requested frames, so the simulation thread can write in one half while the audio thread reads the other half

    const auto first = frameBuffer.begin();
    const auto last = std::next(first, static_cast<long>(std::min(n, frameBuffer.size())));
    auto subList = FrameList(first, last);
    frameBuffer.erase(first, last);
    return subList;
}

size_t SimulationThread::frameReadyCount() {
    std::lock_guard lock(frameMutex);
    return frameBuffer.size();
}
