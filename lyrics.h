#ifndef LYRICS_H
#define LYRICS_H

#include <QtWidgets>

class Lyrics : public QObject {
    Q_OBJECT

public:
    Lyrics();

    void set(QString qstr);
    QString show(bool show_tags = true, bool ordered = false);

private:
    QString lrc = "";

    QString remove_tags(QString lrc_raw);
    QString order_tags(QString lrc_raw);
};

#endif // LYRICS_H
