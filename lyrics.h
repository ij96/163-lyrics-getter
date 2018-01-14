#ifndef LYRICS_H
#define LYRICS_H

#include <QtWidgets>

class Lyrics : public QObject {
    Q_OBJECT

public:
    Lyrics();

    void set(QString qstr);
    QString data();

    QStringList meta_map();

    QMap<QTime,QString> time_map();
    void time_map_insert(QTime key, QString value);

    QString show(bool show_tags = true, bool ordered = false);

private:
    QString _data = "";
    QStringList _meta_list;
    QMap<QTime,QString> _time_map;

    QString remove_tags(QString lrc_raw);
    QString order_tags();
    void update_tag_maps();
};

#endif // LYRICS_H
