#ifndef LYRICSHIGHLIGHTER_H
#define LYRICSHIGHLIGHTER_H

#include <QtGui>

class LyricsHighlighter : public QSyntaxHighlighter
{
public:
    explicit LyricsHighlighter(QTextDocument *parent);

protected:
    virtual void highlightBlock(const QString &text);
};

#endif // LYRICSHIGHLIGHTER_H
