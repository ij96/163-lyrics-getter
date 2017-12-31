#ifndef TOGGLEBUTTON_H
#define TOGGLEBUTTON_H

#include <QtWidgets>

class ToggleButton : public QPushButton {
    Q_OBJECT

public:
    explicit ToggleButton(QWidget *parent = 0, QString unchecked_txt = "", QString checked_txt = "");

    void setTexts(QString unchecked_txt, QString checked_txt);

private:
    QString unchecked_text;
    QString checked_text;

signals:
    toggled();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
};

#endif // TOGGLEBUTTON_H
