#include "qwintexttounicode.h"
#include <QTextCodec>

QString
QWinTextToUnicode(const char *text) {
  return QTextCodec::codecForName("CP1251")->toUnicode(text);
}

QString
QWinTextToUnicode(const std::string *text) {
  return QWinTextToUnicode(text->c_str());
}
