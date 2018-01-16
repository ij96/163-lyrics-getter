#include "lyricshighlighter.h"

LyricsHighlighter::LyricsHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {}

void LyricsHighlighter::highlightBlock(const QString &text) {
    QTextCharFormat format;
    QRegularExpression expression;
    QRegularExpressionMatchIterator i;

    expression.setPatternOptions(QRegularExpression::MultilineOption);

    // Time tag
    format.setFontFamily("Courier");
    format.setFontItalic(false);
    format.setFontFixedPitch(true);
    format.setForeground(Qt::blue);
    format.setFontWeight(QFont::Light);
    expression.setPattern("\\[[0-9:.]+\\]");
    i = expression.globalMatch(text);
    while (i.hasNext()) {
      QRegularExpressionMatch match = i.next();
      setFormat(match.capturedStart(), match.capturedLength(), format);
    }

    // Lyric text
    format.setFontFamily(document()->defaultFont().family());
    format.setFontItalic(false);
    format.setFontFixedPitch(false);
    format.setForeground(Qt::black);
    format.setFontWeight(QFont::Bold);
    expression.setPattern("(?:^|\\])([^[\\]]+)(?:$|\\[)");
    i = expression.globalMatch(text);
    while (i.hasNext()) {
      QRegularExpressionMatch match = i.next();
      setFormat(match.capturedStart(1), match.capturedLength(1), format);
    }
}
