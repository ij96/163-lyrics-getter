#ifndef JSONVIEWER_H
#define JSONVIEWER_H

#include <QtWidgets>
#include "jsonhighlighter.h"

class JsonViewer : public QWidget {
    Q_OBJECT

public:
    explicit JsonViewer(QWidget *parent = 0);
    void update(qint32 song_id, QJsonObject info, QJsonObject lyrics);
    void show();

private:
    QLabel *window;
    QGridLayout *window_layout = new QGridLayout;
    QTextEdit *info_json_text;
    QTextEdit *lyrics_json_text;
    JsonHighlighter *info_json_hl;
    JsonHighlighter *lyrics_json_hl;
};

#endif // JSONVIEWER_H
