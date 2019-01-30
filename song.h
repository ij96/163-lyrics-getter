#ifndef SONG_H
#define SONG_H

#include <QtWidgets>
#include <QtNetwork>
#include "lyrics.h"

//status code
#define SONG_STATUS_DEFAULT             0x00 // default - song does not exist
#define SONG_STATUS_EXIST               0x01 // song exists
#define SONG_STATUS_LRC                 0x02 // has lyrics
#define SONG_STATUS_TRANSLRC            0x04 // has translation
#define SONG_STATUS_INSTRUMENTAL        0x08 // instrumental - no lyrics should exist
#define SONG_STATUS_LRC_UPLOADED        0x10 // lyrics already uploaded by someone
#define SONG_STATUS_TRANSLRC_UPLOADED   0x20 // translation already uploaded by someone
#define SONG_STATUS_CAN_UPLOAD_LRC      0x40 // can upload lyrics
#define SONG_STATUS_CAN_UPLOAD_TRANSLRC 0x80 // can upload translation

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
    QDateTime lrc_uptime;       // lyrics upload time
    QDateTime translrc_uptime;  // translated lyrics upload time
    QUrl cover_url;
    QImage cover;
    
    QJsonObject info_json_obj;
    QJsonObject lyrics_json_obj;

    void get_all();

    bool submit_lrc();
    bool submit_translrc();

    void set_id(QString buf);
    qint32 id() const;

    qint8 status() const;

    void clear();

signals:
    void done();

private slots:
    void finished(QNetworkReply *reply);

private:
    qint32 _id = -1;
    qint32 _id_prev = -1;
    bool id_changed = false;

    quint8 _status = SONG_STATUS_DEFAULT;

    QNetworkAccessManager nam;
    QList<QString> requests; // list of running requests

    QString song_lrc_upload_html;
    QString song_translrc_upload_html;

    void get(const QString &type, const QUrl &url);

    void download_info_json(const QString &response_data);
    void download_lyrics_json(const QString &response_data);
    void download_cover(const QByteArray &response_data);

    void parse_info();
    void parse_lyrics();

    void translrc_insert_blanks();

    void check_status();
};

#endif // SONG_H
