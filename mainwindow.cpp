#include "mainwindow.h"
//#include "ui_mainwindow.h"

#include <QDebug>
#include <QFontMetrics>
#include <QKeyEvent>
#include <QTimer>
#include <QScrollBar>
#include <QTextCodec>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "PIg/src/parameter/param.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("Windows-1251"));

    QHBoxLayout *l = new QHBoxLayout(this);  

    l->addWidget(&glb);
    l->addWidget(&oth);


    glb.crtTreeUser();
    glb.crtTreeState();
    glb.crtTreeInterface();

    oth.crtTreePrm();
    oth.crtTreePrd();
    oth.crtTreeGlb();

    glb.initParam();

    Bsp::initClock();
}

//
MainWindow::~MainWindow() {
//    delete ui;
}

//
void MainWindow::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);

    setMinimumHeight(750);
    resize(sizeHint());
}

//
uint8_t MainWindow::calcCrc(QVector<uint8_t> &pkg) {
    uint8_t crc = 0;

    for(uint8_t byte: pkg) {
        crc += byte;
    }

    return crc;
}

//
eGB_COM MainWindow::checkPkg(QVector<uint8_t> &pkg) {
    eGB_COM com = GB_COM_NO;

    uint8_t byte;

    byte = pkg.takeFirst();
    if (byte != 0x55) {
        return GB_COM_NO;
    }

    byte = pkg.takeFirst();
    if (byte != 0xAA) {
        return GB_COM_NO;
    }

    byte = pkg.takeLast();
    if (byte != calcCrc(pkg)) {
        return GB_COM_NO;
    }

    com = static_cast<eGB_COM> (pkg.takeFirst());

    byte = pkg.takeFirst();
    if (byte != pkg.size()) {
        return GB_COM_NO;
    }

    return com;
}

//
pkg_t MainWindow::receiveFromBsp() {
    pkg_t pkg;

    if (!Bsp::pkgTx.isEmpty()) {
        pkg = Bsp::pkgTx;
        Bsp::pkgTx.clear();

        eGB_COM com = static_cast<eGB_COM> (pkg.first());
        if (Bsp::viewCom.contains(com)) {

            qDebug() << showbase << hex <<
                QString("receiveFromBsp: ") << pkg;
        }

        pkg.insert(1, static_cast<uint8_t> (pkg.size() - 1));
        pkg.append(calcCrc(pkg));
        pkg.insert(0, 0xAA);
        pkg.insert(0, 0x55);
    }

    return pkg;
}

//
void MainWindow::sendToBsp(pkg_t pkg) {
    eGB_COM com = checkPkg(pkg);

    if (Bsp::viewCom.contains(com)) {
        qDebug() << showbase << hex <<
            QString("sendToBsp command ") << com <<
            QString(" with data: ") << pkg;
    }

    Bsp::pkgTx.clear();
    if (com != GB_COM_NO) {
        Bsp::procCommand(com, pkg);
    } else {
        qWarning() << "Message check error: " << showbase << hex << pkg;
    }
}
