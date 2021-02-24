#include "wrapper.hpp"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "PIg/src/drivers/keyboard.h"
#include "PIg/src/drivers/ks0108.h"

#include <QTextCodec>

static uint16_t uLedTimeOn = LCD_TIME_LED_ON;
static eLCD_LED eLed = LED_OFF;
static QString text;



void vKEYmain(void) {
}

void vKEYset(eGB_TYPE_DEVICE type) {
}

uint8_t timePressKey() {
}

void vLCDclear(void) {

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

}

void vLCDsetLed(eLCD_LED val) {

}

void vLCDled() {

}
