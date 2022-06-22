#include "NeopixelBall.h"
#include "LittleFS.h"
#include <Preferences.h>

#define LED_COUNT 180
#define LED_PIN    14
#define BRIGHTNESS 32

static Preferences preferences;

NeopixelBall::NeopixelBall() : Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800)
{
	for (size_t i = 0; i < 180; i++)
	{
		pixels[i].index = i;
	}
}

static void listDir(fs::FS& fs, const char* dirname, uint8_t levels)
{
	Serial.printf("Listing directory: %s\r\n", dirname);

	File root = fs.open(dirname);
	if (!root) {
		Serial.println("- failed to open directory");
		return;
	}
	if (!root.isDirectory()) {
		Serial.println(" - not a directory");
		return;
	}

	File file = root.openNextFile();
	while (file) {
		if (file.isDirectory()) {
			Serial.print("  DIR : ");
			Serial.println(file.name());
			if (levels) {
				listDir(fs, file.path(), levels - 1);
			}
		}
		else {
			Serial.print("  FILE: ");
			Serial.print(file.name());
			Serial.print("\tSIZE: ");
			Serial.println(file.size());
		}
		file = root.openNextFile();
	}
}

void NeopixelBall::init()
{
	Effect_Off* effectOff = new Effect_Off(this);
	effectList[BallMode::OFF] = effectOff;
	Effect_NYE_Countdown* effectNYE_Countdown = new Effect_NYE_Countdown(this);
	effectList[BallMode::NYE_Countdown] = effectNYE_Countdown;
	Effect_Rainbow* effectRainbow = new Effect_Rainbow(this);
	effectList[BallMode::Rainbow] = effectRainbow;
	Effect_LedFinder* effectLedFinder = new Effect_LedFinder(this);
	effectList[BallMode::LedFinder] = effectLedFinder;
	Effect_Counter* effectCounter = new Effect_Counter(this);
	effectList[BallMode::Counter] = effectCounter;
	Effect_Pentagons* effectPentagons = new Effect_Pentagons(this);
	effectList[BallMode::Pentagons] = effectPentagons;
	Effect_Stars* effectStars = new Effect_Stars(this);
	effectList[BallMode::Stars] = effectStars;
	Effect_MiddleRings* effectMiddleRings = new Effect_MiddleRings(this);
	effectList[BallMode::MiddleRings] = effectMiddleRings;
	Effect_RandomSnake* effectRandomSnake = new Effect_RandomSnake(this);
	effectList[BallMode::RandomSnake] = effectRandomSnake;
	Effect_SinglePulse* effectSinglePulse = new Effect_SinglePulse(this);
	effectList[BallMode::SinglePulse] = effectSinglePulse;
	Effect_SingleMovingPulse* effectSingleMovingPulse = new Effect_SingleMovingPulse(this);
	effectList[BallMode::SingleMovingPulse] = effectSingleMovingPulse;
	Effect_RandomFill* effectRandomFill = new Effect_RandomFill(this);
	effectList[BallMode::RandomFill] = effectRandomFill;
	Effect_RandomFillRepeating* effectRandomFillRepeating = new Effect_RandomFillRepeating(this);
	effectList[BallMode::RandomFillRepeating] = effectRandomFillRepeating;
	Effect_RandomGlow* effectRandomGlow = new Effect_RandomGlow(this);
	effectList[BallMode::RandomGlow] = effectRandomGlow;
	Effect_RandomBrightGlow* effectRandomBrightGlow = new Effect_RandomBrightGlow(this);
	effectList[BallMode::RandomBrightGlow] = effectRandomBrightGlow;

	for (size_t i = 0; i < 12; i++)
	{
		for (size_t j = 0; j < 9; j++)
		{
			for (size_t k = 0; k < 30; k++)
			{
				rings[i][j][k] = nullptr;
			}
		}
	}

	preferences.begin("NeopixelBall", true);
	effectDelay = preferences.getInt("effectSpeed", 128);

	begin();
	setBrightness(preferences.getInt("brightness", BRIGHTNESS));
	clear();
	show();

	setPixelColor(0, Color(255, 0, 0));
	show();

	LittleFS.begin();
	listDir(LittleFS, "/", 3);
	//Parse the pentagons
	Serial.println("Opening ledRings.tsv . . . ");
	File pentagonLeds = LittleFS.open("/ledRings.tsv", "r");
	if (!pentagonLeds)
	{
		Serial.println("Error opening ledRings.tsv!!!");
		return;
	}
	int i = 0, j = 0;
	char buffer[256];
	memset(buffer, 0, 256);
	while (pentagonLeds.readBytesUntil('\n', buffer, 256))
	{
		int lineCount = 0;
		do
		{
			char* token = strtok(buffer, "\t");
			while (token != NULL)
			{
				int currentPixel = atoi(token);
				//Serial.printf("Setting %d %d %d to index %d\n", i, lineCount, j, currentPixel);
				rings[i][lineCount][j] = &pixels[currentPixel];
				pixels[currentPixel].isPentagon = lineCount == 0;
				j++;
				token = strtok(NULL, "\t");
			}

			memset(buffer, 0, 256);
			if (lineCount < 8)
			{
				pentagonLeds.readBytesUntil('\n', buffer, 256);
			}
			j = 0;
		} while (lineCount++ < 8);

		i++;
		j = 0;
		memset(buffer, 0, 32);
	}
	pentagonLeds.close();

	/*for (size_t i = 0; i < 12; i++)
	{
		for (size_t j = 0; j < 5; j++)
		{
			Serial.print(pentagons[i][j]);
			Serial.print("\t");
		}
		Serial.print("\n");
	}*/

	for (size_t i = 0; i < 12; i++)
	{
		rings[i][0][0]->pentagonCW[0] = rings[i][0][1];
		rings[i][0][0]->pentagonCCW[0] = rings[i][0][4];

		rings[i][0][1]->pentagonCW[0] = rings[i][0][2];
		rings[i][0][1]->pentagonCCW[0] = rings[i][0][0];

		rings[i][0][2]->pentagonCW[0] = rings[i][0][3];
		rings[i][0][2]->pentagonCCW[0] = rings[i][0][1];

		rings[i][0][3]->pentagonCW[0] = rings[i][0][4];
		rings[i][0][3]->pentagonCCW[0] = rings[i][0][2];

		rings[i][0][4]->pentagonCW[0] = rings[i][0][0];
		rings[i][0][4]->pentagonCCW[0] = rings[i][0][3];
	}

	//Parse the neighbors
	File neighbors = LittleFS.open("/ledNeighbors.tsv", "r");
	if (!neighbors)
	{
		Serial.println("Error opening ledNeighbors.tsv!!!");
		return;
	}
	memset(buffer, 0, 32);
	while (neighbors.readBytesUntil('\n', buffer, 32))
	{
		int index, n1, n2, n3;
		char* token = strtok(buffer, "\t");
		index = atoi(token);
		token = strtok(NULL, "\t");
		n1 = atoi(token);
		token = strtok(NULL, "\t");
		n2 = atoi(token);
		token = strtok(NULL, "\t");
		n3 = atoi(token);

		pixels[index].neighbors[0] = &pixels[n1];
		pixels[index].neighbors[1] = &pixels[n2];
		pixels[index].neighbors[2] = &pixels[n3];

		memset(buffer, 0, 32);
	}
	neighbors.close();

	LittleFS.end();

	//setMode(preferences.getString("mode", "OFF"));
	setMode("NYECountdown");

	clear();
	setPixelColor(0, Color(0, 255, 0));
	show();

	preferences.end();
}

