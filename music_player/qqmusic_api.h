#ifndef QQMUSIC_API_H
#define QQMUSIC_API_H

//使用qq音乐url API获取网络资源

#include <QObject>
#include <QJsonObject>
#include <QMap>
#include <QPixmap>

class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;
class QUrl;
class QPixmap;

class QQMusicAPI : public QObject
{
    Q_OBJECT

public:
    QQMusicAPI(QObject *parent = 0);

    void getKey();
    void getHotWord();
    QString getSongUrl(const QString &song_mid);
    void getCover(const QString &coverUrl);
    void getLrc(const QString &lrcUrl);
    void search(const QString &keyword, const int &page);

    //static QByteArray getResources(QString url);

private slots:
    void searchFinished();
    void getKeyFinished();
    void getHotWordFinished();

private:
    QNetworkAccessManager *http;
    QString key;

    //QMap <int, QString> lrcMap;


signals:
    void searchList(QStringList, QStringList, QStringList, QStringList, QStringList, QStringList, QList <int>);
    void hotWordList(QStringList);
    void signalCover(QPixmap cover);
    void signalLrc(QList <int> timeList, QStringList lrcList);
};

#endif // QQMUSIC_API_H
