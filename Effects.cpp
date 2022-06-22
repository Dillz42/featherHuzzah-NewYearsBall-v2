#include "NeopixelBall.h"
#include <Preferences.h>
#include <vector>
#include <mutex>

static Preferences preferences;
int Effect_SinglePulse::hue = -1;
int Effect_SinglePulse::brightness = -1;
int Effect_SinglePulse::color_r = -1;
int Effect_SinglePulse::color_g = -1;
int Effect_SinglePulse::color_b = -1;

int Effect_Off::run(long loopCount)
{
	clear();
	return 0;
}

int Effect_NYE_Countdown::init()
{
	//TODO: Set midnightMillis to actual midnight
	midnightMillis = millis() + 210000;
	prev_NYE_mode = NeopixelBall::BallMode::OFF;
	NYE_mode = NeopixelBall::BallMode::OFF;
	return 0;
}
int Effect_NYE_Countdown::cleanup()
{
	midnightMillis = 2147483647;
	prev_NYE_mode = NeopixelBall::BallMode::OFF;
	NYE_mode = NeopixelBall::BallMode::OFF;
	return 0;
}
int Effect_NYE_Countdown::run(long loopCount)
{
	SetModeBasedOnTime();

	neopixelBall->effectList[NYE_mode]->run(loopCount);

	return 0;
}
void Effect_NYE_Countdown::SetModeBasedOnTime()
{
	long millisToMidnight = midnightMillis - millis();

	if (millisToMidnight > 190000)
	{
		static bool firstCall = true;
		if (firstCall)
		{
			firstCall = false;
			prev_NYE_mode = NeopixelBall::BallMode::OFF;
			NYE_mode = NeopixelBall::BallMode::RandomFillRepeating;
			neopixelBall->effectDelay = 5;
		}
	}
	else if (millisToMidnight > 186000)
	{
		static int callCount = 0;
		if (callCount == 0)
		{
			callCount++;
			NYE_mode = NeopixelBall::BallMode::OFF;
		}
		else if (callCount == 1)
		{
			callCount++;
			NYE_mode = NeopixelBall::BallMode::SinglePulse;
			Effect_SinglePulse::hue = 1;
			Effect_SinglePulse::brightness = 255;
			neopixelBall->effectDelay = Effect_SinglePulse::delayFromRunTime(4000);
		}
	}
	else if (millisToMidnight > 185000)
	{
		static int callCount = 0;
		if (callCount == 0)
		{
			callCount++;
			NYE_mode = NeopixelBall::BallMode::OFF;
		}
		else if (callCount == 1)
		{
			callCount++;
			NYE_mode = NeopixelBall::BallMode::SinglePulse;
			Effect_SinglePulse::hue = 21845;
			Effect_SinglePulse::brightness = 128;
			neopixelBall->effectDelay = Effect_SinglePulse::delayFromRunTime(1000);
		}
	}
	else if (millisToMidnight > 184000)
	{
		static int callCount = 0;
		if (callCount == 0)
		{
			callCount++;
			NYE_mode = NeopixelBall::BallMode::OFF;
		}
		else if (callCount == 1)
		{
			callCount++;
			NYE_mode = NeopixelBall::BallMode::SinglePulse;
			Effect_SinglePulse::hue = 43691;
			Effect_SinglePulse::brightness = 64;
			neopixelBall->effectDelay = Effect_SinglePulse::delayFromRunTime(1000);
		}
	}
	/*else if (millisToMidnight > 182000)
	{
		
	}
	else if (millisToMidnight > 181000)
	{
		static bool firstCall = true;
		if (firstCall)
		{
			firstCall = false;
			prev_NYE_mode = NeopixelBall::BallMode::OFF;
			NYE_mode = NeopixelBall::BallMode::SinglePulse;
			neopixelBall->effectDelay = delayFromRunTime(1000);
		}
	}*/
	else if (millisToMidnight > 120000) //More than 2 minutes
	{
		static bool firstCall = true;
		if (firstCall)
		{
			firstCall = false;
			if (NYE_mode != NeopixelBall::BallMode::RandomSnake)
			{
				NYE_mode = NeopixelBall::BallMode::RandomSnake;
				neopixelBall->effectDelay = 50;
			}
		}
	}
	else if (millisToMidnight > 60000) //More than 60 seconds
	{
		static int callCount = 0;
		if (callCount == 0)
		{
			callCount++;
			NYE_mode = NeopixelBall::BallMode::OFF;
		}
		else if(callCount == 1)
		{
			callCount++;
			if (NYE_mode != NeopixelBall::BallMode::RandomGlow)
			{
				NYE_mode = NeopixelBall::BallMode::RandomGlow;
				neopixelBall->effectDelay = 1000;
				neopixelBall->effectList[NYE_mode]->updateArgs("glowingPixelCount", "20");
			}
		}
	}
	else if (millisToMidnight > 10000) //More than 10 seconds
	{
		static int callCount = 0;
		if (callCount == 0)
		{
			callCount++;
			NYE_mode = NeopixelBall::BallMode::OFF;
		}
		else if(callCount == 1)
		{
			callCount++;
			if (NYE_mode != NeopixelBall::BallMode::RandomGlow)
			{
				NYE_mode = NeopixelBall::BallMode::RandomGlow;
				neopixelBall->effectDelay = 1000;
				neopixelBall->effectList[NYE_mode]->updateArgs("glowingPixelCount", "100");
			}
		}
	}
	else if (millisToMidnight > 9000)
	{
		static int callCount = 0;
		if (callCount == 0)
		{
			callCount++;
			NYE_mode = NeopixelBall::BallMode::OFF;
		}
		else if(callCount == 1)
		{
			callCount++;
			prev_NYE_mode = NeopixelBall::BallMode::OFF;
			NYE_mode = NeopixelBall::BallMode::SinglePulse;
			neopixelBall->effectDelay = Effect_SinglePulse::delayFromRunTime(1000);
		}
	}
	else if (millisToMidnight > 8000)
	{
		static int callCount = 0;
		if (callCount == 0)
		{
			callCount++;
			NYE_mode = NeopixelBall::BallMode::OFF;
		}
		else if(callCount == 1)
		{
			callCount++;
			prev_NYE_mode = NeopixelBall::BallMode::OFF;
			NYE_mode = NeopixelBall::BallMode::SinglePulse;
			neopixelBall->effectDelay = Effect_SinglePulse::delayFromRunTime(1000);
		}
	}
	else if (millisToMidnight > 7000)
	{
		static int callCount = 0;
		if (callCount == 0)
		{
			callCount++;
			NYE_mode = NeopixelBall::BallMode::OFF;
		}
		else if(callCount == 1)
		{
			callCount++;
			prev_NYE_mode = NeopixelBall::BallMode::OFF;
			NYE_mode = NeopixelBall::BallMode::SinglePulse;
			neopixelBall->effectDelay = Effect_SinglePulse::delayFromRunTime(1000);
		}
	}
	else if (millisToMidnight > 6000)
	{
		static int callCount = 0;
		if (callCount == 0)
		{
			callCount++;
			NYE_mode = NeopixelBall::BallMode::OFF;
		}
		else if(callCount == 1)
		{
			callCount++;
			prev_NYE_mode = NeopixelBall::BallMode::OFF;
			NYE_mode = NeopixelBall::BallMode::SinglePulse;
			neopixelBall->effectDelay = Effect_SinglePulse::delayFromRunTime(1000);
		}
	}
	else if (millisToMidnight > 5000)
	{
		static int callCount = 0;
		if (callCount == 0)
		{
			callCount++;
			NYE_mode = NeopixelBall::BallMode::OFF;
		}
		else if(callCount == 1)
		{
			callCount++;
			prev_NYE_mode = NeopixelBall::BallMode::OFF;
			NYE_mode = NeopixelBall::BallMode::SinglePulse;
			neopixelBall->effectDelay = Effect_SinglePulse::delayFromRunTime(1000);
		}
	}
	else if (millisToMidnight > 4000)
	{
		static int callCount = 0;
		if (callCount == 0)
		{
			callCount++;
			NYE_mode = NeopixelBall::BallMode::OFF;
		}
		else if(callCount == 1)
		{
			callCount++;
			prev_NYE_mode = NeopixelBall::BallMode::OFF;
			NYE_mode = NeopixelBall::BallMode::SinglePulse;
			neopixelBall->effectDelay = Effect_SinglePulse::delayFromRunTime(1000);
		}
	}
	else if (millisToMidnight > 3000)
	{
		static int callCount = 0;
		if (callCount == 0)
		{
			callCount++;
			NYE_mode = NeopixelBall::BallMode::OFF;
		}
		else if(callCount == 1)
		{
			callCount++;
			prev_NYE_mode = NeopixelBall::BallMode::OFF;
			NYE_mode = NeopixelBall::BallMode::SinglePulse;
			neopixelBall->effectDelay = Effect_SinglePulse::delayFromRunTime(1000);
		}
	}
	else if (millisToMidnight > 2000)
	{
		static int callCount = 0;
		if (callCount == 0)
		{
			callCount++;
			NYE_mode = NeopixelBall::BallMode::OFF;
		}
		else if(callCount == 1)
		{
			callCount++;
			prev_NYE_mode = NeopixelBall::BallMode::OFF;
			NYE_mode = NeopixelBall::BallMode::SinglePulse;
			neopixelBall->effectDelay = Effect_SinglePulse::delayFromRunTime(1000);
		}
	}
	else if (millisToMidnight > 1000)
	{
		static int callCount = 0;
		if (callCount == 0)
		{
			callCount++;
			NYE_mode = NeopixelBall::BallMode::OFF;
		}
		else if(callCount == 1)
		{
			callCount++;
			prev_NYE_mode = NeopixelBall::BallMode::OFF;
			NYE_mode = NeopixelBall::BallMode::SinglePulse;
			neopixelBall->effectDelay = Effect_SinglePulse::delayFromRunTime(1000);
		}
	}
	else if (millisToMidnight > 0) //Last bit before midnight
	{
		static int callCount = 0;
		if (callCount == 0)
		{
			callCount++;
			NYE_mode = NeopixelBall::BallMode::OFF;
		}
		else if(callCount == 1)
		{
			callCount++;
			prev_NYE_mode = NeopixelBall::BallMode::OFF;
			NYE_mode = NeopixelBall::BallMode::SinglePulse;
			Effect_SinglePulse::color_r = 128;
			Effect_SinglePulse::color_g = 128;
			Effect_SinglePulse::color_b = 128;
			neopixelBall->effectDelay = Effect_SinglePulse::delayFromRunTime(1000);
		}
	}
	else if (millisToMidnight > -250) // MIDNIGHT!!!
	{
		static bool firstCall = true;
		if (firstCall)
		{
			firstCall = false;
			prev_NYE_mode = NeopixelBall::BallMode::OFF;
			NYE_mode = NeopixelBall::BallMode::SinglePulse;
			Effect_SinglePulse::color_r = 255;
			Effect_SinglePulse::color_g = 255;
			Effect_SinglePulse::color_b = 255;
			neopixelBall->effectDelay = Effect_SinglePulse::delayFromRunTime(500);
		}
	}
	else if (millisToMidnight > -15000)
	{
		static bool firstCall = true;
		if (firstCall)
		{
			firstCall = false;
			if (NYE_mode != NeopixelBall::BallMode::RandomBrightGlow)
			{
				NYE_mode = NeopixelBall::BallMode::RandomBrightGlow;
				neopixelBall->effectDelay = 250;
				neopixelBall->effectList[NYE_mode]->updateArgs("glowingPixelCount", "45");
			}
		}
	}
	else // END OF SEQUENCE
	{
		static bool firstCall = true;
		if (firstCall)
		{
			firstCall = false;
			if (NYE_mode != NeopixelBall::BallMode::Rainbow)
			{
				NYE_mode = NeopixelBall::BallMode::Rainbow;
			}
		}
	}

	if (prev_NYE_mode != NYE_mode)
	{
		//Serial.println("Cleaning up old mode");
		neopixelBall->effectList[prev_NYE_mode]->cleanup();
		//Serial.println("Setting previous mode to current mode");
		prev_NYE_mode = NYE_mode;
		//Serial.println("new mode init");
		neopixelBall->effectList[NYE_mode]->init();
	}
}

