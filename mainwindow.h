#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QString>
#include <QShortcut>
#include <QTranslator>
#include "image.h"
#include "lineseparator.h"
#include "song.h"
#include "togglebutton.h"

class MainWindow : public QWidget{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void quit();
    void about();

    void get_info_lyrics();

    bool save_lrc();
    bool save_translrc();

    bool submit_lrc();
    bool submit_translrc();

    bool save_info_cover();

    void display_song_status();

    void set_language(QAction* action);

    void display_lrc_translrc();
    void show_or_hide_tags();

private:
    bool show_tags = true;   // whether the displayed lyrics should have LRC tags

    bool save(bool save_translated);
    void load_settings();
    void save_settings();
    bool setup_settings_file();
    QString remove_tags(QString lrc);
    void retranslate_ui();

    //---widgets---
    // ID input field
    QLabel *input_id_label;
    QLineEdit *input_id_edit;
    QPushButton *input_button;

    // song info
    QLabel *info_title_label;
    QLineEdit *info_title_edit;
    QLabel *info_artist_label;
    QLineEdit *info_artist_edit;
    QLabel *info_album_label;
    QLineEdit *info_album_edit;
    ImageCanvas *info_cover_image;
    QPushButton *info_cover_save_button;

    // lyrics (lrc) & translated lyrics (translrc)
    QLabel *lrc_label;
    QTextEdit *lrc_text;
    QPushButton *lrc_save_button;
    QPushButton *lrc_submit_button;

    QLabel *translrc_label;
    QTextEdit *translrc_text;
    QPushButton *translrc_save_button;
    QPushButton *translrc_submit_button;

    ToggleButton *hide_tags_button;

    // song status (displays if song/lrc/translrc exists)
    QLabel *status_label;
    QLineEdit *status_edit;
    //---END widgets---

    // menu
    QMenu *file_menu;
    QMenu *options_menu;
    QMenu *language_menu;
    QMenu *about_menu;

    QAction *save_lrc_action;
    QAction *save_translrc_action;
    QAction *save_info_cover_action;
    QAction *quit_action;
    QAction *about_action;
    //--END menu---

    // application
    QString app_version;
    QString app_name;

    // multi-language
    QLocale *locale;
    QTranslator *translator;

    // settings
    QString settings_file_path;

    // song object
    Song *song;

};

#endif // MAINWINDOW_H
