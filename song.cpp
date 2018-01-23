#include "song.h"

Song::Song(QObject *parent) : QObject(parent) {
    QObject::connect(&nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
}

Song::~Song() {}

void Song::finished(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QString type = reply->property("type").toString();
        if (type == "info") {
            QString response_data = reply->readAll();
            //download_info_json(response_data);
            QJsonDocument json_response = QJsonDocument::fromJson(response_data.toUtf8());
            song_info_json_obj = json_response.object();
        } else if (type == "lyrics") {
            QString response_data = reply->readAll();
            //download_lyrics_json(response_data);
            QJsonDocument json_response = QJsonDocument::fromJson(response_data.toUtf8());
            song_lyrics_json_obj = json_response.object();
        } else if (type == "cover") {
            QByteArray response_data = reply->readAll();
            //download_cover(response_data);
            if (cover_url.isEmpty()) {
                cover = QImage();
            } else {
                cover = QImage::fromData(response_data);
            }
        } else if (type == "lrc_uploaded") {
            QString response_data = reply->readAll();
            qDebug() << response_data;
            if (response_data.indexOf("n-lytips") != -1) {
                qDebug() << "lyrics uploaded";
            }
        } else if (type == "translrc_uploaded") {
            QString response_data = reply->readAll();
            qDebug() << response_data;
            if (response_data.indexOf("n-lytips") != -1) {
                qDebug() << "translation uploaded";
            }
        }
        requests.removeOne(reply->property("type").toString());
        if (requests.isEmpty()) {
            emit done();
        }
    } else {
        qDebug() << "Failure to download " << reply->property("type");
        qDebug() << "Request error:" << reply->errorString();
    }
    delete reply;
}

void Song::get(const QString &type, const QUrl &url) {
    QNetworkReply *reply = nam.get(QNetworkRequest(url));
    reply->setProperty("type", type);
    requests.append(type);
}

void Song::download_info_json(const QString &response_data) {
    QJsonDocument json_response = QJsonDocument::fromJson(response_data.toUtf8());
    song_info_json_obj = json_response.object();
}

void Song::download_lyrics_json(const QString &response_data) {
    QJsonDocument json_response = QJsonDocument::fromJson(response_data.toUtf8());
    song_lyrics_json_obj = json_response.object();
}

void Song::download_cover(const QByteArray &response_data) {
    if (cover_url.isEmpty()) {
        cover = QImage();
    } else {
        cover = QImage::fromData(response_data);
    }
}

void Song::parse_info() {
    QJsonObject json_songs_0_obj = song_info_json_obj.value("songs").toArray()[0].toObject();

    QJsonValue title_json = json_songs_0_obj.value("name");
    QJsonValue artist_json = json_songs_0_obj.value("artists").toArray()[0].toObject()
                                             .value("name");
    QJsonValue album_json = json_songs_0_obj.value("album").toObject()
                                            .value("name");
    QJsonValue cover_url_json = json_songs_0_obj.value("album").toObject()
                                                .value("picUrl");

    title = title_json.toString();
    artist = artist_json.toString();
    album = album_json.toString();
    cover_url = QUrl(cover_url_json.toString());

    //qDebug() << "Song title: " << title_json;
    //qDebug() << "Artist: " << artist_json;
    //qDebug() << "Album: " << album_json;
    //qDebug() << "Cover URL: " << cover_url_json;
}

void Song::parse_lyrics() {
    QJsonValue lrc_json = song_lyrics_json_obj.value("lrc").toObject()
                                              .value("lyric");
    QJsonValue translrc_json = song_lyrics_json_obj.value("tlyric").toObject()
                                                   .value("lyric");
    QJsonValue lrc_user_json = song_lyrics_json_obj.value("lyricUser").toObject()
                                                   .value("nickname");
    QJsonValue translrc_user_json = song_lyrics_json_obj.value("transUser").toObject()
                                                        .value("nickname");

    lrc.set(lrc_json.toString());
    translrc.set(translrc_json.toString());
    translrc_insert_blanks();
    lrc_uploader = lrc_user_json.toString();
    translrc_uploader = translrc_user_json.toString();

    //qDebug() << "Lyrics: " << lrc_json;
    //qDebug() << "Translated lyrics: " << translrc_json;
}

void Song::check_status() {
    status_code = 0; // clear
    // check if song exists, by counting items in "songs" array: if 0, then song does not exist
    if (song_info_json_obj.value("songs").toArray().size() == 0) {
        status_code = SONG_STATUS_NOT_EXIST;
    } else {
        if (song_lyrics_json_obj.value("nolyric").toBool() == true) {
            status_code = SONG_STATUS_INSTRUMENTAL; // instrumental - no lyrics should exist
        } else if (song_lyrics_json_obj.value("lrc").toObject().value("lyric").toString() == "") {
            status_code = SONG_STATUS_NO_LRC;       // lyrics not found
        } else if (song_lyrics_json_obj.value("tlyric").toObject().value("lyric").toString() == "") {
            status_code = SONG_STATUS_NO_TRANSLRC;  // lyrics found, but translated lyrics not found
        } else {
            status_code = SONG_STATUS_HAS_LRC_TRANSLRC;
        }
    }
}

void Song::get_all() {
    if (!_id_changed) return;
    QEventLoop event_loop;
    QObject::connect(this, SIGNAL(done()), &event_loop, SLOT(quit()));
    get("info", QUrl(QString("http://music.163.com/api/song/detail?ids=[%1]")
                     .arg(QString::number(_id))));
    get("lyrics", QUrl(QString("http://music.163.com/api/song/lyric?lv=-1&tv=-1&id=%1")
                       .arg(QString::number(_id))));

    event_loop.exec(); // wait for current requests to finish

    check_status();
    if (status_code == SONG_STATUS_NOT_EXIST) {
        clear();
    } else {
        parse_info();
        parse_lyrics();
        get("cover", cover_url);
        get("lrc_uploaded", QUrl(QString("http://music.163.com/lyric/up?id=%1")
                                 .arg(QString::number(_id))));
        get("translrc_uploaded", QUrl(QString("http://music.163.com/lyric/translrc?id=%1")
                                      .arg(QString::number(_id))));
        event_loop.exec(); // wait for current requests to finish
    }
}

void Song::clear() { // clear all variables
    title = "";
    artist = "";
    album = "";
    lrc.set("");
    translrc.set("");
    lrc_uploader = "";
    translrc_uploader = "";
    cover = QPixmap(0, 0).toImage();
}

bool Song::submit_lrc() {
    return QDesktopServices::openUrl(QUrl(QString("http://music.163.com/lyric/up?id=%1")
                                          .arg(QString::number(_id))));
}

bool Song::submit_translrc() {
    return QDesktopServices::openUrl(QUrl(QString("http://music.163.com/lyric/translrc?id=%1")
                                          .arg(QString::number(_id))));
}

void Song::set_id(QString buf) {
    _id_prev = _id;
    QRegularExpression re("(?:[&?/]id=|^)([0-9]+)");
    QRegularExpressionMatch match = re.match(buf);
    if (match.hasMatch()) {
        _id = match.captured(1).toInt();
    } else {
        _id = 0;
    }
    if (_id == _id_prev) {
        _id_changed = false;
    } else {
        _id_changed = true;
    }
}

int Song::id() {
    return _id;
}

void Song::translrc_insert_blanks() {
    foreach (const QTime &time, lrc.time_map().keys()) {
        if (!translrc.time_map().contains(time)) {
            translrc.time_map_insert(time, "");
        }
    }
}
