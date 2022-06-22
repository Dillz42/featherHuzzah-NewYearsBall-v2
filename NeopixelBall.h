#ifndef _NYE_NEOPIXEL_BALL_H
#define _NYE_NEOPIXEL_BALL_H

#include <Adafruit_NeoPixel.h>
//#include </*ESP8266*/WebServer.h>
#include <WebServer.h>
#include <map>
#include <vector>
#include "SpoolMotor.h"

class Effect;
class SpoolMotor;

class NeopixelBall : public Adafruit_NeoPixel
{
public:
	enum BallRing
	{
		Middle,
		FirstLower,
		SecondLower,
		ThirdLower,
		ForthLower,
		FirstUpper,
		SecondUpper,
		ThirdUpper,
		ForthUpper,
	};

	enum BallMode
	{
		OFF,
		NYE_Countdown,
		Rainbow,
		LedFinder,
		Counter,
		Pentagons,
		Stars,
		MiddleRings,
		RandomSnake,
		SinglePulse,
		SingleMovingPulse,
		RandomFill,
		RandomFillRepeating,
		RandomGlow,
		RandomBrightGlow,
		LAST
	};

	Effect* effectList[BallMode::LAST];

	NeopixelBall();
	void init();
	void loop(long loopCount);

	

	void fillRing(uint32_t c, BallRing ring, int startPentagon);
	void setMode(String newMode);
	void setNextMode();
	BallMode mode;

	class Pixel
	{
	public:
		int index;
		Pixel* neighbors[3];
		bool isPentagon = false;
		Pixel* pentagonCW[12];
		Pixel* pentagonCCW[12];
	};
	Pixel pixels[180];
	Pixel* rings[12][9][30];

	void SetServerEndpoints(/*ESP8266*/WebServer* server)
	{
		server->on("/led", HTTPMethod::HTTP_POST, std::bind(&NeopixelBall::handleServerUpdate, this, server));
	}

	void setNeighborPixels(Pixel* pixel, int depth, int distance = 0);

	/***OVERRIDES***/
	void setPixelColor(uint16_t n, uint32_t c);
	void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
	void show();

	int effectDelay = 128;

private:
	void saveConfig();

	void handleServerUpdate(WebServer* server);
};

class Effect
{
public:
	Effect(NeopixelBall* neopixelBall)
	{
		this->neopixelBall = neopixelBall;
	}

	const char* name;
	const char* name_human;
	virtual int init() { return 0; }
	virtual int run(long loopCount) { return 0; }
	virtual int cleanup() { return 0; }
	static int delayFromRunTime(long runtimeMillis) { return 1000; }
	virtual void updateArgs(String argName, String value)
	{
		Serial.println("UpdateArgs in base class");
	}

	String nameString()
	{
		return String(name);
	}
	
	String nameHumanString()
	{
		return String(name_human);
	}

	uint32_t Color(uint8_t r, uint8_t g, uint8_t b)
	{
		return neopixelBall->Color(r, g, b);
	}
	uint32_t ColorHSV(uint16_t hue, uint8_t sat = 255, uint8_t val = 255)
	{
		return neopixelBall->ColorHSV(hue, sat, val);
	}
	void setPixelColor(int16_t n, int32_t c)
	{
		neopixelBall->setPixelColor(n, c);
	}
	int32_t getPixelColor(int16_t n)
	{
		return neopixelBall->getPixelColor(n);
	}
	void clear()
	{
		neopixelBall->clear();
	}
	void fill(uint32_t c = 0, uint16_t first = 0, uint16_t count = 0)
	{
		neopixelBall->fill(c, first, count);
	}
	void fillRing(uint32_t c, NeopixelBall::BallRing ring, int startPentagon)
	{
		neopixelBall->fillRing(c, ring, startPentagon);
	}

protected:
	NeopixelBall* neopixelBall;
};

class Effect_Off : public Effect
{
public:
	Effect_Off(NeopixelBall* neopixelBall) : Effect(neopixelBall)
	{
		this->name = "OFF";
		this->name_human = "OFF";
	}

	int init() { Serial.println("Effect_Off::init"); return 0; }
	int run(long loopCount);
	int cleanup() { Serial.println("Effect_Off::cleanup"); return 0; }
};