void NeopixelBall::loop(long loopCount)
{
	effectList[mode]->run(loopCount);
	show();
}

void NeopixelBall::handleServerUpdate(WebServer* server)
{
	/*for(int i = 0; i < 25; i++)
	{
		Serial.printf("ArgName %d: %s (%d) %s\n", i, server->argName(i).c_str(), 
			server->hasArg(server->argName(i).c_str()), server->arg(i));
	}*/
	Serial.printf("ArgCount: %d\n", server->args());
	for (size_t argIndex = 0; argIndex < server->args(); argIndex++)
	{
		Serial.printf("Arg Name: %s\n", server->argName(argIndex).c_str());
		effectList[mode]->updateArgs(server->argName(argIndex).c_str(), server->arg(argIndex));
	}

	if (server->hasArg("brightness"))
	{
		Serial.printf("Setting brightness to %d\n", server->arg("brightness").toInt());
		setBrightness(server->arg("brightness").toInt());
		brightness = server->arg("brightness").toInt();
	}
	if (server->hasArg("effectSpeed"))
	{
		Serial.printf("Setting effectSpeed to %d\n", server->arg("effectSpeed").toInt());
		effectDelay = 2048 - server->arg("effectSpeed").toInt();
	}
	if (server->hasArg("mode"))
	{
		Serial.printf("Setting effectSpeed to %s\n", server->arg("mode").c_str());
		setMode(server->arg("mode"));
	}

	saveConfig();
}

