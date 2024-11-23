
#include "rfid.hpp"

RfId rfid;

RfId::RfId() : nfcISO15693(PN5180_NSS, PN5180_BUSY, PN5180_RST), nfcISO14443(PN5180_NSS, PN5180_BUSY, PN5180_RST)
{
}

void RfId::setup()
{
    nfcISO15693.begin();
    Serial.println(F("----------------------------------"));
    Serial.println(F("PN5180 Hard-Reset..."));
    nfcISO15693.reset();

    Serial.println(F("----------------------------------"));
    Serial.println(F("Reading product version..."));
    uint8_t productVersion[2];
    nfcISO15693.readEEprom(PRODUCT_VERSION, productVersion, sizeof(productVersion));
    Serial.print(F("Product version="));
    Serial.print(productVersion[1]);
    Serial.print(".");
    Serial.println(productVersion[0]);

    if (0xff == productVersion[1])
    { // if product version 255, the initialization failed
        Serial.println(F("Initialization failed!?"));
        Serial.println(F("Press reset to restart..."));
        Serial.flush();
        exit(-1); // halt
    }

    Serial.println(F("----------------------------------"));
    Serial.println(F("Reading firmware version..."));
    uint8_t firmwareVersion[2];
    nfcISO15693.readEEprom(FIRMWARE_VERSION, firmwareVersion, sizeof(firmwareVersion));
    Serial.print(F("Firmware version="));
    Serial.print(firmwareVersion[1]);
    Serial.print(".");
    Serial.println(firmwareVersion[0]);

    Serial.println(F("----------------------------------"));
    Serial.println(F("Reading EEPROM version..."));
    uint8_t eepromVersion[2];
    nfcISO15693.readEEprom(EEPROM_VERSION, eepromVersion, sizeof(eepromVersion));
    Serial.print(F("EEPROM version="));
    Serial.print(eepromVersion[1]);
    Serial.print(".");
    Serial.println(eepromVersion[0]);

    /*
    Serial.println(F("----------------------------------"));
    Serial.println(F("Reading IRQ pin config..."));
    uint8_t irqConfig;
    nfc.readEEprom(IRQ_PIN_CONFIG, &irqConfig, 1));
    Serial.print(F("IRQ_PIN_CONFIG=0x"));
    Serial.println(irqConfig, HEX);

    Serial.println(F("----------------------------------"));
    Serial.println(F("Reading IRQ_ENABLE register..."));
    uint32_t irqEnable;
    nfc.readRegister(IRQ_ENABLE, &irqEnable));
    Serial.print(F("IRQ_ENABLE=0x"));
    Serial.println(irqConfig, HEX);
    */

    Serial.println(F("----------------------------------"));
    Serial.println(F("Enable RF field..."));
    nfcISO15693.setupRF();

    lastCheck = 0;

    xTaskCreatePinnedToCore(
        checkCards,            // Function to implement the task
        "checkCards",          // Name of the task
        2048,                  // Stack size in words
        this,                  // Task input parameter
        2 | portPRIVILEGE_BIT, // Priority of the task
        &rfidTaskHandle,       // Task handle.
        0);                    // Core where the task should run
}

void RfId::setCode(String newCode)
{
    if (newCode != previousCode)
    {
        m.lock();
        code = newCode;
        m.unlock();
        previousCode = code;
    }
}

String RfId::getCode()
{
    String ret;
    m.lock();
    ret = String(code);
    m.unlock();

    return ret;
}

void RfId::checkCards(void *pvParameters)
{
    RfId *_this = (RfId *)pvParameters;

    const TickType_t xDelay = 500 / portTICK_PERIOD_MS;
    for (;;)
    {
        vTaskDelay(xDelay);
        // if (millis() - _this->lastCheck < 1000)
        // {
        //     _this->code = "Same";
        // }

        _this->lastCheck = millis();

        // _this->nfcISO14443.reset();
        // _this->nfcISO14443.setupRF();
        // if (_this->nfcISO14443.isCardPresent())
        // {
        //     Serial.println("isCardPresent");
        //     int8_t uidLength = _this->nfcISO14443.readCardSerial(_this->uid);
        //     if (uidLength > 0)
        //     {
        //         String tempCode = "";
        //         for (byte i = 0; i < uidLength; i++)
        //         {
        //             tempCode += _this->uid[i] < 0x10 ? "0" : "";
        //             tempCode += itoa(_this->uid[i], _this->buffer, HEX);
        //         }

        //         tempCode.toUpperCase();
        //         _this->setCode(tempCode);

        //         continue;
        //     }
        //     else
        //     {
        //         Serial.println("empty uid");
        //     }
        // }

        _this->nfcISO15693.reset();
        _this->nfcISO15693.setupRF();
        uint8_t password[] = {0x5B, 0x6E, 0xFD, 0x7F};
        ISO15693ErrorCode myrc = _this->nfcISO15693.disablePrivacyMode(password);
        if (ISO15693_EC_OK == myrc)
        {
            // Serial.println("disabling privacy-mode successful");
        }

        ISO15693ErrorCode rc = _this->getInventoryRetry(3);
        if (ISO15693_EC_OK == rc)
        {
            String tempcode = "";
            for (int i = 0; i < 7; i++)
            {
                tempcode += itoa(_this->uid[7 - i], _this->buffer, HEX);
            }

            tempcode.toUpperCase();
            // if (_this->code != "No" && _this->previousCode != _this->code)
            // {
            //     Serial.printf("Not possible to read two codes differently %s\n", _this->code);
            //     continue;
            // }

            _this->setCode(tempcode);

            continue;
        }

        if (rc != EC_NO_CARD)
        {
            Serial.printf("Card error %d\n", rc);
        }
        _this->setCode("No");
    }
}

ISO15693ErrorCode RfId::getInventoryRetry(int retryNum)
{
    ISO15693ErrorCode rc;
    for (int i = 0; i < retryNum; i++)
    {
        vTaskDelay(50 / portTICK_PERIOD_MS);
        rc = this->nfcISO15693.getInventory(this->uid);
        if (rc == ISO15693_EC_OK)
        {
            return rc;
        }

        Serial.printf("No card found %d\n", rc);
        nfcISO15693.reset();
        nfcISO15693.setupRF();
    }

    return rc;
}
