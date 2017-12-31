#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    //---settings file---
    settings_file_path = QApplication::applicationDirPath() + "/config.ini";
    setup_settings_file();  // create settings file, if it does not exist
    load_settings();        // load from settings

    //---translator---
    if(translator.load(QString(":/language/%1.qm").arg(locale.name())))
        qApp->installTranslator(&translator);
    //---END translator---

    //---version---
    app_version = "v1.0.2 (beta)";
    app_name = tr("163 Lyrics Getter %1").arg(app_version);
    //---END version---

    //------initialisation------
    song = new Song();
    // widgets
    input_id_label = new QLabel();
    input_id_edit = new QLineEdit();
    input_button = new QPushButton();

    info_title_label = new QLabel();
    info_title_edit = new QLineEdit();
    info_artist_label = new QLabel();
    info_artist_edit = new QLineEdit();
    info_album_label = new QLabel();
    info_album_edit = new QLineEdit();
    info_cover_image = new ImageCanvas();
    info_cover_save_button = new QPushButton();

    lrc_label = new QLabel();
    lrc_text = new QTextEdit();
    lrc_save_button = new QPushButton();
    lrc_submit_button = new QPushButton();

    translrc_label = new QLabel();
    translrc_text = new QTextEdit();
    translrc_save_button = new QPushButton();
    translrc_submit_button = new QPushButton();

    hide_tags_button = new ToggleButton();

    status_label = new QLabel();
    status_edit = new QLineEdit();
    status_edit->setReadOnly(true);

    // menu bar
    file_menu = new QMenu();
    options_menu = new QMenu();
    language_menu = new QMenu();
    about_menu = new QMenu();

    save_lrc_action = new QAction();
    save_translrc_action = new QAction();
    save_info_cover_action = new QAction();
    quit_action = new QAction();
    about_action = new QAction();
    //------END initialisation------

    //------UI------
    //---menu bar---
    // file menu
    file_menu->addAction(save_lrc_action);
    file_menu->addAction(save_translrc_action);
    file_menu->addSeparator();
    file_menu->addAction(save_info_cover_action);
    file_menu->addSeparator();
    file_menu->addAction(quit_action);

    // options menu
    QAction *language_en_GB_action = new QAction("English (UK)"); // do not translate
    language_en_GB_action->setData("en_GB");
    QAction *language_zh_CN_action = new QAction("\u4E2D\u6587"); // do not translate
    language_zh_CN_action->setData("zh_CN");

    language_menu->addAction(language_en_GB_action);
    language_menu->addAction(language_zh_CN_action);

    options_menu->addMenu(language_menu);

    // about menu
    about_menu->addAction(about_action);

    // full menu bar
    QMenuBar *menu_bar = new QMenuBar();
    menu_bar->addMenu(file_menu);
    menu_bar->addMenu(options_menu);
    menu_bar->addMenu(about_menu);
    //---END menu bar---

    //---layouts---
    // 1st
    QGridLayout *toolbar_layout = new QGridLayout();
    toolbar_layout->addWidget(input_id_label,           0,0);       // input
    toolbar_layout->addWidget(input_id_edit,            0,1);
    toolbar_layout->addWidget(input_button,             0,2);

    toolbar_layout->addWidget(status_label,             1,0);       // status
    toolbar_layout->addWidget(status_edit,              1,1,1,2);

    toolbar_layout->addWidget(hide_tags_button,         2,0,1,3);   // hide LRC tags

    toolbar_layout->addWidget(info_title_label,         3,0);       // title
    toolbar_layout->addWidget(info_title_edit,          3,1,1,2);

    toolbar_layout->addWidget(info_artist_label,        4,0);       // artist
    toolbar_layout->addWidget(info_artist_edit,         4,1,1,2);

    toolbar_layout->addWidget(info_album_label,         5,0);       // album
    toolbar_layout->addWidget(info_album_edit,          5,1,1,2);

    toolbar_layout->addWidget(info_cover_image,         6,0,1,3);   // cover

    toolbar_layout->addWidget(info_cover_save_button,   7,0,1,3);

    QVBoxLayout *lrc_layout = new QVBoxLayout();
    lrc_layout->addWidget(lrc_label);
    lrc_layout->addWidget(lrc_text);
    lrc_layout->addWidget(lrc_save_button);
    lrc_layout->addWidget(lrc_submit_button);

    QVBoxLayout *translrc_layout = new QVBoxLayout();
    translrc_layout->addWidget(translrc_label);
    translrc_layout->addWidget(translrc_text);
    translrc_layout->addWidget(translrc_save_button);
    translrc_layout->addWidget(translrc_submit_button);

    // 0th
    QGridLayout *main_layout = new QGridLayout(this);
    main_layout->addLayout(toolbar_layout, 0,0);
    main_layout->addLayout(lrc_layout,     0,1);
    main_layout->addLayout(translrc_layout,0,2);

    main_layout->setColumnStretch(0,3);
    main_layout->setColumnStretch(1,4);
    main_layout->setColumnStretch(2,4);

    main_layout->setMenuBar(menu_bar);

    setLayout(main_layout);
    resize(840,600);
    setWindowTitle(app_name);
    //---END layouts---
    //------END UI------

    //---connect---
    connect(input_id_edit,&QLineEdit::returnPressed,input_button,&QPushButton::click);

    connect(input_button,SIGNAL(clicked()),this,SLOT(get_info_lyrics()));
    connect(lrc_save_button,SIGNAL(clicked()),this,SLOT(save_lrc()));
    connect(lrc_submit_button,SIGNAL(clicked()),this,SLOT(submit_lrc()));
    connect(translrc_save_button,SIGNAL(clicked()),this,SLOT(save_translrc()));
    connect(translrc_submit_button,SIGNAL(clicked()),this,SLOT(submit_translrc()));
    connect(info_cover_save_button,SIGNAL(clicked()),this,SLOT(save_info_cover()));
    connect(hide_tags_button,SIGNAL(toggled()),this,SLOT(show_or_hide_tags()));

    connect(save_lrc_action,SIGNAL(triggered()),this,SLOT(save_lrc()));
    connect(save_translrc_action,SIGNAL(triggered()),this,SLOT(save_translrc()));
    connect(save_info_cover_action,SIGNAL(triggered()),this,SLOT(save_info_cover()));
    connect(quit_action,SIGNAL(triggered()),this,SLOT(quit()));
    connect(about_action,SIGNAL(triggered()),this,SLOT(about()));

    connect(language_menu,SIGNAL(triggered(QAction*)),this,SLOT(set_language(QAction*)));
    //---END connect---

    retranslate_ui();
}

