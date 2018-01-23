#include "togglebutton.h"

ToggleButton::ToggleButton(QWidget *parent,
                           QString unchecked_txt,
                           QString checked_txt) : QPushButton(parent) {
    setCheckable(true);
    setTexts(unchecked_txt, checked_txt);
}

void ToggleButton::setTexts(QString unchecked_txt, QString checked_txt) {
    unchecked_text = unchecked_txt;
    checked_text = checked_txt;
    if (isChecked()) {
        setText(checked_text);
    } else {
        setText(unchecked_text);
    }
}

void ToggleButton::mousePressEvent(QMouseEvent *event) {
    Q_UNUSED(event);

    if (isChecked()) {
        setChecked(false);
        setText(unchecked_text);
    } else {
        setChecked(true);
        setText(checked_text);
    }

    emit toggled();
}
