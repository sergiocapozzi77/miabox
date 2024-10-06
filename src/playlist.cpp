#include "playlist.hpp"
#include <ArduinoJson.h>
#include "player.h"
#include "downloader.h"

Playlist playlist;

#define REST_API "http://192.168.0.141:4040/rest/%s?u=admin&p=admin&v=1.12.0&c=myapp&f=json%s"

Playlist::Playlist()
{
    currentSong = 1;
}

void Playlist::stop()
{
    stopSong();
}

void Playlist::playNext()
{
    Serial.println("Play next");
    char url[200];
    sprintf(url, REST_API, "stream", (String("&format=mp3&id=") + songs.at(currentSong)).c_str());
    Serial.printf("Playing %s\n", url);
    currentSong++;
    playStream(url);
}

bool Playlist::loadPlaylist(const char *name)
{
    String data = downloadIndex(name);
    if (data == "")
    {
        return false;
    }

    // String content = readAllFile(activeFS, indexFileName.c_str());
    // Serial.println(content);
    readPlayList(data);

    return true;
}

bool Playlist::readPlayList(String content)
{
    // Allocate the JSON document
    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, content.c_str());

    // Test if parsing succeeds
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return false;
    }

    String result = doc["subsonic-response"]["status"].as<String>();
    if (result != "ok")
    {
        Serial.println("Error downloading playlist");
        return false;
    }
    else
    {
        Serial.println("Playlist download ok");
    }

    JsonDocument playlist = doc["subsonic-response"]["playlist"];

    JsonArray items = playlist["entry"].as<JsonArray>();

    songs.clear();
    for (int i = 0; i < items.size(); i++)
    {
        String id = items[i]["id"].as<String>();
        songs.push_back(id);
    }

    Serial.printf("Loaded %d songs\n", songs.size());

    return true;
}

String Playlist::downloadIndex(const char *name)
{
    char url[100];
    sprintf(url, REST_API, "getPlaylist", (String("&id=") + "3").c_str());
    String fileName = "/" + String(name) + "/index.json";

    return fetchData(url);
}

// String Playlist::downloadIndex(const char *name)
// {
//     char url[100];
//     sprintf(url, REST_API, "getPlaylist", String("&id=") + "3");
//     String fileName = "/" + String(name) + "/index.json";

//     // if (fileExists(activeFS, fileName.c_str()))
//     // {
//     //     Serial.println("Index exist, skip download");
//     //     return fileName;
//     // }
//     Serial.printf("Url: %s\n", url);
//     if (!downloadFile2(url, fileName, activeFS, false, false))
//     {
//         return "";
//     }

//     Serial.println("Reading index file");
//     listDir(activeFS, ("/" + String(name)).c_str(), 0);

//     return fileName;
// }