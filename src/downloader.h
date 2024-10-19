#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <Arduino.h>

struct DownloadParams
{
    String url;
    String filename;
};

String fetchData(const char *link);
void downloadFileTask(void *parameter);

#endif