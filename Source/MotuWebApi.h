#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class MotuWebApi
{
public:
    bool setVolume(const String& subtree, float value) const;
    bool setPanning(const String& subtree, float value) const;
    void setHostname(String hostname);

    MotuWebApi();
    ~MotuWebApi();

private:
    String hostname;

    bool postJson(const String& json) const;
};

