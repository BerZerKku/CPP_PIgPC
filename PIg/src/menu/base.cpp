#include "base.hpp"
#include "src/avr.h"
#include "src/drivers/ks0108.h"
#include "src/protocols/standart/protocolBspS.h"
#include "src/protocols/standart/protocolPcS.h"
#include "src/protocols/modbus/protocolPcM.h"
#include "src/protocols/iec101/protocolPcI.h"

/// Время работы одного цикла (зависит от настройки таймеров), мс
#define TIME_CYLCE 100
/// Максимальное кол-во неполученных сообщений от БСП для ошибки связи
#define MAX_LOST_COM_FROM_BSP 10
/// Максимальное время отсуствия сообщений для определения обрыва связи с ПК
#define TIME_LOST_PC_CONNECTION 3
/// Количество пропавших сообщений подряд для определения обрыва связи с ПК
#define MAX_LOST_COM_FROM_PC (TIME_LOST_PC_CONNECTION * (1000 / TIME_CYLCE))
/// адрес датчика температуры
#define TEMP_IC_ADR 0x48

/// Последняя команда полученная с ПК
static uint8_t lastPcCom = 0;
/// Буфер для связи с ПК по последовательному порту
uint8_t uBufUartPc[BUFF_SIZE_PC];
/// Буфер для свящи с БСП по последовательному порту
uint8_t uBufUartBsp[BUFF_SIZE_BSP];

/// Класс меню
clMenu menu;
/// Класс стандартного протокола работающего с ПК
static clProtocolPcS protPCs(uBufUartPc, BUFF_SIZE_PC, &menu.sParam);
/// Класс работы протокола MODBUS
static TProtocolPcM protPCm(&menu.sParam, uBufUartPc, BUFF_SIZE_PC);
/// Класс протокола МЭК 101 работающего с ПК
static TProtocolPcI protPCi(&menu.sParam, uBufUartPc, BUFF_SIZE_PC);
/// Класс стандартного протокола работающего с БСП
static clProtocolBspS protBSPs(uBufUartBsp, BUFF_SIZE_BSP, &menu.sParam);

static void checkInterface();
static void checkNetAddress();
static void setProtocol(TProtocol::PROTOCOL protocol, uint16_t baud);

void bspRead() {
    // кол-во неполученных сообщений с БСП
    static uint8_t cntLostCom = MAX_LOST_COM_FROM_BSP;

    if (cntLostCom < MAX_LOST_COM_FROM_BSP) {
        cntLostCom++;
    }

    // перед приемом проверим статус на залипание
    protBSPs.checkStat();
    // Проверка наличия сообщения с БСП и ее обработка
    if (protBSPs.getCurrentStatus() == PRTS_STATUS_READ_OK) {
        // проверка контрольной суммы полученного сообщения и
        // обработка данных если она соответствует полученной
        if (protBSPs.checkReadData()) {
            // обработка принятого сообщения
            protBSPs.getData(lastPcCom == protBSPs.getCurrentCom());

            // проверка соответствия команды запрошенной с ПК и команды
            // полученной от БСП и если совпадают пересылка сообщения на ПК
            // для команды GB_COM_GET_VERS происходит добавление версии БСП-ПИ
            if (lastPcCom == protBSPs.getCurrentCom()) {
                if (protPCs.isEnable()) {
                    if (protPCs.copyCommandFrom(protBSPs.buf))
                        protPCs.modifyCommand();
                }
            }
        }
        lastPcCom = 0;
        cntLostCom = 0;
    }

    menu.sParam.connectionBsp = (cntLostCom < MAX_LOST_COM_FROM_BSP);
}

