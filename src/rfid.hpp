
#ifndef RFID_H
#define RFID_H

#include <Arduino.h>

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 7
#define RST_PIN 0

#define SCK 18
#define MISO 16
#define MOSI 17
#define SS 7

class RfId
{
    int jumpCheck;
    unsigned long lastCheck;
    MFRC522 rfid; // Instance of the class

    MFRC522::MIFARE_Key key;

    // Init array that will store new NUID
    byte nuidPICC[4];

public:
    RfId();
    String checkCard();
    void setup();
};

#endif