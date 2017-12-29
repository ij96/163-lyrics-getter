#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    //---settings file---
    settings_file_path = QApplication::applicationDirPath() + "/config.ini";
    setup_settings_file();  // create settings file, if it does not exist
    load_settings();        // load from settings

    //---translator---
    translator.load(QString(":/language/%1.qm").arg(locale.name()));
    qApp->installTranslator(&translator);
    //---END translator---

    app_name = tr("163 Lyrics Getter");
    song = new Song();

    //---widgets---
    input_id_label = new QLabel(tr("Song ID:"));
    input_id_edit = new QLineEdit();
    input_button = new QPushButton(tr("Get lyrics"));

    info_title_label = new QLabel(tr("Title:"));
    info_title_edit = new QLineEdit();
    info_artist_label = new QLabel(tr("Artist:"));
    info_artist_edit = new QLineEdit();
    info_album_label = new QLabel(tr("Album:"));
    info_album_edit = new QLineEdit();
    info_cover_image = new ImageCanvas();
    info_cover_save_button = new QPushButton(tr("Save cover"));

    lrc_label = new QLabel(tr("Original lyrics:"));
    lrc_text = new QTextEdit();
    lrc_save_button = new QPushButton(tr("Save original lyrics"));
    lrc_submit_button = new QPushButton(tr("Submit lyrics to 163"));
    translrc_label = new QLabel(tr("Translated lyrics:"));
    translrc_text = new QTextEdit();
    translrc_save_button = new QPushButton(tr("Save translated lyrics"));
    translrc_submit_button = new QPushButton(tr("Submit translation to 163"));

    status_label = new QLabel(tr("Status:"));
    status_edit = new QLineEdit();
    //---END widgets---

    //---menu bar---
    QMenuBar* menu_bar = new QMenuBar();
    QMenu *file_menu = new QMenu(tr("File"));
        menu_bar->addMenu(file_menu);
        QAction *save_lrc_action = file_menu->addAction(tr("Save original lyrics"));
        QAction *save_translrc_action = file_menu->addAction(tr("Save translated lyrics"));
        file_menu->addSeparator();
        QAction *quit_action = file_menu->addAction(tr("Quit"));
    QMenu *options_menu = new QMenu(tr("Options"));
        menu_bar->addMenu(options_menu);
        QMenu *language_menu = new QMenu(tr("Language"));
            options_menu->addMenu(language_menu);
            QAction *language_en_GB_action = language_menu->addAction("English (UK)"); // do not translate
                language_en_GB_action->setData("en_GB");
            QAction *language_zh_CN_action = language_menu->addAction("\u4E2D\u6587"); // do not translate
                language_zh_CN_action->setData("zh_CN");
    QMenu *about_menu = new QMenu(tr("About"));
        menu_bar->addMenu(about_menu);
        QAction *about_action = about_menu->addAction(tr("About"));
    //---END menu bar---

    //---layouts---
    // 2nd
    QHBoxLayout *input_id_layout = new QHBoxLayout();
    input_id_layout->addWidget(input_id_label,1);
    input_id_layout->addWidget(input_id_edit,3);
    input_id_layout->addWidget(input_button,1);

    QHBoxLayout *status_layout = new QHBoxLayout();
    status_layout->addWidget(status_label,1);
    status_layout->addWidget(status_edit, 4);

    QHBoxLayout *info_title_layout = new QHBoxLayout();
    info_title_layout->addWidget(info_title_label,1);
    info_title_layout->addWidget(info_title_edit,4);

    QHBoxLayout *info_artist_layout = new QHBoxLayout();
    info_artist_layout->addWidget(info_artist_label,1);
    info_artist_layout->addWidget(info_artist_edit,4);

    QHBoxLayout *info_album_layout = new QHBoxLayout();
    info_album_layout->addWidget(info_album_label,1);
    info_album_layout->addWidget(info_album_edit,4);

    /*// Image Canvas
    QHBoxLayout *art_sub_layout = new QHBoxLayout();
    art_sub_layout->addStretch();
    art_sub_layout->addWidget(info_cover_image->image());

    QVBoxLayout *art_layout = new QVBoxLayout(info_cover_image);
    art_layout->setContentsMargins(0, 0, 0, 0);
    art_layout->addLayout(art_sub_layout);
    art_layout->addStretch();*/

    QGridLayout *info_cover_layout = new QGridLayout();
    info_cover_layout->addWidget(info_cover_image);
    info_cover_layout->addWidget(info_cover_save_button);

    // 1st
    QGridLayout *toolbar_layout = new QGridLayout();
    toolbar_layout->addLayout(input_id_layout,   0,0);
    toolbar_layout->addLayout(status_layout,     1,0);

    toolbar_layout->addLayout(info_title_layout, 2,0);
    toolbar_layout->addLayout(info_artist_layout,3,0);
    toolbar_layout->addLayout(info_album_layout, 4,0);

    toolbar_layout->addLayout(info_cover_layout, 5,0);

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
    info_cover_image->image()->window()->setWindowTitle(tr("Album cover art"));
    //---END layouts---

    //---connect---
    connect(input_button,SIGNAL(clicked()),this,SLOT(get_info_lyrics()));
    connect(lrc_save_button,SIGNAL(clicked()),this,SLOT(save_lrc()));
    connect(lrc_submit_button,SIGNAL(clicked()),this,SLOT(submit_lrc()));
    connect(translrc_save_button,SIGNAL(clicked()),this,SLOT(save_translrc()));
    connect(translrc_submit_button,SIGNAL(clicked()),this,SLOT(submit_translrc()));
    connect(info_cover_save_button,SIGNAL(clicked()),this,SLOT(save_info_cover()));

    connect(save_lrc_action,SIGNAL(triggered()),this,SLOT(save_lrc()));
    connect(save_translrc_action,SIGNAL(triggered()),this,SLOT(save_translrc()));
    connect(quit_action,SIGNAL(triggered()),this,SLOT(quit()));
    connect(about_action,SIGNAL(triggered()),this,SLOT(about()));

    connect(language_menu,SIGNAL(triggered(QAction*)),this,SLOT(set_language(QAction*)));

    //---END connect---
}

MainWindow::~MainWindow() {}

void MainWindow::get_info_lyrics() {
    status_edit->setText(tr("Getting song information and lyrics..."));

    song->id = input_id_edit->text().toDouble();
    song->get_info_lyrics();

    info_title_edit->setText(song->title);
    info_artist_edit->setText(song->artist);
    info_album_edit->setText(song->album);
    info_cover_image->setPixmap(QPixmap::fromImage(song->cover));
    lrc_text->setText(song->lrc);
    translrc_text->setText(song->translrc);

    display_song_status();
}

void MainWindow::quit() {
    qApp->quit();
}

bool MainWindow::save(bool save_translated) {
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    QString file_name = "*";
    if(song->status_code != SONG_STATUS_NOT_EXIST) {
        if(save_translated)
            file_name = song->title + " - " + song->artist + " (translated)";
        else
            file_name = song->title + " - " + song->artist;
    }
    file_name += ".lrc";
    dialog.selectFile(file_name);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    QFile file(file_name);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
            tr("Cannot write file %1:\n%2.").arg(QDir::toNativeSeparators(file_name),file.errorString()));
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
        QMessageBox::warning(this, tr("Language changed"),
            tr("Language changed, please restart application to apply the change."));
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
