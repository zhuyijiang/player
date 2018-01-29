#ifndef DIALOG_H
#define DIALOG_H

//搜索框

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class QQMusicAPI;
class QVBoxLayout;
class QListWidget;
class QPushButton;
class QLineEdit;
class QLabel;

class QTableWidget;

namespace Ui {
class SearchWidget;
}

class SearchWidget : public QWidget
{
    Q_OBJECT

public:
    SearchWidget(QWidget *parent = 0);
    ~SearchWidget();

public slots:
    void signalItme(int,int);
    void slotData(QStringList, QStringList, QStringList, QStringList, QStringList, QStringList, QList <int>);
    void search();
    void setHotLabel(QStringList wordList);

    void setLineEdit();
    void historySearch(int row, int column);
    void clearHistory();
    void deleteWidget(QString text);

signals:
    void currentItem(QString songName,
                     QString singerName,
                     QString albumName,
                     QString songUrl,
                     QString coverUrl,
                     QString lrcUrl,
                     int songTime);//发生当前选择歌曲的的url

private:
    QStringList names;
    QStringList singer;
    QStringList album;
    QStringList urls;
    QStringList imageUrls;
    QStringList lrcUrls;
    QList <int> timeList;

    QTableWidget* widget;

    QQMusicAPI *api;

    Ui::SearchWidget* ui;
    QStringList historyKey;
};


#endif // DIALOG_H
