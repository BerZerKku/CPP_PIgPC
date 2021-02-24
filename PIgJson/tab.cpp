#include "tab.h"
#include "ui_tab.h"
#include "QDebug"
#include <QTextCodec>
#include "common/qwintexttounicode.h"

TTab::TTab(QWidget *parent) :
                            QWidget(parent),
                            ui(new Ui::TTab)
{
    ui->setupUi(this);
}

TTab::~TTab()
{
    delete ui;
}

bool TTab::set(QVariantMap &content)
{
    return parseContent(content, this);
}

bool TTab::parseContent(QVariantMap &content, QWidget *widget)
{
    QString field;

    if (!parseName(content, widget)) {
        return false;
    }

    if (!parseType(content, widget)) {
        return false;
    }

    if (!parseDescription(content, widget)) {
        return false;
    }

    if (!parseField(content, widget)) {
        return false;
    }

    return true;
}

bool TTab::parseDescription(QVariantMap &content, QWidget *widget)
{
    QString field = "description";

    if (content.count(field) == 0) {
        return true;
    }

    if (content.count(field) > 1) {
        qDebug() << "Field " << field << " is not single";
        return false;
    }

    if (content.value(field).isNull()) {
        return true;
    }

    if (content.value(field).type() != QVariant::String) {
          qDebug() << "Field " << field << " is not 'QVariant::String'";
          return false;
    }



    return true;
}

bool TTab::parseField(QVariantMap &content, QWidget *widget)
{
    QString field = "field";

    if (content.count(field) == 0) {
        return true;
    }

    if (content.count(field) > 1) {
        qDebug() << "Field " << field << " is not single";
        return false;
    }

    if (content.value(field).isNull()) {
        return true;
    }

    QVariant::Type type = content.value(field).type();
    if (type == QVariant::Map) {
        for(auto c: content.value(field).toMap()) {
            QVariantMap m = c.toMap();
            parseContent(m, widget);
        }
    } else if (type == QVariant::List) {
        qDebug() << " Field type QVariant::List= " << type;
    } else {
        qDebug() << "Field " << field <<
            " type " << type <<" does not support";
        return false;
    }

    return true;
}

bool TTab::parseName(QVariantMap &content, QWidget *widget)
{
    QString field = "name";

    qDebug() << "Name = " << content.value(field).toString();
    if (content.count(field) != 1) {
        qDebug() << "Field " << field << " not found";
        return false;
    }

    widget->setWindowTitle(content.value(field).toString());
    return true;
}

bool TTab::parseType(QVariantMap &content, QWidget *widget)
{
    QString field = "type";

    if (content.count(field) != 1) {
        qDebug() << "Field " << field << " not found";
        return false;
    }

    return true;
}
