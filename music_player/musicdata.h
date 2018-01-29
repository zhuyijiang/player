#ifndef MUSICDATA_H
#define MUSICDATA_H

#include <QString>

struct MusicData{
    QString songName;
    QString singerName;
    QString albumName;
    QString songUrl;
    QString coverUrl;
    QString lrcUrl;
    int songTime;
};

#endif // MUSICDATA_H
