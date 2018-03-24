#ifndef IMAGE_H
#define IMAGE_H

#include <QtWidgets>
#include <QtGui>

class Image: public QLabel {
    Q_OBJECT

public:
    explicit Image(QWidget *parent = 0);

    void update_image();
    void set_window_title(QString title);

protected:
    virtual void mousePressEvent(QMouseEvent *ev);

private:
    QScrollArea *viewer;
    QLabel *window_label;
};


class ImageCanvas : public QWidget{
    Q_OBJECT

public:
    explicit ImageCanvas(QWidget *parent = 0);

    void setPixmap(const QPixmap &pixmap);
    void set_window_title(QString title);

protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    Image *image_label;
};

#endif // IMAGE_H
