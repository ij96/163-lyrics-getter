#include "image.h"

Image::Image(QWidget *parent) : QLabel(parent) {
    window_label = new QLabel(this);

    viewer = new QScrollArea(this);
    viewer->setWindowModality(Qt::NonModal);
    viewer->setWindowFlags(Qt::Window);
    viewer->setWidget(window_label);
}

QWidget *Image::window() const {
    return viewer;
}

void Image::mousePressEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    update_image();
    viewer->show();
}

void Image::update_image() {
    const QPixmap *pm = pixmap();
    if (!pm || pm->isNull()) {
        window_label->setPixmap(QPixmap(0,0));
    } else {
        window_label->setPixmap(*pm);
    }
    window_label->setFixedSize(pm->size());

    QSize viewer_max_size = QSize(640,640);
    QSize viewer_min_size = QSize(200,200);
    viewer->setWidget(window_label);
    if((pm->height() > viewer_max_size.height()) | (pm->width() > viewer_max_size.width())) {
        viewer->resize(viewer_max_size);
    } else if ((pm->height() < viewer_min_size.height()) | (pm->width() < viewer_min_size.width())){
        viewer->resize(viewer_min_size);
    } else {
        QSize viewer_size = QSize(pm->width()+2,pm->height()+2);
        viewer->resize(viewer_size);
    }
    updateGeometry();
}

//------------

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
    image_label->update_image();

    QResizeEvent event(size(), size());
    resizeEvent(&event);
}

void ImageCanvas::resizeEvent(QResizeEvent *event) {
    const QPixmap *pm = image_label->pixmap();
    if (!pm || pm->isNull())
        return;

    QSize s = event->size();
    int h = s.width() * pm->height() / pm->width();
    int w = s.height() * pm->width() / pm->height();

    if (h < s.height())
        s.setHeight(h);
    if (w < s.width())
        s.setWidth(w);

    image_label->setFixedSize(s);
}
