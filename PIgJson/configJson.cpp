#include "configJson.h"
#include "QDir"
#include "QFile"
#include "QJsonDocument"
#include "QJsonObject"
#include <QCoreApplication>

TConfigJson::TConfigJson(QObject *parent) : QObject(parent)
{

}

void TConfigJson::setFileName(QString filename)
{
    // step 1
    QString path = QCoreApplication::applicationDirPath();

    file.setFileName(path + filename);
}

bool TConfigJson::read()
{
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open '" << file << "'";
        return false;
    }

    // step 2
    QTextStream file_text(&file);
    QString json_string;
    json_string = file_text.readAll();

    file.close();

    QByteArray json_bytes = json_string.toLocal8Bit();

    // step 3
    auto json_doc = QJsonDocument::fromJson(json_bytes);

    if (json_doc.isNull()) {
        qDebug() << "Failed to create JSON doc.";
        return false;
    }
    if (!json_doc.isObject()) {
        qDebug() << "JSON is not an object.";
        return false;
    }

    QJsonObject json_obj = json_doc.object();

    if (json_obj.isEmpty()) {
        qDebug() << "JSON object is empty.";
        return false;
    }

    // step 4
    config = json_obj.toVariantMap();

    return true;
}

bool TConfigJson::save()
{
    return true;
}

bool TConfigJson::getObjectName(QVariantMap &obj, QString &value) const
{
    QString field = "name";

    value = obj.value(field).toString();
    qDebug() << field << " = " << value;

    if (obj.count(field) != 1) {
        qDebug() << "Field " << field << " not found";
        return false;
    }

    return true;
}

bool TConfigJson::getObjectType(QVariantMap &obj, QString &value) const
{
    QString field = "type";

    value = obj.value(field).toString();
    qDebug() << field << " = " << value;

    if (obj.count(field) != 1) {
        qDebug() << "Field " << field << " not found";
        return false;
    }

    return true;
}

bool TConfigJson::getObjectDesc(QVariantMap &obj, QString &value) const
{
    QString field = "desc";

    value = obj.value(field).toString();
    qDebug() << field << " = " << value;

    if (obj.count(field) == 0) {
        return true;
    }

    if (obj.count(field) > 1) {
        qDebug() << "Field " << field << " is not single";
        return false;
    }

    if (obj.value(field).isNull()) {
        return true;
    }

    if (obj.value(field).type() != QVariant::String) {
        qDebug() << "Field " << field << " is not 'QVariant::String'";
        return false;
    }

    return true;
}

bool TConfigJson::getObjectData(QVariantMap &obj, QVariantMap &value) const
{
    QString field = "data";

    if (obj.count(field) == 0) {
        return true;
    }

    if (obj.count(field) > 1) {
        qDebug() << "Field " << field << " is not single";
        return false;
    }

    if (obj.value(field).isNull()) {
        return true;
    }

    return true;
}

bool TConfigJson::getNextTab(QVariantMap &object) const
{
    static QMapIterator<QString, QVariant> i(config);
    bool istab = i.hasNext();

    if (istab) {
        auto next = i.next();
        if (next->type() != QVariant::Map) {
            qDebug() << "Tab object is not 'QVariant::Map'";
            return false;
        }

        object = next->toMap();

        QString type;
        if (!getObjectType(object, type)) {
            qDebug() << "Tab object have wrong 'type' member";
            return false;
        }
    }

    return istab;
}

bool TConfigJson::getNextObject(QVariantMap &object) const
{
    static QVector<QMapIterator<QString, QVariant>> iterator;
    bool isobject = false;

    if (iterator.size() == 0) {
        if (config.first().type() == QVariant::Map) {
            iterator.append(QMapIterator<QString, QVariant> (config));
        }
    }

    while (iterator.size() > 0) {
        isobject = iterator.last().hasNext();

        if (isobject) {
            auto i = iterator.last().next();

            if (i->type() == QVariant::Map) {
                object = i->toMap();
                iterator.append(QMapIterator<QString, QVariant> (object));
                break;
            }
        }

        iterator.pop_back();
    }

    return isobject;
}
