#ifndef TCONFIGJSON_H
#define TCONFIGJSON_H

#include <QObject>
#include <QFile>
#include <QVariantMap>

class TConfigJson : public QObject
{
    Q_OBJECT

    QStringList typeList = {
        "property_group",
        "tab"
    };

   public:

    typedef struct {
        QString name;
        QString type;
        QString description;
        QVariantMap value;
    } object_t;

    explicit TConfigJson(QObject *parent = nullptr);

    void setFileName(QString filename);

    bool read();
    bool save();

    bool getObjectName(QVariantMap &obj, QString &value) const;
    bool getObjectType(QVariantMap &obj, QString &value) const;
    bool getObjectDesc(QVariantMap &obj, QString &value) const;
    bool getObjectData(QVariantMap &obj, QVariantMap &value) const;
    bool getNextTab(QVariantMap &object) const;

    bool getNextObject(QVariantMap &object) const;

    bool getObjectField(QVariantMap &object, QString fieldname, bool exist) const;

    QVariantMap config;

   signals:

   private:
    QFile file;



};

#endif // TCONFIGJSON_H
