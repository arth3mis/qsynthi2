#ifndef VIDEOSIMULATION_H
#define VIDEOSIMULATION_H

#include "QSynthi2/Simulation/Simulation.h"
#include "QSynthi2/Video.h"
#include <mutex>

typedef Eigen::ArrayXX<cv::Vec3b> VideoMatrix;

class VideoSimulationFrame final : public SimulationFrame {
public:
    explicit VideoSimulationFrame(const VideoMatrix& m) {
        frame = m;
    }
    [[nodiscard]] SimulationFrame* clone() override {
        return new VideoSimulationFrame(frame);
    }
    [[nodiscard]] Decimal toDecimal(long row, long col) const override;

    [[nodiscard]] Decimal toDecimalDisplay(const long row, const long col) const override {
        return toDecimal(row, col);
    }
    [[nodiscard]] Decimal toPhase(const long row, const long col) const override {
        return frame(row, col)[1] / 255.0;
    }
    [[nodiscard]] size_t cols() const override {
        return frame.cols();
    }
    [[nodiscard]] size_t rows() const override {
        return frame.rows();
    }
private:
    VideoMatrix frame;
};

class VideoSimulation : public Simulation {
public:

    VideoSimulation(int targetWidth, int targetHeight, bool autoSizeIfImage, const juce::String& filename);
    VideoSimulation(const VideoSimulation& s);
    ~VideoSimulation() override;

    void reset() override;
    void updateParameters(const ParameterCollection *p, const List<ModulationData*> &m) override;

    // getters
    SimulationFramePointer getStartFrame() override;
    SimulationFramePointer getNextFrame(Decimal timestep, const ModulationData& modulationData) override;
    bool isContinuous() override;
    bool isStationary() override;
    int getWidth() override { return simulationWidth; }
    int getHeight() override { return simulationHeight; }

    [[nodiscard]] bool captureOpened() const;
    [[nodiscard]] int videoFps() const;

private:

    int simulationWidth;
    int simulationHeight;
    bool autoSize;
    juce::String file;
    cv::VideoCapture capture;
    std::mutex captureMutex;
    bool isCam;
    bool isSingleFrame;
    List<VideoSimulationFrame> frames;
    Decimal currentFrameIndex;

    bool convertNextVideoFrame();
};



#endif //VIDEOSIMULATION_H
