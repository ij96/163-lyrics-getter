#include "song.h"

Song::Song() {}

Song::~Song() {}

bool Song::get_info_json() {
    QEventLoop event_loop;
    QNetworkAccessManager qnam;
    QObject::connect(&qnam, SIGNAL(finished(QNetworkReply*)),&event_loop,SLOT(quit()));
    QNetworkRequest req(QUrl(QString("http://music.163.com/api/song/detail?ids=["+QString::number(id)+"]")));
    QNetworkReply *reply = qnam.get(req);

    event_loop.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) { // success
        QString response_data = reply->readAll();
        QJsonDocument json_response = QJsonDocument::fromJson(response_data.toUtf8());

        song_info_json_obj = json_response.object();

        delete reply;
        return true;
    } else {  // failure
        qDebug() << "Failure to load song information: " << reply->errorString();
        delete reply;
        return false;
    }
}

bool Song::get_lyrics_json() {
    QEventLoop event_loop;
    QNetworkAccessManager qnam;
    QObject::connect(&qnam, SIGNAL(finished(QNetworkReply*)),&event_loop,SLOT(quit()));
    QNetworkRequest req(QUrl(QString("http://music.163.com/api/song/lyric?lv=-1&tv=-1&id="+QString::number(id))));
    QNetworkReply *reply = qnam.get(req);

    event_loop.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) { // success
        QString response_data = reply->readAll();
        QJsonDocument json_response = QJsonDocument::fromJson(response_data.toUtf8());

        song_lyrics_json_obj = json_response.object();

        delete reply;
        return true;
    } else {  // failure
        qDebug() << "Failure to load song lyrics: " << reply->errorString();
        delete reply;
        return false;
    }
}

QImage Song::get_art(const QString &url) {
    if (url.isEmpty())
        return QImage();

    QEventLoop event_loop;
    QNetworkAccessManager qnam;
    QObject::connect(&qnam, SIGNAL(finished(QNetworkReply*)),&event_loop,SLOT(quit()));
    QNetworkRequest req = QNetworkRequest(QUrl(url));
    QNetworkReply *reply = qnam.get(req);

    event_loop.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) { // success
        QImage img(QImage::fromData(reply->readAll()));

        delete reply;
        return img;
    } else {  // failure
        qDebug() << "Failure to download album art: " << reply->errorString();
        delete reply;
        return QImage();
    }
}

void Song::get_info() {
    QJsonObject json_songs_0_obj = song_info_json_obj.value("songs").toArray()[0].toObject();

    QJsonValue title_json = json_songs_0_obj.value("name");
    QJsonValue artist_json = json_songs_0_obj.value("artists").toArray()[0].toObject().value("name");
    QJsonValue album_json = json_songs_0_obj.value("album").toObject().value("name");
    QJsonValue art_json = json_songs_0_obj.value("album").toObject().value("picUrl");

    title = title_json.toString();
    artist = artist_json.toString();
    album = album_json.toString();
    art = get_art(art_json.toString());
    
    qDebug() << "Song title: " << title_json;
    qDebug() << "Artist: " << artist_json;
    qDebug() << "Album: " << album_json;
    qDebug() << "Art: " << art_json;
}

void Song::get_lyrics() {
    QJsonValue lrc_json = song_lyrics_json_obj.value("lrc").toObject().value("lyric");
    QJsonValue translrc_json = song_lyrics_json_obj.value("tlyric").toObject().value("lyric");

    lrc = lrc_json.toString();
    translrc = translrc_json.toString();

    qDebug() << "Lyrics: " << lrc_json;
    qDebug() << "Translated lyrics: " << translrc_json;
}

void Song::check_status() {
    // check if song exists, by counting items in "songs" array: if 0, then song does not exist
    if(song_info_json_obj.value("songs").toArray().size() == 0) {
        status_code = SONG_STATUS_NOT_EXIST;
    } else {
        if(song_lyrics_json_obj.value("nolyric").toBool() == true) {
            status_code = SONG_STATUS_INSTRUMENTAL;    // instrumental - no lyrics should exist
        } else if(song_lyrics_json_obj.value("lrc").toObject().value("lyric").toString() == "") {
            status_code = SONG_STATUS_NO_LRC;    // lyrics not found
        } else if(song_lyrics_json_obj.value("tlyric").toObject().value("lyric").toString() == "") {
            status_code = SONG_STATUS_NO_TRANSLRC;    // lyrics found, but translated lyrics not found
        } else {
            status_code = SONG_STATUS_HAS_LRC_TRANSLRC;
        }
    }
}

void Song::get_info_lyrics() {
    get_info_json();
    get_lyrics_json();
    check_status();
    if(status_code == SONG_STATUS_NOT_EXIST) { // clear all fields
        title = "";
        artist = "";
        album = "";
        lrc = "";
        translrc = "";
    } else {
        get_info();
        get_lyrics();
    }
}


bool Song::submit_lrc() {
    return QDesktopServices::openUrl(QUrl("http://music.163.com/#/lyric/up?id=" + QString::number(id)));
}

bool Song::submit_translrc() {
    return QDesktopServices::openUrl(QUrl("http://music.163.com/#/lyric/translrc?id=" + QString::number(id)));
}
