//#include <ESP8266WiFi.h>
//#include <ESP8266mDNS.h>
//#include </*ESP8266*/WebServer.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "NeopixelBall.h"
#include "NewYearsBall_Time.h"
#include <string>
#include "LittleFS.h"
#include "NeopixelBallWiFi.h"
#include "OTA.h"

const char* phil_ssid = "WayneManor_2.4GHz";
const char* phil_password = "aquaman225";
const char* pifi_ssid = "PiFi";
const char* pifi_password = "raspberry314";

static WiFiMulti wifiMulti;
static /*ESP8266*/WebServer server(80);
static bool canSleep = false;

static bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if (start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}

void NeopixelBallWiFi::handleRoot()
{
    Serial.println("handleRoot");

    LittleFS.begin();
    File index = LittleFS.open("/index.html", "r");
    if (!index)
    {
        Serial.println("Error opening index.html!!!");
        return;
    }

    String content = index.readString();
    //Serial.printf("Current content: %s\n\n", index.readString().c_str());
    index.close();
    LittleFS.end();

    //Insert uptime
    content.replace("{{UPTIME}}", NewYearsBall_Time::uptime());
    //Insert brightness and speed
    content.replace("{{LED_BRIGHTNESS}}", String(neopixelBall->getBrightness()));
    content.replace("{{EFFECT_SPEED}}", String(2048 - neopixelBall->effectDelay));

    //Setup tabs
    String tabContent = "";
    for (int mode = NeopixelBall::BallMode::OFF; mode != NeopixelBall::BallMode::LAST; mode++)
    {
        //Serial.printf("Mode: %c (%c)\n", neopixelBall->effectList[mode]->name, neopixelBall->effectList[mode]->name_human);
        
        tabContent.concat("<button class=\"tablinks\" onclick=\"openTab(event, '");
        tabContent.concat(neopixelBall->effectList[mode]->name);
        tabContent.concat("')\">");
        tabContent.concat(neopixelBall->effectList[mode]->name_human);
        tabContent.concat("</button>");
    }
    content.replace("{{TAB_CONTENT}}", tabContent);

    //Setup tab content
    tabContent = "";
    for (int mode = NeopixelBall::BallMode::OFF; mode != NeopixelBall::BallMode::LAST; mode++)
    {
        tabContent.concat("<div id=\"");
        tabContent.concat(neopixelBall->effectList[mode]->name);
        tabContent.concat("\" class=\"tabcontent\">");

        tabContent.concat("<h3>");
        tabContent.concat(neopixelBall->effectList[mode]->name_human);
        tabContent.concat("</h3>");

        char fileName[64];
        memset(fileName, 0, 64);
        sprintf(fileName, "/%s.html", neopixelBall->effectList[mode]->name);
        //Serial.printf("Checking for file %s (mode: %s [%d])\n", fileName, NeopixelBall::GetBallModeString(mode).c_str(), mode);
        LittleFS.begin();
        if (LittleFS.exists(fileName))
        {
            LittleFS.end();
            LittleFS.begin();
            //Serial.println("\tFile exists!!!");
            File tabContentFile = LittleFS.open(fileName, "r");
            if (!tabContentFile)
            {
                //Serial.println("\tFile not opened!!!");
                return;
            }
            else
            {
                //Serial.println("\tFile opened!!!");
                tabContent.concat(tabContentFile.readString());
            }
            tabContentFile.close();
        }
        else
        {
            //Serial.println("\tFile does not exist");
        }
        LittleFS.end();

        tabContent.concat("</div>");
    }
    content.replace("{{TAB_BODIES}}", tabContent);

    LittleFS.end();
    //Serial.printf("Content: %s\n\n", content.c_str());
    server.send(200, "text/html", content);
}