void NeopixelBall::saveConfig()
{
	preferences.begin("NeopixelBall", false);

	preferences.putInt("brightness", brightness);
	preferences.putInt("effectSpeed", effectDelay);
	preferences.putString("mode", effectList[mode]->name);
	//preferences.putInt("mainLoopAxis", rainbowAxis);

	preferences.end();
}

void NeopixelBall::setMode(String newMode)
{
	effectList[mode]->cleanup();

	Serial.printf("Setting new mode: %s\n", newMode.c_str());

	for (int i_mode = BallMode::OFF; i_mode != BallMode::LAST; i_mode++)
	{
		if (newMode == effectList[i_mode]->name)
		{
			mode = (BallMode)i_mode;
			break;
		}
	}

	effectList[mode]->init();

	clear();
}

void NeopixelBall::setNextMode()
{
	mode = (BallMode)(((int)mode) + 1);
	if (mode == BallMode::LAST)
	{
		mode = BallMode::OFF;
	}
	clear();
}

void NeopixelBall::setPixelColor(uint16_t n, uint32_t c)
{
	Adafruit_NeoPixel::setPixelColor(n, c);
}

void NeopixelBall::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b)
{
	setPixelColor(n, Color(r, g, b));
}

void NeopixelBall::show()
{
	Adafruit_NeoPixel::show();
}

