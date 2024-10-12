#ifndef PLAYER_H
#define PLAYER_H

#include <Arduino.h>

void setupPlayer();
bool loopPlayer();
void playStream(String);
void stopSong();
bool isAudioRunning();
#endif