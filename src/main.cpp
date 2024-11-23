#include <Arduino.h>
#include <WiFiManager.h>
#include "player.h"
#include "playlist.hpp"
#include "esp_wifi.h"
#include "rfid.hpp"
#include "fsHelper.h"
#include "buttonmanager.hpp"
#include "ledmanager.hpp"

const char *ssid = "TP-Link_8724";
const char *password = "40950211";

#define BUTTON_NEXTSONG_PIN 38
#define BUTTON_STOPSONG_PIN 39
#define BUTTON_VOLUMEUP_PIN 1
#define BUTTON_VOLUMEDOWN_PIN 2

ButtonManager buttonNext(BUTTON_NEXTSONG_PIN, "Next");
ButtonManager buttonStop(BUTTON_STOPSONG_PIN, "Stop");
ButtonManager buttonVolUp(BUTTON_VOLUMEUP_PIN, "Volume Up");
ButtonManager buttonVolDown(BUTTON_VOLUMEDOWN_PIN, "Volume Down");

boolean pressed = false;
String currentCard = "";
String previousCard = "";

void playlistStop();
void playlistNext();

void setupWifi()
{
  WiFi.mode(WIFI_AP_STA); // explicitly set mode, esp defaults to STA+AP

  // WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;

  // reset settings - wipe credentials for testing
  // wm.resetSettings();

  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect("miabox"); // password protected ap

  if (!res)
  {
    Serial.println("Failed to connect");
    ESP.restart();
  }
  else
  {
    // if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
  }
}

void setup()
{
  Serial.begin(115200);
  esp_wifi_set_ps(WIFI_PS_NONE);
  // WiFi.mode(WIFI_STA);
  // WiFi.begin(ssid, password);

  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("wifi connected");
  setupWifi();

  ledManager.setup();

  // if (!SDSetup())
  //  {
  //    Serial.println("Unable to read SD");
  //    delay(3000);
  //    ESP.restart();
  //  }
  //  else
  //  {
  //    // listDir(SD, "/", 0);
  //  }

  Serial.println("Setup player");
  setupPlayer();
  Serial.println("Setup rfid");
  rfid.setup();

  // Initialize the GPIO pin as an input
  pinMode(BUTTON_NEXTSONG_PIN, INPUT_PULLUP);
  pinMode(BUTTON_STOPSONG_PIN, INPUT_PULLUP);
  pinMode(BUTTON_VOLUMEUP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_VOLUMEDOWN_PIN, INPUT_PULLUP);

  buttonStop.attachAction(playlistStop);
  buttonNext.attachAction(playlistNext);
  buttonVolDown.attachAction(volumeDown);
  buttonVolUp.attachAction(volumeUp);

  playlist.getPlaylists();

  // playlist.loadPlaylist("7304CC2C");
  // playlist.playNext();
}

void playlistStop()
{
  playlist.stop();
}

void playlistNext()
{
  playlist.playNext();
}

void loop()
{
  playlist.loopPlaylist();

  buttonStop.eval();
  buttonNext.eval();
  buttonVolUp.eval();
  buttonVolDown.eval();

  String card = rfid.getCode();
  if (card == "Same")
  {
    // don't do anything
    // Serial.println("Samecard do nothing");
  }
  else if (card == "No")
  {
    currentCard = "";
    if (playlist.isPlaying)
    {
      Serial.println("Current card is No, stopping");
      playlist.stop();
    }
  }
  else
  {
    if (card != currentCard)
    {
      if (card != "No" && currentCard != "No" && currentCard != "")
      {
        Serial.printf("Suspicious new card %s current card %s\n", card, currentCard);
        return;
      }

      Serial.println("New card found: " + card);
      Serial.println("CurrentCard was: " + currentCard);
      if (card != previousCard)
      {
        playlist.resetPosition();
      }
      else
      {
      }

      currentCard = card;
      previousCard = card;
      if (playlist.loadPlaylist(card))
      {
        playlist.play();
      }
      // createFolder(SD, ("/" + card).c_str());
      // Serial.printf("Created folder %s\n", "/" + card);
      // listDir(SD, ("/" + card).c_str(), 0);
    }
    else
    {
      // Serial.printf("Same card found: is %s\n", card.c_str());
    }
  }
}
