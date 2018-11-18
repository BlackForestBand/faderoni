#include "MotuWebApi.h"


MotuWebApi::MotuWebApi()
{
}

MotuWebApi::~MotuWebApi()
{
}

bool MotuWebApi::setVolume(int channel, float value) const
{
    return postJson("{ \"mix/chan/" + String(channel) + "/matrix/fader\": " + String(value, 10) + " }");
}

void MotuWebApi::setHostname(String hostname)
{
    this->hostname = hostname;
}

bool MotuWebApi::postJson(const String& json) const
{
    URL url = URL("http://" + hostname + "/datastore")
        .withPOSTData("json=" + json);

    StringPairArray responseHeaders;
    int statusCode = 0;

    ScopedPointer<InputStream> stream(
        url.createInputStream(true, nullptr, nullptr, { "Content-Type: application/x-www-form-urlencoded" },
        10000, // timeout in millisecs
        &responseHeaders, &statusCode));

    if (stream == nullptr)
        return false;

    auto debug = stream->readString();

    return statusCode == 204;
}
