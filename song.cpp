#include "song.h"

Song::Song(){
    id = 0;
    title = "";
    album = "";
    artist = "";
    lrc = "";
    translrc = "";
    lrc_file_name = "";
    translrc_file_name = "";
}

Song::~Song(){}

void Song::get_info(){
    QEventLoop event_loop;
    QNetworkAccessManager qnam;
    QObject::connect(&qnam, SIGNAL(finished(QNetworkReply*)),&event_loop,SLOT(quit()));

    QNetworkRequest req(QUrl(QString("http://music.163.com/api/song/detail?ids=["+QString::number(id)+"]")));
    QNetworkReply *reply = qnam.get(req);

    event_loop.exec(); //blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) {
        //success
        QString response_data = reply->readAll();
        QJsonDocument json_response = QJsonDocument::fromJson(response_data.toUtf8());

        QJsonObject json_obj = json_response.object();

        QJsonObject json_songs_0_obj = json_obj.value("songs").toArray()[0].toObject();

        QJsonValue title_json = json_songs_0_obj.value("name");
        QJsonValue artist_json = json_songs_0_obj.value("artists").toArray()[0].toObject().value("name");
        QJsonValue album_json = json_songs_0_obj.value("album").toObject().value("name");

        qDebug() << "Song title: " << title_json;
        qDebug() << "Artist: " << artist_json;
        qDebug() << "Album: " << album_json;

        title = title_json.toString();
        artist = artist_json.toString();
        album = album_json.toString();

        delete reply;
    }
    else {
        //failure
        qDebug() << "Failure to load song information:" << reply->errorString();
        delete reply;
    }
}

void Song::get_lyrics(){
    QEventLoop event_loop;
    QNetworkAccessManager qnam;
    QObject::connect(&qnam, SIGNAL(finished(QNetworkReply*)),&event_loop,SLOT(quit()));

    QNetworkRequest req(QUrl(QString("http://music.163.com/api/song/lyric?lv=-1&tv=-1&id="+QString::number(id))));
    QNetworkReply *reply = qnam.get(req);

    event_loop.exec(); //blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) {
        //success
        QString response_data = reply->readAll();
        QJsonDocument json_response = QJsonDocument::fromJson(response_data.toUtf8());

        QJsonObject json_obj = json_response.object();

        QJsonValue lrc_json = json_obj.value("lrc").toObject().value("lyric");
        QJsonValue translrc_json = json_obj.value("tlyric").toObject().value("lyric");

        qDebug() << "Lyrics: " << lrc_json;
        qDebug() << "Translated lyrics: " << translrc_json;

        lrc = lrc_json.toString();
        translrc = translrc_json.toString();

        delete reply;
    }
    else {
        //failure
        qDebug() << "Failure to load lyrics:" << reply->errorString();
        delete reply;
    }
}