int Effect_Rainbow::run(long loopCount)
{
	float timestamp = millis() * (4096 / (float)neopixelBall->effectDelay);

	int hueStep = 65536 / 9;
	int currentHue = ((int)timestamp) % 65536;

	fillRing(ColorHSV(currentHue), NeopixelBall::BallRing::ForthUpper, rainbowAxis);
	currentHue += hueStep;
	fillRing(ColorHSV(currentHue), NeopixelBall::BallRing::ThirdUpper, rainbowAxis);
	currentHue += hueStep;
	fillRing(ColorHSV(currentHue), NeopixelBall::BallRing::SecondUpper, rainbowAxis);
	currentHue += hueStep;
	fillRing(ColorHSV(currentHue), NeopixelBall::BallRing::FirstUpper, rainbowAxis);
	currentHue += hueStep;
	fillRing(ColorHSV(currentHue), NeopixelBall::BallRing::Middle, rainbowAxis);
	currentHue += hueStep;
	fillRing(ColorHSV(currentHue), NeopixelBall::BallRing::FirstLower, rainbowAxis);
	currentHue += hueStep;
	fillRing(ColorHSV(currentHue), NeopixelBall::BallRing::SecondLower, rainbowAxis);
	currentHue += hueStep;
	fillRing(ColorHSV(currentHue), NeopixelBall::BallRing::ThirdLower, rainbowAxis);
	currentHue += hueStep;
	fillRing(ColorHSV(currentHue), NeopixelBall::BallRing::ForthLower, rainbowAxis);
	currentHue += hueStep;

	return 0;
}

