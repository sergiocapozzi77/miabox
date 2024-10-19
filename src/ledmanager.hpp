

#ifndef LEDMANAGER_H
#define LEDMANAGER_H

#include <Arduino.h>

class LedManager
{
public:
    void setup();

    void bluOn();
    void bluOff();

    void redOn();
    void redOff();

    void greenOn();
    void greenOff();
};

extern LedManager ledManager;

#endif