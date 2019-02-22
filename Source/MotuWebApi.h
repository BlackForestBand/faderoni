#pragma once
#include "FaderoniConstants.h"
#include "../JuceLibraryCode/JuceHeader.h"

class MotuWebApi
{
public:
    bool setVolume(const String& subtree, float value) const;
    bool setPanning(const String& subtree, float value) const;
    bool setEq(const String& subtree, float value) const;
    void setHostname(String hostname);
    void setTimeout(const int& timeout);

    MotuWebApi();
    ~MotuWebApi();

private:
    String hostname;
    int timeout; // timeout in millisecs

    bool postJson(const String& json) const;
};

