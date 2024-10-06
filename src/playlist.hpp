// http://192.168.0.141:4040/rest/getPlaylists?u=sergio&p=sergio&v=1.16.1&c=myapp&f=json

#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <Arduino.h>
#include <vector>

class Playlist
{
    std::vector<String> songs;
    int currentSong;

    String downloadIndex(const char *name);
    bool readPlayList(String content);

public:
    Playlist();
    bool loadPlaylist(const char *name);
    void playNext();
    void stop();
};

extern Playlist playlist;

#endif