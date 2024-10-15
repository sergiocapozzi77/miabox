#include "player.h"

#include "AudioTools.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"

URLStream url;
I2SStream out;                                       // Final output of decoded stream.
EncodedAudioStream dec(&out, new MP3DecoderHelix()); // Decoding stream
StreamCopy copier(dec, url);                         // copy url to decoder
// AudioPlayer player(url, out, dec);

// Digital I/O used
#define I2S_DOUT 6
#define I2S_BCLK 5
#define I2S_LRC 4

void setupPlayer()
{
    AudioLogger::instance().begin(Serial, AudioLogger::Info);
    auto config = out.defaultConfig(TX_MODE);
    config.pin_bck = I2S_BCLK;
    config.pin_ws = I2S_LRC;
    config.pin_data = I2S_DOUT;
    // config.i2s_format = I2S_LSB_FORMAT;

    // config.pin_mck = 0; // optional master clock pin
    // config.i2s_format = I2S_STD_FORMAT; // default format
    // config.is_master = true; // default esp32 is master

    out.begin(config);

    // setup I2S based on sampling rate provided by decoder
    dec.begin();

    // refreshDirContent2();
}

void playStream(String urlToplay)
{
    // if (out.isActive())
    // {
    //     Serial.println("Stop player");
    //     audio2.stopSong();
    // }

    Serial.println("Open stream");
    Serial.println(urlToplay.c_str());
    url.begin(urlToplay.c_str(), "audio/mp3");
    // ur.connecttohost(url.c_str(), 0, "sergio", "sergio"); //  128k
}

unsigned int getLastPosition()
{
    return 0;
}

void setPosition(unsigned int pos)
{
}

void stopSong()
{
    Serial.println("Stop song");
    copier.end();
}

bool isAudioRunning()
{
    return true;
}

bool loopPlayer()
{
    copier.copy();

    return true;
}