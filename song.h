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

    qint32 id;

    QString title;
    QString album;
    QString artist;
    QString lrc;                // lyrics
    QString translrc;           // translated lyrics

    QJsonObject song_info_json_obj;
    QJsonObject song_lyrics_json_obj;

    qint8 status_code = 0;

    bool get_info_json();
    bool get_lyrics_json();
    void get_info();
    void get_lyrics();
    void get_info_lyrics();
    void check_status();
};

#endif // SONG_H
