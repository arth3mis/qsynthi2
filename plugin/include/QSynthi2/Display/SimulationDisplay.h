#ifndef SIMULATIONDISPLAY_H
#define SIMULATIONDISPLAY_H

#include <QSynthi2/Juce.h>

class SimulationDisplay : public juce::Component, juce::Timer {
public:
    SimulationDisplay();
    ~SimulationDisplay();

    void paint (juce::Graphics&) override;
    void resized() override;

    void timerCallback() override;

private:
    void drawSimulation(juce::Graphics& g);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimulationDisplay)
};

#endif //SIMULATIONDISPLAY_H
