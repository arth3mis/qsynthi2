#include "QSynthi2/Synthesizer/Sonifier.h"


Sonifier::Sonifier() = default;



Eigen::ArrayX<Decimal> Sonifier::generateNextBlock(const ModulationData &modulationData) {
    jassert(modulationData.contains(Modulation::Sources::PITCH)); // Pitch isn't already evaluated
    auto frequency = modulationData.at(Modulation::Sources::PITCH);
    jassert(frequency.size() == samplesPerBlock); // Pitch wasn't set properly

    auto phases = Eigen::ArrayX<Decimal>(samplesPerBlock);

    for(int i = 0; i < phases.size(); i++) {
        phase0to1 += frequency[i] / sampleRate;
        phase0to1 = fmod(phase0to1, 1);

        phases[i] = phase0to1;
    }

    return scanner.getValuesAt(phases, modulationData);
}



void Sonifier::prepareToPlay(Decimal newSampleRate, int samplesPerBlock) {
    sampleRate = newSampleRate;
    this->samplesPerBlock = samplesPerBlock;

    scanner.prepareToPlay(newSampleRate);
}



void Sonifier::restart() {
    phase0to1 = 0;
    scanner.restart();
}