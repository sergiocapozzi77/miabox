#include <Arduino.h>
#include "WiFiMulti.h"
#include "player.h"
#include "playlist.hpp"
#include "esp_wifi.h"
#include "rfid.hpp"

const char *ssid = "TP-Link_8724";
const char *password = "40950211";

#define BUTTON1_PIN 12 // The ESP32 pin GPIO21 connected to the button
#define BUTTON2_PIN 14 // The ESP32 pin GPIO21 connected to the button
void IRAM_ATTR handleButton1Press();
void IRAM_ATTR handleButton2Press();

boolean pressed = false;
String currentCard = "";

RfId rfid;

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
  // esp_sleep_enable_timer_wakeup(uS_TO_S_FACTOR * TIME_TO_SLEEP);
  Serial.println("wifi connected");

  Serial.println("Setup player");
  setupPlayer();
  Serial.println("Setup rfid");
  rfid.setup();

  // Initialize the GPIO pin as an input
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);

  // // Attach interrupt to the GPIO pin
  // attachInterrupt(digitalPinToInterrupt(BUTTON1_PIN), handleButton1Press, RISING);
  // attachInterrupt(digitalPinToInterrupt(BUTTON2_PIN), handleButton2Press, RISING);

  // playlist.loadPlaylist("1");

  // playlist.playNext();
  playlist.getPlaylists();
}

void IRAM_ATTR handleButton1Press()
{
  Serial.println("Button 1 pressed");
  playlist.stop();
}

void IRAM_ATTR handleButton2Press()
{
  Serial.println("Button 2 pressed");
  playlist.playNext();
}

void loop()
{
  playlist.loopPlaylist();

  if (!pressed && digitalRead(BUTTON1_PIN) == LOW)
  {
    pressed = true;
  }

  if (pressed && digitalRead(BUTTON1_PIN) == HIGH)
  {
    playlist.playNext();
    pressed = false;
  }

  String card = rfid.checkCard();
  if (card == "Same")
  {
    // don't do anything
    // Serial.println("Samecard do nothing");
  }
  else if (card == "No")
  {
    Serial.println("Current card is No, stopping");
    playlist.stop();
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
