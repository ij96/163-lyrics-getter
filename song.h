#ifndef SONG_H
#define SONG_H

#include <QtWidgets>
#include <QtNetwork>
#include "lyrics.h"

//status code
#define SONG_STATUS_HAS_LRC_TRANSLRC    0
#define SONG_STATUS_NOT_EXIST           1
#define SONG_STATUS_NO_LRC              2
#define SONG_STATUS_NO_TRANSLRC         3
#define SONG_STATUS_INSTRUMENTAL        4
//#define SONG_STATUS_EXIST         0x01
//#define SONG_STATUS_LRC           0x02
//#define SONG_STATUS_TRANSLRC      0x04
//#define SONG_STATUS_INSTRUMENTAL  0x08

class Song : public QObject {
    Q_OBJECT

public:
    explicit Song(QObject *parent = 0);
    ~Song();

    QString title;
    QString album;
    QString artist;
    Lyrics lrc;                 // lyrics
    Lyrics translrc;            // translated lyrics
    QString lrc_uploader;       // lyrics uploader name
    QString translrc_uploader;  // translated lyrics uploader name
    QUrl cover_url;
    QImage cover;

    QJsonObject song_info_json_obj;
    QJsonObject song_lyrics_json_obj;

    qint8 status_code = SONG_STATUS_NOT_EXIST;

    void get_all();

    bool submit_lrc();
    bool submit_translrc();

    void set_id(QString buf);
    int id();

signals:
    void done();

private slots:
    void finished(QNetworkReply *reply);

private:
    qint32 _id = -1;
    qint32 _id_prev = -1;
    bool _id_changed = false;

    QNetworkAccessManager nam;
    QList<QString> requests; // running requests

    void get(const QString &type, const QUrl &url);

    void download_info_json(const QString &response_data);
    void download_lyrics_json(const QString &response_data);
    void download_cover(const QByteArray &response_data);

    void parse_info();
    void parse_lyrics();

    void translrc_insert_blanks();

    void check_status();

    void clear();
};

#endif // SONG_H
