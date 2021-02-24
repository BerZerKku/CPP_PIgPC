#ifndef PIGJSON_H
#define PIGJSON_H

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QVariantMap>
#include "flashParams.h"
#include "configJson.h"
#include "tab.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PIgJson; }
QT_END_NAMESPACE

class PIgJson : public QMainWindow
{
    Q_OBJECT

   public:
    PIgJson(QWidget *parent = nullptr);
    ~PIgJson();

   private:
    Ui::PIgJson *ui;

   private:

    TConfigJson config;

    TTab* addTab(QVariantMap &obj);
    void fillDeviceWidget();

    bool setup();
    bool configTab();
    bool getNextObject(QVariantMap &obj, QString currtype) const;

};
#endif // PIGJSON_H
