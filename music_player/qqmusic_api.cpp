#include "qqmusic_api.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>
#include <QJsonParseError>
#include <QList>
#include <QTextCodec>
#include <QPixmap>
#include <QByteArray>

QQMusicAPI::QQMusicAPI(QObject *parent)
        : QObject(parent)
{
    http = new QNetworkAccessManager(this);

    getKey();
}

void QQMusicAPI::search(const QString &keyword, const int &page)
{
    QUrl url = QString("http://c.y.qq.com/soso/fcgi-bin/client_search_cp");
    QUrlQuery query;
    query.addQueryItem("t", "0");
    query.addQueryItem("aggr", "1");
    query.addQueryItem("lossless", "1");
    query.addQueryItem("cr", "1");
    query.addQueryItem("catZhida", "1");
    query.addQueryItem("format", "json");
    query.addQueryItem("p", QString::number(page));
    query.addQueryItem("n", "20");
    query.addQueryItem("w", keyword);
    url.setQuery(query.toString(QUrl::FullyEncoded));
    qDebug() << url;


    QNetworkRequest request(url);
    QNetworkReply *reply = http->get(request);

    connect(reply, &QNetworkReply::finished, this, &QQMusicAPI::searchFinished);
}

void QQMusicAPI::getKey()
{
    QUrl url = QString("https://c.y.qq.com/base/fcgi-bin/fcg_musicexpress.fcg");
    QUrlQuery query;
    query.addQueryItem("json", "3");
    query.addQueryItem("format", "json");
    query.addQueryItem("guid", "85880580");
    url.setQuery(query.toString(QUrl::FullyEncoded));

    QNetworkRequest request(url);
    QNetworkReply *reply = http->get(request);

    connect(reply, &QNetworkReply::finished, this, &QQMusicAPI::getKeyFinished);
}

QString QQMusicAPI::getSongUrl(const QString &song_mid)
{
    return "http://dl.stream.qqmusic.qq.com/M500" + song_mid + ".mp3?vkey=" + key + "&guid=85880580&fromtag=30";
}

void QQMusicAPI::searchFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (!reply->error()) {
        QByteArray array = reply->readAll();
        QJsonParseError error;
        QJsonDocument document = QJsonDocument::fromJson(array, &error);
        QJsonObject object = document.object();
        QJsonObject song = object.value("data").toObject().value("song").toObject();
        QJsonArray list = song["list"].toArray();

        QStringList songList;
        QStringList singList;
        QStringList ablumList;
        QStringList songUrlList;
        QStringList coverList;
        QStringList lrcUrlList;
        QList <int> timeList;

        for (int i=0; i<list.size(); ++i) {
            QString song_name = list.at(i).toObject().value("songname").toString();
            QString singer_name = list.at(i).toObject().value("singer").toArray().at(0).toObject().value("name").toString();
            QString song_mid = list.at(i).toObject().value("songmid").toString();
            qint64 lrc_id = list.at(i).toObject().value("songid").toInt();
            QString lrc = QString::number(lrc_id);
            QString lr = QString::number(lrc_id % 100);
            int time = list.at(i).toObject().value("interval").toInt();
            QString lrc_url =  QString("http://music.qq.com/miniportal/static/lyric/%1/%2.xml").arg(lr).arg(lrc);
            QString ablum_name = list.at(i).toObject().value("albumname").toString();
            QString album_mid = list.at(i).toObject().value("albummid").toString();
            QString album_cover = QString("https://y.gtimg.cn/music/photo_new/T002R300x300M000%1.jpg").arg(album_mid);
            QString album_cover_big = QString("https://y.gtimg.cn/music/photo_new/T002R500x500M000%1.jpg").arg(album_mid);
            QString album_cover_small = QString("https://y.gtimg.cn/music/photo_new/T002R150x150M000%1.jpg").arg(album_mid);

            songList << song_name;
            singList << singer_name;
            ablumList << ablum_name;
            songUrlList << "http://dl.stream.qqmusic.qq.com/M500" + song_mid + ".mp3?vkey=" + key +"&guid=85880580&fromtag=30";
            coverList << album_cover_big;
            lrcUrlList << lrc_url;
            timeList << time;

        }
        emit searchList(songList, singList, ablumList, songUrlList, coverList, lrcUrlList, timeList);
    }
    reply->deleteLater();
}

