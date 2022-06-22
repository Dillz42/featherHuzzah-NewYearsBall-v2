#include <Adafruit_MotorShield.h>
#include <AccelStepper.h>

#include "SpoolMotor.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor* spoolStepperMotor = AFMS.getStepper(200, 1);

SpoolMotor::Action currentAction = SpoolMotor::Action::Release;
uint8_t currentStep = SpoolMotor::Step::Double;

static SpoolMotor* instance;

void spoolForwardStep();
void spoolBackwardStep();

AccelStepper SpoolStepperAccel(spoolForwardStep, spoolBackwardStep); // use functions to step

void spoolForwardStep()
{
	if (instance->isMotorShieldFound)
	{
		spoolStepperMotor->onestep(FORWARD, currentStep);
	}
}
void spoolBackwardStep()
{
	if (instance->isMotorShieldFound)
	{
		spoolStepperMotor->onestep(BACKWARD, currentStep);
	}
}

void SpoolMotor::init(NeopixelBall* neopixelBall)
{
	instance = this;

	neopixelBall->setPixelColor(1, neopixelBall->Color(0, 0, 255));
	neopixelBall->show();

	// create with the default frequency 1.6KHz
	if (!AFMS.begin()) 
	{
		neopixelBall->setPixelColor(1, neopixelBall->Color(255, 0, 0));
		neopixelBall->show();
		Serial.println("Could not find Motor Shield. Check wiring.");
		isMotorShieldFound = false;
	}
	else
	{
		neopixelBall->setPixelColor(1, neopixelBall->Color(0, 255, 0));
		neopixelBall->show();
		Serial.println("Motor Shield found.");
		isMotorShieldFound = true;
	}

	SpoolStepperAccel.setMaxSpeed(400);
	SpoolStepperAccel.setSpeed(400);
	SpoolStepperAccel.setAcceleration(10000);
	SpoolStepperAccel.setCurrentPosition(0);

	currentAction = Release;
}

void SpoolMotor::loop()
{
	switch (currentAction)
	{
	case Stop:
		SpoolStepperAccel.stop();
		delay(10);
		break;
	case Release:
		SpoolStepperAccel.disableOutputs();
		spoolStepperMotor->release();
		break;
	case Forward:
		SpoolStepperAccel.run();
		break;
	case Backward:
		SpoolStepperAccel.run();
		break;
	}
}

void SpoolMotor::setCurrentAction(SpoolMotor::Action action)
{
	/*Serial.printf("Current action: %d\tNew Action: %d\tPosition: %d\tTarget: %d\n",
		currentAction, action, SpoolStepperAccel.currentPosition(), SpoolStepperAccel.targetPosition());*/
	if (currentAction != action)
	{
		currentAction = action;

		SpoolStepperAccel.setCurrentPosition(0);

		switch (action)
		{
		case SpoolMotor::Forward:
			SpoolStepperAccel.move(99999);
			break;
		case SpoolMotor::Backward:
			SpoolStepperAccel.move(-99999);
			break;
		}
	}
}

void SpoolMotor::setCurrentStep(SpoolMotor::Step step)
{
	currentStep = step;
}

void SpoolMotor::SetServerEndpoints(/*ESP8266*/WebServer* server)
{
	server->on("/motor/forward", std::bind(&SpoolMotor::handleWebForward, this, server));
	server->on("/motor/backward", std::bind(&SpoolMotor::handleWebBackward, this, server));
	server->on("/motor/stop", std::bind(&SpoolMotor::handleWebStop, this, server));
	server->on("/motor/release", std::bind(&SpoolMotor::handleWebRelease, this, server));
	server->on("/motor/stepMode", std::bind(&SpoolMotor::handleWebStepMode, this, server));
	server->on("/motor/acceleration", std::bind(&SpoolMotor::handleWebAcceleration, this, server));
	server->on("/motor/speed", std::bind(&SpoolMotor::handleWebSpeed, this, server));
}

void SpoolMotor::handleWebForward(/*ESP8266*/WebServer* server)
{
	Serial.printf("Forward with speed %d\n", server->arg("plain").toInt());
	setCurrentAction(Action::Forward);
	SpoolStepperAccel.setMaxSpeed(server->arg("plain").toInt());
	server->send(200, "text/html", server->arg("plain"));
}
void SpoolMotor::handleWebBackward(/*ESP8266*/WebServer* server)
{
	Serial.printf("Backward with speed %d\n", server->arg("plain").toInt());
	setCurrentAction(Action::Backward);
	SpoolStepperAccel.setMaxSpeed(server->arg("plain").toInt());
	server->send(200, "text/html", server->arg("plain"));
}
void SpoolMotor::handleWebStop(/*ESP8266*/WebServer* server)
{
	Serial.printf("Motor stop\n");
	setCurrentAction(Action::Stop);
	server->send(200, "text/html", server->arg("plain"));
}
void SpoolMotor::handleWebRelease(/*ESP8266*/WebServer* server)
{
	Serial.printf("Motor release\n");
	setCurrentAction(Action::Release);
	server->send(200, "text/html", server->arg("plain"));
}
void SpoolMotor::handleWebStepMode(/*ESP8266*/WebServer* server)
{
	String newStep = server->arg("plain");
	Serial.printf("New Step: %s\n", newStep);

	if (newStep == "Microstep")
	{
		currentStep = SpoolMotor::Step::Microstep;
	}
	else if (newStep == "Single Step")
	{
		currentStep = SpoolMotor::Step::Single;
	}
	else if (newStep == "Double Step")
	{
		currentStep = SpoolMotor::Step::Double;
	}
	else if (newStep == "Interweave")
	{
		currentStep = SpoolMotor::Step::Interleave;
	}

	server->send(200, "text/html", server->arg("plain"));
}
void SpoolMotor::handleWebAcceleration(/*ESP8266*/WebServer* server)
{
	Serial.printf("New Acceleration %d\n", server->arg("plain").toInt());
	SpoolStepperAccel.setAcceleration(server->arg("plain").toInt());
	server->send(200, "text/html", server->arg("plain"));
}
void SpoolMotor::handleWebSpeed(/*ESP8266*/WebServer* server)
{
	Serial.printf("New speed %s\n", server->arg("plain").c_str());
	SpoolStepperAccel.setMaxSpeed(server->arg("plain").toInt());
	server->send(200, "text/html", server->arg("plain"));
}
