#include "togglebutton.h"

ToggleButton::ToggleButton(QWidget *parent, QString unchecked_txt, QString checked_txt) : QPushButton(parent){
    unchecked_text = unchecked_txt;
    checked_text = checked_txt;

    setCheckable(true);
    setText(unchecked_text);
}

void ToggleButton::mousePressEvent(QMouseEvent *event) {
    Q_UNUSED(event);

    if(isChecked()) {
        setChecked(false);
        setText(unchecked_text);
    } else {
        setChecked(true);
        setText(checked_text);
    }

    emit toggled();
}
