#include "lyrics.h"

Lyrics::Lyrics() {}

void Lyrics::set(QString qstr) {
    _data = qstr;
    update_tag_maps();
}

QString Lyrics::data() {
    return _data;
}

QStringList Lyrics::meta_map() {
    return _meta_list;
}

QMap<QTime,QString> Lyrics::time_map() {
    return _time_map;
}

void Lyrics::update_tag_maps() {
    // clear old data
    _time_map.clear();
    _meta_list.clear();

    // extract time and meta tags
    QStringList lines = _data.split("\n");
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
            QStringList time_patterns;
            bool valid_time = false;
            QTime time;
            int j = 0;
            time_patterns << "mm:ss.z" << "mm:ss";

            // enumerate through possible time tag patterns
            while(!valid_time && j < time_patterns.length()) {
                time = QTime::fromString(tags[i],time_patterns[j]);
                if(time.isValid()) {
                    _time_map.insert(time,content);
                    valid_time = true;
                }
                j++;
            }
            if(valid_time) {
                _time_map.insert(time,content);
            } else {
                _meta_list << tags[i];
            }
        }
    }
    //_meta_list.sort(); // order meta tags by alphabet
}

void Lyrics::time_map_insert(QTime key, QString value) {
    _time_map.insert(key, value);
}

QString Lyrics::show(bool show_tags, bool ordered) {
    QString lrc_shown = _data;
    if(ordered) {
        lrc_shown = order_tags();
    }
    if(!show_tags) {
        lrc_shown = remove_tags(lrc_shown);
    }
    return lrc_shown;
}

QString Lyrics::remove_tags(QString lrc_raw) {
    QRegularExpression re_lrc_tags("\\[[^\\]]*\\]");
    QRegularExpression re_multiple_blank_rows("\\n\\n[\\n]+");
    QRegularExpression re_opening_blank_rows("^[\\n]+");
    QString lrc_no_tags = lrc_raw;
    lrc_no_tags = lrc_no_tags.replace(re_lrc_tags,"");
    lrc_no_tags = lrc_no_tags.replace(re_multiple_blank_rows,"\n\n");
    lrc_no_tags = lrc_no_tags.replace(re_opening_blank_rows,"");
    return lrc_no_tags;
}

QString Lyrics::order_tags() {
    // determine suitable number of decimal places for fractional seconds
    int decimal_places = 2;
    foreach(const QTime &time, _time_map.keys()) {
        if(time.toString("zzz")[2] != "0")
            decimal_places = 3;
            break;
    }

    // output LRC
    QString lrc_ordered = "";
    for(int i = 0; i < _meta_list.length(); i++) {
        lrc_ordered += "[" + _meta_list[i] + "]\n";
    }
    foreach(const QTime &time, _time_map.keys()) {
        QString tag = time.toString("[mm:ss.zzz]");
        if(decimal_places == 2) {
            tag.remove(tag.length() - 2,1);
        }
        lrc_ordered += tag + _time_map.value(time) + "\n";
    }
    return lrc_ordered;
}
