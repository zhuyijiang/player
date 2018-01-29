#ifndef COVERITEM_H
#define COVERITEM_H

#include "musicdata.h"

#include <QWidget>
#include <QPainter>
#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>
#include <QColor>
#include <QBrush>
#include <QRectF>
#include <QTimer>
#include <QMediaPlayer>
#include <QList>
#include <QImage>

class QGraphicsScene;
class QQMusicAPI;


namespace Ui{
class MusicCover;
}

const static int r = 200;//绘制圆的半径

class CoverOutItem : public QGraphicsItem
{
public:
    //绘制转动黑圈
    CoverOutItem()
    {
        gradient = QRadialGradient(r, r, r, r, r);
        gradient.setColorAt(0 ,QColor(230, 230, 230, 0));
        gradient.setColorAt(0.595 ,QColor(230, 230, 230, 0));
        gradient.setColorAt(0.6 ,QColor(15, 15, 15));
        gradient.setColorAt(0.645 ,QColor(35, 35, 35));
        gradient.setColorAt(0.65 ,QColor(55, 55, 55));
        gradient.setColorAt(0.936 ,QColor(40, 40, 40));
        gradient.setColorAt(0.94 ,QColor(0, 0, 0));
        gradient.setColorAt(0.964 ,QColor(0, 0, 0));
        gradient.setColorAt(0.97 ,QColor(30, 30, 30, 10));

        brush = QBrush(gradient);
    }

    QRectF boundingRect() const
    {
        return QRectF(0, 0, 2 * r, 2 * r);
    }

    virtual void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setBrush(brush);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(QPoint(r, r), r, r);
    }

protected:
    QBrush brush;
    QRadialGradient gradient;
};

class Foreground : public QGraphicsItem
{
 public:
    Foreground()
    {
        gradient = QConicalGradient(r, r, r);
        gradient.setColorAt(0, QColor(0, 0, 0));
        gradient.setColorAt(0.25, QColor(150, 150, 150, 50));
        gradient.setColorAt(0.5, QColor(0, 0, 0));
        gradient.setColorAt(0.75, QColor(150, 150, 150, 50));
        gradient.setColorAt(1, QColor(0, 0, 0));


        brush = QBrush(gradient);
    }

    QRectF boundingRect() const
    {
        return QRectF(0, 0, 2 * r, 2 * r);
    }

    virtual void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setBrush(brush);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(QPoint(r, r), r-6, r-6);
    }

protected:
    QBrush brush;
    QConicalGradient gradient;
};

class CoverInItem : public QGraphicsItem
{
public:
    CoverInItem()
    {
        gradient = QRadialGradient(r, r, r, r, r);
        gradient.setColorAt(0 ,QColor(230, 230, 230, 0));
        gradient.setColorAt(0.595 ,QColor(230, 230, 230, 0));
        gradient.setColorAt(0.6 ,QColor(15, 15, 15));
        gradient.setColorAt(0.645 ,QColor(35, 35, 35));
        gradient.setColorAt(0.65 ,QColor(55, 55, 55, 0));
        gradient.setColorAt(1 ,QColor(230, 230, 230, 0));

        brush = QBrush(gradient);
    }

    QRectF boundingRect() const
    {
        return QRectF(0, 0, 2 * r, 2 * r);
    }

    virtual void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        painter->setRenderHint(QPainter::Antialiasing);
        painter->setBrush(brush);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(QPoint(r, r), r, r);
    }

protected:
    QBrush brush;
    QRadialGradient gradient;
};

class MusicCover : public QWidget
{
    Q_OBJECT

public:
    MusicCover(QWidget *parent = 0);

    ~MusicCover();

public slots:
    void rotate();//转动
    void setRotate(QMediaPlayer::State state);//根据当前媒体状态，设置是否转动
    void setLrcWidget(QList <int> time_list, QStringList lrc_list);
    void setCover(QPixmap cover);//设置封面
    void scrollLrc(qint64 time);
    void setLabel(QString song, QString album, QString singer);
    void setData(MusicData data);
    void setBackground(QImage &image);
    void blur(QPixmap cover);

signals:
    void thisVisible(bool visible);

private:
    QGraphicsScene* coverScene;

    QQMusicAPI* api;

    QGraphicsPixmapItem* pixmapitem;
    CoverOutItem* coverOutItem;
    CoverInItem* coverInItem;
    Foreground* foreground;

    QList <int> timeList;

    QTimer* timer;
    qreal angle;
    MusicData coverData;
    Ui::MusicCover* ui;
};

#endif // COVERITEM_H