int Effect_LedFinder::init()
{
	preferences.begin("Effect_LedFinder", false);
	ledFinderIndex = preferences.getInt("ledFinderIndex");
	ledFinderDepth = preferences.getInt("ledFinderDepth");
	preferences.end();
	return 0;
}
int Effect_LedFinder::cleanup()
{
	preferences.begin("Effect_LedFinder", false);
	preferences.putInt("ledFinderIndex", ledFinderIndex);
	preferences.putInt("ledFinderDepth", ledFinderDepth);
	preferences.end();
	return 0;
}
int Effect_LedFinder::run(long loopCount)
{
	clear();

	for (int i = ledFinderDepth; i >= 1; i--)
	{
		neopixelBall->setNeighborPixels(&neopixelBall->pixels[ledFinderIndex], i);
	}
	/*setPixelColor(pixels[LedFinderIndex].neighbors[0]->index, 255, 0, 0);
	setPixelColor(pixels[LedFinderIndex].neighbors[0]->neighbors[0]->index, 0, 0, 255);
	setPixelColor(pixels[LedFinderIndex].neighbors[0]->neighbors[1]->index, 0, 0, 255);
	setPixelColor(pixels[LedFinderIndex].neighbors[0]->neighbors[2]->index, 0, 0, 255);

	setPixelColor(pixels[LedFinderIndex].neighbors[1]->index, 255, 0, 0);
	setPixelColor(pixels[LedFinderIndex].neighbors[1]->neighbors[0]->index, 255, 0, 0);
	setPixelColor(pixels[LedFinderIndex].neighbors[1]->neighbors[1]->index, 0, 0, 255);
	setPixelColor(pixels[LedFinderIndex].neighbors[1]->neighbors[2]->index, 0, 0, 255);

	setPixelColor(pixels[LedFinderIndex].neighbors[2]->index, 255, 0, 0);
	setPixelColor(pixels[LedFinderIndex].neighbors[2]->neighbors[0]->index, 0, 0, 255);
	setPixelColor(pixels[LedFinderIndex].neighbors[2]->neighbors[1]->index, 0, 0, 255);
	setPixelColor(pixels[LedFinderIndex].neighbors[2]->neighbors[2]->index, 0, 0, 255);*/

	setPixelColor(ledFinderIndex, Color(255, 255, 255));

	return 0;
}
void NeopixelBall::setNeighborPixels(NeopixelBall::Pixel* pixel, int depth, int distance)
{
	if (depth > 1)
	{
		for (NeopixelBall::Pixel* neighbor : pixel->neighbors)
		{
			setNeighborPixels(neighbor, depth - 1, distance + 1);
		}
	}
	
	if (true)
	{
		switch (distance)
		{
		case 8:
			setPixelColor(pixel->neighbors[0]->index, Color(64, 0, 0));
			setPixelColor(pixel->neighbors[1]->index, Color(64, 0, 0));
			setPixelColor(pixel->neighbors[2]->index, Color(64, 0, 0));
			break;
		case 7:
			setPixelColor(pixel->neighbors[0]->index, Color(0, 64, 0));
			setPixelColor(pixel->neighbors[1]->index, Color(0, 64, 0));
			setPixelColor(pixel->neighbors[2]->index, Color(0, 64, 0));
			break;
		case 6:
			setPixelColor(pixel->neighbors[0]->index, Color(0, 0, 64));
			setPixelColor(pixel->neighbors[1]->index, Color(0, 0, 64));
			setPixelColor(pixel->neighbors[2]->index, Color(0, 0, 64));
			break;
		case 5:
			setPixelColor(pixel->neighbors[0]->index, Color(0, 0, 64));
			setPixelColor(pixel->neighbors[1]->index, Color(0, 0, 64));
			setPixelColor(pixel->neighbors[2]->index, Color(0, 0, 64));
			break;
		case 4:
			setPixelColor(pixel->neighbors[0]->index, Color(64, 64, 64));
			setPixelColor(pixel->neighbors[1]->index, Color(64, 64, 64));
			setPixelColor(pixel->neighbors[2]->index, Color(64, 64, 64));
			break;
		case 3:
			setPixelColor(pixel->neighbors[0]->index, Color(0, 0, 128));
			setPixelColor(pixel->neighbors[1]->index, Color(0, 0, 128));
			setPixelColor(pixel->neighbors[2]->index, Color(0, 0, 128));
			break;
		case 2:
			setPixelColor(pixel->neighbors[0]->index, Color(128, 0, 0));
			setPixelColor(pixel->neighbors[1]->index, Color(128, 0, 0));
			setPixelColor(pixel->neighbors[2]->index, Color(128, 0, 0));
			break;
		case 1:
			setPixelColor(pixel->neighbors[0]->index, Color(0, 128, 0));
			setPixelColor(pixel->neighbors[1]->index, Color(0, 128, 0));
			setPixelColor(pixel->neighbors[2]->index, Color(0, 128, 0));
			break;
		default:
			setPixelColor(pixel->neighbors[0]->index, Color(128, 128, 128));
			setPixelColor(pixel->neighbors[1]->index, Color(128, 128, 128));
			setPixelColor(pixel->neighbors[2]->index, Color(128, 128, 128));
			break;
		}
	}
}

