#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class MotuWebApi
{
public:
    bool setVolume(int channel, float value) const;
    void setHostname(String hostname);

    MotuWebApi();
    ~MotuWebApi();

private:
    String hostname;

    bool postJson(const String& json) const;
};