class Effect_NYE_Countdown : public Effect
{
public:
	Effect_NYE_Countdown(NeopixelBall* neopixelBall)
		: Effect(neopixelBall)
	{
		name = "NYECountdown";
		name_human = "NYE Countdown";
		spoolMotor = nullptr;
		debugMidnight = true;
		NYE_mode = NeopixelBall::BallMode::RandomGlow;
		prev_NYE_mode = NeopixelBall::BallMode::OFF;
	}

	int init();
	int run(long loopCount);
	int cleanup();
	SpoolMotor* spoolMotor;

private:
	long midnightMillis;
	bool debugMidnight;
	NeopixelBall::BallMode NYE_mode;
	NeopixelBall::BallMode prev_NYE_mode;

	void SetModeBasedOnTime();
};

class Effect_Rainbow : public Effect
{
public:
	Effect_Rainbow(NeopixelBall* neopixelBall) : Effect(neopixelBall)
	{
		this->name = "Rainbow";
		this->name_human = "Rainbow";
	}

	int rainbowAxis = 0;

	int init() { return 0; }
	int run(long loopCount);
	int cleanup() { return 0; }

	void updateArgs(String argName, String value)
	{
		Serial.println("UpdateArgs in Effect_Rainbow");

		if (argName == "rainbowAxis")
		{
			rainbowAxis = value.toInt();
			Serial.printf("rainbowAxis set to %d\n", rainbowAxis);
		}
	}
};

class Effect_Counter : public Effect
{
public:
	Effect_Counter(NeopixelBall* neopixelBall) : Effect(neopixelBall)
	{
		this->name = "Counter";
		this->name_human = "Counter";
	}

	int currentCount = 10;

	int init();
	int run(long loopCount);
	int cleanup();

	void updateArgs(String argName, String value)
	{
		Serial.println("UpdateArgs in Effect_Counter");

		if (argName == "currentCount")
		{
			currentCount = value.toInt();
			Serial.printf("currentCount set to %d\n", currentCount);
		}
	}
};

class Effect_LedFinder : public Effect
{
public:
	Effect_LedFinder(NeopixelBall* neopixelBall) : Effect(neopixelBall)
	{
		this->name = "LedFinder";
		this->name_human = "Led Finder";
	}

	int ledFinderIndex = 0, ledFinderDepth = 0;

	int init();
	int run(long loopCount);
	int cleanup();
	void updateArgs(String argName, String value)
	{
		Serial.println("UpdateArgs in Effect_LedFinder");

		if (argName == "ledFinderNumber")
		{
			ledFinderIndex = value.toInt();
			Serial.printf("finder index set to %d\n", ledFinderIndex);
		}
		else if (argName == "ledFinderDepth")
		{
			ledFinderDepth = value.toInt();
			Serial.printf("finder depth set to %d\n", ledFinderDepth);
		}
		else
		{
			Serial.printf("Arg not set! arg: %s\tvalue: %s\n",
				argName, value);
		}
	}
};

class Effect_Pentagons : public Effect
{
public:
	Effect_Pentagons(NeopixelBall* neopixelBall) : Effect(neopixelBall)
	{
		this->name = "Pentagons";
		this->name_human = "Pentagons";
	}

	int init() { return 0; }
	int run(long loopCount);
	int cleanup() { return 0; }
};

class Effect_Stars : public Effect
{
public:
	Effect_Stars(NeopixelBall* neopixelBall) : Effect(neopixelBall)
	{
		this->name = "Stars";
		this->name_human = "Stars";
	}

	int init() { return 0; }
	int run(long loopCount);
	int cleanup() { return 0; }
};

class Effect_MiddleRings : public Effect
{
public:
	Effect_MiddleRings(NeopixelBall* neopixelBall) : Effect(neopixelBall)
	{
		this->name = "MiddleRings";
		this->name_human = "Middle Rings";
	}

	int init() { return 0; }
	int run(long loopCount);
	int cleanup() { return 0; }
};

class Effect_RandomSnake : public Effect
{
public:
	Effect_RandomSnake(NeopixelBall* neopixelBall) : Effect(neopixelBall)
	{
		this->name = "RandomSnake";
		this->name_human = "Random Snake";
	}

	int init();
	int run(long loopCount);
	int cleanup();
private:
	NeopixelBall::Pixel* snakeHead;
	NeopixelBall::Pixel* snakeBody[7];
	NeopixelBall::Pixel* snakeTail;
};

