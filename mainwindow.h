#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QtWidgets>
#include <QString>
#include "song.h"

class MainWindow : public QWidget{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void get_info_lyrics();
    void quit();
    void about();
    bool save_lrc();
    bool save_translrc();

private:
    bool save(bool save_translated);

    //---widgets---
    //ID input field
    QLabel *input_id_label;
    QLineEdit *input_id_edit;
    QPushButton *input_button;

    //song info
    QLabel *info_title_label;
    QLineEdit *info_title_edit;
    QLabel *info_artist_label;
    QLineEdit *info_artist_edit;
    QLabel *info_album_label;
    QLineEdit *info_album_edit;

    //lyrics (lrc) & translated lyrics (translrc)
    QLabel *lrc_label;
    QTextEdit *lrc_text;
    QPushButton *lrc_save_button;
    QLabel *translrc_label;
    QTextEdit *translrc_text;
    QPushButton *translrc_save_button;

    Song *song;
};

#endif // MAINWINDOW_H
