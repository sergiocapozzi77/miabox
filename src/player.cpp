// #include "player.h"

// #include "AudioFileSourceSD.h"
// #include "AudioGeneratorMP3.h"
// #include "AudioGeneratorWav.h"
// #include "AudioOutputI2S.h"
// #include "AudioFileSourceHTTPStream.h"
// #include "AudioFileSourceICYStream.h"
// #include "AudioFileSourceBuffer.h"

// AudioGeneratorMP3 *mp3;
// AudioGeneratorWAV *wav;
// AudioFileSourceSD *fileToPlay;
// AudioOutputI2S *out;
// AudioGenerator *audio = NULL;

// AudioFileSourceHTTPStream *httpStream;
// AudioFileSourceBuffer *buff;

// // Digital I/O used
// #define I2S_DOUT 25
// #define I2S_BCLK 27
// #define I2S_LRC 26

// int mp3FilesCount;
// String mp3Files[255];

// void setupPlayer()
// {
//     httpStream = NULL;
//     buff = NULL;

//     fileToPlay = new AudioFileSourceSD();
//     out = new AudioOutputI2S();
//     mp3 = new AudioGeneratorMP3();
//     wav = new AudioGeneratorWAV();

//     randomSeed(2343);

//     out->SetPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
// }

// void playStream(String url)
// {
//     if (mp3->isRunning())
//     {
//         mp3->stop();
//     }

//     audio = mp3;

//     if (buff != NULL)
//     {
//         buff->close();
//         delete buff;
//     }

//     if (httpStream != NULL)
//     {
//         httpStream->close();
//         delete httpStream;
//     }

//     httpStream = new AudioFileSourceHTTPStream(url.c_str());
//     httpStream->setAuthorization("admin", "admin");
//     // httpStream = new AudioFileSourceICYStream("http://localhost:4040/rest/stream?format=mp3&id=6&u=sergio&p=sergio&v=1.12.0&c=myapp");
//     buff = new AudioFileSourceBuffer(httpStream, 512);

//     Serial.println("Playing stream");
//     mp3->begin(buff, out);
// }

// bool loopPlayer()
// {
//     if (audio == NULL)
//     {
//         Serial.println("audio null");
//         return false;
//     }

//     if (audio->isRunning())
//     {
//         if (!audio->loop())
//         {
//             audio->stop();
//             //  Serial.println("stopping");
//         }

//         return true;
//     }
//     else
//     {
//         //  Serial.println("finish");
//     }

//     return false;
// }