int Effect_Counter::init()
{
	preferences.begin("Effect_Counter", false);
	currentCount = preferences.getInt("currentCount", 10);
	preferences.end();
	Serial.printf("CurrentCount: %d\n", currentCount);
	return 0;
}
int Effect_Counter::cleanup()
{
	preferences.begin("Effect_Counter", false);
	preferences.putInt("currentCount", currentCount);
	preferences.end();
	return 0;
}
int Effect_Counter::run(long loopCount)
{
	clear();

	fill(Color(255, 255, 255), 0, currentCount + 1);

	return 0;
}

int Effect_Pentagons::run(long loopCount)
{
	fill(Color(0, 0, 0));
	for (size_t i = 0; i < 180; i++)
	{
		//*((bool*)pixels[i].effectDataMap[Pentagons]) = false;
	}

	float timestamp = millis() / (float)neopixelBall->effectDelay;

	float color1 = (sin(timestamp) + 1) * 128;//abs((int)((millis() / 64)%256) - 128);
	//float color2 = (sin(timestamp + 0.41887902047863909846168578443727) + 1) * 128;//abs((int)((millis() / 64)%256) - 128);
	//float color3 = (sin(timestamp + 0.83775804095727819692337156887453) + 1) * 128;//abs((int)((millis() / 64)%256) - 128);
	//float color4 = (sin(timestamp + 1.2566370614359172953850573533118) + 1) * 128;//abs((int)((millis() / 64)%256) - 128);
	//float color5 = (sin(timestamp + 1.6755160819145563938467431377491) + 1) * 128;//abs((int)((millis() / 64)%256) - 128);
	float color6 = (sin(timestamp + 2.0943951023931954923084289221863) + 1) * 128;//abs((int)((millis() / 64)%256) - 128);
	//float color7 = (sin(timestamp + 2.5132741228718345907701147066236) + 1) * 128;//abs((int)((millis() / 64)%256) - 128);
	//float color8 = (sin(timestamp + 2.9321531433504736892318004910609) + 1) * 128;//abs((int)((millis() / 64)%256) - 128);
	//float color9 = (sin(timestamp + 3.3510321638291127876934862754981) + 1) * 128;//abs((int)((millis() / 64)%256) - 128);
	//float color10 = (sin(timestamp + 3.7699111843077518861551720599354) + 1) * 128;//abs((int)((millis() / 64)%256) - 128);
	float color11 = (sin(timestamp + 4.1887902047863909846168578443727) + 1) * 128;//abs((int)((millis() / 64)%256) - 128);
	//float color12 = (sin(timestamp + 4.6076692252650300830785436288099) + 1) * 128;//abs((int)((millis() / 64)%256) - 128);
	//float color13 = (sin(timestamp + 5.0265482457436691815402294132472) + 1) * 128;//abs((int)((millis() / 64)%256) - 128);
	//float color14 = (sin(timestamp + 5.4454272662223082800019151976845) + 1) * 128;//abs((int)((millis() / 64)%256) - 128);
	//float color15 = (sin(timestamp + 5.8643062867009473784636009821217) + 1) * 128;//abs((int)((millis() / 64)%256) - 128);

	for (size_t i = 0; i < 12; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			setPixelColor(neopixelBall->rings[i][0][j]->index, Color(color1, color6, color11));
		}
	}

	return 0;
}

