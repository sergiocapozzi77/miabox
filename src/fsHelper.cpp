#include "fsHelper.h"

#include "SPI.h"
#include "SD.h"

// #include "SdFat.h"

// SdFat sd;

#define SD_CS_PIN -1 // 15  // or 5

// These pins will be use for SPI2
#define SD_CLK_PIN 12
#define SD_MOSI_PIN 11
#define SD_MISO_PIN 13

// Sd2Card card;

bool SDSetup()
{
    // pinMode(SD_CS_PIN, OUTPUT);
    // SdSpiConfig cfg(SD_CS_PIN, )
    // SPI.begin(SD_CLK_PIN, SD_MISO_PIN, SD_MOSI_PIN, SD_CS_PIN);

    if (!SD.begin())
    //  if (!SD.begin())
    {
        Serial.println("Card Mount Failed");
        return false;
    }
    // uint8_t cardType = SD.cardType();

    // if (cardType == CARD_NONE)
    // {
    //     Serial.println("No SD card attached");
    //     return false;
    // }

    // Serial.print("SD Card Type: ");
    // if (cardType == CARD_MMC)
    // {
    //     Serial.println("MMC");
    // }
    // else if (cardType == CARD_SD)
    // {
    //     Serial.println("SDSC");
    // }
    // else if (cardType == CARD_SDHC)
    // {
    //     Serial.println("SDHC");
    // }
    // else
    // {
    //     Serial.println("UNKNOWN");
    // }

    // uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    // Serial.printf("SD Card Size: %lluMB\n", cardSize);

    // if (!SD.exists("/mp3"))
    // {
    //     SD.mkdir("/mp3");
    //     Serial.println("Created mp3 folder");
    // }

    return true;
}

/*
bool deleteIfExists(fs::FS &fs, const char *path)
{
    Serial.printf("deleting: %s\r\n", path);
    if (fs.exists(path))
    {
        Serial.printf("deleted: %s\r\n", path);
        return fs.remove(path);
    }

    return true;
}

bool fileExists(fs::FS &fs, const char *path)
{
    return fs.exists(path);
}

bool createFolder(fs::FS &fs, const char *path)
{
    return fs.mkdir(path);
}

String getExtension(String fileName)
{
    return fileName.substring(fileName.lastIndexOf('.'));
}
/*
void getDirContent(fs::FS &fs, const char *dirname, int &count, String *files)
{
    count = 0;

    File root = fs.open(dirname);
    if (!root)
    {
        Serial.println("- failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (!file.isDirectory())
        {
            Serial.print("FILE: ");
            Serial.print(file.name());
            Serial.print(" - ");
            Serial.println(file.size());
            files[count++] = String(file.name());
        }
        file = root.openNextFile();
    }
}

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
    Serial.printf("Listing directory: %s\r\n", dirname);

    int count;
    String files[255];
    getDirContent(fs, dirname, count, files);
    for (int i = 0; i < count; i++)
    {
        Serial.print("  FILE: ");
        Serial.println(files[i]);
    }

}

bool writeFile(fs::FS &fs, const char *path, const char *message, const int len)
{
    Serial.printf("Writing file: %s\r\n", path);

    bool ret = false;
    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("- failed to open file for writing");
        return false;
    }
    if (file.write((uint8_t *)message, len))
    {
        Serial.println("- file written");
        ret = true;
    }
    else
    {
        Serial.println("- frite failed");
    }

    file.close();

    return ret;
}

File openFile(fs::FS &fs, const char *mode, const char *path)
{
    File file = fs.open(path, mode);
    if (!file)
    {
        Serial.println("- failed to open file for appending");
        return (File)NULL;
    }

    return file;
}

void closeFile(File file)
{
    file.flush();
    file.close();
    Serial.println("Closing file");
}

bool appendFile(File file, const char *message, const int len)
{
    // Serial.printf("Appending to file: %s\r\n", path);

    bool ret = false;
    if (file.write((uint8_t *)message, len))
    {
        ret = true;
    }
    else
    {
        Serial.println("- append failed");
    }

    return ret;
}

bool startReadingIndex(fs::FS &fs)
{
    indexFile = fs.open("/index.txt");
    if (!indexFile || indexFile.isDirectory())
    {
        Serial.println("- failed to open file for reading");
        return false;
    }

    return true;
}

void endReadingIndex()
{
    indexFile.close();
}

void readNextIndexConfig(StaticJsonDocument<200> &indexDoc)
{
    if (indexFile.available())
    {
        String line = indexFile.readStringUntil('\n');
        Serial.print("Reading: ");
        Serial.println(line);
        // Deserialize the JSON document
        DeserializationError error = deserializeJson(indexDoc, line.c_str());

        // Test if parsing succeeds.
        if (error)
        {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
            return;
        }
        return;
    }
    else
    {
        indexDoc = StaticJsonDocument<200>();
    }

    return;
}

void readFile(fs::FS &fs, const char *path)
{
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if (!file || file.isDirectory())
    {
        Serial.println("- failed to open file for reading");
        return;
    }

    Serial.println("- read from file:");
    while (file.available())
    {
        Serial.println(file.readStringUntil('\n'));
        Serial.println("--------");
    }

    file.close();
}

String readAllFile(fs::FS &fs, const char *path)
{
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if (!file || file.isDirectory())
    {
        Serial.println("- failed to open file for reading");
        return String("");
    }

    String content = file.readString();

    Serial.print("Read: ");
    Serial.print(content);

    Serial.println("Closing file");
    file.close();

    return content;
}*/