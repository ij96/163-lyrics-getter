#ifndef IMAGE_H
#define IMAGE_H

#include <QtWidgets>

class Image : public QLabel{
    Q_OBJECT

public:
    explicit Image(QWidget *parent = 0);

    virtual QSize minimumSizeHint() const;
    virtual QSize sizeHint() const;
    virtual void setPixmap(const QPixmap &pm);

protected:
    virtual void mousePressEvent(QMouseEvent *ev);
};

#endif // IMAGE_H
