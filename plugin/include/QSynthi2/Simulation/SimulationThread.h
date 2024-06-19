#ifndef SIMTHREAD_H
#define SIMTHREAD_H

#include "QSynthi2/types.h"
#include "QSynthi2/Simulation/Simulation.h"
#include <thread>
#include <mutex>
#include <memory>

class ParameterCollection;

class SimulationThread {
public:

    explicit SimulationThread(const std::shared_ptr<Simulation>& s);
    ~SimulationThread();

    void simulationLoop();
    void updateParameters(const ParameterCollection* parameterCollection, const List<ModulationData*>& modulationDataList);
    void appendFrame(const SimulationFramePointer& f);
    FrameList getFrames(size_t n);
    SimulationFramePointer getStartFrame();
    size_t frameReadyCount();
    bool isSimulationContinuous();

    void resetSimulation();

    std::atomic<bool> started;
    std::atomic<bool> terminate;

    std::atomic<bool> reset;

    std::atomic<long> newestFrame;
    size_t sleepCounter = 0;

private:
    std::thread t;
    std::mutex simulationMutex;
    std::shared_ptr<Simulation> simulation, newSimulation;

    // parameters
    std::mutex parameterMutex;
    std::atomic<bool> newParameters;
    Decimal timestep = 0.2;

    // frames
    std::mutex frameMutex;
    std::atomic<size_t> bufferTargetSize;
    FrameList frameBuffer;
};

#endif //SIMTHREAD_H
