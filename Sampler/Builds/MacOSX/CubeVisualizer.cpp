#include "CubeVisualizer.h"

void CubeVisualizer::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds());

    for (const auto& [name, position] : sources)
    {
        g.setColour(juce::Colours::red);
        g.fillEllipse(position.x, position.y, 10, 10);
    }
}

void CubeVisualizer::addSource(const juce::String& name)
{
    sources[name] = Position3D(50, 50, 0); // Default position
    repaint();
}
