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

    if (previousBlockState() == 0) {
        // Structural error
        setCurrentBlockState(0);
        setFormat(0, text.length(), error_format);
        return;
    }

    static const QRegularExpression key_exp("^\\s*(?:,|(\\})|(\"(?:\\\\\"|[^\"])*\")|([^\\s]))\\s*");
    static const QRegularExpression value_exp("^\\s*(?:[,:]|(\\[)|(\\])|(\\{)|(-?(?:0|[1-9][0-9]*)(?:\\.[0-9]+)?(?:[eE][+-]?[0-9]+)?)|(\"(?:\\\\\"|[^\"])*\")|(true|false)|(null)|([^\\s]))\\s*");

    node_t *np = map[previousBlockState()];
    const QRegularExpression *exp = &value_exp;
    if (np->type == node_t::Object)
        exp = &key_exp;

    QRegularExpressionMatch match;
    QString s = text;
    int i = 0;
    while ((match = exp->match(s)).hasMatch()) {
        int m = 0;
        if (exp == &key_exp) {
            if (match.capturedLength(++m)) {
                // End of object
                np = np->parent;
                if (np->type == node_t::Value)
                    np = np->parent;
                if (!np) {
                    // Error: top-level
                    i += match.capturedEnd(m);
                    break;
                }
            } else if (match.capturedLength(++m)) {
                // Key
                setFormat(i + match.capturedStart(m), match.capturedLength(m), key_format);
                if (!np->value)
                    np->value = newNode(np, node_t::Value);
                np = np->value;
            } else if (match.capturedLength(++m)) {
                // Error: unexpected
                i += match.capturedStart(m);
                np = 0;
                break;
            }
        } else {
            if (match.capturedLength(++m)) {
                // Array
                if (!np->array)
                    np->array = newNode(np, node_t::Array);
                np = np->array;
            } else if (match.capturedLength(++m)) {
                // End of array
                if (np->type != node_t::Array) {
                    // Error: unexpected
                    i += match.capturedStart(m);
                    np = 0;
                    break;
                }
                np = np->parent;
                if (np->type == node_t::Value)
                    np = np->parent;
                if (!np) {
                    // Error: top-level
                    i += match.capturedEnd(m);
                    break;
                }
            } else if (match.capturedLength(++m)) {
                // Object
                if (!np->value)
                    np->value = newNode(np, node_t::Object);
                np = np->value;
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
                    np = 0;
                    break;
                }
                // Value captured
                if (match.lastCapturedIndex() != 0 && np->type == node_t::Value)
                    np = np->parent;
            }
        }
        i += match.capturedLength();
        s.remove(0, match.capturedLength());
        // Object expects new key, Array/Value expects new value
        exp = np->type == node_t::Object ? &key_exp : &value_exp;
    }

    if (!np) {
        // Structural error
        setCurrentBlockState(0);
        setFormat(i, text.length() - i, error_format);
    } else
        setCurrentBlockState(np->id);
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
