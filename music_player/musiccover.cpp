#include "musiccover.h"
#include "ui_musiccover.h"
#include "qqmusic_api.h"
#include "blur_image.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QGraphicsPixmapItem>
#include <QStringList>
#include <QScrollBar>
#include <QBitmap>
#include <QPalette>
#include <QImage>

static QPixmap PixmapToRound(QPixmap &src, int radius);

MusicCover::MusicCover(QWidget *parent)
    :QWidget(parent)
    ,angle(0.5)
    ,ui(new Ui::MusicCover)
{
    ui->setupUi(this);

    QPalette palete;
    palete.setColor(QPalette::Background, QColor(245, 245, 245));
    ui->coverWidget->setAutoFillBackground(true);
    ui->coverWidget->setPalette(palete);

    api = new QQMusicAPI(this);
    connect(api, &QQMusicAPI::signalCover, this, &MusicCover::setCover);
    connect(api, &QQMusicAPI::signalCover, this, &MusicCover::blur);
    connect(api, &QQMusicAPI::signalLrc, this, &MusicCover::setLrcWidget);

    coverScene = new QGraphicsScene(this);

    coverOutItem = new CoverOutItem;
    coverScene->addItem(coverOutItem);
    coverOutItem->setTransformOriginPoint(200, 200);

    foreground = new Foreground;
    coverScene->addItem(foreground);
    foreground->setTransformOriginPoint(200, 200);

    QPixmap map = QPixmap(":/images/cover");
    map = PixmapToRound(map, 126);
    pixmapitem = coverScene->addPixmap(map);
    pixmapitem->moveBy(74, 74);
    pixmapitem->setTransformOriginPoint(126, 126);

    coverInItem = new CoverInItem;
    coverScene->addItem(coverInItem);
    coverInItem->setTransformOriginPoint(200, 200);

    ui->coverGraphicsView->setScene(coverScene);
    ui->coverGraphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->coverGraphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->lrcListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);//取消滚动条
    ui->lrcListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    timer = new QTimer(this);
    timer->setInterval(50);
    connect(timer, &QTimer::timeout, this, &MusicCover::rotate);
}

MusicCover::~MusicCover()
{
    delete ui;
}

void MusicCover::setRotate(QMediaPlayer::State state)
{
    if(state == QMediaPlayer::PlayingState)
        timer->start();
    else
        timer->stop();
}

void MusicCover::rotate()
{ 
    if(angle > 359.4)
        angle = 0;
    foreground->setRotation(angle);
    pixmapitem->setRotation(angle);
    angle = angle + 0.6;
}

void MusicCover::setCover(QPixmap map)
{
    disconnect(timer, &QTimer::timeout, this, &MusicCover::rotate);
    coverScene->clear();

    coverOutItem = new CoverOutItem;
    coverScene->addItem(coverOutItem);
    coverOutItem->setTransformOriginPoint(200, 200);

    foreground = new Foreground;
    coverScene->addItem(foreground);
    foreground->setTransformOriginPoint(200, 200);

    map = PixmapToRound(map, 126);
    pixmapitem = coverScene->addPixmap(map);
    pixmapitem->moveBy(74, 74);
    pixmapitem->setTransformOriginPoint(126, 126);

    coverInItem = new CoverInItem;
    coverScene->addItem(coverInItem);
    coverInItem->setTransformOriginPoint(200, 200);
    connect(timer, &QTimer::timeout, this, &MusicCover::rotate);
}

void MusicCover::setLrcWidget(QList <int> time_list, QStringList lrc_list)
{
    ui->lrcListWidget->clear();
    if(lrc_list.isEmpty()) {
        ui->lrcListWidget->addItem("暂无歌词");
    } else {
        timeList = time_list;
        ui->lrcListWidget->addItems(lrc_list);
    }
}

void MusicCover::scrollLrc(qint64 time)
{
    int t = time / 1000;
    int row = timeList.indexOf(t);
    if(row != -1) {
        if(row == 0){
            ui->lrcListWidget->verticalScrollBar()->setValue(row - 7);
            ui->lrcListWidget->item(row)->setTextColor(QColor(255, 255, 255));
        } else {
            ui->lrcListWidget->verticalScrollBar()->setValue(row - 7);
            ui->lrcListWidget->item(row)->setTextColor(QColor(255, 255, 255));
            ui->lrcListWidget->item(row - 1)->setTextColor(QColor(0, 0, 0));
        }
    }
}

void MusicCover::setLabel(QString song, QString album, QString singer)
{
    ui->songLabel->setText(song);
    ui->albumLabel->setText(album);
    ui->singerLabel->setText(singer);
}

void MusicCover::setData(MusicData data)
{
    coverData = data;
    api->getCover(data.coverUrl);
    api->getLrc(data.lrcUrl);
    setLabel(coverData.songName, coverData.albumName, coverData.singerName);
}

QPixmap PixmapToRound(QPixmap &src, int radius)
{
    if (src.isNull()) {
        return QPixmap();
    }

    QSize size(2*radius, 2*radius);
    QBitmap mask(size);
    QPainter painter(&mask);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);
    painter.setBrush(QColor(0, 0, 0));
    painter.drawRoundedRect(0, 0, size.width(), size.height(), radius, radius);

    QPixmap image = src.scaled(size);
    image.setMask(mask);
    return image;
}

void MusicCover::setBackground(QImage &image)
{
    qDebug() << "call";

    image = image.copy(radius, radius, image.width() - (2 * radius), image.height() - (2 * radius));
    QPalette palette;
    if(ui->coverWidget->width() > ui->coverWidget->height())
        palette.setBrush(QPalette::Window, QBrush(image.scaled(ui->coverWidget->size(), Qt::KeepAspectRatioByExpanding)));
    else
        palette.setBrush(QPalette::Window, QBrush(image.scaled(ui->coverWidget->size(), Qt::KeepAspectRatio)));
    ui->coverWidget->setPalette(palette);
    sender()->deleteLater();
}

void MusicCover::blur(QPixmap cover)
{
    QImage image = cover.toImage();
    Controller* controller = new Controller;
    qRegisterMetaType<QImage>("QImage&");
    connect(controller, &Controller::handleResults, this, &MusicCover::setBackground, Qt::QueuedConnection);
    controller->operate(image);
}

