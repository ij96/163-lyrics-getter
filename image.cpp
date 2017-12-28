#include "image.h"

Image::Image(QWidget *parent) : QLabel(parent) {
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Ignored);
    setScaledContents(true);
}

QSize Image::minimumSizeHint() const {
    return QSize();
}

QSize Image::sizeHint() const {
    const QPixmap *pm = pixmap();
    if (!pm || pm->isNull())
        return QSize();

    QSize s = size();
    int h = s.width() * pm->width() / pm->height();
    int w = s.height() * pm->height() / pm->width();
    qDebug() << QSize(w, h) << s << pm->size();
    return QSize(w, s.height());
}

void Image::setPixmap(const QPixmap &pm) {
    QLabel::setPixmap(pm);
    updateGeometry();
}

void Image::mousePressEvent(QMouseEvent *ev) {
    Q_UNUSED(ev);
    const QPixmap *pm = pixmap();
    if (!pm || pm->isNull())
        return;

    QLabel *label = new QLabel(this);
    label->setPixmap(*pm);
    label->setFixedSize(pm->size());
    label->setWindowModality(Qt::WindowModal);
    label->setWindowFlags(Qt::Dialog);
    label->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    label->show();
}
