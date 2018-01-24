#ifndef JSONHIGHLIGHTER_H
#define JSONHIGHLIGHTER_H

#include <QtGui>

class JsonHighlighter : public QSyntaxHighlighter {
public:
    explicit JsonHighlighter(QTextDocument *parent);

protected:
    virtual void highlightBlock(const QString &text);
};

#endif // JSONHIGHLIGHTER_H
