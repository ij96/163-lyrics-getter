#include "lyricshighlighter.h"

LyricsHighlighter::LyricsHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {}

void LyricsHighlighter::highlightBlock(const QString &text) {
    QRegularExpression expression;
    QRegularExpressionMatchIterator i;

    expression.setPatternOptions(QRegularExpression::MultilineOption);

    // Time tags
    QTextCharFormat time_format;
    time_format.setFontFamily("Courier");
    time_format.setFontItalic(false);
    time_format.setFontFixedPitch(true);
    time_format.setForeground(Qt::blue);
    time_format.setFontWeight(QFont::Light);
    expression.setPattern("^(?:\\[[0-9:.]+\\])+");

    i = expression.globalMatch(text);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        setFormat(match.capturedStart(), match.capturedLength(), time_format);
    }

    // Meta tags
    QTextCharFormat meta_tag_format;
    meta_tag_format.setFontFamily("Courier");
    meta_tag_format.setFontItalic(false);
    meta_tag_format.setFontFixedPitch(true);
    meta_tag_format.setForeground(Qt::red);
    meta_tag_format.setFontWeight(QFont::Light);
    QTextCharFormat meta_info_format;
    meta_info_format.setFontFamily(document()->defaultFont().family());
    meta_info_format.setFontItalic(false);
    meta_info_format.setFontFixedPitch(false);
    meta_info_format.setForeground(Qt::darkRed);
    meta_info_format.setFontWeight(QFont::Normal);

    expression.setPattern("^\\[[^\\]:]*[^0-9\\]:]+[^\\]:]*:([^\\]]*)\\]");
    i = expression.globalMatch(text);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        setFormat(match.capturedStart(0), match.capturedLength(0), meta_tag_format);
        setFormat(match.capturedStart(1), match.capturedLength(1), meta_info_format);
    }

    // Unknown tags
    QTextCharFormat unknown_format;
    unknown_format.setFontFamily(document()->defaultFont().family());
    unknown_format.setFontItalic(false);
    unknown_format.setFontFixedPitch(false);
    unknown_format.setForeground(Qt::gray);
    unknown_format.setFontWeight(QFont::Normal);

    expression.setPattern("(?:\\][^[]+)"
                          "(?=((?:\\[[^\\[]+\\])+))"
                          "|(\\[[^:]*\\]|\\[[0-9]+:[^\\]:]*[^0-9.:\\]]+[^\\]:]*\\])");
    i = expression.globalMatch(text);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        setFormat(match.capturedStart(1), match.capturedLength(1), unknown_format);
        setFormat(match.capturedStart(2), match.capturedLength(2), unknown_format);
    }

    // Lyrics text
    QTextCharFormat text_format;
    text_format.setFontFamily(document()->defaultFont().family());
    text_format.setFontItalic(false);
    text_format.setFontFixedPitch(false);
    text_format.setForeground(Qt::black);
    text_format.setFontWeight(QFont::Normal);

    expression.setPattern("(?:^|\\])([^[\\]]+)(?:$|\\[)");
    i = expression.globalMatch(text);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        setFormat(match.capturedStart(1), match.capturedLength(1), text_format);
    }
}
