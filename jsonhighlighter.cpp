#include "jsonhighlighter.h"

JsonHighlighter::JsonHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {}

void JsonHighlighter::highlightBlock(const QString &text) {
    QRegularExpression expression;
    QRegularExpressionMatchIterator i;

    expression.setPatternOptions(QRegularExpression::MultilineOption);

    // key
    QTextCharFormat key_format;
    key_format.setFontItalic(false);
    key_format.setFontFixedPitch(true);
    key_format.setForeground(Qt::darkBlue);
    key_format.setFontWeight(QFont::Light);
    // value: numerical
    QTextCharFormat numerical_format;
    numerical_format.setFontItalic(false);
    numerical_format.setFontFixedPitch(true);
    numerical_format.setForeground(Qt::blue);
    numerical_format.setFontWeight(QFont::Light);
    // value: boolean
    QTextCharFormat boolean_format;
    boolean_format.setFontItalic(false);
    boolean_format.setFontFixedPitch(true);
    boolean_format.setForeground(Qt::black);
    boolean_format.setFontWeight(QFont::Bold);
    // value: text
    QTextCharFormat text_format;
    text_format.setFontItalic(false);
    text_format.setFontFixedPitch(true);
    text_format.setForeground(Qt::red);
    text_format.setFontWeight(QFont::Light);
    // value: other
    QTextCharFormat other_format;
    other_format.setFontItalic(false);
    other_format.setFontFixedPitch(true);
    other_format.setForeground(Qt::black);
    other_format.setFontWeight(QFont::Light);

    expression.setPattern("(\"[^\"]*\")\\s*:\\s*((.*[^,])(?(?=,)(?=,$)|(?=$))|([{[]))");

    i = expression.globalMatch(text);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        setFormat(match.capturedStart(1), match.capturedLength(1), key_format);
        QString value = match.captured(2);
        if (QRegularExpression("^(true|false|null)$").match(value).hasMatch()) {
            setFormat(match.capturedStart(2), match.capturedLength(2), boolean_format);
        } else if (QRegularExpression("^[0-9]+(.[0-9]+)?$"
                                      "|^[1-9](.[0-9]+)?([eE][+-]?[0-9]+)?$")
                                      .match(value).hasMatch()) {
            setFormat(match.capturedStart(2), match.capturedLength(2), numerical_format);
        } else if (QRegularExpression("^\".*\"$").match(value).hasMatch()) {
            setFormat(match.capturedStart(2), match.capturedLength(2), text_format);
        }
    }
}
