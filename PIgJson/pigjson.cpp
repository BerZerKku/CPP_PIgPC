#include "pigjson.h"
#include "ui_pigjson.h"
#include "QTextCodec"
#include "QVariantMap"
#include "qwintexttounicode.h"
#include "src/parameter/param.h"

PIgJson::PIgJson(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::PIgJson)
{
    ui->setupUi(this);

//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));
    setWindowTitle(QWinTextToUnicode("Конфигуратор параметров"));

    QString apppath = QCoreApplication::applicationDirPath();

//    QVariantMap object;
//    while(getNextObject(object)) {
//        qDebug() << "  size = " << object.size() << ":" << object;
//    }

    setup();
}

PIgJson::~PIgJson()
{
    delete ui;
}

TTab* PIgJson::addTab(QVariantMap &obj)
{
    TTab *tab = new TTab(ui->tab);



//    if (!tab->set(content)) {
//        qDebug() << "Tab does not created. Content: " << content;

//        delete tab;
//        tab = nullptr;
//    }

    return tab;
}

void PIgJson::fillDeviceWidget()
{
//    Q_ASSERT(GB_PARAM_NULL_PARAM == 0);
//    Q_ASSERT(strlen(getNameOfParam(GB_PARAM_NULL_PARAM)) == 0);

//    for(size_t i = 1; i < GB_PARAM_MAX; i++) {
//        eGB_PARAM paramindex = static_cast<eGB_PARAM> (i);
////        ui->tree->topLevelItem(0)->addChild(addTreeParameter(paramindex));
    //    }
}



bool PIgJson::setup()
{
    config.setFileName("/config/config.json");
    config.read();

    QMapIterator<QString, QVariant> i(config.config);

    QString type;
    getNextObject(config.config, type);

    return true;

}

bool PIgJson::configTab()
{
//    static QMapIterator<QString, QVariant> i(config);

    return false;
}

bool PIgJson::getNextObject(QVariantMap &obj, QString currtype) const
{
    QMapIterator<QString, QVariant> i(obj);

    while (i.hasNext()) {
        auto next = i.next();

        if (next->type() == QVariant::Map) {
            auto obj = next->toMap();

            if (obj.count("name") == 1) {
                qDebug() << "Map name: " << obj.value("name").toString();
            }

            getNextObject(obj, currtype);
        }

        qDebug() << "next";
    }

    return false;
}

