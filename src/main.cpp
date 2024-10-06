#include <Arduino.h>
#include "WiFiMulti.h"
#include "player.h"
#include "playlist.hpp"

const char *ssid = "TP-Link_8724";
const char *password = "40950211";
#define I2S_DOUT 25
#define I2S_BCLK 27
#define I2S_LRC 26

#define BUTTON1_PIN 12 // The ESP32 pin GPIO21 connected to the button
#define BUTTON2_PIN 14 // The ESP32 pin GPIO21 connected to the button
void IRAM_ATTR handleButton1Press();
void IRAM_ATTR handleButton2Press();

boolean pressed = false;

void setup()
{
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);


  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  // esp_sleep_enable_timer_wakeup(uS_TO_S_FACTOR * TIME_TO_SLEEP);
  Serial.println("wifi connected");

  setupPlayer();

  // Initialize the GPIO pin as an input
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);

  // // Attach interrupt to the GPIO pin
  // attachInterrupt(digitalPinToInterrupt(BUTTON1_PIN), handleButton1Press, RISING);
  // attachInterrupt(digitalPinToInterrupt(BUTTON2_PIN), handleButton2Press, RISING);

  playlist.loadPlaylist("1");

  playlist.playNext();
}

void IRAM_ATTR handleButton1Press()
{
  Serial.println("Button pressed");
  playlist.stop();
}

void IRAM_ATTR handleButton2Press()
{
  Serial.println("Button pressed");
  playlist.playNext();
}

void loop()
{
  loopPlayer();

  if (!pressed && digitalRead(BUTTON1_PIN) == LOW)
  {
    pressed = true;
  }

  if (pressed && digitalRead(BUTTON1_PIN) == HIGH)
  {
    playlist.playNext();
    pressed = false;
  }
  // if (Serial.available())
  // { // put streamURL in serial monitor
  //   audio.stopSong();
  //   String r = Serial.readString();
  //   r.trim();
  //   if (r.length() > 5)
  //     audio.connecttohost(r.c_str());
  //   log_i("free heap=%i", ESP.getFreeHeap());
  // }
}
