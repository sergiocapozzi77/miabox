#include "player.h"
#include "Audio.h"
#include "ledmanager.hpp"

Audio audio2;

// Digital I/O used
#define I2S_DOUT 6
#define I2S_BCLK 5
#define I2S_LRC 4

void setupPlayer()
{
    //  randomSeed(2343);
    audio2.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio2.setVolume(21);

    // refreshDirContent2();
}

void playStream(String url)
{
    if (audio2.isRunning())
    {
        Serial.println("Stop player");
        audio2.stopSong();
    }

    Serial.println("Open stream");
    Serial.println(url.c_str());
    ledManager.bluOn();
    audio2.connecttohost(url.c_str(), "sergio", "sergio"); //  128k
}

unsigned int getLastPosition()
{
    return audio2.getAudioCurrentTime();
}

void setPosition(unsigned int pos)
{
    audio2.setTimeOffset(pos);
}

void volumeUp()
{
    Serial.println("Volume Up");
    audio2.setVolume(audio2.getVolume() + 1);
}

void volumeDown()
{
    Serial.println("Volume Down");
    audio2.setVolume(audio2.getVolume() - 1);
}

void stopSong()
{
    Serial.println("Stop song");
    audio2.stopSong();
}

bool isAudioRunning()
{
    return audio2.isRunning();
}

bool loopPlayer()
{
    if (audio2.isRunning())
    {
        audio2.loop();
    }
    else
    {
        return false;
    }

    return true;
}

// optional
void audio_info(const char *info)
{
    Serial.printf("%d: info        ", millis());
    Serial.println(info);
    if (String(info).startsWith("BitRate"))
    {
        ledManager.bluOff();
    }
}
void audio_id3data(const char *info)
{ // id3 metadata
    Serial.printf("%d: id3data     ", millis());
    Serial.println(info);
}
void audio_eof_mp3(const char *info)
{ // end of file
    Serial.print("eof_mp3     ");
    Serial.println(info);
}
void audio_showstation(const char *info)
{
    Serial.print("station     ");
    Serial.println(info);
}
void audio_showstreamtitle(const char *info)
{
    Serial.print("streamtitle ");
    Serial.println(info);
}
void audio_bitrate(const char *info)
{
    ledManager.bluOff();
    Serial.print("bitrate     ");
    Serial.println(info);
}
void audio_commercial(const char *info)
{ // duration in sec
    Serial.print("commercial  ");
    Serial.println(info);
}
void audio_icyurl(const char *info)
{ // homepage
    Serial.print("icyurl      ");
    Serial.println(info);
}
void audio_lasthost(const char *info)
{ // stream URL played
    Serial.print("lasthost    ");
    Serial.println(info);
}
