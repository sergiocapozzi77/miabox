
#ifndef RFID_H
#define RFID_H

#include <Arduino.h>

#include <PN5180.h>
#include <PN5180ISO15693.h>
#include <PN5180ISO14443.h>

#define PN5180_NSS 7
#define PN5180_BUSY 8
#define PN5180_RST 15

class RfId
{
    String code;
    uint8_t uid[8];
    char buffer[5];
    unsigned long lastCheckISO15693;
    PN5180ISO15693 nfcISO15693;
    PN5180ISO14443 nfcISO14443;
    void showIRQStatus(uint32_t irqStatus);

public:
    RfId();
    String checkISO15693Card();
    void setup();
};

extern RfId rfid;

#endif