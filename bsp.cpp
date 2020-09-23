#include "bsp.h"
#include "QTextCodec"
#include <QTimer>
#include <QHeaderView>

Bsp::Bsp(QWidget *parent) : QTreeWidget(parent)
{
    codec = QTextCodec::codecForName("CP1251");

    initParam();
    initClock();

    QTimer *timerClock = new QTimer(this);
    connect(timerClock, &QTimer::timeout, this, &Bsp::updateClock);
    timerClock->start(1000);

    QTreeWidgetItem* top = new QTreeWidgetItem();
    insertTopLevelItem(0, top);
    top->setText(0, "Parameters");

    crtLineEdit(GB_PARAM_IS_PWD_ENGINEER);
    crtLineEdit(GB_PARAM_IS_PWD_ADMIN);

    header()->setFixedWidth(500);
}


//
quint8 Bsp::bcd2int(quint8 bcd, bool &ok) const
{
    quint8 value = 0;

    ok = ((bcd & 0x0F) < 0x0A) && ((bcd & 0xF0) < 0xA0);
    if (!ok) {
        qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << " Error: " << bcd;
    }

    value = bcd & 0x0F;
    value += ((bcd >> 4) & 0x0F) * 10;

    return value;
}

//
uint8_t Bsp::calcCrc(QVector<uint8_t> &pkg)
{
    uint8_t crc = 0;

    for(uint8_t byte: pkg) {
        crc += byte;
    }

    return crc;
}

