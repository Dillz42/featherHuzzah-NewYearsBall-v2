// SpoolMotor.h

#ifndef _SPOOLMOTOR_h
#define _SPOOLMOTOR_h
//#include <WebServer.h>
#include "NeopixelBall.h"

class NeopixelBall;

class SpoolMotor
{
 public:
	void init(NeopixelBall* neopixelBall);
	void loop();

	enum Action
	{
		Stop, Release, Forward, Backward
	};

	enum Step
	{
		Single = 1,
		Double = 2,
		Interleave = 3,
		Microstep = 4
	};

	bool isMotorShieldFound = false;

	void setCurrentAction(Action action);
	void setCurrentStep(Step step);

	void SetServerEndpoints(/*ESP8266*/WebServer* server);

	void handleWebForward(/*ESP8266*/WebServer* server);
	void handleWebBackward(/*ESP8266*/WebServer* server);
	void handleWebStop(/*ESP8266*/WebServer* server);
	void handleWebRelease(/*ESP8266*/WebServer* server);
	void handleWebStepMode(/*ESP8266*/WebServer* server);
	void handleWebAcceleration(/*ESP8266*/WebServer* server);
	void handleWebSpeed(/*ESP8266*/WebServer* server);

 protected:
	
};

#endif