void pcRead() {
    // кол-во неполученных сообщений с БСП
    static uint8_t cntLostCom = MAX_LOST_COM_FROM_PC;

    if (cntLostCom < MAX_LOST_COM_FROM_PC) {
        cntLostCom++;
    }

    if (protPCs.isEnable()) {
        // перед приемом проверим статус на залипание
        protPCs.checkStat();
        // проверка наличия команды с ПК и ее обработка
        if (protPCs.getCurrentStatus() == PRTS_STATUS_READ_OK) {
            // проверка контрольной суммы полученного сообщения и
            // обработка данных если она соответствует полученной
            if (protPCs.checkReadData()) {
                // обработка принятого сообщения
                // если сообщение небыло обработано, перешлем его в БСП
                // (т.е. если это не запрос/изменение пароля)
                if (!protPCs.getData()) {
                    // сохранение запрашиваемой ПК команды
                    lastPcCom = protPCs.getCurrentCom();

                    // пересылка сообщения в БСП
                    if (protBSPs.getCurrentStatus() == PRTS_STATUS_NO) {
                        protBSPs.copyCommandFrom(protPCs.buf);
                        protPCs.setCurrentStatus(PRTS_STATUS_WAIT_ANSWER);
                    }
                }
            }
            cntLostCom = 0;
        }
    } else if (protPCm.isEnable()) {
        if (protPCm.isReadData()) {
            protPCm.readData();
        }
    } else if (protPCi.isEnable()) {
        if (protPCi.isReadData()) {
            protPCi.readData();
        }
    }

    menu.sParam.connectionPc = (cntLostCom < MAX_LOST_COM_FROM_PC);
}

uint8_t pcWrite() {
    uint8_t num = 0;
    if (protPCs.isEnable()) {
        // Перед передачей проверим статус протокола на залипание.
        protPCs.checkStat();
        // проверка необходимости передачи команды на ПК и ее отправка
        ePRTS_STATUS stat = protPCs.getCurrentStatus();
        if (stat == PRTS_STATUS_WRITE_PC) {
            // пересылка ответа БСП
            num = protPCs.trCom();
        } else if (stat == PRTS_STATUS_WRITE) {
            // отправка ответа ПИ
            num = protPCs.trCom();
        }
    } else if (protPCm.isEnable()) {
        num = protPCm.sendData();
    } else if (protPCi.isEnable()) {
        num =protPCi.sendData();
    }

    return num;
}

uint8_t bspWrite()
{
    uint8_t num = 0;

    // Перед передачей проверим статус протокола на залипание.
    protBSPs.checkStat();
    // проверим нет ли необходимости передачи команды с ПК
    // если нет, то возьмем команду с МЕНЮ
    ePRTS_STATUS stat = protBSPs.getCurrentStatus();
    if (stat == PRTS_STATUS_WRITE_PC) {
        // пересылка запроса ПК
        num = protBSPs.trCom();
    } else if (stat == PRTS_STATUS_NO) {
        // отправка запроса БСП
        eGB_COM com = menu.getTxCommand();
        // если есть команда, отправляем ее в БСП
        if (com != GB_COM_NO) {
            num = protBSPs.sendData(com);

//            if ((com == GB_COM_SET_NET_ADR) || (com == GB_COM_PRM_SET_BLOCK_ALL)) {
//                QString msg = QString("Com 0x%1:").arg(com, 2, 16, QLatin1Char('0'));
//                for(uint8_t i = 0; i < num; i++) {
//                    msg += QString(" 0x%1").arg(uBufUartBsp[i], 2, 16, QLatin1Char('0'));
//                }
//                qDebug() << msg;
//            }
        }
    }

    return num;
}

