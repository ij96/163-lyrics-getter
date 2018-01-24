#ifndef JSONHIGHLIGHTER_H
#define JSONHIGHLIGHTER_H

#include <QtGui>

class JsonHighlighter : public QSyntaxHighlighter {
public:
    explicit JsonHighlighter(QTextDocument *parent);

protected:
    virtual void highlightBlock(const QString &text);

private:
    struct node_t {
        ~node_t() {delete next; delete array; delete comma;}

        struct node_t *next = 0, *array = 0, *comma = 0, *parent = 0;
        enum Types {Key, Value, Array, Object, Colon, Comma} type = Value;
        int id = -1;
    } tree, error;
    QMap<int, node_t *> map;

    node_t *newNode(node_t *parent, node_t::Types type);
};

#endif // JSONHIGHLIGHTER_H
