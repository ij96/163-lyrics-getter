#include "lyrics.h"

Lyrics::Lyrics() {}

void Lyrics::set(QString qstr) {
    lrc = qstr;
}

QString Lyrics::show(bool show_tags, bool ordered) {
    QString lrc_shown = lrc;
    if(ordered) {
        lrc_shown = order_tags(lrc_shown);
    }
    if(!show_tags) {
        lrc_shown = remove_tags(lrc_shown);
    }
    return lrc_shown;
}

QString Lyrics::remove_tags(QString lrc_raw) {
    QRegExp rx_lrc_tags("\\[[^\\]]*\\]");
    QRegExp rx_multiple_blank_rows("\\n\\n[\\n]+");
    QRegExp rx_opening_blank_rows("^[\\n]+");
    QString lrc_no_tags = lrc_raw;
    lrc_no_tags = lrc_no_tags.replace(rx_lrc_tags,"");
    lrc_no_tags = lrc_no_tags.replace(rx_multiple_blank_rows,"\n\n");
    lrc_no_tags = lrc_no_tags.replace(rx_opening_blank_rows,"");
    return lrc_no_tags;
}

QString Lyrics::order_tags(QString lrc_raw) {
    // extract time and meta tags
    QStringList lines = lrc_raw.split("\n");
    QMap<QTime,QString> time_tagged;
    QStringList meta_tagged;
    for(int i = 0; i < lines.length(); i++) {
        QRegularExpression tag_re("(?<=\\[)((?!\\]).)+");
        QRegularExpressionMatchIterator tag_match_iter = tag_re.globalMatch(lines[i]);
        QStringList tags;
        while(tag_match_iter.hasNext()) {
            QString tag = tag_match_iter.next().captured();
            tags << tag;
        }
        QRegularExpression all_tags_re("^\\[.*\\]");
        QString content = lines[i].replace(all_tags_re,"");
        for(int i = 0; i < tags.length(); i++) {
            QTime time = QTime::fromString(tags[i],"mm:ss.z");
            if(time.isValid()) {
                time_tagged.insert(time,content);
            } else {
                meta_tagged << tags[i];
            }
        }
    }
    //meta_tagged.sort(); // order meta tags by alphabet

    // determine suitable number of decimal places for fractional seconds
    int decimal_places = 2;
    foreach(const QTime &time, time_tagged.keys()) {
        if(time.toString("zzz")[2] != "0")
            decimal_places = 3;
            break;
    }

    // output LRC
    QString lrc_ordered = "";
    for(int i = 0; i < meta_tagged.length(); i++) {
        lrc_ordered += "[" + meta_tagged[i] + "]\n";
    }
    foreach(const QTime &time, time_tagged.keys()) {
        QString tag = time.toString("[mm:ss.zzz]");
        if(decimal_places == 2) {
            tag.remove(tag.length() - 2,1);
        }
        lrc_ordered += tag + time_tagged.value(time) + "\n";
    }

    return lrc_ordered;
}
