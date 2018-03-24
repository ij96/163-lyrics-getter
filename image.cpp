#include "image.h"

Image::Image(QWidget *parent) : QLabel(parent) {
    window_label = new QLabel(this);

    viewer = new QScrollArea(this);
    viewer->setWindowModality(Qt::NonModal);
    viewer->setWindowFlags(Qt::Window);
    viewer->setWidget(window_label);
}

void Image::set_window_title(QString title) {
    viewer->setWindowTitle(title);
}

void Image::mousePressEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    update_image();
    viewer->show();
}

void Image::update_image() {
    const QPixmap *pm = pixmap();
    if (!pm || pm->isNull()) {
        window_label->setPixmap(QPixmap(0, 0));
        viewer->hide();
    } else {
        window_label->setPixmap(*pm);
        window_label->setFixedSize(pm->size());
    }

    QSize viewer_max_size = QSize(640, 640);
    viewer->setWidget(window_label);
    if ((pm->height() > viewer_max_size.height())
            || (pm->width() > viewer_max_size.width())) {
        viewer->resize(viewer_max_size);
    } else {
        QSize viewer_size = QSize(pm->width() + 2, pm->height() + 2);
        viewer->resize(viewer_size);
    }
    updateGeometry();
}


ImageCanvas::ImageCanvas(QWidget *parent) : QWidget(parent) {
    image_label = new Image(this);
    image_label->setScaledContents(true);

    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

void ImageCanvas::setPixmap(const QPixmap &pixmap) {
    image_label->setPixmap(pixmap);
    image_label->update_image();

    QResizeEvent event(size(), size());
    resizeEvent(&event);
}

void ImageCanvas::set_window_title(QString title) {
    image_label->setWindowTitle(title);
}

void ImageCanvas::resizeEvent(QResizeEvent *event) {
    const QPixmap *pm = image_label->pixmap();
    if (!pm || pm->isNull()) {
        image_label->setFixedSize(0, 0);
    } else {
        QSize s = event->size();
        int h, w;

        if (pm->height() != 0) {
            h = s.width() * pm->height() / pm->width();
        } else {
            h = 0;
        }
        if (pm->width() != 0) {
            w = s.height() * pm->width() / pm->height();
        } else {
            w = 0;
        }

        if (h < s.height()) s.setHeight(h);
        if (w < s.width()) s.setWidth(w);

        image_label->setFixedSize(s);
    }
}