int Effect_Stars::run(long loopCount)
{
	fill(Color(0, 0, 128));

	for (int i = 0; i < 180; i++)
	{
		NeopixelBall::Pixel pixel = neopixelBall->pixels[i];
		if (pixel.isPentagon)
		{
			setPixelColor(pixel.index, Color(0, 255, 0));
			setPixelColor(pixel.neighbors[0]->index, Color(0, 255, 0));
			setPixelColor(pixel.neighbors[1]->index, Color(0, 255, 0));
			setPixelColor(pixel.neighbors[2]->index, Color(0, 255, 0));
		}
	}

	return 0;
}

int Effect_MiddleRings::run(long loopCount)
{
	int hue = 0;
	for (int i = 0; i < 6; i++)
	{
		fillRing(ColorHSV(hue), NeopixelBall::BallRing::Middle, i);
		hue += 65536 / 6;
	}

	return 0;
}

int Effect_RandomSnake::init()
{
	//Serial.println("Effect_RandomSnake::init");
	clear();
	snakeHead = &neopixelBall->pixels[0];
	snakeTail = nullptr;
	for (size_t i = 0; i < 7; i++)
	{
		snakeBody[i] = nullptr;
	}

	return 0;
}
int Effect_RandomSnake::cleanup()
{
	//Serial.println("Effect_RandomSnake::cleanup");
	snakeHead = nullptr;
	snakeTail = nullptr;
	for (size_t i = 0; i < 7; i++)
	{
		snakeBody[i] = nullptr;
	}

	return 0;
}
int Effect_RandomSnake::run(long loopCount)
{
	//Serial.println("Effect_RandomSnake::run");
	static long lastUpdate = 0;
	if (millis() - lastUpdate < neopixelBall->effectDelay)
	{
		return 0;
	}
	else
	{
		lastUpdate = millis();
	}

	if (true)
	{
		NeopixelBall::Pixel* nextPixel = snakeHead->neighbors[random() % 3];
		while (getPixelColor(nextPixel->index) != Color(0, 0, 0))
		{
			nextPixel = snakeHead->neighbors[random() % 3];
		}

		snakeTail = snakeBody[6];
		snakeBody[6] = snakeBody[5];
		snakeBody[5] = snakeBody[4];
		snakeBody[4] = snakeBody[3];
		snakeBody[3] = snakeBody[2];
		snakeBody[2] = snakeBody[1];
		snakeBody[1] = snakeBody[0];
		snakeBody[0] = snakeHead;
		snakeHead = nextPixel;

		setPixelColor(snakeHead->index, Color(0, 255, 0));
		if (snakeBody[1] != nullptr)
		{
			setPixelColor(snakeBody[1]->index, Color(0, 255, 255));
		}
		if (snakeTail != nullptr)
		{
			setPixelColor(snakeTail->index, Color(0, 0, 0));
		}
	}

	return 0;
}

