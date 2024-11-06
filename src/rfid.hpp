
#ifndef RFID_H
#define RFID_H

#include <Arduino.h>

#include <PN5180.h>
#include <PN5180ISO15693.h>
#include <PN5180ISO14443.h>
#include <mutex>

#define PN5180_NSS 7
#define PN5180_BUSY 8
#define PN5180_RST 15

class RfId
{
    TaskHandle_t rfidTaskHandle;
    String code;
    String previousCode;
    std::mutex m;

    // String code;
    uint8_t uid[10];
    char buffer[5];
    unsigned long lastCheck;
    PN5180ISO15693 nfcISO15693;
    PN5180ISO14443 nfcISO14443;
    void showIRQStatus(uint32_t irqStatus);
    void setCode(String code);
    ISO15693ErrorCode getInventoryRetry(int retryNum);

public:
    RfId();
    static void checkCards(void *pvParameters);
    void setup();
    String getCode();
};

extern RfId rfid;

#endif