MainWindow::~MainWindow() {}

void MainWindow::get_info_lyrics() {
    status_edit->setText(tr("Getting song information and lyrics..."));

    song->set_id(input_id_edit->text());
    song->get_info_lyrics();

    input_id_edit->setText(QString::number(song->id()));
    info_title_edit->setText(song->title);
    info_artist_edit->setText(song->artist);
    info_album_edit->setText(song->album);
    info_cover_image->setPixmap(QPixmap::fromImage(song->cover));
    info_cover_image->image()->window()->setWindowTitle(tr("Album cover art: %1").arg(song->album));

    display_lrc_translrc();

    display_song_status();
}

void MainWindow::quit() {
    qApp->quit();
}

bool MainWindow::save(bool save_translated) {
    bool no_tags = hide_tags_button->isChecked();
    QString default_file_name = "*";
    if(song->status_code != SONG_STATUS_NOT_EXIST)
        default_file_name = QString("%1 - %2%3.%4").arg(song->title,song->artist,
                                                        save_translated?tr(" (translated)"):"",
                                                        no_tags?"txt":"lrc");
    QString file_name;
    QString filter_txt = tr("Text file (*.txt)");
    QString filter_lrc = tr("LRC lyrics file (*.lrc)");
    QString filters = QString("%1;;%2").arg(filter_txt,filter_lrc);
    file_name = QFileDialog::getSaveFileName(this,
                                             tr("Save As"),
                                             default_file_name,
                                             filters,no_tags?&filter_txt:&filter_lrc);

    if(file_name.isEmpty())
        return false;

    QFile file(file_name);
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
            tr("Cannot write file %1:\n%2.").arg(QDir::toNativeSeparators(file_name),
                                                 file.errorString()));
        return false;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    if(save_translated)
        out << translrc_text->toPlainText();
    else
        out << lrc_text->toPlainText();
    out.setGenerateByteOrderMark(true);
    out.flush();

    file.close();
    return true;
}

bool MainWindow::save_lrc() {
    return save(false);
}

bool MainWindow::save_translrc() {
    return save(true);
}

bool MainWindow::submit_lrc() {
    return song->submit_lrc();
}

bool MainWindow::submit_translrc() {
    return song->submit_translrc();
}

bool MainWindow::save_info_cover() {
    QString default_file_name = "*";
    if(song->status_code != SONG_STATUS_NOT_EXIST) {
        default_file_name = song->album + " - cover";
    }
    QString file_name = QFileDialog::getSaveFileName(this,tr("Save As"),default_file_name,tr("Images (*.jpg *.png)"));

    if(file_name.isEmpty())
        return false;

    if(song->cover.save(file_name)) {
        qDebug() << "Successfully saved album cover to " << file_name;
        return true;
    } else {
        qDebug() << "Album cover not saved";
        return false;
    }
}

