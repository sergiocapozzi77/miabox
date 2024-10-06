#include "downloader.h"
#include <Arduino.h>
#include <HTTPClient.h>

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
