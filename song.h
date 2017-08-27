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

class Song{
public:
    Song();
    ~Song();

    qint32 id;
    QString title;
    QString album;
    QString artist;
    QString lrc;
    QString translrc;
    QString lrc_file_name;
    QString translrc_file_name;

    void generate_file_names();
    void get_info();
    void get_lyrics();
};

#endif // SONG_H
