
#include "rfid.hpp"

RfId::RfId() : nfc(RF_SS, RF_BUSY_PIN, RF_RST_PIN)
{
}

void RfId::setup()
{
    Serial.println(F("PN5180 ISO15693 Demo Sketch"));

    nfc.begin(RF_SCK, RF_MISO, RF_MOSI, RF_SS);
    Serial.println(F("----------------------------------"));
    Serial.println(F("PN5180 Hard-Reset..."));
    nfc.reset();

    Serial.println(F("----------------------------------"));
    Serial.println(F("Reading product version..."));
    uint8_t productVersion[2];
    nfc.readEEprom(PRODUCT_VERSION, productVersion, sizeof(productVersion));
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
    nfc.readEEprom(FIRMWARE_VERSION, firmwareVersion, sizeof(firmwareVersion));
    Serial.print(F("Firmware version="));
    Serial.print(firmwareVersion[1]);
    Serial.print(".");
    Serial.println(firmwareVersion[0]);

    Serial.println(F("----------------------------------"));
    Serial.println(F("Reading EEPROM version..."));
    uint8_t eepromVersion[2];
    nfc.readEEprom(EEPROM_VERSION, eepromVersion, sizeof(eepromVersion));
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
    nfc.setupRF();
}

String RfId::checkCard()
{
}