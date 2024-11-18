#pragma once

#include <JuceHeader.h>
#include <map>

struct Position3D
{
    float x, y, z;
    Position3D(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
};

class CubeVisualizer : public juce::Component
{
public:
    void paint(juce::Graphics& g) override;
    void addSource(const juce::String& name);

private:
    std::map<juce::String, Position3D> sources;
};
