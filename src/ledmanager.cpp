#include "ledmanager.hpp"

#define GREEN_LED_PIN 41
#define RED_LED_PIN 42
#define BLU_LED_PIN 40

LedManager ledManager;

void LedManager::setup()
{
    pinMode(RED_LED_PIN, OUTPUT);
    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(BLU_LED_PIN, OUTPUT);

    redOn();
    greenOn();
    bluOn();
    delay(1000);
    redOff();
    greenOff();
    bluOff();
}

void LedManager::bluOn()
{
    digitalWrite(BLU_LED_PIN, HIGH);
}

void LedManager::bluOff()
{
    digitalWrite(BLU_LED_PIN, LOW);
}

void LedManager::redOn()
{
    digitalWrite(RED_LED_PIN, HIGH);
}

void LedManager::redOff()
{
    digitalWrite(RED_LED_PIN, LOW);
}

void LedManager::greenOn()
{
    digitalWrite(GREEN_LED_PIN, HIGH);
}

void LedManager::greenOff()
{
    digitalWrite(GREEN_LED_PIN, LOW);
}