//
eGB_COM Bsp::checkPkg(QVector<uint8_t> &pkg)
{
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
void Bsp::initClock()
{
    QDate date;
    QTime time;

    date.setDate(params.DateTime.getYear()+2000,
                 params.DateTime.getMonth(),
                 params.DateTime.getDay());

    time.setHMS(params.DateTime.getHour(),
                params.DateTime.getMinute(),
                params.DateTime.getSecond());


    dt.setDate(date);
    dt.setTime(time);
}

//
void Bsp::initParam()
{
    params.security.pwdAdmin.set((uint8_t *) "12345678");
    params.security.pwdEngineer.set((uint8_t *)"87654321");

    params.Uart.Interface.set(TInterface::USB);
    params.Uart.Protocol.set(TProtocol::STANDART);
    params.Uart.BaudRate.set(TBaudRate::_19200);
    params.Uart.DataBits.set(TDataBits::_8);
    params.Uart.Parity.set(TParity::EVEN);
    params.Uart.StopBits.set(TStopBits::ONE);

    // Параметры получаемые из БСП

    params.glb.setNetAddress(15);

    params.DateTime.setYear(20);
    params.DateTime.setMonth(9);
    params.DateTime.setDay(9);
    params.DateTime.setHour(22);
    params.DateTime.setMinute(6);
    params.DateTime.setSecond(37);
    params.DateTime.setMsSecond(987);

    params.def.status.setEnable(false);
    params.prm.setNumCom(32);
    params.prd.setNumCom(32);
    params.glb.setNumDevices(GB_NUM_DEVICES_2);
    params.glb.setDeviceNum(2);
    params.glb.setTypeLine(GB_TYPE_LINE_OPTO);
    params.glb.setVersProgIC16(0x1112, GB_IC_BSP_MCU);
    params.glb.setVersProgIC16(0x2122, GB_IC_BSP_DSP);
    params.glb.setVersProgIC16(0x3132, GB_IC_PI_MCU);
    params.glb.setVersProgIC16(0x4142, GB_IC_BSP_DSP_PLIS);
    params.glb.setVersProgIC8(0x51, GB_IC_BSK_PLIS_PRD1);
    params.glb.setVersProgIC8(0x61, GB_IC_BSK_PLIS_PRD2);
    params.glb.setVersProgIC8(0x71, GB_IC_BSK_PLIS_PRM1);
    params.glb.setVersProgIC8(0x81, GB_IC_BSK_PLIS_PRM2);
    params.glb.setVersProgIC8(0x91, GB_IC_BSZ_PLIS);
    params.glb.setCompatibility(GB_COMPATIBILITY_AVANT);
    params.glb.setCompK400(GB_COMP_K400_AVANT);

    // TypeDevice + TypeOpto = TypeDevice(AVANT_OPTO)
    params.glb.setTypeDevice(AVANT_K400);
    params.glb.setTypeOpto(TYPE_OPTO_STANDART);

    params.def.status.setEnable(false);
    params.def.status.setRegime(GB_REGIME_ENABLED);
    params.def.status.setState(1);
    params.def.status.setDopByte(0);
    params.def.status.setFault(0);
    params.def.status.setWarning(0);
    params.def.status.setRemoteNumber(1);

    params.prm.status.setRegime(GB_REGIME_ENABLED);
    params.prm.status.setState(1);
    params.prm.status.setDopByte(0);
    params.prm.status.setFault(0);
    params.prm.status.setWarning(0);

    params.prd.status.setRegime(GB_REGIME_ENABLED);
    params.prd.status.setState(1);
    params.prd.status.setDopByte(0);
    params.prd.status.setFault(0);
    params.prd.status.setWarning(0);

    params.glb.status.setFault(0);
    params.glb.status.setWarning(0);

    params.glb.setDInputState(0);
    params.prm.setIndCom8(0, 0);
    params.prm.setIndCom8(1, 0);
    params.prm.setIndCom8(2, 0);
    params.prm.setIndCom8(3, 0);
    params.prd.setIndCom8(0, 0);
    params.prd.setIndCom8(1, 0);
    params.prd.setIndCom8(2, 0);
    params.prd.setIndCom8(3, 0);
}

//
quint8 Bsp::int2bcd(quint8 val, bool &ok) const
{
    quint8 bcd = 0;

    ok = (val < 100);
    if (!ok) {
        qWarning("Can't convert value 0x%.2X to BCD code!", val);
    }

    bcd = (val % 10);
    bcd += (val / 10) << 4;

    return bcd;
}

QTreeWidgetItem *Bsp::crtLineEdit(eGB_PARAM param)
{
    QLineEdit *lineedit = new QLineEdit();
    QTreeWidgetItem *item = new QTreeWidgetItem();


    switch(param) {
        case eGB_PARAM::GB_PARAM_IS_PWD_ADMIN: {
            topLevelItem(0)->addChild(item);
            item->setText(0, codec->toUnicode("Пароль администратора"));
        } break;

        case eGB_PARAM::GB_PARAM_IS_PWD_ENGINEER: {
            topLevelItem(0)->addChild(item);
            item->setText(0, codec->toUnicode("Пароль инженера"));
            lineedit->setText("1243121231");
        } break;
    }

    setItemWidget(item, 1, lineedit);
}

//
void Bsp::procCommand(eGB_COM com, pkg_t &data)
{
    switch(com & GB_COM_MASK_GROUP) {
        case GB_COM_MASK_GROUP_READ_PARAM: {
            procCommandReadParam(com, data);
        } break;
        case GB_COM_MASK_GROUP_WRITE_REGIME: {
            procCommandWriteRegime(com, data);
        } break;
        case GB_COM_MASK_GROUP_WRITE_PARAM: {
            procCommandWriteParam(com, data);
        } break;
        case GB_COM_MASK_GROUP_READ_JOURNAL: {
            procCommandReadJournal(com, data);
        }break;
    }
}

//
void Bsp::procCommandReadJournal(eGB_COM com, pkg_t &data)
{
    switch(com) {
        case GB_COM_DEF_GET_JRN_CNT: {
            if (!data.isEmpty()) {
                emit debug(msgSizeError.arg(data.size()));
            }
            // TODO Обработчик для команды чтения количества записей журнала.
            pkgTx.append(com);
            pkgTx.append(0);
            pkgTx.append(0);
        } break;

        case GB_COM_PRM_GET_JRN_CNT: {
            if (!data.isEmpty()) {
                emit debug(msgSizeError.arg(data.size()));
            }
            // TODO Обработчик для команды чтения количества записей журнала.
            pkgTx.append(com);
            pkgTx.append(0);
            pkgTx.append(0);
        } break;

        case GB_COM_PRD_GET_JRN_CNT: {
            if (!data.isEmpty()) {
                emit debug(msgSizeError.arg(data.size()));
            }
            // TODO Обработчик для команды чтения количества записей журнала.
            pkgTx.append(com);
            pkgTx.append(0);
            pkgTx.append(0);
        } break;

        case GB_COM_GET_JRN_CNT : {
            if (!data.isEmpty()) {
                emit debug(msgSizeError.arg(data.size()));
            }
            // TODO Обработчик для команды чтения количества записей журнала.
            pkgTx.append(com);
            pkgTx.append(0);
            pkgTx.append(0);
        } break;

        default: {
            qWarning("No command handler: 0x%.2X", com);
        }
    }
}

//
void Bsp::procCommandReadParam(eGB_COM com, pkg_t &data)
{
    switch(com) {
        case GB_COM_NO: {
        } break;

        case GB_COM_DEF_GET_LINE_TYPE: {
            if (!data.isEmpty()) {
                emit debug(msgSizeError.arg(data.size()));
            }
            //
            pkgTx.append(com);
            pkgTx.append(params.glb.getNumDevices());
        } break;

        case GB_COM_GET_SOST: {
            if (data.size() != 1) {
                emit debug(msgSizeError.arg(data.size()));
            }
            params.measParam.setTemperature(data.first());

            pkgTx.append(com);
            pkgTx.append(params.def.status.getRegime());
            pkgTx.append(params.def.status.getState());
            pkgTx.append(params.def.status.getDopByte());
            pkgTx.append(params.prm.status.getRegime());
            pkgTx.append(params.prm.status.getState());
            pkgTx.append(params.prm.status.getDopByte());
            pkgTx.append(params.prd.status.getRegime());
            pkgTx.append(params.prd.status.getState());
            pkgTx.append(params.prd.status.getDopByte());
            // TODO Разобраться зачем нужен второй приемник
            pkgTx.append(params.prm.status.getRegime());
            pkgTx.append(params.prm.status.getState());
            pkgTx.append(params.prm.status.getDopByte());
            pkgTx.append(params.prm.getIndCom8(0));
            pkgTx.append(params.prm.getIndCom8(1));
            pkgTx.append(params.prm.getIndCom8(2));
            pkgTx.append(params.prm.getIndCom8(3));
            pkgTx.append(params.prd.getIndCom8(0));
            pkgTx.append(params.prd.getIndCom8(1));
            pkgTx.append(params.prd.getIndCom8(2));
            pkgTx.append(params.prd.getIndCom8(3));

            uint8_t di = 0;
            di |= params.glb.getDInputPuskPrm() ? 0x01 : 0x00;
            di |= params.glb.getDInputResetInd() ? 0x02 : 0x00;
            di |= params.glb.getDInputTmControl() ? 0x04 : 0x00;
            pkgTx.append(di);
        } break;

        case GB_COM_GET_FAULT: {
            uint16_t value = 0;
            if (!data.isEmpty()) {
                emit debug(msgSizeError.arg(data.size()));
            }

            pkgTx.append(com);
            value = params.def.status.getFaults();
            pkgTx.append(value >> 8);
            pkgTx.append(value);
            value = params.def.status.getWarnings();
            pkgTx.append(value >> 8);
            pkgTx.append(value);
            value = params.prm.status.getFaults();
            pkgTx.append(value >> 8);
            pkgTx.append(value);
            value = params.prm.status.getWarnings();
            pkgTx.append(value >> 8);
            pkgTx.append(value);
            value = params.prd.status.getFaults();
            pkgTx.append(value >> 8);
            pkgTx.append(value);
            value = params.prd.status.getWarnings();
            pkgTx.append(value >> 8);
            pkgTx.append(value);
            value = params.glb.status.getFaults();
            pkgTx.append(value >> 8);
            pkgTx.append(value);
            value = params.glb.status.getWarnings();
            pkgTx.append(value >> 8);
            pkgTx.append(value);

            // TODO Разобраться зачем нужен второй приемник
            value = params.prm.status.getFaults();
            pkgTx.append(value >> 8);
            pkgTx.append(value);
            value = params.prm.status.getWarnings();
            pkgTx.append(value >> 8);
            pkgTx.append(value);

            // TODO Добавить байты данных для РЗСК
        } break;

        case GB_COM_GET_TIME: {
            // TODO Добавить обработку флага новой записи журнала
            if (data.size() != 1) {
                emit debug(msgSizeError.arg(data.size()));
            }

            bool ok;
            pkgTx.append(com);
            pkgTx.append(int2bcd(params.DateTime.getYear(), ok));
            pkgTx.append(int2bcd(params.DateTime.getMonth(), ok));
            pkgTx.append(int2bcd(params.DateTime.getDay(), ok));
            pkgTx.append(int2bcd(params.DateTime.getHour(), ok));
            pkgTx.append(int2bcd(params.DateTime.getMinute(), ok));
            pkgTx.append(int2bcd(params.DateTime.getSecond(), ok));

            uint16_t value = params.DateTime.getMsSecond();
            pkgTx.append(value);
            pkgTx.append(value >> 8);

            // TODO Добавить считывание записи журнала для АСУ ТП.
        } break;

        case GB_COM_GET_NET_ADR: {
            if (!data.isEmpty()) {
                emit debug(msgSizeError.arg(data.size()));
            }
            //
            pkgTx.append(com);
            pkgTx.append(params.glb.getNetAddress());
            pkgTx.append(params.Uart.Interface.get());
            pkgTx.append(params.Uart.Protocol.get());
            pkgTx.append(params.Uart.BaudRate.get());
            pkgTx.append(params.Uart.DataBits.get());
            pkgTx.append(params.Uart.Parity.get());
            pkgTx.append(params.Uart.StopBits.get());
            uint8_t* val = params.security.pwdEngineer.get();
            for(uint8_t i = 0; i < PWD_LEN; i++) {
                pkgTx.append(*val++);
            }
            val = params.security.pwdAdmin.get();
            for(uint8_t i = 0; i < PWD_LEN; i++) {
                pkgTx.append(*val++);
            }
        } break;

        case GB_COM_GET_DEVICE_NUM : {
            if (!data.isEmpty()) {
                emit debug(msgSizeError.arg(data.size()));
            }
            //
            pkgTx.append(com);
            pkgTx.append(params.glb.getDeviceNum());
        } break;

        case GB_COM_GET_VERS: {
            uint16_t vers = 0;

            pkgTx.append(com);
            pkgTx.append(params.def.status.isEnable() ? 1 : 0);
            pkgTx.append(params.prm.getNumCom() / 4); // прм1
            pkgTx.append(params.prm.getNumCom() / 4); // прм2
            pkgTx.append(params.prd.getNumCom() / 4);
            pkgTx.append(params.glb.getNumDevices());
            pkgTx.append(params.glb.getTypeLine());

            vers = params.glb.getVersProgIC(GB_IC_BSP_MCU);
            pkgTx.append(vers >> 8);
            pkgTx.append(vers);
            vers = params.glb.getVersProgIC(GB_IC_BSP_DSP);
            pkgTx.append(vers >> 8);
            pkgTx.append(vers);

            pkgTx.append(params.glb.getCompatibility());

            vers = params.glb.getVersProgIC(GB_IC_BSK_PLIS_PRD1);
            pkgTx.append((vers >> 4) + vers);
            vers = params.glb.getVersProgIC(GB_IC_BSK_PLIS_PRD2);
            pkgTx.append((vers >> 4) + vers);
            vers = params.glb.getVersProgIC(GB_IC_BSK_PLIS_PRM1);
            pkgTx.append((vers >> 4) + vers);
            vers = params.glb.getVersProgIC(GB_IC_BSK_PLIS_PRM2);
            pkgTx.append((vers >> 4) + vers);
            vers = params.glb.getVersProgIC(GB_IC_BSZ_PLIS);
            pkgTx.append((vers >> 4) + vers);

            pkgTx.append(params.glb.getTypeDevice());

            vers = params.glb.getVersProgIC(GB_IC_PI_MCU);
            pkgTx.append(vers >> 8);
            pkgTx.append(vers);

            pkgTx.append(params.glb.getTypeOpto());

            vers = params.glb.getVersProgIC(GB_IC_BSP_DSP);
            pkgTx.append(vers >> 8);
            pkgTx.append(vers);
        } break;

        default: {
            qWarning("No command handler: 0x%.2X", com);
        }
    }
}

//
void Bsp::procCommandWriteParam(eGB_COM com, pkg_t &data)
{
    switch(com) {
        case GB_COM_SET_TIME: {
            if (data.size() != 9) {
                emit debug(msgSizeError.arg(data.size()));
            } else {
                bool ok = false;
                params.DateTime.setYear(bcd2int(data.takeFirst(), ok));
                params.DateTime.setMonth(bcd2int(data.takeFirst(), ok));
                params.DateTime.setDay(bcd2int(data.takeFirst(), ok));
                params.DateTime.setHour(bcd2int(data.takeFirst(), ok));
                params.DateTime.setMinute(bcd2int(data.takeFirst(), ok));
                params.DateTime.setSecond(bcd2int(data.takeFirst(), ok));

                quint16 ms = data.takeFirst();
                ms += data.takeFirst() << 8;
                params.DateTime.setMsSecond(ms);

                if (data.first() != 0) {
                    emit debug(msgTimeSourceError.arg(data.first()));
                }
                data.removeFirst();
                initClock();
            }
        } break;

        case GB_COM_SET_NET_ADR: {
            qDebug() << "Com GB_COM_SET_NET_ADR package = " <<
                        showbase << hex << data;

            uint8_t dop = data.takeFirst();
            switch(dop) {
                case 1: {
                    uint8_t byte = data.takeFirst();
                    params.glb.setNetAddress(byte);
                } break;
                case 2: {
                    uint8_t byte = data.takeFirst();
                    params.Uart.Interface.set((TInterface::INTERFACE) byte);
                } break;
                case 3: {
                    uint8_t byte = data.takeFirst();
                    params.Uart.Protocol.set((TProtocol::PROTOCOL) byte);
                } break;
                case 4: {
                    uint8_t byte = data.takeFirst();
                    params.Uart.BaudRate.set((TBaudRate::BAUD_RATE) byte);
                } break;
                case 5: {
                    uint8_t byte = data.takeFirst();
                    params.Uart.DataBits.set((TDataBits::DATA_BITS) byte);
                } break;
                case 6: {
                    uint8_t byte = data.takeFirst();
                    params.Uart.Parity.set((TParity::PARITY) byte);
                } break;
                case 7: {
                    uint8_t byte = data.takeFirst();
                    params.Uart.StopBits.set((TStopBits::STOP_BITS) byte);
                } break;                
                case 8: {
                    if (data.size() != 8) {
                        emit debug(msgSizeError.arg(data.size()));
                    } else {
                        uint8_t value[PWD_LEN];
                        for(uint8_t i = 0; i < PWD_LEN; i++) {
                            value[i] = data.takeFirst();
                        }
                        params.security.pwdAdmin.set(value);
                    }
                } break;
                case 9: {
                    if (data.size() != 8) {
                        emit debug(msgSizeError.arg(data.size()));
                    } else {
                        uint8_t value[PWD_LEN];
                        for(uint8_t i = 0; i < PWD_LEN; i++) {
                            value[i] = data.takeFirst();
                        }
                        params.security.pwdEngineer.set(value);
                    }
                } break;
                default: qDebug("No dop byte handler: 0x%.2X", dop);
            }
        } break;

        default: {
            qWarning("No command handler: 0x%.2X", com);
        }
    }
}

//
void Bsp::procCommandWriteRegime(eGB_COM com, pkg_t &data)
{
    switch(com) {
        case GB_COM_SET_REG_DISABLED: {
            if (!data.isEmpty()) {
                emit debug(msgSizeError.arg(data.size()));
            }

            params.def.status.setRegime(GB_REGIME_DISABLED);
            params.prd.status.setRegime(GB_REGIME_DISABLED);
            params.prm.status.setRegime(GB_REGIME_DISABLED);

            pkgTx.append(com);
        } break;
        case GB_COM_SET_REG_ENABLED: {
            if (!data.isEmpty()) {
                emit debug(msgSizeError.arg(data.size()));
            }

            params.def.status.setRegime(GB_REGIME_ENABLED);
            params.prd.status.setRegime(GB_REGIME_ENABLED);
            params.prm.status.setRegime(GB_REGIME_ENABLED);

            pkgTx.append(com);
        } break;

        default: {
            qWarning("No command handler: 0x%.2X", com);
        }
    }
}

//
pkg_t Bsp::receiveFromBsp()
{
    pkg_t pkg;

    if (!pkgTx.isEmpty()) {
        pkg = pkgTx;
        pkgTx.clear();

        pkg.insert(1, pkg.size() - 1);
        pkg.append(calcCrc(pkg));
        pkg.insert(0, 0xAA);
        pkg.insert(0, 0x55);

//        qDebug() << "receiveFromBsp: " << showbase << hex << pkg;
    }

    return pkg;
}

//
void Bsp::sendToBsp(pkg_t pkg)
{
    eGB_COM com = checkPkg(pkg);

    pkgTx.clear();
    if (com != GB_COM_NO) {
        procCommand(com, pkg);

        if (com == GB_COM_SET_NET_ADR) {
            qDebug() << QString("sendToBsp: ") << showbase << hex << pkg;
        }
    } else {
        qWarning() << "Message check error: " << showbase << hex << pkg;
    }
}

//
void Bsp::updateClock()
{
    dt = dt.addSecs(1);

    params.DateTime.setYear(dt.date().year()-2000);
    params.DateTime.setMonth(dt.date().month());
    params.DateTime.setDay(dt.date().day());

    params.DateTime.setHour(dt.time().hour());
    params.DateTime.setMinute(dt.time().minute());
    params.DateTime.setSecond(dt.time().second());
}
