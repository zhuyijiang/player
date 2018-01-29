#ifndef MUSICCONTROLS_H
#define MUSICCONTROLS_H

#include "musicdata.h"

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>

class QQMusicAPI;
class MusicData;
class VolumeButton;

namespace Ui{
class MusicControl;
}

class MusicControl : public QWidget
{
    Q_OBJECT

public:
    MusicControl(QWidget* parent);
    ~MusicControl();

    static QString numder(qint64 time);

public slots:
    void setData(MusicData data);
    void setCurrentTime(qint64 time);
    void setIcon(QMediaPlayer::State state);
    void setOrder(QMediaPlaylist::PlaybackMode mode);
    void sliderMove(int time);
    void setMusicCover(QPixmap cover);

signals:
    void next();
    void pause();
    void previous();
    void orderChange();
    void visible();

    void sliderValue(qint64 time);
    void volumeValue(int volume);

private:
    QQMusicAPI* api;
    VolumeButton* volumeButton;
    Ui::MusicControl* ui;
};

#endif // MUSICCONTROLS_H
