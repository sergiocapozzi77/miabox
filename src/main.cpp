#include <Arduino.h>
#include "WiFiMulti.h"
#include "player.h"
#include "playlist.hpp"
#include "esp_wifi.h"
#include "rfid.hpp"
#include "buttonmanager.hpp"
#include "ledmanager.hpp"

const char *ssid = "TP-Link_8724";
const char *password = "40950211";

#define BUTTON1_PIN 12
#define BUTTON2_PIN 14
#define BUTTON_VOLUMEUP_PIN 1
#define BUTTON_VOLUMEDOWN_PIN 2

ButtonManager buttonNext(BUTTON1_PIN, "Next");
ButtonManager buttonStop(BUTTON2_PIN, "Stop");
ButtonManager buttonVolUp(BUTTON_VOLUMEUP_PIN, "Volume Up");
ButtonManager buttonVolDown(BUTTON_VOLUMEDOWN_PIN, "Volume Down");

boolean pressed = false;
String currentCard = "";

RfId rfid;

void playlistStop();
void playlistNext();

void setup()
{
  Serial.begin(115200);
  esp_wifi_set_ps(WIFI_PS_NONE);
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  ledManager.setup();

  Serial.println("wifi connected");

  Serial.println("Setup player");
  setupPlayer();
  Serial.println("Setup rfid");
  rfid.setup();
  // Initialize the GPIO pin as an input
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(BUTTON_VOLUMEUP_PIN, INPUT_PULLUP);
  pinMode(BUTTON_VOLUMEDOWN_PIN, INPUT_PULLUP);

  buttonStop.attachAction(playlistStop);
  buttonNext.attachAction(playlistNext);
  buttonVolDown.attachAction(volumeDown);
  buttonVolUp.attachAction(volumeUp);

  // playlist.playNext();
  playlist.getPlaylists();
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

  String card = rfid.checkCard();
  if (card == "Same")
  {
    // don't do anything
    // Serial.println("Samecard do nothing");
  }
  else if (card == "No")
  {
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
      Serial.printf("New card found: is %s\n", card);
      playlist.resetPosition();
    }
    else
    {
      Serial.printf("Same courd found: is %s\n", card);
    }

    currentCard = card;
    playlist.loadPlaylist(card);
    playlist.play();
  }
}