int Effect_SinglePulse::init()
{
	//Serial.print("Effect_SinglePulse::init");
	if (color_r <= 0 && color_g <= 0 && color_b <= 0)
	{
		if (Effect_SinglePulse::hue == -1)
		{
			Effect_SinglePulse::hue = random(0, 65536);
		}
		int color = ColorHSV(hue, 255, brightness);
		//Serial.print("\tColor: ");
		//Serial.print(color);
		color_r = (color & 0xFF0000) >> 16;
		color_g = (color & 0xFF00) >> 8;
		color_b = (color & 0xFF);
	}

	//Cut the values in half since the sin function goes from 0 to 2
	color_r /= 2;
	color_g /= 2;
	color_b /= 2;

	startTimestamp = (millis() * PI) / (float)neopixelBall->effectDelay;

	//Serial.print("\thue: ");
	//Serial.print(hue);
	//Serial.print("\tbrightness: ");
	//Serial.print(brightness);
	//Serial.print("\tr: ");
	//Serial.print(color_r);
	//Serial.print("\tg: ");
	//Serial.print(color_g);
	//Serial.print("\tb: ");
	//Serial.print(color_b);
	//Serial.print("\n");

	return 0;
}
int Effect_SinglePulse::cleanup()
{
	//Serial.println("Effect_SinglePulse::cleanup");
	hue = -1;
	brightness = 255;
	color_r = 0;
	color_g = 0;
	color_b = 0;
	return 0;
}
int Effect_SinglePulse::run(long loopCount)
{
	//Serial.printf("Effect_SinglePulse::run\tStartupTime: %d\tCurrentTime: %d\n", startTimestamp, millis());
	clear();
	float timestamp = ((millis() * PI) / (float)neopixelBall->effectDelay) - startTimestamp;

	if (timestamp > 0 && timestamp < (PI * 2))
	{
		float sin_value = (sin(timestamp - 1.57079632679) + 1);
		float value_r = sin_value * color_r;
		float value_g = sin_value * color_g;
		float value_b = sin_value * color_b;
		fill(Color(value_r, value_g, value_b));
	}

	return 0;
}
int Effect_SinglePulse::delayFromRunTime(long runtimeMillis)
{
	return runtimeMillis / 2;
}

int Effect_SingleMovingPulse::init()
{
	Effect_SinglePulse::init();
	//hue = random(0, 65536);
	startTimestamp = millis() / (float)neopixelBall->effectDelay;
	randomAxis = random(0, 11);
	return 0;
}
int Effect_SingleMovingPulse::cleanup()
{
	return 0;
}
int Effect_SingleMovingPulse::run(long loopCount)
{
	clear();
	float timestamp = (millis() / (float)neopixelBall->effectDelay) - startTimestamp;
	//timestamp *= 9;

	if (timestamp > 0 && timestamp < (PI * 2))
	{
		float value = (sin(timestamp - 1.57079632679) + 1) * 128;
		//fillRing(ColorHSV(hue, 255U, value), NeopixelBall::BallRing::ForthUpper, randomAxis);
	}
	if (timestamp > (PI * 0.5) && timestamp < (PI * 2.5))
	{
		float value = (sin(timestamp - 1.57079632679 - (PI * .5)) + 1) * 128;
		//fillRing(ColorHSV(hue, 255U, value), NeopixelBall::BallRing::ThirdUpper, randomAxis);
	}
	if (timestamp > (PI * 1) && timestamp < (PI * 3))
	{
		float value = (sin(timestamp - 1.57079632679 - (PI) * 1) + 1) * 128;
		//fillRing(ColorHSV(hue, 255U, value), NeopixelBall::BallRing::SecondUpper, randomAxis);
	}
	if (timestamp > (PI * 1.5) && timestamp < (PI * 3.5))
	{
		float value = (sin(timestamp - 1.57079632679 - (PI*1.5)) + 1) * 128;
		//fillRing(ColorHSV(hue, 255U, value), NeopixelBall::BallRing::FirstUpper, randomAxis);
	}
	if (timestamp > (PI * 2) && timestamp < (PI * 4))
	{
		float value = (sin(timestamp - 1.57079632679 - (PI * 2)) + 1) * 128;
		//fillRing(ColorHSV(hue, 255U, value), NeopixelBall::BallRing::Middle, randomAxis);
	}
	if (timestamp > (PI * 2.5) && timestamp < (PI * 4.5))
	{
		float value = (sin(timestamp - 1.57079632679 - (PI * 2.5)) + 1) * 128;
		//fillRing(ColorHSV(hue, 255U, value), NeopixelBall::BallRing::FirstLower, randomAxis);
	}
	if (timestamp > (PI * 3) && timestamp < (PI * 5))
	{
		float value = (sin(timestamp - 1.57079632679 - (PI * 3)) + 1) * 128;
		//fillRing(ColorHSV(hue, 255U, value), NeopixelBall::BallRing::SecondLower, randomAxis);
	}
	if (timestamp > (PI * 3.5) && timestamp < (PI * 5.5))
	{
		float value = (sin(timestamp - 1.57079632679 - (PI * 3.5)) + 1) * 128;
		//fillRing(ColorHSV(hue, 255U, value), NeopixelBall::BallRing::ThirdLower, randomAxis);
	}
	if (timestamp > (PI * 4) && timestamp < (PI * 6))
	{
		float value = (sin(timestamp - 1.57079632679 - (PI * 4)) + 1) * 128;
		//fillRing(ColorHSV(hue, 255U, value), NeopixelBall::BallRing::ForthLower, randomAxis);
	}

	return 0;
}

