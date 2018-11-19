#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class MotuWebApi
{
public:
    bool setVolume(String subtree, float value) const;
    bool setPanning(String subtree, float value) const;
    void setHostname(String hostname);

    MotuWebApi();
    ~MotuWebApi();

private:
    String hostname;

    bool postJson(const String& json) const;
};