void NeopixelBall::fillRing(uint32_t c, BallRing ring, int startPentagon)
{
	//Serial.printf("Ring: %d\tStartPentagon: %d\n", ring, startPentagon);

	switch (ring)
	{
	case ForthUpper:
		for (size_t i = 0; i < 5; i++)
		{
			//Serial.printf("Setting pixel %d %d %d", startPentagon, 8, i);
			//Serial.printf("\t(index %d)\n", rings[startPentagon][8][i]->index);
			if (rings[startPentagon][8][i] != nullptr)
			{
				setPixelColor(rings[startPentagon][8][i]->index, c);
			}
			else
			{
				Serial.printf("rings[%d][%d][%d] not defined\n",
					startPentagon, 8, i);
			}
		}
		break;
	case ThirdUpper:
		for (size_t i = 0; i < 15; i++)
		{
			//Serial.printf("Setting pixel %d %d %d", startPentagon, 7, i);
			//Serial.printf("\t(index %d)\n", rings[startPentagon][7][i]->index);
			if (rings[startPentagon][7][i] != nullptr)
			{
				setPixelColor(rings[startPentagon][7][i]->index, c);
			}
			else
			{
				Serial.printf("rings[%d][%d][%d] not defined\n",
					startPentagon, 7, i);
			}
		}
		break;
	case SecondUpper:
		for (size_t i = 0; i < 25; i++)
		{
			//Serial.printf("Setting pixel %d %d %d", startPentagon, 6, i);
			//Serial.printf("\t(index %d)\n", rings[startPentagon][6][i]->index);
			if (rings[startPentagon][6][i] != nullptr)
			{
				setPixelColor(rings[startPentagon][6][i]->index, c);
			}
			else
			{
				Serial.printf("rings[%d][%d][%d] not defined\n",
					startPentagon, 6, i);
			}
		}
		break;
	case FirstUpper:
		for (size_t i = 0; i < 30; i++)
		{
			//Serial.printf("Setting pixel %d %d %d", startPentagon, 5, i);
			//Serial.printf("\t(index %d)\n", rings[startPentagon][5][i]->index);
			if (rings[startPentagon][5][i] != nullptr)
			{
				setPixelColor(rings[startPentagon][5][i]->index, c);
			}
			else
			{
				Serial.printf("rings[%d][%d][%d] not defined\n",
					startPentagon, 5, i);
			}
		}
		break;
	case Middle:
		for (size_t i = 0; i < 30; i++)
		{
			//Serial.printf("Setting pixel %d %d %d", startPentagon, 4, i);
			//Serial.printf("\t(index %d)\n", rings[startPentagon][4][i]->index);
			if (rings[startPentagon][4][i] != nullptr)
			{
				setPixelColor(rings[startPentagon][4][i]->index, c);
			}
			else
			{
				Serial.printf("rings[%d][%d][%d] not defined\n",
					startPentagon, 4, i);
			}
		}
		break;
	case FirstLower:
		for (size_t i = 0; i < 30; i++)
		{
			//Serial.printf("Setting pixel %d %d %d", startPentagon, 3, i);
			//Serial.printf("\t(index %d)\n", rings[startPentagon][3][i]->index);
			if (rings[startPentagon][3][i] != nullptr)
			{
				setPixelColor(rings[startPentagon][3][i]->index, c);
			}
			else
			{
				Serial.printf("rings[%d][%d][%d] not defined\n",
					startPentagon, 3, i);
			}
		}
		break;
	case SecondLower:
		for (size_t i = 0; i < 25; i++)
		{
			//Serial.printf("Setting pixel %d %d %d", startPentagon, 2, i);
			//Serial.printf("\t(index %d)\n", rings[startPentagon][2][i]->index);
			if (rings[startPentagon][2][i] != nullptr)
			{
				setPixelColor(rings[startPentagon][2][i]->index, c);
			}
			else
			{
				Serial.printf("rings[%d][%d][%d] not defined\n",
					startPentagon, 2, i);
			}
		}
		break;
	case ThirdLower:
		for (size_t i = 0; i < 15; i++)
		{
			//Serial.printf("Setting pixel %d %d %d", startPentagon, 1, i);
			//Serial.printf("\t(index %d)\n", rings[startPentagon][1][i]->index);
			if (rings[startPentagon][1][i] != nullptr)
			{
				setPixelColor(rings[startPentagon][1][i]->index, c);
			}
			else
			{
				Serial.printf("rings[%d][%d][%d] not defined\n",
					startPentagon, 1, i);
			}
		}
		break;
	case ForthLower:
		for (size_t i = 0; i < 5; i++)
		{
			//Serial.printf("Setting pixel %d %d %d", startPentagon, 0, i);
			//Serial.printf("\t(index %d)\n", rings[startPentagon][0][i]->index);
			if (rings[startPentagon][0][i] != nullptr)
			{
				setPixelColor(rings[startPentagon][0][i]->index, c);
			}
			else
			{
				Serial.printf("rings[%d][%d][%d] not defined\n",
					startPentagon, 0, i);
			}
		}
		break;
	}
}