void checkInterface() {
    bool changed = false;
    TInterface::INTERFACE intf = menu.sParam.Uart.Interface.get();

    if (menu.sParam.Uart.Interface.isChanged()) {
        changed = true;
    }

//    if (intf != TInterface::USB) {
//        changed |= menu.sParam.Uart.Protocol.isChanged();
//        changed |= menu.sParam.Uart.BaudRate.isChanged();
//        changed |= menu.sParam.Uart.DataBits.isChanged();
//        changed |= menu.sParam.Uart.Parity.isChanged();
//        changed |= menu.sParam.Uart.StopBits.isChanged();
//    }

    if (changed) {
//        if (intf == TInterface::USB) {
            setupUart(intf, 19200, TDataBits::_8, TParity::NONE, TStopBits::TWO);
            setProtocol(TProtocol::STANDART, 19200);
//        } else {
//            TProtocol::PROTOCOL protocol = menu.sParam.Uart.Protocol.get();
//            uint16_t baudrate = menu.sParam.Uart.BaudRate.getValue();
//            TDataBits::DATA_BITS dbits = menu.sParam.Uart.DataBits.get();
//            TParity::PARITY parity = menu.sParam.Uart.Parity.get();
//            TStopBits::STOP_BITS sbits = menu.sParam.Uart.StopBits.get();

//            setupUart(intf, baudrate, dbits, parity, sbits);
//            setProtocol(protocol, baudrate);
//        }
    }
}

void checkNetAddress() {
    if (menu.sParam.Uart.NetAddress.isChanged()) {
        uint8_t nadr = menu.sParam.Uart.NetAddress.get();
        if (protPCm.getAddressLan() != nadr) {
            protPCm.setAddressLan(nadr);
        }
        if (protPCi.getAddressLan() != nadr) {
            protPCi.setAddressLan(nadr);
        }
    }
}

void setProtocol(TProtocol::PROTOCOL protocol, uint16_t baud) {
    protPCs.setDisable();
    protPCi.setDisable();
    protPCm.setDisable();

    switch(protocol) {
        case TProtocol::STANDART:
            protPCs.setEnable(PRTS_STATUS_NO);
            menu.sParam.jrnScada.setIdle();
            break;
        case TProtocol::MODBUS:
            protPCm.setTick(baud, 50);
            protPCm.setEnable();
            menu.sParam.jrnScada.setIdle();
            break;
        case TProtocol::IEC_101:
            protPCi.setTick(baud, 50);
            protPCi.setEnable();
            menu.sParam.jrnScada.setReadyToRead();
            break;
        case TProtocol::MAX: break;
    }
}

void mainInit() {
    protBSPs.setEnable(PRTS_STATUS_NO);

//    setupUart(TInterface::BVP, 19200, TDataBits::_8, TParity::NONE, TStopBits::TWO);
//    setProtocol(TProtocol::STANDART, 19200);

    vLCDinit();
    vLCDclear();
}

void mainCycle(void) {
    static uint8_t cnt_lcd = 0;
    static uint8_t cnt_1s = 0;

    checkInterface();
//    checkNetAddress();

    if (++cnt_1s >= 10) {
        cnt_1s = 0;
    }

    if (++cnt_lcd >= (MENU_TIME_CYLCE / TIME_CYLCE)) {
        cnt_lcd = 0;
        menu.proc();
    }
}

/// Прерывание по окончанию передачи данных UART1
void pcTxEnd() {
    if (protPCs.isEnable()) {
        protPCs.setCurrentStatus(PRTS_STATUS_NO);
    } else if (protPCm.isEnable()) {
        protPCm.setReadState();
    } else if (protPCi.isEnable()) {
        protPCi.setReadState();
    }
}


void pcPushByteFrom(uint8_t byte, bool error) {
    if (error) {
        protPCs.setCurrentStatus(PRTS_STATUS_NO);
    } else {
        if (protPCs.isEnable()) {
            protPCs.checkByte(byte);
        } else if (protPCm.isEnable()) {
            // протокол MODBUS
            protPCm.push(byte);
        } else if (protPCi.isEnable()) {
            protPCi.push(byte);
        }
    }
}

void bspTxEnd() {
    protBSPs.setCurrentStatus(PRTS_STATUS_NO);
}

void bspPushByteFrom(uint8_t byte, bool error) {
    if (error) {
        protBSPs.setCurrentStatus(PRTS_STATUS_NO);
    } else {
        protBSPs.checkByte(byte);
    }
}

void timer50us() {
    if (protPCm.isEnable()) {
        protPCm.tick();
    } else if (protPCi.isEnable()) {
        protPCi.tick();
    }
}


