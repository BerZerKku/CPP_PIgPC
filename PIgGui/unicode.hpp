#ifndef UNICODE_HPP
#define UNICODE_HPP

#include <QString>
#include <QTextCodec>

QString ToUnicode(const std::string &text)
{
    static const QTextCodec *const kCodec = QTextCodec::codecForName("CP1251");

    return kCodec->toUnicode(text.c_str());
}

#endif  // UNICODE_HPP
