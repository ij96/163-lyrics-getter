#ifndef SONG_H
#define SONG_H

#include <QApplication>
#include <QWidget>
#include <QtNetwork>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonParseError>
#include <QDesktopServices>

//status code
#define SONG_STATUS_HAS_LRC_TRANSLRC    0
#define SONG_STATUS_NOT_EXIST           1
#define SONG_STATUS_NO_LRC              2
#define SONG_STATUS_NO_TRANSLRC         3
#define SONG_STATUS_INSTRUMENTAL        4

class Song{
public:
    Song();
    ~Song();

    QString title;
    QString album;
    QString artist;
    QString lrc;                // lyrics
    QString translrc;           // translated lyrics
    QString lrc_ordered;

    QImage cover;

    QJsonObject song_info_json_obj;
    QJsonObject song_lyrics_json_obj;

    qint8 status_code = SONG_STATUS_NOT_EXIST;

    bool get_info_json();
    bool get_lyrics_json();

    QImage get_cover(const QString &url);
    void get_info();
    void get_lyrics();
    void get_info_lyrics();

    void check_status();
    bool submit_lrc();
    bool submit_translrc();

    void order_lrc();

    void set_id(QString buf);
    int id();

private:
    qint32 _id = 0;
};

#endif // SONG_H
