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
        ~node_t() {delete array; delete value;}

        struct node_t *array = 0, *value = 0, *parent = 0;
        enum Types {Array, Object, Value} type = Value;
        int id = -1;
    } tree, error;
    QMap<int, node_t *> map;

    node_t *newNode(node_t *parent, node_t::Types type);
};

#endif // JSONHIGHLIGHTER_H
