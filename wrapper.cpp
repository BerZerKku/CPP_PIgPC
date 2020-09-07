#include "wrapper.hpp"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QTextCodec>

//#include "PIg/src/drivers/keyboard.h"
//#include "PIg/src/drivers/ks0108.h"

extern MainWindow *w;

void vKEYmain(void) {

}

void vKEYset(eGB_TYPE_DEVICE type) {

}

eKEY eKEYget(void) {

}

uint8_t timePressKey() {

}

void vLCDclear(void) {
    qDebug() << __FUNCTION__;
    w->ui->textEdit->clear();
}

void vLCDinit(void) {

}

void vLCDmain(void) {

}

void vLCDrefresh(void) {

}

bool vLCDdrawBoard(uint8_t num) {

}

bool vLCDputchar(const char* buf, uint8_t num) {
    QString text;
    QTextCodec *codec = QTextCodec::codecForName("CP1251");

    for(quint8 row = 0; row < 7; row++) {
        for(quint16 col = 0; col < 20; col++) {
            char symbol = (row == num) ? '-' : *buf++;
            if (symbol == '\0') {
                symbol = ' ';
            }
            text += codec->toUnicode(&symbol, 1);
        }
        text += "\n";
    }

    w->ui->textEdit->setText(text);

    return true;
}

void vLCDsetLed(eLCD_LED val) {

}

void vLCDled() {

}
