#ifndef DATA_H
#define DATA_H
#include <mutex>
#include <atomic>
#include "QSynthi2/Parameter/ParameterCollection.h"
#include "QSynthi2/Synthesizer/VoiceData.h"

class Data {
public:

    // context: audio thread <-> GUI thread
    //
    std::atomic<size_t> simulationWidth;
    std::atomic<size_t> simulationHeight;
    std::atomic<bool> resetSimulation{false};

    // context: simulation thread <-> GUI thread
    std::atomic<int> barrierType{0};
    std::atomic<Decimal> barrierOffset{0};
    std::atomic<Decimal> barrierWidth{0};
    List<V2> barrierSlits{};

    SimulationFramePointer getSimulationDisplayFrame();
    void setSimulationDisplayFrame(const SimulationFramePointer& f);

    // thread-safe
    //
    ParameterCollection* parameters;
    List<std::shared_ptr<VoiceData>> voiceData{};

    // context: audio thread
    //
    FrameList frameBuffer;
    size_t frameBufferFirstFrame = 0;
    Eigen::ArrayX<Decimal> frameBufferTimestamps;

    void appendFrameBuffer(const FrameList& frames);
    void resetFrameBuffer();

private:

    // context: audio thread <-> GUI thread
    //
    SimulationFramePointer simulationDisplayFrame{nullptr};
    std::mutex frameAccessMutex;
};

#endif //DATA_H