int Effect_RandomFill::init()
{
	int newRandomHue = 0;
	do
	{
		newRandomHue = random(0, 65536);
	} while (abs(newRandomHue - randomHue) < 10000);
	randomHue = newRandomHue;
	startTimestamp = millis();
	validPixelCount = 0;
	nextPixelDue = 0;

	for (size_t i = 0; i < 180; i++)
	{
		hueMap[i] = 0;
	}

	return 0;
}
int Effect_RandomFill::cleanup()
{
	return 0;
}
int Effect_RandomFill::run(long loopCount)
{
	if (millis() > nextPixelDue)
	{
		nextPixelDue = millis() + neopixelBall->effectDelay;
		int validPixels[180];
		validPixelCount = 0;
		for (size_t i = 0; i < 180; i++)
		{
			if (hueMap[i] != randomHue)
			{
				validPixels[validPixelCount] = i;
				validPixelCount++;
			}
		}

		if (validPixelCount > 0)
		{
			int randomIndex = validPixels[random(0, validPixelCount)];
			setPixelColor(randomIndex, ColorHSV(randomHue));
			hueMap[randomIndex] = randomHue;
		}
	}

	return validPixelCount;
}

int Effect_RandomFillRepeating::run(long loopCount)
{
	Effect_RandomFill::run(loopCount);
	if (validPixelCount == 0)
	{
		Effect_RandomFill::init();
	}

	return 0;
}

int Effect_RandomGlow::init()
{
	Serial.println("Effect_RandomGlow::init");

	if (glowingPixelCount == 0)
	{
		preferences.begin("Effect_RandomGlow", false);
		glowingPixelCount = preferences.getInt("glowingPixelCount");

		if (glowingPixelCount == 0)
		{
			Serial.printf("glowingPixelCount: %d - Defaulting to 10\n", glowingPixelCount);
			glowingPixelCount = 10;

			preferences.putInt("glowingPixelCount", glowingPixelCount);
		}

		preferences.end();
	}

	startTime = millis();
	int glowTime = (neopixelBall->effectDelay * 8);
	int glowTimeStep = glowTime / glowingPixelCount;
	int currentGlowTimeStep = 0;

	Serial.printf("GlowList.Size: %d\tglowingPixelCount: %d\n",
		glowList.size(), glowingPixelCount); 
	
	while (glowList.size() < glowingPixelCount)
	{
		//Serial.printf("Need to add a pixel to the glow list! (%d/%d)\n",
			//glowList.size(), glowingPixelCount);

		Serial.printf("glowTime: %d\tglowTimeStep: %d\tcurrentGlowTime: %d\n",
			glowTime, glowTimeStep, currentGlowTimeStep);

		int newIndex = randomNonGlowingPixel();

		Glow newGlow;
		newGlow.hue = random(0, 65536);
		newGlow.glowStart = millis() + currentGlowTimeStep;
		newGlow.pixel = &neopixelBall->pixels[newIndex];
		glowList.push_back(newGlow);
		currentGlowTimeStep += glowTimeStep;
	}

	return 0;
}
int Effect_RandomGlow::cleanup()
{
	Serial.println("Effect_RandomGlow::cleanup");

	glowList.clear();

	preferences.begin("Effect_RandomGlow", false);
	preferences.putInt("glowingPixelCount", glowingPixelCount);
	preferences.end();
	return 0;
}
int Effect_RandomGlow::run(long loopCount)
{
	while (glowList.size() < glowingPixelCount)
	{
		//Serial.printf("Need to add a pixel to the glow list! (%d/%d)\n",
			//glowList.size(), glowingPixelCount);

		int newIndex = randomNonGlowingPixel();

		Glow newGlow;
		newGlow.hue = random(0, 65536);
		newGlow.glowStart = millis();
		newGlow.pixel = &neopixelBall->pixels[newIndex];
		glowList.push_back(newGlow);
	}

	std::vector<Glow>::iterator it;
	for (it = glowList.begin(); it != glowList.end(); ++it)
	{
		int glowTime = (neopixelBall->effectDelay * 8);
		long glowMillis = millis() - it->glowStart;
		float glowRad = (glowMillis * PI) / (glowTime);
		double brightness = sin(glowRad) * 255;
		/*Serial.printf("glowTime: %d\tglowMillis: %d\tglowRad: %f\tbrightness: %f\n",
			glowTime, glowMillis, glowRad, brightness);*/
		if (glowMillis < 0) //Don't start glowing yet...
		{
			setPixelColor(it->pixel->index, ColorHSV(it->hue, 255, 0));
		}
		else if (glowRad > PI) //Pixel is done glowing
		{
			setPixelColor(it->pixel->index, ColorHSV(it->hue, 255, 0));
			glowList.erase(it);
		}
		else //GLOW!!!
		{
			setPixelColor(it->pixel->index, ColorHSV(it->hue, 255, brightness));
		}
	}

	return 0;
}
std::vector<int> Effect_RandomGlow::nonGlowingPixelList()
{
	//Serial.println("Finding non-glowing pixels...");
	std::vector<int> validPixelList;
	for (size_t i = 0; i < 180; i++)
	{
		if (isPixelGlowing(i) == false)
		{
			validPixelList.push_back(i);
		}
	}
	return validPixelList;
}
int Effect_RandomGlow::randomNonGlowingPixel()
{
	std::vector<int> validPixelList = nonGlowingPixelList();
	//Serial.println("Picking a non-glowing pixel");
	int randomIndex = validPixelList[random(0, validPixelList.size())];
	return randomIndex;
}

