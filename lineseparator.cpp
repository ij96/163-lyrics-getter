#include "lineseparator.h"

LineSeparator::LineSeparator(QWidget *parent) : QFrame(parent) {
    setGeometry(QRect());
    setFrameShape(QFrame::HLine);
    setFrameShadow(QFrame::Sunken);
}
