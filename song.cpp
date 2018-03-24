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
            info_json_obj = QJsonDocument::fromJson(response_data.toUtf8()).object();
        } else if (type == "lyrics") {
            QString response_data = reply->readAll();
            lyrics_json_obj = QJsonDocument::fromJson(response_data.toUtf8()).object();
        } else if (type == "cover") {
            QByteArray response_data = reply->readAll();
            if (cover_url.isEmpty()) {
                cover = QImage();
            } else {
                cover = QImage::fromData(response_data);
            }
        } else if (type == "lrc_upload") {
            QString response_data = reply->readAll();
            song_lrc_upload_html = response_data;
            //qDebug() << song_lrc_upload_html;
            //qDebug() << "response length:" << song_lrc_upload_html.length();
        } else if (type == "translrc_upload") {
            QString response_data = reply->readAll();
            song_translrc_upload_html = response_data;
        }
        requests.removeOne(reply->property("type").toString());
        if (requests.isEmpty()) {
            emit done();
        }
    } else {
        qDebug() << "Failure to download" << reply->property("type");
        qDebug() << "Request error:" << reply->errorString();
    }
    delete reply;
}

void Song::get(const QString &type, const QUrl &url) {
    QNetworkReply *reply = nam.get(QNetworkRequest(url));
    reply->setProperty("type", type);
    requests.append(type);
}

void Song::parse_info() {
    QJsonObject json_songs_0_obj = info_json_obj.value("songs").toArray()[0].toObject();

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
    QJsonValue lrc_json = lyrics_json_obj.value("lrc").toObject()
                                              .value("lyric");
    QJsonValue translrc_json = lyrics_json_obj.value("tlyric").toObject()
                                                   .value("lyric");
    QJsonValue lrc_user_json = lyrics_json_obj.value("lyricUser").toObject()
                                                   .value("nickname");
    QJsonValue translrc_user_json = lyrics_json_obj.value("transUser").toObject()
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
    _status = SONG_STATUS_DEFAULT; // clear
    // check if song exists, by counting items in "songs" array: if 0, then song does not exist
    if (info_json_obj.value("songs").toArray().size() != 0) {
        _status |= SONG_STATUS_EXIST;
        if (lyrics_json_obj.value("nolyric").toBool() == true) {
            _status |= SONG_STATUS_INSTRUMENTAL;
        }
        if (lyrics_json_obj.value("lrc").toObject().value("lyric").toString() != "") {
            _status |= SONG_STATUS_LRC;
        }
        if (lyrics_json_obj.value("tlyric").toObject().value("lyric").toString() != "") {
            _status |= SONG_STATUS_TRANSLRC;
        }
        if (song_lrc_upload_html.indexOf("n-lytips") != -1) {
            _status |= SONG_STATUS_LRC_UPLOADED;
        }
        if (song_translrc_upload_html.indexOf("n-lytips") != -1) {
            _status |= SONG_STATUS_TRANSLRC_UPLOADED;
        }
    }
}

void Song::get_all() {
    if (!id_changed) return; // skip all if ID has not changed

    QEventLoop event_loop;
    QObject::connect(this, SIGNAL(done()), &event_loop, SLOT(quit()));
    get("info", QUrl(QString("http://music.163.com/api/song/detail?ids=[%1]")
                     .arg(QString::number(_id))));
    get("lyrics", QUrl(QString("http://music.163.com/api/song/lyric?lv=-1&tv=-1&id=%1")
                       .arg(QString::number(_id))));

    event_loop.exec(); // wait for current requests to finish

    check_status();
    if (!(_status & SONG_STATUS_EXIST)) {
        clear();
    } else {
        parse_info();
        parse_lyrics();
        get("cover", cover_url);
        get("lrc_upload", QUrl(QString("http://music.163.com/lyric/up?id=%1")
                               .arg(QString::number(_id))));
        get("translrc_upload", QUrl(QString("http://music.163.com/lyric/translrc?id=%1")
                                    .arg(QString::number(_id))));
        event_loop.exec(); // wait for current requests to finish
        check_status();
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
        id_changed = false;
    } else {
        id_changed = true;
    }
}

qint32 Song::id() const {
    return _id;
}

qint8 Song::status() const {
    return _status;
}

void Song::translrc_insert_blanks() {
    foreach (const QTime &time, lrc.time_map().keys()) {
        if (!translrc.time_map().contains(time)) {
            translrc.time_map_insert(time, "");
        }
    }
}