void MainWindow::about() {
    QMessageBox::about(this, tr("About") + app_name,
        tr("Get lyrics from NetEase Cloud Music (<a href=\"http://music.163.com\">music.163.com</a>)<br/>"
            "Author: ij96<br/>"
            "<a href=\"http://github.com/ij96/163-lyrics-getter\">GitHub page</a>"
            ));
}

void MainWindow::display_song_status() {
    QString msg;
    switch(song->status_code) {
        case SONG_STATUS_HAS_LRC_TRANSLRC :
            msg = tr("All found.");
            break;
        case SONG_STATUS_NOT_EXIST :
            msg = tr("Song does not exist.");
            break;
        case SONG_STATUS_NO_LRC :
            msg = tr("Lyrics do not exist.");
            break;
        case SONG_STATUS_NO_TRANSLRC :
            msg = tr("Lyrics found, but translated lyrics not found.");
            break;
        case SONG_STATUS_INSTRUMENTAL :
            msg = tr("Song is instrumental - no lyrics should exist.");
            break;
    }
    status_edit->setText(msg);
}

void MainWindow::set_language(QAction* action) {
    QLocale locale_new = QLocale(action->data().toString());
    if(locale != locale_new) {
        locale = locale_new;
        save_settings();
        qApp->removeTranslator(&translator);
        if(translator.load(QString(":/language/%1.qm").arg(locale.name())))
            qApp->installTranslator(&translator);
        retranslate_ui();
    }
}

bool MainWindow::setup_settings_file() {
    QFile file(settings_file_path);
    if (!file.exists()) {  // if settings file does not exist, create and initialise it
        if(!file.open(QFile::WriteOnly | QFile::Text))
            return false;
        else {
            QTextStream out(&file);
            out << "[General]\n" << "locale=en_GB";
            out.flush();
            file.close();
        }
    }
    return true;
}

void MainWindow::load_settings() {
    QSettings *settings = new QSettings(settings_file_path, QSettings::IniFormat);
    locale = QLocale(settings->value("locale", "").toString());
}

void MainWindow::save_settings() {
    QSettings *settings = new QSettings(settings_file_path, QSettings::IniFormat);
    settings->setValue("locale", locale.name());
    settings->sync();
}

QString MainWindow::remove_tags(QString lrc) {
    QRegExp rx_lrc_tags("\\[[^\\]]*\\]");
    QRegExp rx_multiple_blank_rows("\\n\\n[\\n]+");
    QRegExp rx_opening_blank_rows("^[\\n]+");
    QString lrc_no_tags = lrc;
    lrc_no_tags = lrc_no_tags.replace(rx_lrc_tags,"");
    lrc_no_tags = lrc_no_tags.replace(rx_multiple_blank_rows,"\n\n");
    lrc_no_tags = lrc_no_tags.replace(rx_opening_blank_rows,"");
    return lrc_no_tags;
}

void MainWindow::display_lrc_translrc() {
    if(show_tags) {
        lrc_text->setText(song->lrc);
        translrc_text->setText(song->translrc);
    } else {
        lrc_text->setText(remove_tags(song->lrc));
        translrc_text->setText(remove_tags(song->translrc));
    }
}

void MainWindow::show_or_hide_tags() {
    show_tags = !hide_tags_button->isChecked();
    display_lrc_translrc();
}

void MainWindow::retranslate_ui() {
    // app name
    app_name = tr("163 Lyrics Getter %1").arg(app_version);
    setWindowTitle(app_name);

    // widgets
    input_id_label->setText(tr("Song ID:"));
    input_button->setText(tr("Get lyrics"));
    info_title_label->setText(tr("Title:"));
    info_artist_label->setText(tr("Artist:"));
    info_album_label->setText(tr("Album:"));
    info_cover_save_button->setText(tr("Save cover image"));

    lrc_label->setText(tr("Original lyrics:"));
    lrc_save_button->setText(tr("Save original lyrics"));
    lrc_submit_button->setText(tr("Submit lyrics to 163"));

    translrc_label->setText(tr("Translated lyrics:"));
    translrc_save_button->setText(tr("Save translated lyrics"));
    translrc_submit_button->setText(tr("Submit translation to 163"));

    hide_tags_button->setTexts(tr("Hide LRC tags"),tr("Show LRC tags"));

    status_label->setText(tr("Status:"));
    if(!status_edit->text().isEmpty())
        display_song_status();

    // menu bar
    file_menu->setTitle(tr("File"));
    options_menu->setTitle(tr("Options"));
    language_menu->setTitle(tr("Language"));
    about_menu->setTitle(tr("About"));

    save_lrc_action->setText(tr("Save original lyrics"));
    save_translrc_action->setText(tr("Save translated lyrics"));
    save_info_cover_action->setText(tr("Save cover image"));
    quit_action->setText(tr("Quit"));
    about_action->setText(tr("About"));

    // cover window
    info_cover_image->image()->window()->setWindowTitle(tr("Album cover art: %1").arg(song->album));
}
