#ifndef _WIFI_H
#define _WIFI_H
#include "NeopixelBall.h"
#include "SpoolMotor.h"

class NeopixelBallWiFi
{
public:
    void init(NeopixelBall* neopixelBall, SpoolMotor* motor);
    void loop(long loopCount);

private:
    void handleRoot();

    NeopixelBall* neopixelBall;

    SpoolMotor* motor;
};

#endif
