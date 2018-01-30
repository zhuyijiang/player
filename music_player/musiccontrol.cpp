#include "musiccontrol.h"
#include "ui_musiccontrol.h"
#include "qqmusic_api.h"
#include "volumebutton.h"

#include <QPixmap>

MusicControl::MusicControl(QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::MusicControl)
{
    ui->setupUi(this);

    QPalette pal;
    pal.setBrush(QPalette::Window, QBrush(QImage(":/images/cover").scaled(ui->coverWidget->size())));
    ui->coverWidget->setPalette(pal);

    api = new QQMusicAPI(this);
    connect(api, &QQMusicAPI::signalCover, this, &MusicControl::setMusicCover);

    volumeButton = new VolumeButton(this);
    ui->controlLayout->addWidget(volumeButton);
    connect(volumeButton, &VolumeButton::volumeChanged, this, &MusicControl::volumeValue);

    connect(ui->coverButton, &QPushButton::clicked, this, &MusicControl::visible);
    connect(ui->previousButton, &QPushButton::clicked, this, &MusicControl::previous);
    connect(ui->nextButton,&QPushButton::clicked, this, &MusicControl::next);
    connect(ui->playButton, &QPushButton::clicked, this, &MusicControl::pause);
    connect(ui->orderButton, &QPushButton::clicked, this, &MusicControl::orderChange);
    connect(ui->timeSlider,&QSlider::sliderMoved, this, &MusicControl::sliderMove);
}

MusicControl::~MusicControl()
{
    delete ui;
}

QString MusicControl::numder(qint64 time)
{
    int second = (time % 60000) / 1000;
    int minute = time / 60000;

    if(second < 10 && minute < 10)
        return QString("0%1:0%2")
                .arg(QString::number(minute))
                .arg(QString::number(second));
    else if(second > 10 && minute < 10)
        return QString("0%1:%2")
                .arg(QString::number(minute))
                .arg(QString::number(second));
    else if(second < 10 && minute > 10)
        return QString("%1:0%2")
                .arg(QString::number(minute))
                .arg(QString::number(second));
    else
        return QString("%1:%2")
                .arg(QString::number(minute))
                .arg(QString::number(second));
}

void MusicControl::setData(MusicData data)
{
    ui->timeSlider->setRange(0, data.songTime);
    ui->musicTime->setText("/" + MusicControl::numder(qint64(data.songTime * 1000)));
    ui->musicTitle->setText(data.songName + " - " + data.singerName);

    api->getCover(data.coverUrl);
}

void MusicControl::setCurrentTime(qint64 time)
{
    ui->timeSlider->setValue(time / 1000);
    ui->currenyTime->setText(MusicControl::numder(time));
}

void MusicControl::setIcon(QMediaPlayer::State state)
{
    if(state == QMediaPlayer::PlayingState) {
        ui->playButton->setIcon(QIcon(":/icon/stop.png"));
    } else {
        ui->playButton->setIcon(QIcon(":/icon/play.png"));
    }
}

void MusicControl::setOrder(QMediaPlaylist::PlaybackMode mode)
{
    switch (mode) {
    case QMediaPlaylist::CurrentItemInLoop:
        ui->orderButton->setIcon(QIcon(":/icon/cycle.png"));
        break;
    case QMediaPlaylist::Loop:
        ui->orderButton->setIcon(QIcon(":/icon/order.png"));
        break;
    case QMediaPlaylist::Random:
        ui->orderButton->setIcon(QIcon(":/icon/random.png"));
        break;
    case QMediaPlaylist::Sequential:
        break;
    case QMediaPlaylist::CurrentItemOnce:
        break;
    }
}

void MusicControl::sliderMove(int time)
{
    emit sliderValue(qint64(time * 1000));
}

void MusicControl::setMusicCover(QPixmap cover)
{
    QPalette pal;
    pal.setBrush(QPalette::Window, QBrush(cover.scaled(ui->coverWidget->size())));
    ui->coverWidget->setPalette(pal);
}

void MusicControl::setCoverButton(bool viaible)
{
    if(viaible)
        ui->coverButton->setStyleSheet(tr("QPushButton#coverButton{ border-style:solid;}"
                                          "QPushButton#coverButton:hover{image: url(:/icon/scaled.png);background-color: rgba(50, 50, 50, 100);}"));
    else
        ui->coverButton->setStyleSheet(tr("QPushButton#coverButton{ border-style:solid;}"
                                          "QPushButton#coverButton:hover{image: url(:/icon/unfold.png);background-color: rgba(50, 50, 50, 100);}"));
}
