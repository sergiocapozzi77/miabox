// http://192.168.0.141:4040/rest/getPlaylists?u=sergio&p=sergio&v=1.16.1&c=myapp&f=json

#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <Arduino.h>
#include <vector>
#include <map>

class Playlist
{
    bool isPlaying;
    std::vector<String> songs;
    std::map<String, String> playlistsMap;
    int currentSong;
    unsigned int lastPosition;

    String fetchPlaylistContent(String name);
    bool readPlayList(String content);

public:
    Playlist();
    void getPlaylists();
    void resetPosition();
    bool loadPlaylist(String name);
    void playNext();
    void play();
    void stop();
    void loopPlaylist();
};

extern Playlist playlist;

#endif