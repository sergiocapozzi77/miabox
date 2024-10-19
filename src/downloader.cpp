#include "downloader.h"
#include <Arduino.h>
#include <HTTPClient.h>
#include <SD.h>

#define MAX_HTTP_RECV_BUFFER 2048

String fetchData(const char *link)
{
    String ret;
    WiFiClient *client = new WiFiClient;
    if (client)
    {
        // client->setCACert(rootCACertificate);
        // client->setInsecure();

        {
            // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is
            HTTPClient https;

            Serial.printf("[HTTPS] begin, connecting to %s\n", link);
            if (https.begin(*client, link))
            { // HTTPS
                Serial.print("[HTTPS] GET...\n");
                // start connection and send HTTP header
                int httpCode = https.GET();

                // httpCode will be negative on error
                if (httpCode > 0)
                {
                    Serial.printf("[HTTPS] GET httpCode: %d\n", httpCode);
                    // file found at server
                    if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
                    {
                        ret = https.getString();
                    }
                    else
                    {
                        Serial.printf("[HTTP] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
                        return "";
                    }
                }
                else
                {
                    Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
                    return "";
                }

                Serial.println("Http End");
                https.end();
            }
            else
            {
                Serial.printf("[HTTPS] Unable to connect\n");
                return "";
            }

            // End extra scoping block
        }

        delete client;
    }
    else
    {
        Serial.println("Unable to create client");
        return "";
    }

    Serial.println("Returning string");
    return ret;
}

void downloadFileTask(void *parameter)
{
    DownloadParams *params = (DownloadParams *)parameter;

    if (WiFi.status() == WL_CONNECTED)
    {
        HTTPClient http;

        http.begin(params->url);
        int httpCode = http.GET();

        if (httpCode > 0)
        {
            if (httpCode == HTTP_CODE_OK)
            {
                File file = SD.open(params->filename, FILE_WRITE);
                if (!file)
                {
                    Serial.println("Failed to open file for writing");
                    vTaskDelete(NULL); // End this task
                    delete params;     // Clean up allocated memory
                    return;
                }

                // Read data in chunks
                const size_t bufferSize = 512; // Adjust chunk size if needed
                uint8_t buffer[bufferSize];
                int len = http.getSize();
                WiFiClient *stream = http.getStreamPtr();

                while (http.connected() && (len > 0 || len == -1))
                {
                    // Read data in chunks
                    size_t bytesRead = stream->readBytes(buffer, sizeof(buffer));
                    file.write(buffer, bytesRead);

                    if (len > 0)
                    {
                        len -= bytesRead;
                    }
                }
                file.close();
                Serial.println("File downloaded and saved to SD card.");
            }
            else
            {
                Serial.printf("Error: %s\n", http.errorToString(httpCode).c_str());
            }
        }
        else
        {
            Serial.printf("Unable to connect: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end(); // Close connection
    }
    else
    {
        Serial.println("WiFi not connected");
    }

    delete params;     // Clean up allocated memory
    vTaskDelete(NULL); // End this task when done
}