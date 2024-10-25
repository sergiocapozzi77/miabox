
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

    lastCheckISO15693 = 0;
}

String RfId::checkISO15693Card()
{
    if (millis() - lastCheckISO15693 < 1000)
    {
        return "Same";
    }

    lastCheckISO15693 = millis();

    nfcISO14443.reset();
    nfcISO14443.setupRF();
    if (nfcISO14443.isCardPresent())
    {
        int8_t uidLength = nfcISO14443.readCardSerial(uid);
        if (uidLength > 0)
        {
            Serial.print(F("ISO-14443 card found, UID="));
            for (int i = 0; i < uidLength; i++)
            {
                Serial.print(uid[i] < 0x10 ? " 0" : " ");
                Serial.print(uid[i], HEX);
            }
            Serial.println();
            Serial.println(F("----------------------------------"));
            delay(1000);
            return;
        }
    }

    nfcISO15693.reset();
    nfcISO15693.setupRF();
    uint8_t password[] = {0x5B, 0x6E, 0xFD, 0x7F};
    ISO15693ErrorCode myrc = nfcISO15693.disablePrivacyMode(password);
    if (ISO15693_EC_OK == myrc)
    {
        Serial.println("disabling privacy-mode successful");
    }

    ISO15693ErrorCode rc = nfcISO15693.getInventory(uid);
    if (ISO15693_EC_OK == rc)
    {
        code = "";
        Serial.print(F("Inventory successful, UID="));
        for (int i = 0; i < 8; i++)
        {
            code += itoa(uid[7 - i], buffer, HEX);
        }

        code.toUpperCase();
        Serial.println(code);
        return code;
    }

    return "No";
}
