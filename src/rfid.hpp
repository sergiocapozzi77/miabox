
#ifndef RFID_H
#define RFID_H

#include <Arduino.h>

#include <PN5180.h>
#include <PN5180ISO15693.h>

#define RF_SS_PIN 7
#define RF_RST_PIN 15
#define RF_BUSY_PIN 8

#define RF_SCK 18
#define RF_MISO 16
#define RF_MOSI 17
#define RF_SS 7

class RfId
{
    int jumpCheck;
    unsigned long lastCheck;
    PN5180ISO15693 nfc;

public:
    RfId();
    String checkCard();
    void setup();
};

#endif