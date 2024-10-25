
#ifndef RFID_H
#define RFID_H

#include <Arduino.h>

#include <PN5180.h>
#include <PN5180ISO15693.h>

#define PN5180_NSS 7
#define PN5180_BUSY 8
#define PN5180_RST 15

class RfId
{

    uint32_t loopCnt = 0;
    bool errorFlag = false;
    int jumpCheck;
    unsigned long lastCheck;
    PN5180ISO15693 nfc;
    void showIRQStatus(uint32_t irqStatus);

public:
    RfId();
    void checkCard();
    void setup();
};

extern RfId rfid;

#endif