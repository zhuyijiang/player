#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMap>

#include "musicdata.h"

class VolumeButton;
class TitleBar;
class SearchDialog;
class QJsonValue;
class QNetworkReply;
class MusicControl;
class SearchWidget;
class MusicCover;

namespace Ui {
class MusicPlayer;
}


class MusicPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit MusicPlayer(QWidget *parent = 0);
    ~MusicPlayer();

public slots:
    void setCoverViaible();
    void jump(const QModelIndex &index);//跳到playlist某个媒体
    void setPlaybackMode();
    void currentIndexChanged(int position);
    void playClicked();
    void addMedia(QString song_name,
                  QString singer_name,
                  QString album_name,
                  QString song_url,
                  QString image_url,
                  QString lrc_url,
                  int song_time);//添加媒体

private:
    TitleBar* titleBar;//自定义标题栏
    SearchWidget* searchWidget;
    MusicControl* musicControl;
    MusicCover* musicCover;//自定义封面

    QMediaPlayer* player;
    QMediaPlaylist* playlist;

    QList <MusicData> dataList;

    Ui::MusicPlayer *ui;
};

#endif // MUSICPLAYER_H
