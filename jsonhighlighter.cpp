#include "jsonhighlighter.h"

JsonHighlighter::JsonHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) {
    map.insert(tree.id++, &tree);
    // Error type, id = 0
    map.insert(tree.id++, 0);
}

void JsonHighlighter::highlightBlock(const QString &text) {
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
    // value: array
    QTextCharFormat array_format;
    array_format.setBackground(Qt::lightGray);
    // value: object
    QTextCharFormat obj_format;
    obj_format.setBackground(Qt::cyan);
    // syntax error
    QTextCharFormat error_format;
    error_format.setFontItalic(false);
    error_format.setFontFixedPitch(true);
    error_format.setBackground(Qt::magenta);
    error_format.setFontWeight(QFont::Light);

    static const QRegularExpression regexp[] = {
        // Key(string)
        QRegularExpression("^\\s*(?:(\"(?:\\\\\"|[^\"])*\")|([^\\s]))\\s*"),
        // Value(array, object, number, string, boolean, null)
        QRegularExpression("^\\s*(?:(\\[)|(\\{)|(-?(?:0|[1-9][0-9]*)(?:\\.[0-9]+)?(?:[eE][+-]?[0-9]+)?)|(\"(?:\\\\\"|[^\"])*\")|(true|false)|(null)|([^\\s]))\\s*"),
        // Array(])
        QRegularExpression("^\\s*(?:(\\])|(?=[^\\s]))\\s*"),
        // Object(})
        QRegularExpression("^\\s*(?:(\\})|(?=[^\\s]))\\s*"),
        // Colon(:)
        QRegularExpression("^\\s*(?:(:)|([^\\s]))\\s*"),
        // Comma(,)
        QRegularExpression("^\\s*(?:(,)|(?=[\\]}])|([^\\s]))\\s*"),
    };

    // Node hierarchy:
    // Array: Array->End|Value->Comma->Value|Array
    // Object: Object->End|Key->Colon->Value->Comma->Key|Object

    int i = 0;
    QString s = text;
    QRegularExpressionMatch match;
    const QRegularExpression *exp;
    node_t *p = map[previousBlockState()];
    while (p && (match = (exp = &regexp[p->type])->match(s)).hasMatch()) {
        int m = 0;
        switch (p->type) {
        case node_t::Key:
            if (match.capturedLength(++m)) {
                // Key
                setFormat(i + match.capturedStart(m), match.capturedLength(m), key_format);
                // Colon follows
                if (!p->next)
                    p->next = newNode(p, node_t::Colon);
                p = p->next;
            } else {
                // Error: unexpected
                i += match.capturedStart(++m);
                p = 0;
                goto error;
            }
            break;
        case node_t::Value:
            if (match.capturedLength(++m)) {
                // Array
                if (!p->array)
                    p->array = newNode(p, node_t::Array);
                p = p->array;
            } else if (match.capturedLength(++m)) {
                // Object
                if (!p->next)
                    p->next = newNode(p, node_t::Object);
                p = p->next;
            } else {
                if (match.capturedLength(++m))
                    // Number
                    setFormat(i + match.capturedStart(m), match.capturedLength(m), numerical_format);
                else if (match.capturedLength(++m))
                    // String
                    setFormat(i + match.capturedStart(m), match.capturedLength(m), text_format);
                else if (match.capturedLength(++m))
                    // Boolean
                    setFormat(i + match.capturedStart(m), match.capturedLength(m), boolean_format);
                else if (match.capturedLength(++m))
                    // Null
                    setFormat(i + match.capturedStart(m), match.capturedLength(m), other_format);
                else if (match.capturedLength(++m)) {
                    // Error: unexpected
                    i += match.capturedStart(m);
                    p = 0;
                    goto error;
                }
                // Finish parsing if reached top-level
                if (!p->parent) {
                    i += match.capturedEnd(m);
                    p = 0;
                    goto error;
                }
                // Comma follows
                if (!p->comma)
                    p->comma = newNode(p, node_t::Comma);
                p = p->comma;
            }
            break;
        case node_t::Array:
        case node_t::Object:
            if (match.capturedLength(++m)) {
                // Found character, return to parent data
                do
                    p = p->parent;
                while (p->type != node_t::Value);
                // Finish parsing if reached top-level
                if (!p->parent) {
                    i += match.capturedEnd(m);
                    p = 0;
                    goto error;
                }
                // Comma follows
                if (!p->comma)
                    p->comma = newNode(p, node_t::Comma);
                p = p->comma;
            } else {
                // Data follows
                if (!p->next)
                    p->next = newNode(p, p->type == node_t::Array ? node_t::Value : node_t::Key);
                p = p->next;
            }
            break;
        case node_t::Colon:
            if (match.capturedLength(++m)) {
                // Found colon, value follows
                if (!p->next)
                    p->next = newNode(p, node_t::Value);
                p = p->next;
            } else {
                // Error: unexpected
                i += match.capturedStart(++m);
                p = 0;
                goto error;
            }
            break;
        case node_t::Comma:
            // Found, return to parent
            do
                p = p->parent;
            while (p->type != node_t::Array && p->type != node_t::Object);
            if (match.capturedLength(++m))
                // Found comma, value/pair follows
                p = p->next;
            else if (match.capturedLength(++m)) {
                // Error: unexpected
                i += match.capturedStart(m);
                p = 0;
                goto error;
            }
            break;
        }
        // Match next string segment
        i += match.capturedLength();
        s.remove(0, match.capturedLength());
    }

error:
    if (!p) {
        // Structural error
        setCurrentBlockState(0);
        setFormat(i, text.length() - i, error_format);
    } else
        setCurrentBlockState(p->id);
}

JsonHighlighter::node_t *JsonHighlighter::newNode(node_t *parent, node_t::Types type)
{
    node_t *p = new node_t;
    p->parent = parent;
    p->type = type;
    p->id = tree.id;
    map.insert(tree.id++, p);
    return p;
}
