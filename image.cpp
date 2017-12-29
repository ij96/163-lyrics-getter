#include "image.h"

Image::Image(QWidget *parent) : QLabel(parent) {
    window_label = new QLabel(this);
    window_label->setWindowModality(Qt::NonModal);
    window_label->setWindowFlags(Qt::Dialog);
    window_label->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
}

QWidget *Image::window() const {
    return window_label;
}

void Image::mousePressEvent(QMouseEvent *ev) {
    Q_UNUSED(ev);
    const QPixmap *pm = pixmap();
    if (!pm || pm->isNull())
        return;

    window_label->setPixmap(*pm);
    window_label->setFixedSize(pm->size());
    window_label->show();
}

ImageCanvas::ImageCanvas(QWidget *parent) : QWidget(parent) {
    image_label = new Image(this);
    image_label->setScaledContents(true);

    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

Image *ImageCanvas::image() const {
    return image_label;
}

void ImageCanvas::setPixmap(const QPixmap &pixmap) {
    image_label->setPixmap(pixmap);

    QResizeEvent event(size(), size());
    resizeEvent(&event);
}

void ImageCanvas::resizeEvent(QResizeEvent *event) {
    const QPixmap *pm = image_label->pixmap();
    if (!pm || pm->isNull())
        return;

    QSize s = event->size();
    int h = s.width() * pm->width() / pm->height();
    int w = s.height() * pm->height() / pm->width();

    if (h < s.height())
        s.setHeight(h);
    if (w < s.width())
        s.setWidth(w);

    image_label->setFixedSize(s);
}