int Effect_RandomBrightGlow::init()
{
	Serial.println("Effect_RandomBrightGlow::init");

	if (glowingPixelCount == 0)
	{
		preferences.begin("Effect_RandomBrightGlow", false);
		glowingPixelCount = preferences.getInt("glowingPixelCount");

		if (glowingPixelCount == 0)
		{
			Serial.printf("glowingPixelCount: %d - Defaulting to 10\n", glowingPixelCount);
			glowingPixelCount = 10;

			preferences.putInt("glowingPixelCount", glowingPixelCount);
		}

		preferences.end();
	}

	startTime = millis();
	int glowTime = (neopixelBall->effectDelay * 8);
	int glowTimeStep = glowTime / glowingPixelCount;
	int currentGlowTimeStep = 0;

	Serial.printf("GlowList.Size: %d\tglowingPixelCount: %d\n",
		glowList.size(), glowingPixelCount);

	while (glowList.size() < glowingPixelCount)
	{
		//Serial.printf("Need to add a pixel to the glow list! (%d/%d)\n",
			//glowList.size(), glowingPixelCount);

		Serial.printf("glowTime: %d\tglowTimeStep: %d\tcurrentGlowTime: %d\n",
			glowTime, glowTimeStep, currentGlowTimeStep);

		int newIndex = randomNonGlowingPixel();

		Glow newGlow;
		newGlow.hue = random(0, 65536);
		newGlow.glowStart = millis() + currentGlowTimeStep;
		newGlow.pixel = &neopixelBall->pixels[newIndex];
		glowList.push_back(newGlow);
		currentGlowTimeStep += glowTimeStep;
	}

	return 0;
}
int Effect_RandomBrightGlow::cleanup()
{
	Serial.println("Effect_RandomBrightGlow::cleanup");

	glowList.clear();

	preferences.begin("Effect_RandomBrightGlow", false);
	preferences.putInt("glowingPixelCount", glowingPixelCount);
	preferences.end();
	return 0;
}
int Effect_RandomBrightGlow::run(long loopCount)
{
	fill(Color(255, 255, 255));

	while (glowList.size() < glowingPixelCount)
	{
		//Serial.printf("Need to add a pixel to the glow list! (%d/%d)\n",
			//glowList.size(), glowingPixelCount);

		int newIndex = randomNonGlowingPixel();

		Glow newGlow;
		newGlow.hue = random(0, 65536);
		newGlow.glowStart = millis();
		newGlow.pixel = &neopixelBall->pixels[newIndex];
		glowList.push_back(newGlow);
	}

	std::vector<Glow>::iterator it;
	for (it = glowList.begin(); it != glowList.end(); ++it)
	{
		int glowTime = (neopixelBall->effectDelay * 8);
		long glowMillis = millis() - it->glowStart;
		float glowRad = (glowMillis * PI) / (glowTime);
		double saturation = sin(glowRad) * 255;
		/*Serial.printf("glowTime: %d\tglowMillis: %d\tglowRad: %f\tbrightness: %f\n",
			glowTime, glowMillis, glowRad, brightness);*/
		if (glowMillis < 0) //Don't start glowing yet...
		{
			setPixelColor(it->pixel->index, ColorHSV(it->hue, 0, 255));
		}
		else if (glowRad > PI) //Pixel is done glowing
		{
			setPixelColor(it->pixel->index, ColorHSV(it->hue, 0, 255));
			glowList.erase(it);
		}
		else //GLOW!!!
		{
			setPixelColor(it->pixel->index, ColorHSV(it->hue, saturation, 255));
		}
	}

	return 0;
}
std::vector<int> Effect_RandomBrightGlow::nonGlowingPixelList()
{
	//Serial.println("Finding non-glowing pixels...");
	std::vector<int> validPixelList;
	for (size_t i = 0; i < 180; i++)
	{
		if (isPixelGlowing(i) == false)
		{
			validPixelList.push_back(i);
		}
	}
	return validPixelList;
}
int Effect_RandomBrightGlow::randomNonGlowingPixel()
{
	std::vector<int> validPixelList = nonGlowingPixelList();
	//Serial.println("Picking a non-glowing pixel");
	int randomIndex = validPixelList[random(0, validPixelList.size())];
	return randomIndex;
}