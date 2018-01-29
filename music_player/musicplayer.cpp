#include "ui_musicplayer.h"
#include "musicplayer.h"
#include "musiccontrol.h"
#include "titlebar.h"
#include "searchwidget.h"
#include "musiccover.h"

#include <QMessageBox>
#include <QMediaMetaData>
#include <QUrl>
#include <QFileInfo>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTextCodec>
#include <QTime>

MusicPlayer::MusicPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MusicPlayer)
{
    ui->setupUi(this);

    dataList = QList <MusicData>();

    titleBar = new TitleBar(this);

    searchWidget = new SearchWidget(this);
    searchWidget->move(240, 40);

    musicCover = new MusicCover(this);
    musicCover->move(0, 40);
    musicCover->show();

    musicControl = new MusicControl(this);
    musicControl->move(0, 600);

    playlist = new QMediaPlaylist(this);    
    playlist->setPlaybackMode(QMediaPlaylist::Loop);

    player = new QMediaPlayer(this);
    player->setPlaylist(playlist);
    player->setVolume(50);

    connect(searchWidget, &SearchWidget::currentItem, this, &MusicPlayer::addMedia);

    connect(player, &QMediaPlayer::stateChanged, musicControl, &MusicControl::setIcon);
    connect(player, &QMediaPlayer::positionChanged, musicControl, &MusicControl::setCurrentTime);
    connect(player, &QMediaPlayer::stateChanged, musicCover, &MusicCover::setRotate);
    connect(player, &QMediaPlayer::positionChanged, musicCover, &MusicCover::scrollLrc);

    connect(playlist, &QMediaPlaylist::currentIndexChanged, this, &MusicPlayer::currentIndexChanged);
    connect(playlist, &QMediaPlaylist::playbackModeChanged, musicControl, &MusicControl::setOrder);

    connect(musicControl, &MusicControl::previous, playlist, &QMediaPlaylist::previous);
    connect(musicControl, &MusicControl::pause, this, &MusicPlayer::playClicked);
    connect(musicControl, &MusicControl::next, playlist, &QMediaPlaylist::next);
    connect(musicControl, &MusicControl::orderChange, this, &MusicPlayer::setPlaybackMode);
    connect(musicControl, &MusicControl::volumeValue, player, &QMediaPlayer::setVolume);
    connect(musicControl, &MusicControl::sliderValue, player, &QMediaPlayer::setPosition);
    connect(musicControl, &MusicControl::visible, this, &MusicPlayer::setCoverViaible);

    ui->playlistWidget->setSpacing(2);
    ui->playlistWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//取消滚动条
    ui->playlistWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//取消滚动条
    connect(ui->playlistWidget, &QListWidget::doubleClicked, this, &MusicPlayer::jump);

    setWindowFlags(Qt::FramelessWindowHint | windowFlags());//取消标题栏
}

MusicPlayer::~MusicPlayer()
{
    delete ui;
}


void MusicPlayer::addMedia(QString song_name,
                           QString singer_name,
                           QString album_name,
                           QString song_url,
                           QString image_url,
                           QString lrc_url,
                           int song_time)
{
    QListWidgetItem* item = new QListWidgetItem(song_name + " - " + singer_name);
    item->setIcon(QIcon(":/icon/song.png"));
    ui->playlistWidget->addItem(item);
    playlist->addMedia(QMediaContent(QUrl(song_url)));

    MusicData data;
    data.songName = song_name;
    data.singerName = singer_name;
    data.albumName = album_name;
    data.songUrl = song_url;
    data.coverUrl = image_url;
    data.lrcUrl = lrc_url;
    data.songTime = song_time;

    dataList.append(data);
}

void MusicPlayer::setPlaybackMode()
{
    switch (playlist->playbackMode()) {
    case QMediaPlaylist::CurrentItemInLoop:
        playlist->setPlaybackMode(QMediaPlaylist::Random);
        break;
    case QMediaPlaylist::Loop:
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        break;
    case QMediaPlaylist::Random:
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
        break;
    case QMediaPlaylist::Sequential:
        break;
    case QMediaPlaylist::CurrentItemOnce:
        break;
    }
}


void MusicPlayer::jump(const QModelIndex &index)
{
    if (index.isValid()) {
        playlist->setCurrentIndex(index.row());
        player->play();
    }
}


void MusicPlayer::playClicked()
{
    switch (player->state()) {
    case QMediaPlayer::StoppedState:
    case QMediaPlayer::PausedState:
        player->play();
        break;
    case QMediaPlayer::PlayingState:
        player->pause();
        break;
    }
}

void MusicPlayer::currentIndexChanged(int position)
{
    musicControl->setData(dataList.at(position));
    musicCover->setData(dataList.at(position));
}

void MusicPlayer::setCoverViaible()
{
    if(musicCover->isVisible()) {
        musicCover->hide();
    }
    else {
        musicCover->show();
    }
}


