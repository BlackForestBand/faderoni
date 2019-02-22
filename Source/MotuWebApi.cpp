#include "MotuWebApi.h"


//==============================================================================
MotuWebApi::MotuWebApi()
{
}

MotuWebApi::~MotuWebApi()
{
}

//==============================================================================
bool MotuWebApi::setVolume(const String& subtree, float value) const
{
    return postJson("{ \"" + subtree + "/fader\": " + String(value, 4) + " }");
}

bool MotuWebApi::setEq(const String& subtree, float value) const
{
    //ex: {"mix/aux/6/eq/highshelf/freq":2884}
    return postJson("{ \"" + subtree + "/eq/highshelf/freq\": " + String(value,0) + " }");
}

bool MotuWebApi::setPanning(const String& subtree, float value) const
{
    return postJson("{ \"" + subtree + "/pan\": " + String(value, 4) + " }");
}

void MotuWebApi::setHostname(const String hostname)
{
    this->hostname = hostname;
}

void MotuWebApi::setTimeout(const int& timeout)
{
    this->timeout = timeout;
}

bool MotuWebApi::postJson(const String& json) const
{
    URL url = URL("http://" + hostname + "/datastore")
        .withPOSTData("json=" + json);

    StringPairArray responseHeaders;
    auto statusCode = 0;

    ScopedPointer<InputStream> stream(
        url.createInputStream(true, nullptr, nullptr, { "Content-Type: application/x-www-form-urlencoded" },
            timeout, &responseHeaders, &statusCode));

    if (stream == nullptr)
        return false;
        
    return statusCode == 204;   
}
