

#include "buttonmanager.hpp"

void ButtonManager::eval()
{
    if (!pressed && digitalRead(pin) == LOW)
    {
        pressed = true;
    }
    if (pressed && digitalRead(pin) == HIGH)
    {
        Serial.printf("Button %s pressed\n", name);

        action();
        pressed = false;
    }
}