void NeopixelBallWiFi::init(NeopixelBall* neopixelBall, SpoolMotor* motor)
{
    this->neopixelBall = neopixelBall;

    wifiMulti.addAP(pifi_ssid, pifi_password);
    wifiMulti.addAP(phil_ssid, phil_password);

    neopixelBall->setPixelColor(2, neopixelBall->Color(0, 0, 128));
    neopixelBall->show();
    Serial.println("Connecting to WiFi...");
    if (wifiMulti.run() == WL_CONNECTED)
    {
        neopixelBall->setPixelColor(2, neopixelBall->Color(0, 128, 0));
        neopixelBall->show();

        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());

        OTA_setup();

        if (MDNS.begin("esp32")) {
            Serial.println("MDNS responder started");
        }
        else
        {
            Serial.println("MDNS not started :(");
        }

        server.onNotFound([]() {
            canSleep = false;
            server.send(404, "text/plain", "Not found...");
            canSleep = true;
        });
        /*server.on("/", []() {
            canSleep = false;
            server.send(200, "text/plain", "Hello from ESP8266!");
            canSleep = true;
        });*/
        server.on("/", std::bind(&NeopixelBallWiFi::handleRoot, this));

        neopixelBall->SetServerEndpoints(&server);

        //motor endpoints
        this->motor = motor;
        motor->SetServerEndpoints(&server);

        server.begin();
    }
    else
    {
        neopixelBall->setPixelColor(2, neopixelBall->Color(255, 0, 0));
        neopixelBall->setPixelColor(1, neopixelBall->Color(255, 0, 0));
        neopixelBall->setPixelColor(0, neopixelBall->Color(255, 0, 0));
        neopixelBall->show();
        Serial.println("Not Connected to WiFi!!!");
        delay(1000);
        ESP.restart();
    }
}

//void NeopixelBallWiFi::init(NeopixelBall* neopixelBall, SpoolMotor* motor)
//{
//    this->neopixelBall = neopixelBall;
//
//    WiFi.mode(WIFI_STA);
//    WiFi.disconnect();
//    WiFi.setSleep(true);
//
//    neopixelBall->setPixelColor(2, neopixelBall->Color(128, 0, 0));
//    neopixelBall->show();
//    Serial.println("Scanning networks...");
//
//    int scanResult = WiFi.scanNetworks();
//
//    if (scanResult == 0)
//    {
//        Serial.println("No Networks found!");
//    }
//    else
//    {
//        Serial.printf("%d networks found. Checking for known networks\n", scanResult);
//        for (int i = 0; i < scanResult; i++)
//        {
//            String ssid;
//            int32_t rssi;
//            uint8_t encryptionType;
//            uint8_t* bssid;
//            int32_t channel;
//
//            WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel);
//            if (ssid == pifi_ssid)
//            {
//                neopixelBall->setPixelColor(2, neopixelBall->Color(0, 0, 128));
//                neopixelBall->show();
//                Serial.print("Connecting to PiFi.");
//                WiFi.begin(pifi_ssid, pifi_password);
//                while (WiFi.status() != WL_CONNECTED) {
//                    delay(500);
//                    Serial.print(".");
//                }
//            }
//            else if (ssid == phil_ssid)
//            {
//                neopixelBall->setPixelColor(2, neopixelBall->Color(0, 0, 128));
//                neopixelBall->show();
//                Serial.print("Connecting to WayneManor_2.4GHz.");
//                WiFi.begin(phil_ssid, phil_password);
//                while (WiFi.status() != WL_CONNECTED) {
//                    delay(500);
//                    Serial.print(".");
//                }
//            }
//        }
//
//        if (WiFi.isConnected())
//        {
//            neopixelBall->setPixelColor(2, neopixelBall->Color(0, 128, 0));
//            neopixelBall->show();
//
//            Serial.println("");
//            Serial.println("Connected!");
//            Serial.print("IP address: ");
//            Serial.println(WiFi.localIP());
//
//            if (MDNS.begin("esp32")) {
//                Serial.println("MDNS responder started");
//            }
//            else
//            {
//                Serial.println("MDNS not started :(");
//            }
//
//            server.onNotFound([]() {
//                canSleep = false;
//                server.send(404, "text/plain", "Not found...");
//                canSleep = true;
//            });
//            /*server.on("/", []() {
//                canSleep = false;
//                server.send(200, "text/plain", "Hello from ESP8266!");
//                canSleep = true;
//            });*/
//            server.on("/", std::bind(&NeopixelBallWiFi::handleRoot, this));
//
//            neopixelBall->SetServerEndpoints(&server);
//
//            //motor endpoints
//            this->motor = motor;
//            //motor->SetServerEndpoints(&server);
//
//            server.begin();
//        }
//        else
//        {
//            neopixelBall->setPixelColor(2, neopixelBall->Color(255, 0, 0));
//            neopixelBall->setPixelColor(1, neopixelBall->Color(255, 0, 0));
//            neopixelBall->setPixelColor(0, neopixelBall->Color(255, 0, 0));
//            neopixelBall->show();
//            Serial.println("Not Connected to WiFi!!!");
//            delay(1000);
//            ESP.restart();
//        }
//    }
//
//    //WiFi.disconnect();
//}

void NeopixelBallWiFi::loop(long loopCount)
{
    if (loopCount % 1000 == 0 && wifiMulti.run() != WL_CONNECTED) {
        Serial.println("WiFi not connected!");
    }

    OTA_loop();
    server.handleClient();
}