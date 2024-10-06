#include "player.h"
#include "Audio.h"

Audio audio2;

// Digital I/O used
#define I2S_DOUT 25
#define I2S_BCLK 27
#define I2S_LRC 26

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
        //  audio2.stopSong();
    }

    Serial.println("Open stream");
    Serial.println(url.c_str());
    audio2.connecttohost(url.c_str(), "sergio", "sergio"); //  128k mp3
}

void stopSong()
{
    audio2.stopSong();
}

bool loopPlayer()
{
    if (audio2.isRunning())
    {
        audio2.loop();
    }
    else
    {
        //  Serial.println("finish");
    }

    return true;
}

// optional
void audio_info(const char *info)
{
    Serial.print("info        ");
    Serial.println(info);
}
void audio_id3data(const char *info)
{ // id3 metadata
    Serial.print("id3data     ");
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
