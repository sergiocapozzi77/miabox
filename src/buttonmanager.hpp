#ifndef BUTTONMANAGER_H
#define BUTTONMANAGER_H

#include <Arduino.h>

class ButtonManager
{
    uint8_t pin;
    const char *name;
    bool pressed;
    std::function<void(void)> action;

public:
    ButtonManager(uint8_t pin, const char *name)
    {
        this->pin = pin;
        this->name = name;
        pressed = false;
    };

    void attachAction(std::function<void(void)> action)
    {
        this->action = action;
    }

    void eval();
};

#endif