#include "jsonviewer.h"

JsonViewer::JsonViewer(QWidget *parent) : QWidget(parent) {
    window = new QLabel;
    window_layout = new QGridLayout;
    info_json_text = new QTextEdit;
    lyrics_json_text = new QTextEdit;

    info_json_hl = new JsonHighlighter(info_json_text->document());
    lyrics_json_hl = new JsonHighlighter(lyrics_json_text->document());

    info_json_text->setFontFamily("Courier");
    lyrics_json_text->setFontFamily("Courier");

    window_layout->addWidget(info_json_text,   0, 0);
    window_layout->addWidget(lyrics_json_text, 0, 1);

    window->setWindowModality(Qt::NonModal);
    window->setWindowFlags(Qt::Window);
    window->setLayout(window_layout);
    window->resize(800, 640);
}

void JsonViewer::update(QJsonObject info, QJsonObject lyrics) {
    QJsonDocument doc;
    QString formatted_json_string;

    doc.setObject(info);
    formatted_json_string = doc.toJson(QJsonDocument::Indented);
    info_json_text->setText(formatted_json_string);

    doc.setObject(lyrics);
    formatted_json_string = doc.toJson(QJsonDocument::Indented);
    lyrics_json_text->setText(formatted_json_string);
}

void JsonViewer::show() const {
    window->show();
}

void JsonViewer::set_window_title(QString title) {
    window->setWindowTitle(title);
}
