#include "SpoolMotor.h"
#include "LittleFS.h"
#include "NeopixelBall.h"
#include "SpoolMotor.h"
#include "NeopixelBallWiFi.h"
#include "NewYearsBall_Time.h"

NeopixelBall neopixelBall;
SpoolMotor spoolMotor;
NeopixelBallWiFi wifi;

TaskHandle_t MotorTask;

void motorLoop(void* pvParameters)
{
	while (true)
	{
		spoolMotor.loop();
	}
}

void setup() {
	Serial.begin(115200);

	Serial.println("~~~ New Years Ball V2 ~~~");

	Serial.println("Neopixel Setup");
	neopixelBall.init();

	Serial.println("Motor Setup");
	spoolMotor.init(&neopixelBall);
	((Effect_NYE_Countdown*)neopixelBall.effectList[NeopixelBall::BallMode::NYE_Countdown])
		->spoolMotor = &spoolMotor;
	xTaskCreatePinnedToCore(motorLoop, "MotorTask", 10000, NULL, 1, &MotorTask, 0);

	Serial.println("WiFi Setup");
	wifi.init(&neopixelBall, &spoolMotor);
	//OTA_setup();

	Serial.println("Setup Complete!");

	neopixelBall.clear();
	neopixelBall.show();
}

int prevSecond = 0;
static int loopCount = 0;

void loop()
{
	loopCount++;
	////Serial.printf("Loop %d\n", loopCount);

	wifi.loop(loopCount);
	neopixelBall.loop(loopCount);
	//spoolMotor.loop();

	if (loopCount % 100 == 50)
	{
		//OTA_loop();
		//wifi.loop();
	}
	else if (loopCount % 200 == 0)
	{
		//neopixelBall.loop(loopCount);
	}
	else if (loopCount % 200 == 25)
	{
		//spoolMotor.loop();
	}

	if (prevSecond != millis() / 1000)
	{
		prevSecond = millis() / 1000;
		//Serial.println(NewYearsBall_Time::uptime());
		/*switch (WiFi.status())
		{
		default:
			break;
		}*/
	}
}

