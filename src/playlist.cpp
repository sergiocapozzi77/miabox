#include "playlist.hpp"
#include <ArduinoJson.h>
#include "player.h"
#include "downloader.h"
#include "common.h"
#include "ledmanager.hpp"

Playlist playlist;

// #define REST_API "http://192.168.0.141:4040/rest/%s?u=admin&p=admin&v=1.12.0&c=myapp&f=json%s"

Playlist::Playlist()
{
    currentSong = 0;
    isPlaying = false;
}

void Playlist::getPlaylists()
{
    ledManager.bluOn();

    char url[100];
    sprintf(url, REST_API, "getPlaylists", "");
    String playlists = fetchData(url);

    // Allocate the JSON document
    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, playlists.c_str());

    // Test if parsing succeeds
    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
    }

    String result = doc["subsonic-response"]["status"].as<String>();
    if (result != "ok")
    {
        Serial.println("Error downloading playlist");
        return;
    }
    else
    {
        Serial.println("Playlist download ok");
    }

    JsonDocument playlist = doc["subsonic-response"]["playlists"];

    JsonArray items = playlist["playlist"].as<JsonArray>();

    playlistsMap.clear();
    for (int i = 0; i < items.size(); i++)
    {
        String id = items[i]["id"].as<String>();
        String name = items[i]["name"].as<String>();
        playlistsMap.insert({name, id});
    }

    Serial.printf("Loaded %d playlists\n", playlistsMap.size());
    ledManager.bluOff();
    return;
}

void Playlist::stop()
{
    ledManager.greenOff();
    isPlaying = false;
    lastPosition = getLastPosition();
    Serial.printf("Stop playlist at position %d\n", lastPosition);
    stopSong();
}

void Playlist::resetPosition()
{
    currentSong = 0;
    lastPosition = 0;
}

void Playlist::playNext()
{
    play();
    currentSong++;
}

void Playlist::play()
{
    ledManager.greenOn();
    if (currentSong >= songs.size())
    {
        if (isAudioRunning())
        {
            Serial.println("Playlist finished, stopping sound");
            stop();
        }
        return;
    }
    isPlaying = true;

    Serial.println("Play next");
    char url[200];
    sprintf(url, REST_API, "stream", (String("&format=raw&id=") + songs.at(currentSong)).c_str());
    Serial.printf("Playing %s\n", url);
    playStream(url);
    // if (lastPosition > 0)
    // {
    //     Serial.printf("Set position to %d\n", lastPosition);
    //     setPosition(lastPosition);
    // }
}

bool Playlist::loadPlaylist(String cardCode)
{
    std::map<String, String>::iterator playlistElem = playlistsMap.find(cardCode);
    if (playlistElem == playlistsMap.end())
    {
        Serial.println("Playlist not found");
        return false;
    }

    String playlistId = playlistsMap[cardCode];

    Serial.printf("Downloading playlist %s\n", playlistId);
    String data = fetchPlaylistContent(playlistId);
    if (data == "")
    {
        Serial.printf("No data in playlist %s\n", playlistId);
        return false;
    }

    // String content = readAllFile(activeFS, indexFileName.c_str());
    // Serial.println(content);
    Serial.printf("Reading playlist data for %s\n");
    readPlayList(data);

    return true;
}

void Playlist::loopPlaylist()
{
    if (!loopPlayer())
    {
        if (isPlaying)
        {
            this->playNext();
        }
    }
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

String Playlist::fetchPlaylistContent(String name)
{
    char url[100];
    sprintf(url, REST_API, "getPlaylist", (String("&id=") + name).c_str());

    return fetchData(url);
}