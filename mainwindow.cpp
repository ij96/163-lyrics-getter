#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
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

    lrc_label = new QLabel(tr("Original lyrics:"));
    lrc_text = new QTextEdit();
    lrc_text->setFont(QFont("Microsoft Yahei",10));
    lrc_save_button = new QPushButton(tr("Save original lyrics"));
    translrc_label = new QLabel(tr("Translated lyrics:"));
    translrc_text = new QTextEdit();
    translrc_text->setFont(QFont("Microsoft Yahei",10));
    translrc_save_button = new QPushButton(tr("Save translated lyrics"));
    //---END widgets---

    //---layouts---
    //3rd
    QHBoxLayout *input_id_layout = new QHBoxLayout();
    input_id_layout->addWidget(input_id_label);
    input_id_layout->addWidget(input_id_edit);

    QHBoxLayout *info_title_layout = new QHBoxLayout();
    info_title_layout->addWidget(info_title_label);
    info_title_layout->addWidget(info_title_edit);

    QHBoxLayout *info_artist_layout = new QHBoxLayout();
    info_artist_layout->addWidget(info_artist_label);
    info_artist_layout->addWidget(info_artist_edit);

    QHBoxLayout *info_album_layout = new QHBoxLayout();
    info_album_layout->addWidget(info_album_label);
    info_album_layout->addWidget(info_album_edit);

    QVBoxLayout *lrc_layout = new QVBoxLayout();
    lrc_layout->addWidget(lrc_label);
    lrc_layout->addWidget(lrc_text);
    lrc_layout->addWidget(lrc_save_button);

    QVBoxLayout *translrc_layout = new QVBoxLayout();
    translrc_layout->addWidget(translrc_label);
    translrc_layout->addWidget(translrc_text);
    translrc_layout->addWidget(translrc_save_button);

    //2nd
    QVBoxLayout *info_layout = new QVBoxLayout();
    info_layout->addLayout(info_title_layout);
    info_layout->addLayout(info_artist_layout);
    info_layout->addLayout(info_album_layout);

    QVBoxLayout *input_layout = new QVBoxLayout();
    input_layout->addLayout(input_id_layout);
    input_layout->addWidget(input_button);

    //1st
    QHBoxLayout *lrc_translrc_layout = new QHBoxLayout();
    lrc_translrc_layout->addLayout(lrc_layout);
    lrc_translrc_layout->addLayout(translrc_layout);

    QHBoxLayout *toolbar_layout = new QHBoxLayout();
    toolbar_layout->addLayout(input_layout);
    toolbar_layout->addLayout(info_layout);

    //0th
    QVBoxLayout *main_layout = new QVBoxLayout();
    main_layout->addLayout(toolbar_layout);
    main_layout->addLayout(lrc_translrc_layout);

    setLayout(main_layout);
    setFixedSize(840,600);
    setWindowTitle(tr("163-get-lyrics"));
    //---END layouts---

    song = new Song();

    //---connect---
    connect(input_button,SIGNAL(clicked()),this,SLOT(get_info_lyrics()));
    connect(lrc_save_button,SIGNAL(clicked()),this,SLOT(save_lrc()));
    connect(translrc_save_button,SIGNAL(clicked()),this,SLOT(save_translrc()));
    //---END connect---
}

MainWindow::~MainWindow(){}

void MainWindow::get_info_lyrics(){
    qDebug() << "\nGetting info & lyrics...";
    song->id = input_id_edit->text().toDouble();
    song->get_info();
    song->get_lyrics();
    lrc_text->setText(song->lrc);
    translrc_text->setText(song->translrc);
    info_title_edit->setText(song->title);
    info_artist_edit->setText(song->artist);
    info_album_edit->setText(song->album);
}

void MainWindow::quit(){
    qApp->quit();
}

bool MainWindow::save(bool save_translated){
    qDebug() << "\nSaving...";
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    QString file_name = dialog.selectedFiles().first();
    QFile file(file_name);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
            tr("Cannot write file %1:\n%2.")
            .arg(QDir::toNativeSeparators(file_name),
            file.errorString()));
        return false;
    }

    QTextStream out(&file);
    out.setCodec("UTF-8");
    if(save_translated){
        out << translrc_text->toPlainText();
    }
    else{
        out << lrc_text->toPlainText();
    }
    out.setGenerateByteOrderMark(true);
    out.flush();

    file.close();

    return true;
}

bool MainWindow::save_lrc(){
    return save(false);
}

bool MainWindow::save_translrc(){
    return save(true);
}

void MainWindow::about(){
    QMessageBox::about(this, tr("About 163-get-lyrics"),
        tr("Get lyrics from CloudMusic (<a href=\"http://music.163.com\">music.163.com</a>)<br/>"
            "Author: ij96<br/>"
            "<a href=\"http://github.com/ij96/163-get-lyrics\">Source code</a>"
            ));
}