class Effect_SinglePulse : public Effect
{
public:
	Effect_SinglePulse(NeopixelBall* neopixelBall) : Effect(neopixelBall)
	{
		this->name = "SinglePulse";
		this->name_human = "Single Pulse";
		hue = -1;
	}

	int init();
	int run(long loopCount);
	int cleanup();
	static int delayFromRunTime(long runtimeMillis);
	static int hue;
	static int brightness;
	static int color_r;
	static int color_g;
	static int color_b;

protected:
	float startTimestamp;
};

class Effect_SingleMovingPulse : public Effect_SinglePulse
{
public:
	Effect_SingleMovingPulse(NeopixelBall* neopixelBall) : Effect_SinglePulse(neopixelBall)
	{
		this->name = "SingleMovingPulse";
		this->name_human = "Single Moving Pulse";
	}

	int init();
	int run(long loopCount);
	int cleanup();

protected:
	int randomAxis;
};

class Effect_RandomFill : public Effect
{
public:
	Effect_RandomFill(NeopixelBall* neopixelBall) : Effect(neopixelBall)
	{
		this->name = "RandomFill";
		this->name_human = "Random Fill";
	}

	int init();
	int run(long loopCount);
	int cleanup();

protected:
	int randomHue;
	long startTimestamp;
	int validPixelCount;
	int hueMap[180];
	long nextPixelDue;
};

class Effect_RandomFillRepeating : public Effect_RandomFill
{
public:
	Effect_RandomFillRepeating(NeopixelBall* neopixelBall) : Effect_RandomFill(neopixelBall)
	{
		this->name = "RandomFillRepeating";
		this->name_human = "Random Fill - Repeating";
	}

	//int init() { return 0; }
	int run(long loopCount);
	//int cleanup() { return 0; }
};

class Effect_RandomGlow : public Effect
{
public:
	Effect_RandomGlow(NeopixelBall* neopixelBall) : Effect(neopixelBall)
	{
		this->name = "RandomGlow";
		this->name_human = "Random Glow";
	}

	int init();
	int run(long loopCount);
	int cleanup();

	void updateArgs(String argName, String value)
	{
		Serial.println("UpdateArgs in Effect_RandomGlow");

		if (argName == "glowingPixelCount")
		{
			glowingPixelCount = value.toInt();
			Serial.printf("glowingPixelCount set to %d\n", glowingPixelCount);
			cleanup();
			init();
		}
	}

private:
	long startTime;
	int glowingPixelCount = 0;

	class Glow
	{
	public:
		NeopixelBall::Pixel* pixel;
		int hue;
		long glowStart;
	};
	std::vector<Glow> glowList;

	bool isPixelGlowing(int index)
	{
		std::vector<Glow>::iterator it;
		for (it = glowList.begin(); it != glowList.end(); ++it)
		{
			if (it->pixel->index == index)
			{
				return true;
			}
		}
		return false;
	}
	std::vector<int> nonGlowingPixelList();
	int randomNonGlowingPixel();
};

class Effect_RandomBrightGlow : public Effect
{
public:
	Effect_RandomBrightGlow(NeopixelBall* neopixelBall) : Effect(neopixelBall)
	{
		this->name = "RandomBrightGlow";
		this->name_human = "Random Bright Glow";
	}

	int init();
	int run(long loopCount);
	int cleanup();

	void updateArgs(String argName, String value)
	{
		Serial.println("UpdateArgs in Effect_RandomBrightGlow");

		if (argName == "glowingPixelCount")
		{
			glowingPixelCount = value.toInt();
			Serial.printf("glowingPixelCount set to %d\n", glowingPixelCount);
			cleanup();
			init();
		}
	}

private:
	long startTime;
	int glowingPixelCount = 0;

	class Glow
	{
	public:
		NeopixelBall::Pixel* pixel;
		int hue;
		long glowStart;
	};
	std::vector<Glow> glowList;

	bool isPixelGlowing(int index)
	{
		std::vector<Glow>::iterator it;
		for (it = glowList.begin(); it != glowList.end(); ++it)
		{
			if (it->pixel->index == index)
			{
				return true;
			}
		}
		return false;
	}
	std::vector<int> nonGlowingPixelList();
	int randomNonGlowingPixel();
};

#endif
