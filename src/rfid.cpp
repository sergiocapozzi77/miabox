
#include "rfid.hpp"

RfId::RfId() : rfid(SS_PIN, RST_PIN)
{
}

void RfId::setup()
{
    SPI.begin(SCK, MISO, MOSI, SS); // Init SPI bus
    rfid.PCD_Init();                // Init MFRC522

    for (byte i = 0; i < 6; i++)
    {
        key.keyByte[i] = 0xFF;
    }

    Serial.println(F("This code scan the MIFARE Classsic NUID."));
    lastCheck = millis();
    jumpCheck = 2;
}

String RfId::checkCard()
{
    if (millis() - lastCheck < 1000)
    {
        return "Same";
    }

    lastCheck = millis();

    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if (!rfid.PICC_IsNewCardPresent())
    {
        Serial.println("No new card present");
    }

    // Verify if the NUID has been readed
    if (!rfid.PICC_ReadCardSerial())
    {
        if (jumpCheck > 0)
        {
            Serial.println("Jump check");
            jumpCheck--;
        }
        else
        {
            Serial.println("Read card serial error");
            return "No";
        }
    }

    Serial.print(F("PICC type: "));
    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    Serial.println(rfid.PICC_GetTypeName(piccType));
    String code = "";

    // Check is the PICC of Classic MIFARE type
    if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
        piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
        piccType != MFRC522::PICC_TYPE_MIFARE_4K)
    {
        Serial.println(F("Your tag is not of type MIFARE Classic."));
        return "No";
    }

    if (rfid.uid.uidByte[0] != nuidPICC[0] ||
        rfid.uid.uidByte[1] != nuidPICC[1] ||
        rfid.uid.uidByte[2] != nuidPICC[2] ||
        rfid.uid.uidByte[3] != nuidPICC[3])
    {
        Serial.println(F("A new card has been detected."));

        // Store NUID into nuidPICC array
        for (byte i = 0; i < 4; i++)
        {
            nuidPICC[i] = rfid.uid.uidByte[i];
        }

        Serial.println(F("The NUID tag is:"));
        Serial.println();

        char buffer[5];
        for (byte i = 0; i < rfid.uid.size; i++)
        {
            code += rfid.uid.uidByte[i] < 0x10 ? "0" : "";
            code += itoa(rfid.uid.uidByte[i], buffer, HEX);
        }

        code.toUpperCase();
        jumpCheck = 2;

        Serial.println(code);
    }
    else
    {
        return "Same";
    }

    // Halt PICC
    rfid.PICC_HaltA();

    // Stop encryption on PCD
    // rfid.PCD_StopCrypto1();

    return code;
}