void QQMusicAPI::getKeyFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());

    if (!reply->error()) {
        QByteArray array = reply->readAll();
        QJsonDocument document = QJsonDocument::fromJson(array);
        QJsonObject object = document.object();

        key = object.value("key").toString();
    }
    reply->deleteLater();
}
void QQMusicAPI::getHotWord()
{
    QUrl url = QString("https://c.y.qq.com/splcloud/fcgi-bin/gethotkey.fcg");

    QUrlQuery query;
    query.addQueryItem("g_tk", "5381");
    query.addQueryItem("jsonpCallback", "hotSearchKeysmod_top_search");
    query.addQueryItem("loginUin", "0");
    query.addQueryItem("hostUin", "0");
    query.addQueryItem("format", "json");
    query.addQueryItem("inCharset", "UTF-8");
    query.addQueryItem("notice", "0");
    query.addQueryItem("platform", "yqq");
    query.addQueryItem("needNewCode", "0");
    url.setQuery(query.toString(QUrl::FullyEncoded));

    QNetworkRequest request(url);
    QNetworkReply* reply = http->get(request);
    connect(reply, &QNetworkReply::finished, this, &QQMusicAPI::getHotWordFinished);
}

void QQMusicAPI::getHotWordFinished()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

    if(!reply->error()) {
        QByteArray array = reply->readAll();
        //这里刚开始有个bug，当转化为QJsonDocument的时候为空
        //原因是获取的网络资源里，开头有几个不相关的字符
        //解决办法，将多余的字符删除后回复正常
        array.remove(0, 28);
        array.remove(array.size() - 1, 1);
        QJsonParseError error;
        QJsonDocument document = QJsonDocument::fromJson(array, &error);

        QJsonObject object = document.object();
        QJsonObject data = object.value("data").toObject();
        QJsonArray hotArray = data["hotkey"].toArray();


        QStringList hotList;
        for(int i = 0; i < 10; i++) {
            QString hotWord = hotArray.at(i).toObject().value("k").toString();
            hotWord.insert(0, " ");
            qDebug() << hotWord;
            hotList << hotWord;
        }

        emit hotWordList(hotList);
    }
    reply->deleteLater();
}

void QQMusicAPI::getLrc(const QString &lrcUrl)
{
    QUrl url = QUrl(lrcUrl);
    QNetworkRequest request(url);
    QNetworkReply* reply = http->get(request);

    connect(reply, &QNetworkReply::finished, this, [=](){
        QList <int> timeList;
        QStringList lrcList;
        if(reply->error() == QNetworkReply::NoError) {
            QTextCodec *tc=QTextCodec::codecForName("gbk");
            QByteArray ba=reply->readAll();
            QString lrcString = tc->toUnicode(ba);
            int size = lrcString.size();
            lrcString.insert(size - 11, '\n');

            int j = 0;
            while ((j = lrcString.indexOf(QRegExp("\\[\\d{2}:\\d{2}\\.\\d{2}\\]"), j)) != -1) {
                if(lrcString.at(j + 10) == '\n'){
                    lrcString.remove(j, 11);
                }
                int k = lrcString.indexOf("\n", j);
                if (k != -1 ){
                   QString timeString = lrcString.mid(j, k);
                   const int time = timeString.mid(1, 2).toInt() * 60 + timeString.mid(4, 2).toInt();
                   const QString lrc = lrcString.mid(j +10, k - j - 10);
                   timeList << time;
                   lrcList << lrc;
                }
                ++j;
            }
            emit signalLrc(timeList, lrcList);
            reply->deleteLater();
            qDebug() << "lrc_ture";
        } else {
            emit signalLrc(timeList, lrcList);
            qDebug() << "lrc_error";
            reply->deleteLater();
        }
    });
}

void QQMusicAPI::getCover(const QString &coverUrl)
{ 
    QUrl url = QUrl(coverUrl);
    QNetworkRequest request(url);
    QNetworkReply* reply = http->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if(reply->error()==QNetworkReply::NoError) {
            QByteArray bytes = reply->readAll();
            QPixmap pixmap;
            //error: passing 'const QPixmap' as 'this' argument discards qualifiers [-fpermissive]
            //相同解决办法，将pixmap申明到此处即解决
            if(pixmap.loadFromData(bytes)) {
                qDebug() << "load_cover_ture";
                emit signalCover(pixmap);
            } else {
                qDebug() << "load_cover_fales";
                emit signalCover(QPixmap(":/images/cover.jpg"));
            }
            reply->deleteLater();
        } else {
            qDebug() << "cover_error";
            emit signalCover(QPixmap(":/images/cover.jpg"));
            reply->deleteLater();
        }
    });
}
