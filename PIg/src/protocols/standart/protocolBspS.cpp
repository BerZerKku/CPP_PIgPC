/*
 * protocolBspS.cpp
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */
#include "protocolBspS.h"
#include "glbDefine.h"
#include "menu/dateTime.h"
#include "paramBsp.h"

clProtocolBspS::clProtocolBspS(uint8_t* buf, uint8_t size, stGBparam* sParam) :
    clProtocolS(buf, size, sParam)
{
}

/**	Обработка принятого сообщения.
 *
 * 	@param pc True - команда запрошенная с ПК, False - запрошенная с ПИ-БСП.
 * 	@return True - в случае успешной обработки, False - в случае ошибки.
 */
bool clProtocolBspS::getData(bool pc)
{
    bool    stat = false;
    uint8_t mask = 0;
    eGB_COM com  = static_cast<eGB_COM>(m_buf[2]);

    // сообщение обработано, выставим флаг на чтение
    setCurrentStatus(PRTS_STATUS_NO);

    mask = static_cast<uint8_t>(com) & GB_COM_MASK_GROUP;
    // ответ на команду изменения параметра/режима не требуется
    if ((mask == GB_COM_MASK_GROUP_WRITE_PARAM) || (mask == GB_COM_MASK_GROUP_WRITE_REGIME))
    {
        if (com == GB_COM_SET_TIME)
        {
            if (m_buf[NUM] >= 8)
            {
                stat = sParam_->DateTimeReq.setYear(m_buf[B1], true);
                stat &= sParam_->DateTimeReq.setMonth(m_buf[B2], true);
                stat &= sParam_->DateTimeReq.setDay(m_buf[B3], true);
                stat &= sParam_->DateTimeReq.setHour(m_buf[B4], true);
                stat &= sParam_->DateTimeReq.setMinute(m_buf[B5], true);
                stat &= sParam_->DateTimeReq.setSecond(m_buf[B6], true);

                // миллисекунды устанавливаются, только если они есть в посылке
                uint16_t ms = 0;
                if (m_buf[NUM] >= 8)
                {
                    ms = *reinterpret_cast<uint16_t*>(&m_buf[B7]);

                    sParam_->DateTimeReq.setTimeBsp_ = true;
                }
                stat &= sParam_->DateTimeReq.setMsSecond(ms);
            }
            else
            {
                stat = true;
            }
        }
        else
        {
            stat = true;
        }
    }
    else
    {
        mask = static_cast<uint8_t>(com) & GB_COM_MASK_DEVICE;

        if (mask == GB_COM_MASK_DEVICE_DEF)
            stat = getDefCommand(com, pc);  // команды защиты
        else if (mask == GB_COM_MASK_DEVICE_PRM)
            stat = getPrmCommand(com, pc);  // команды приемника
        else if (mask == GB_COM_MASK_DEVICE_PRD)
            stat = getPrdCommand(com, pc);  // команды передатчика
        else
            stat = getGlbCommand(com, pc);  // команды общие

        getLocalParam(com);
    }

    return stat;
}

/**	Формирование посылки и отправка заданной команды.
 *
 * 	По-умолчанию в сообщении опроса параметра посылается только код команды.
 * 	По-умолчанию в сообщениии изменения параметра посылается 1 байт данных.
 *
 * 	@param com Команда на передачу
 * 	@return Кол-во передаваемых байт
 */
uint8_t clProtocolBspS::sendData(eGB_COM com)
{
    uint8_t num  = 0;
    uint8_t mask = 0;

    mask = static_cast<uint8_t>(com) & GB_COM_MASK_GROUP;

    if (mask == GB_COM_MASK_GROUP_WRITE_PARAM)
    {
        // команды изменения параметров

        eGB_SEND_TYPE sendType = sParam_->txComBuf.getSendType();

        if (com == GB_COM_SET_TIME)
        {
            num = addCom(com, 9, sParam_->txComBuf.getBuferAddress());
        }
        else if (com == GB_COM_PRM_RES_IND)
        {
            num = addCom(com);
        }
        else if (com == GB_COM_DEF_SET_TYPE_AC)
        {
            num = addCom(com, sParam_->txComBuf.getInt8());
        }
        else if (sendType != GB_SEND_NO)
        {
            uint8_t val = sParam_->txComBuf.getInt8(0);
            uint8_t dop = sParam_->txComBuf.getInt8(1);

            switch (sendType)
            {
            case GB_SEND_INT8: num = addCom(com, val); break;
            case GB_SEND_BITES_DOP:
            case GB_SEND_INT8_DOP: num = addCom(com, val, dop); break;
            case GB_SEND_DOP_INT8:  // DOWN
            case GB_SEND_DOP_BITES: num = addCom(com, dop, val); break;
            case GB_SEND_INT16_BE: num = addCom(com, val, dop); break;

            case GB_SEND_COR_I: [[fallthrough]];
            case GB_SEND_COR_U: num = addCom(com, 3, sParam_->txComBuf.getBuferAddress());

            case GB_SEND_NO: break;
            }
        }
    }
    else if (mask == GB_COM_MASK_GROUP_WRITE_REGIME)
    {
        // команды изменения Режимов Работы и работы с Тестами
        // по умолчанию отправляется только код команды

        if (com == GB_COM_SET_CONTROL)
        {
            num = addCom(com, sParam_->txComBuf.getInt8());
        }
        else if (com == GB_COM_SET_REG_TEST_1)
        {
            // есть две возможные ситуации:
            // отсылается команда включения Тестов
            // 		при этом передается 0 байт данных
            // отсылаются команды установки сигналов в тесте
            // 		тут последовательно передаются две команды
            // 		сначала команда для КЧ (первый байт 1)
            // 		а при следующем заходе для РЗ (первый байт 2)
            uint8_t t = sParam_->txComBuf.getInt8(0);
            if (t != 0)
            {
                num = addCom(com, t, sParam_->txComBuf.getInt8(1));
            }
            else
            {
                num = addCom(com);  // вкл.
            }
        }
        else
        {
            // GB_COM_PRM_ENTER
            // GB_COM_SET_REG_DISABLED
            // GB_COM_SET_REG_ENABLED
            // GB_COM_SET_REG_TEST_2
            // GB_COM_SET_REG_TEST_1
            num = addCom(com);
        }
    }
    else if (mask == GB_COM_MASK_GROUP_READ_PARAM)
    {
        // команды опроса параметров
        // по умолчанию отправляется только код команды
        if (com == GB_COM_GET_MEAS)
        {
            num = addCom(com, 0);
        }
        else if (com == GB_COM_GET_SOST)
        {
            num = addCom(com, static_cast<uint8_t>(sParam_->measParam.getTemperature()));
        }
        else if (com == GB_COM_GET_TIME)
        {
            num = addCom(com, sParam_->jrnScada.getState());
        }
        else
        {
            num = addCom(com);
        }
    }
    else if (mask == GB_COM_MASK_GROUP_READ_JOURNAL)
    {
        // команды считывания журналов, в том числе и кол-ва записей
        num = sendReadJrnCommand(com);
    }

    // установка статуса, в зависимости от необходимости передачи сообщения
    if (num != 0)
    {
        setCurrentStatus(PRTS_STATUS_WRITE);
    }
    else
    {
        setCurrentStatus(PRTS_STATUS_NO);
    }

    return num;
}

/**	Обработка принятой команды Защиты.
 *
 * 	@param com	Код команды
 * 	@param pc True - команда запрошенная с ПК, False - запрошенная с ПИ-БСП.
 * 	@return True - в случае успешной обработки, False - в случае ошибки.
 */
bool clProtocolBspS::getDefCommand(eGB_COM com, bool pc)
{
    bool stat = false;

    if (com == GB_COM_DEF_GET_LINE_TYPE)
    {
        stat        = sParam_->def.setNumDevices(static_cast<eGB_NUM_DEVICES>(m_buf[B1]));
        uint8_t act = sParam_->glb.setNumDevices(static_cast<eGB_NUM_DEVICES>(m_buf[B1]));
        sParam_->local.setNumDevices(sParam_->glb.getNumDevices() + 1);
        if (act & GB_ACT_NEW)
            sParam_->device = false;
    }
    else if (com == GB_COM_DEF_GET_TYPE_AC)
    {
        // В РЗСК тут передается 1 байт для параметра односторонний режим
        if (m_buf[NUM] >= 5)
        {
            // 1 байт - тип АК
            // 2-5 - время до АК
            stat = sParam_->def.setTypeAC(static_cast<eGB_TYPE_AC>(m_buf[B1]));
            // uint32_t t = (buf[B2]<<24) + (buf[B3]<<16) + (buf[B4]<<8) + buf[B5];
            stat |= sParam_->def.setTimeToAC(*reinterpret_cast<uint32_t*>(&m_buf[B2]));
        }
    }
    else if (com == GB_COM_DEF_GET_JRN_CNT)
    {
        uint16_t t = *reinterpret_cast<uint16_t*>(&m_buf[B1]);
        if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_DEF)
        {
            stat = sParam_->jrnEntry.setNumJrnEntry(t);
        }
    }
    else if (com == GB_COM_DEF_GET_JRN_ENTRY)
    {
        if ((sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_DEF) && (!pc))
        {
            if (sParam_->typeDevice == AVANT_OPTO)
            {
                // дата
                sParam_->jrnEntry.dateTime.setYear(m_buf[B3], true);
                sParam_->jrnEntry.dateTime.setMonth(m_buf[B4], true);
                sParam_->jrnEntry.dateTime.setDay(m_buf[B5], true);
                // время
                sParam_->jrnEntry.dateTime.setHour(m_buf[B6], true);
                sParam_->jrnEntry.dateTime.setMinute(m_buf[B7], true);
                sParam_->jrnEntry.dateTime.setSecond(m_buf[B8], true);
                uint16_t t = TO_UINT16(m_buf[B9], m_buf[B10]);
                sParam_->jrnEntry.dateTime.setMsSecond(t);
                //
                //				sParam_->jrnEntry.setDeviceJrn((eGB_DEVICE_K400)
                // buf[B1]);
                // sParam_->jrnEntry.setNumCom(buf[B2]);
                uint8_t signal_def = static_cast<uint8_t>((m_buf[B1] << 4) + (m_buf[B2] & 0x0F));
                sParam_->jrnEntry.setSignalDef(signal_def);
                //				sParam_->jrnEntry.setEventType(buf[B3]);
                sParam_->jrnEntry.setReady();
                stat = true;
            }
            else
            {
                sParam_->jrnEntry.dateTime.setYear(m_buf[B16], true);
                sParam_->jrnEntry.dateTime.setMonth(m_buf[B15], true);
                sParam_->jrnEntry.dateTime.setDay(m_buf[B14], true);
                // B13 - день недели
                sParam_->jrnEntry.dateTime.setHour(m_buf[B12], true);
                sParam_->jrnEntry.dateTime.setMinute(m_buf[B11], true);
                sParam_->jrnEntry.dateTime.setSecond(m_buf[B10], true);
                uint16_t t = TO_UINT16(m_buf[B9], m_buf[B8]);
                sParam_->jrnEntry.dateTime.setMsSecond(t);
                sParam_->jrnEntry.setDeviceJrn(static_cast<eGB_DEVICE_K400>(m_buf[B1]));
                // sParam_->jrnEntry.setNumCom(buf[B2]);
                uint8_t signal_def = static_cast<uint8_t>((m_buf[B2] << 4) + (m_buf[B4] & 0x0F));
                sParam_->jrnEntry.setSignalDef(signal_def);
                sParam_->jrnEntry.setEventType(m_buf[B3]);
                sParam_->jrnEntry.setReady();
                stat = true;
            }
        }
    }

    return stat;
}

/**	Обработка принятой команды Приемника.
 *
 * 	@param com	Код команды
 * 	@param pc True - команда запрошенная с ПК, False - запрошенная с ПИ-БСП.
 * 	@retval True - в случае успешной обработки
 * 	@retval False - в случае ошибки.
 */
bool clProtocolBspS::getPrmCommand(eGB_COM com, bool pc)
{
    bool stat = false;

    switch (com)
    {

    case GB_COM_PRM_GET_COM:
        {
            uint8_t act = GB_ACT_NO;
            if (sParam_->typeDevice == AVANT_K400)
            {
                act = sParam_->prm.setNumCom(m_buf[B1] * 4);
                sParam_->local.setNumComPrm(sParam_->prm.getNumCom());
            }
            // в случае записи нового значения, сбросим флаг конфигурации
            if (act & GB_ACT_NEW)
                sParam_->device = false;
        }
        break;

    case GB_COM_PRM_GET_JRN_CNT:
        {
            uint16_t t = *reinterpret_cast<uint16_t*>(&m_buf[B1]);
            if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_PRM)
            {
                stat = sParam_->jrnEntry.setNumJrnEntry(t);
            }
        }
        break;

    case GB_COM_PRM_GET_JRN_ENTRY:
        {
            if ((sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_PRM) && (!pc))
            {
                if (sParam_->typeDevice == AVANT_OPTO)
                {
                    // дата
                    sParam_->jrnEntry.dateTime.setYear(m_buf[B5], true);
                    sParam_->jrnEntry.dateTime.setMonth(m_buf[B6], true);
                    sParam_->jrnEntry.dateTime.setDay(m_buf[B7], true);
                    // время
                    sParam_->jrnEntry.dateTime.setHour(m_buf[B8], true);
                    sParam_->jrnEntry.dateTime.setMinute(m_buf[B9], true);
                    sParam_->jrnEntry.dateTime.setSecond(m_buf[B10], true);
                    uint16_t t = TO_UINT16(m_buf[B11], m_buf[B12]);
                    sParam_->jrnEntry.dateTime.setMsSecond(t);
                    //
                    sParam_->jrnEntry.setOpticEntry(static_cast<uint8_t*>(&m_buf[B1]));
                    sParam_->jrnEntry.setReady();
                }
                else
                {
                    sParam_->jrnEntry.dateTime.setYear(m_buf[B16], true);
                    sParam_->jrnEntry.dateTime.setMonth(m_buf[B15], true);
                    sParam_->jrnEntry.dateTime.setDay(m_buf[B14], true);
                    // B13 - день недели
                    sParam_->jrnEntry.dateTime.setHour(m_buf[B12], true);
                    sParam_->jrnEntry.dateTime.setMinute(m_buf[B11], true);
                    sParam_->jrnEntry.dateTime.setSecond(m_buf[B10], true);
                    uint16_t t = TO_UINT16(m_buf[B9], m_buf[B8]);
                    sParam_->jrnEntry.dateTime.setMsSecond(t);
                    sParam_->jrnEntry.setDeviceJrn(static_cast<eGB_DEVICE_K400>(m_buf[B1]));
                    sParam_->jrnEntry.setNumCom(m_buf[B2]);
                    sParam_->jrnEntry.setEventType(m_buf[B3]);
                    sParam_->jrnEntry.setSrcCom(m_buf[B4]);  // 3-концевая

                    eGB_SOURCE_COM source = GB_SOURCE_COM_DI;
                    if (sParam_->typeDevice == AVANT_K400)
                    {
                        source = (m_buf[B4] == 0x80) ? GB_SOURCE_COM_DR : GB_SOURCE_COM_DI;
                    }
                    sParam_->jrnEntry.setSourceCom(source);

                    sParam_->jrnEntry.setReady();
                }
                stat = true;
            }
        }
        break;

    default: break;
    }

    return stat;
}

/**	Обработка принятой команды Передатчика.
 *
 * 	@param com	Код команды
 * 	@param pc True - команда запрошенная с ПК, False - запрошенная с ПИ-БСП.
 * 	@return True - в случае успешной обработки, False - в случае ошибки.
 */
bool clProtocolBspS::getPrdCommand(eGB_COM com, bool pc)
{
    bool stat = false;

    switch (com)
    {

    case GB_COM_PRD_GET_COM:
        {
            uint8_t act = GB_ACT_NO;
            if (sParam_->typeDevice == AVANT_K400)
            {
                act = sParam_->prd.setNumCom(m_buf[B1] * 4);
                sParam_->local.setNumComPrd(sParam_->prd.getNumCom());
            }
            // в случае записи нового значения, сбросим флаг конфигурации
            if (act & GB_ACT_NEW)
                sParam_->device = false;
        }
        break;

    case GB_COM_PRD_GET_JRN_CNT:
        {
            uint16_t t = *reinterpret_cast<uint16_t*>(&m_buf[B1]);
            if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_PRD)
            {
                stat = sParam_->jrnEntry.setNumJrnEntry(t);
            }
        }
        break;

    case GB_COM_PRD_GET_JRN_ENTRY:
        {
            if ((sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_PRD) && (!pc))
            {
                if (sParam_->typeDevice == AVANT_OPTO)
                {
                    // дата
                    sParam_->jrnEntry.dateTime.setYear(m_buf[B5], true);
                    sParam_->jrnEntry.dateTime.setMonth(m_buf[B6], true);
                    sParam_->jrnEntry.dateTime.setDay(m_buf[B7], true);
                    // время
                    sParam_->jrnEntry.dateTime.setHour(m_buf[B8], true);
                    sParam_->jrnEntry.dateTime.setMinute(m_buf[B9], true);
                    sParam_->jrnEntry.dateTime.setSecond(m_buf[B10], true);
                    uint16_t t = TO_UINT16(m_buf[B11], m_buf[B12]);
                    sParam_->jrnEntry.dateTime.setMsSecond(t);
                    //
                    sParam_->jrnEntry.setOpticEntry(static_cast<uint8_t*>(&m_buf[B1]));

                    if (m_buf[NUM] >= 16)
                    {
                        // В ОПТИКЕ передаются флаги ЦПП
                        sParam_->jrnEntry.setOpticEntryDR(static_cast<uint8_t*>(&m_buf[B13]));
                    }
                    sParam_->jrnEntry.setReady();
                }
                else
                {
                    sParam_->jrnEntry.dateTime.setYear(m_buf[B16], true);
                    sParam_->jrnEntry.dateTime.setMonth(m_buf[B15], true);
                    sParam_->jrnEntry.dateTime.setDay(m_buf[B14], true);
                    // B13 - день недели
                    sParam_->jrnEntry.dateTime.setHour(m_buf[B12], true);
                    sParam_->jrnEntry.dateTime.setMinute(m_buf[B11], true);
                    sParam_->jrnEntry.dateTime.setSecond(m_buf[B10], true);
                    uint16_t t = TO_UINT16(m_buf[B9], m_buf[B8]);
                    sParam_->jrnEntry.dateTime.setMsSecond(t);
                    sParam_->jrnEntry.setDeviceJrn(static_cast<eGB_DEVICE_K400>(m_buf[B1]));
                    sParam_->jrnEntry.setNumCom(m_buf[B2]);
                    sParam_->jrnEntry.setEventType(m_buf[B3]);

                    eGB_SOURCE_COM source = GB_SOURCE_COM_DI;
                    if (sParam_->typeDevice == AVANT_K400)
                    {
                        // К400 ЦПП
                        source = (m_buf[B4] == 1) ? GB_SOURCE_COM_DR : GB_SOURCE_COM_DI;
                    }
                    sParam_->jrnEntry.setSourceCom(source);

                    sParam_->jrnEntry.setReady();
                }
                stat = true;
            }
        }
        break;

    default: break;
    }

    return stat;
}

/**	Обработка принятой Общей команды.
 *
 * 	@param com	Код команды
 * 	@param pc True - команда запрошенная с ПК, False - запрошенная с ПИ-БСП.
 * 	@return True - в случае успешной обработки, False - в случае ошибки.
 */
bool clProtocolBspS::getGlbCommand(eGB_COM com, bool pc)
{
    static uint8_t  cntTimeFrame = 0;
    bool            stat         = false;
    eGB_TYPE_DEVICE device       = sParam_->typeDevice;

    switch (com)
    {
    case GB_COM_GET_TIME:
        {
            stat = sParam_->DateTime.setYear(m_buf[B1], true);
            stat &= sParam_->DateTime.setMonth(m_buf[B2], true);
            stat &= sParam_->DateTime.setDay(m_buf[B3], true);
            stat &= sParam_->DateTime.setHour(m_buf[B4], true);
            stat &= sParam_->DateTime.setMinute(m_buf[B5], true);
            stat &= sParam_->DateTime.setSecond(m_buf[B6], true);
            // миллисекунды устанавливаются, только если они есть в посылке
            uint16_t ms = 0;
            if (m_buf[NUM >= 8])
            {
                ms = *reinterpret_cast<uint16_t*>(&m_buf[B7]);
            }
            stat &= sParam_->DateTime.setMsSecond(ms);
            stat = true;

            // новая запись журнала, для передачи в АСУ ТП
            if (m_buf[NUM] >= 21)
            {
                TJrnSCADA* jrn = &sParam_->jrnScada;
                if (jrn->isReadyToWrite())
                {
                    jrn->setJrn(m_buf[B9]);
                    jrn->setEvent(m_buf[B10]);
                    jrn->setCom(m_buf[B11]);
                    jrn->setComSource(m_buf[B12]);
                    // B13
                    jrn->dtime.setYear(m_buf[B14], true);
                    jrn->dtime.setMonth(m_buf[B15], true);
                    jrn->dtime.setDay(m_buf[B16], true);
                    jrn->dtime.setHour(m_buf[B17], true);
                    jrn->dtime.setMinute(m_buf[B18], true);
                    jrn->dtime.setSecond(m_buf[B19], true);
                    jrn->dtime.setMsSecond(*reinterpret_cast<uint16_t*>(&m_buf[B20]));

                    jrn->setReadyToSend();
                    cntTimeFrame = 0;
                }
            }
            else
            {
                if (sParam_->jrnScada.isReadyToWrite())
                {
                    if (cntTimeFrame > 5)
                    {
                        sParam_->jrnScada.setReadyToEvent();
                    }
                    else
                    {
                        cntTimeFrame++;
                    }
                }
            }
        }
        break;

    case GB_COM_GET_SOST:
        {
            sParam_->def.status.setRegime(static_cast<eGB_REGIME>(m_buf[B1]));
            sParam_->def.status.setState(m_buf[B2]);
            sParam_->def.status.setDopByte(m_buf[B3]);

            sParam_->prm.status.setRegime(static_cast<eGB_REGIME>(m_buf[B4]));
            sParam_->prm.status.setState(m_buf[B5]);
            sParam_->prm.status.setDopByte(m_buf[B6]);

            sParam_->prd.status.setRegime(static_cast<eGB_REGIME>(m_buf[B7]));
            sParam_->prd.status.setState(m_buf[B8]);
            sParam_->prd.status.setDopByte(m_buf[B9]);

            // далее проверяется кол-во принятых байт
            uint8_t num = m_buf[NUM];

            if (sParam_->prm.status.isEnable())
            {
                if (num >= 13)
                    sParam_->prm.setIndCom8(0, m_buf[B13]);
                if (num >= 14)
                    sParam_->prm.setIndCom8(1, m_buf[B14]);
                if (num >= 15)
                    sParam_->prm.setIndCom8(2, m_buf[B15]);
                if (num >= 16)
                    sParam_->prm.setIndCom8(3, m_buf[B16]);
            }

            if (sParam_->prd.status.isEnable())
            {
                if (num >= 17)
                    sParam_->prd.setIndCom8(0, m_buf[B17]);
                if (num >= 18)
                    sParam_->prd.setIndCom8(1, m_buf[B18]);
                if (num >= 19)
                    sParam_->prd.setIndCom8(2, m_buf[B19]);
                if (num >= 20)
                    sParam_->prd.setIndCom8(3, m_buf[B20]);
            }

            // текущее состояние дискретных входов (Пуск ПРМ, Сброс индикации и т.д.)
            // проверка необходимости включить подсветку
            if (num >= 21)
            {
                sParam_->glb.setLedOn((m_buf[B21] > 0));
                sParam_->glb.setDInputState(m_buf[B21]);
            }

            eGB_REGIME reg    = GB_REGIME_MAX;
            eGB_REGIME regTmp = GB_REGIME_MAX;
            // определение общего режима аппарата
            // по умолчанию оно будет GB_REGIME_ENABLED
            // другое состояние возможно, если все устройства аппарата
            // имеют один и тот же режим
            if (sParam_->prd.status.isEnable())
            {
                regTmp = sParam_->prd.status.getRegime();
                if (reg == GB_REGIME_MAX)
                    reg = regTmp;
                else if (reg != regTmp)
                    reg = GB_REGIME_ENABLED;
            }

            if (sParam_->prm.status.isEnable())
            {
                regTmp = sParam_->prm.status.getRegime();
                if (reg == GB_REGIME_MAX)
                    reg = regTmp;
                else if (reg != regTmp)
                    reg = GB_REGIME_ENABLED;
            }

            if (sParam_->def.status.isEnable())
            {
                regTmp = sParam_->def.status.getRegime();
                if (reg == GB_REGIME_MAX)
                    reg = regTmp;
                else if (reg != regTmp)
                    reg = GB_REGIME_ENABLED;
            }

            sParam_->glb.status.setRegime(reg);
            stat = true;
        }
        break;

    case GB_COM_GET_FAULT:
        {
            sParam_->def.status.setFault(TO_UINT16(m_buf[B1], m_buf[B2]));
            sParam_->def.status.setWarning(TO_UINT16(m_buf[B3], m_buf[B4]));

            // установка номера(ов) уд.аппаратов ддля Р400(М) в режиме ПВЗУ / ПВЗУ-Е
            uint8_t n = 0;
            if ((device == AVANT_R400) || (device == AVANT_R400M))
            {
                eGB_COMP_R400M comp = sParam_->glb.getCompR400m();

                if ((comp == GB_COMP_R400M_PVZUE) || (comp == GB_COMP_R400M_PVZU))
                {
                    n = m_buf[B5];
                }
            }
            sParam_->def.status.setRemoteNumber(n);

            sParam_->prm.status.setFault(TO_UINT16(m_buf[B5], m_buf[B6]));
            sParam_->prm.status.setWarning(TO_UINT16(m_buf[B7], m_buf[B8]));

            sParam_->prd.status.setFault(TO_UINT16(m_buf[B9], m_buf[B10]));
            sParam_->prd.status.setWarning(TO_UINT16(m_buf[B11], m_buf[B12]));

            sParam_->glb.status.setFault(TO_UINT16(m_buf[B13], m_buf[B14]));
            sParam_->glb.status.setWarning(TO_UINT16(m_buf[B15], m_buf[B16]));
            stat = true;
        }
        break;

    case GB_COM_GET_MEAS:
        {
            // обработаем посылку, если стоит флаг опроса всех параметров
            if (m_buf[B1] == 0)
            {
                sParam_->measParam.setResistOut(TO_UINT16(m_buf[B2], m_buf[B3]));
                sParam_->measParam.setCurrentOut(TO_UINT16(m_buf[B4], m_buf[B5]));
                // в buf[B7] передатся дробная часть напряжения * 100
                // т.е. если там 90, то это 0.9В.
                sParam_->measParam.setVoltageOut(m_buf[B6], (m_buf[B7] / 10));
                sParam_->measParam.setVoltageDef(static_cast<int8_t>(m_buf[B8]));
                sParam_->measParam.setD(static_cast<int8_t>(m_buf[B8]));  // для К400
                sParam_->measParam.setVoltageDef2(static_cast<int8_t>(m_buf[B9]));
                sParam_->measParam.setVoltageCf(static_cast<int8_t>(m_buf[B10]));
                sParam_->measParam.setVoltageCf2(static_cast<int8_t>(m_buf[B11]));
                sParam_->measParam.setVoltageNoise(static_cast<int8_t>(m_buf[B12]));
                sParam_->measParam.setVoltageNoise2(static_cast<int8_t>(m_buf[B13]));
                sParam_->measParam.setPulseWidth(TO_UINT16(m_buf[B14], m_buf[B15]));
                // B16 - температура, она и так уже известна
                sParam_->measParam.setFreqDev(m_buf[B17]);
                stat = true;
            }
        }
        break;

    case GB_COM_GET_VERS:
        {
            uint8_t num_com;
            uint8_t act = GB_ACT_NO;

            // данные о типе аппарата
            act |= sParam_->def.status.setEnable(m_buf[B1] == 1);
            num_com = (m_buf[B2] == 5) ? (5) : (m_buf[B2] * 4);
            act |= sParam_->prm.setNumCom(num_com);
            sParam_->local.setNumComPrm(sParam_->prm.getNumCom());
            // buf[B3] - прм2
            num_com = (m_buf[B4] == 5) ? (5) : (m_buf[B4] * 4);
            act |= sParam_->prd.setNumCom(num_com);
            sParam_->local.setNumComPrd(sParam_->prd.getNumCom());
            // кол-во аппаратов в линии
            // в def хранится значение параметра
            // все действия с меню производятся относительно значения в glb.
            // !!! и значение == кол-ву аппаратов, а раньше было на 1 меньше
            act |= sParam_->glb.setNumDevices(static_cast<eGB_NUM_DEVICES>(m_buf[B5] - 1));
            sParam_->def.setNumDevices(static_cast<eGB_NUM_DEVICES>(m_buf[B5] - 1));
            sParam_->local.setNumDevices(sParam_->glb.getNumDevices() + 1);
            // тип линии (вч, оптика, ...)
            act |= sParam_->glb.setTypeLine(static_cast<eGB_TYPE_LINE>(m_buf[B6]));
            // версия прошивки АТмега БСП
            TDeviceGlb* glb = &sParam_->glb;
            glb->setVersProgIC16(TO_UINT16(m_buf[B7], m_buf[B8]), GB_IC_BSP_MCU);
            glb->setVersProgIC16(TO_UINT16(m_buf[B9], m_buf[B10]), GB_IC_BSP_DSP);

            glb->setVersProgIC8(m_buf[B12], GB_IC_BSK_PLIS_PRD1);
            glb->setVersProgIC8(m_buf[B13], GB_IC_BSK_PLIS_PRD2);
            glb->setVersProgIC8(m_buf[B14], GB_IC_BSK_PLIS_PRM1);
            glb->setVersProgIC8(m_buf[B15], GB_IC_BSK_PLIS_PRM2);
            glb->setVersProgIC8(m_buf[B16], GB_IC_BSZ_PLIS);
            glb->setVersProgIC16(TO_UINT16(m_buf[B21], m_buf[B22]), GB_IC_BSP_DSP_PLIS);

            if (m_buf[3] >= 17)
            {
                // Тип аппарата, в сентябре 2014 появился у РЗСК и Р400
                // в ноябре 2014 появился  у К400
                act |= glb->setTypeDevice(static_cast<eGB_TYPE_DEVICE>(m_buf[B17]));
            }
            else
            {
                glb->setTypeDevice(AVANT_NO);
            }

            // Желательно установить тип аппарата перед совместимостью
            act |= glb->setCompatibility(m_buf[B11]);

            // B18 - старший байт прошивки БСП-ПИ
            // B19 - младший байт прошивки БСП-ПИ

            // в апреле 2017  появилось однонаправленное кольцо
            if (m_buf[3] >= 20)
            {
                if (m_buf[B20] == 0xAB)
                {
                    glb->setTypeOpto(TYPE_OPTO_RING_UNI);
                }
                else
                {
                    glb->setTypeOpto(TYPE_OPTO_STANDART);
                }
            }
            else
            {
                glb->setTypeOpto(TYPE_OPTO_STANDART);
            }

            // B21, B22 - версия прошивки DSP, смотри выше.

            // проверим необходимость обновления типа аппарата
            if (act & GB_ACT_NEW)
            {
                sParam_->device = false;
            }

            stat = ((act & GB_ACT_ERROR) != GB_ACT_ERROR);
        }
        break;

    case GB_COM_GET_DEVICE_NUM:
        {
            stat = sParam_->glb.setDeviceNum(m_buf[B1]);
        }
        break;

    case GB_COM_GET_COM_PRD_KEEP:
        {
            uint8_t act = GB_ACT_NO;

            if (m_buf[NUM] >= 1)
            {
                if (sParam_->typeDevice == AVANT_R400M)
                {
                    act = sParam_->glb.setCompatibility(m_buf[B1]);
                }

                if (m_buf[NUM] >= 2)
                {
                    if (sParam_->typeDevice == AVANT_K400)
                    {
                        act = sParam_->glb.setCompatibility(m_buf[B2]);
                    }
                    else if (sParam_->typeDevice == AVANT_RZSK)
                    {
                        act = sParam_->glb.setCompatibility(m_buf[B2]);
                    }
                }
            }

            // в случае записи нового значения, сбросим флаг конфигурации
            if (act & GB_ACT_NEW)
            {
                sParam_->device = false;
            }
        }
        break;

    case GB_COM_GET_NET_ADR:
        {
            stat = sParam_->Uart.NetAddress.set(m_buf[B1]);
        }
        break;

    case GB_COM_GET_TEST:
        {
            eGB_TYPE_DEVICE type = sParam_->typeDevice;
            eGB_TYPE_OPTO   opto = sParam_->glb.getTypeOpto();
            sParam_->test.setCurrentSignal(&m_buf[B1], type, opto);
        }
        break;

    case GB_COM_GET_JRN_CNT:
        {
            uint16_t t = *reinterpret_cast<uint16_t*>(&m_buf[B1]);
            if (sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_GLB)
            {
                stat = sParam_->jrnEntry.setNumJrnEntry(t);
            }
        }
        break;

    case GB_COM_GET_JRN_ENTRY:
        {
            if ((sParam_->jrnEntry.getCurrentDevice() == GB_DEVICE_GLB) && (!pc))
            {
                sParam_->jrnEntry.val = true;
                if (sParam_->typeDevice == AVANT_OPTO)
                {
                    // дата
                    sParam_->jrnEntry.dateTime.setYear(m_buf[B6], true);
                    sParam_->jrnEntry.dateTime.setMonth(m_buf[B7], true);
                    sParam_->jrnEntry.dateTime.setDay(m_buf[B8], true);
                    // время
                    sParam_->jrnEntry.dateTime.setHour(m_buf[B9], true);
                    sParam_->jrnEntry.dateTime.setMinute(m_buf[B10], true);
                    sParam_->jrnEntry.dateTime.setSecond(m_buf[B11], true);
                    uint16_t t = TO_UINT16(m_buf[B12], m_buf[B13]);
                    sParam_->jrnEntry.dateTime.setMsSecond(t);
                    //
                    sParam_->jrnEntry.setRegime(static_cast<eGB_REGIME>(m_buf[B1]));
                    sParam_->jrnEntry.setOpticEntry(static_cast<uint8_t*>(&m_buf[B2]));
                    sParam_->jrnEntry.setReady();
                    stat = true;
                }
                else
                {
                    // дата
                    sParam_->jrnEntry.dateTime.setYear(m_buf[B16], true);
                    sParam_->jrnEntry.dateTime.setMonth(m_buf[B15], true);
                    sParam_->jrnEntry.dateTime.setDay(m_buf[B14], true);
                    sParam_->jrnEntry.dateTime.setDayOfWeek(m_buf[B13]);
                    // время
                    sParam_->jrnEntry.dateTime.setHour(m_buf[B12], true);
                    sParam_->jrnEntry.dateTime.setMinute(m_buf[B11], true);
                    sParam_->jrnEntry.dateTime.setSecond(m_buf[B10], true);
                    uint16_t t = TO_UINT16(m_buf[B9], m_buf[B8]);
                    sParam_->jrnEntry.dateTime.setMsSecond(t);
                    sParam_->jrnEntry.setRemoteNumbers(m_buf[B1]);
                    sParam_->jrnEntry.setEventType(m_buf[B2]);
                    sParam_->jrnEntry.setRegime(static_cast<eGB_REGIME>(m_buf[B3]));
                    sParam_->jrnEntry.setReady();
                    stat = true;
                }
            }
        }
        break;

    default: break;
    }

    return stat;
}

/**	Формирование сообщения команды считавания кол-ва и самих записей журнала.
 *
 * 	@param com Команда на передачу
 * 	@return Кол-во передаваемых бйт
 */
uint8_t clProtocolBspS::sendReadJrnCommand(eGB_COM com)
{
    uint8_t num  = 0;
    uint8_t mask = 0;

    // команды работы с журналом
    mask = static_cast<uint8_t>(com) & GB_COM_MASK_DEVICE;
    if (mask == GB_COM_MASK_DEVICE_DEF)
    {
        if (com == GB_COM_DEF_GET_JRN_CNT)
        {
            num = addCom(com);
        }
        else if (com == GB_COM_DEF_GET_JRN_ENTRY)
        {
            uint16_t t = sParam_->txComBuf.getInt16();
            if (sParam_->typeDevice == AVANT_R400M)
                num = addCom(com, static_cast<uint8_t>(t), static_cast<uint8_t>(t >> 8));
            else
                num = addCom(com, static_cast<uint8_t>(t >> 8), static_cast<uint8_t>(t));
        }
    }
    else if (mask == GB_COM_MASK_DEVICE_PRM)
    {
        if (com == GB_COM_PRM_GET_JRN_CNT)
        {
            num = addCom(com);
        }
        else if (com == GB_COM_PRM_GET_JRN_ENTRY)
        {
            uint16_t t = sParam_->txComBuf.getInt16();
            num        = addCom(com, static_cast<uint8_t>(t >> 8), static_cast<uint8_t>(t));
        }
    }
    else if (mask == GB_COM_MASK_DEVICE_PRD)
    {
        if (com == GB_COM_PRD_GET_JRN_CNT)
        {
            num = addCom(com);
        }
        else if (com == GB_COM_PRD_GET_JRN_ENTRY)
        {
            uint16_t t = sParam_->txComBuf.getInt16();
            num        = addCom(com, static_cast<uint8_t>(t >> 8), static_cast<uint8_t>(t));
        }
    }
    else
    {
        if (com == GB_COM_GET_JRN_CNT)
        {
            num = addCom(com);
        }
        else if (com == GB_COM_GET_JRN_ENTRY)
        {
            uint16_t t = sParam_->txComBuf.getInt16();

            if (sParam_->typeDevice == AVANT_R400M)
                num = addCom(com, static_cast<uint8_t>(t), static_cast<uint8_t>(t >> 8));
            else
                num = addCom(com, static_cast<uint8_t>(t >> 8), static_cast<uint8_t>(t));
        }
    }

    return num;
}

//
bool clProtocolBspS::getLocalParam(eGB_COM com)
{
    LocalParams* local       = &sParam_->local;
    eGB_PARAM    local_param = local->getParam();
    bool         is_param    = (getCom(local_param) == com);

    if (is_param)
    {
        // по умолчанию загружается значение первого байта,
        // на отличные от этого параметры далее ведется проверка
        int16_t val = -1000;
        switch (local_param)
        {
        case GB_PARAM_IN_DEC: val = m_buf[B2 + local->getNumOfCurrSameParam() - 1]; break;
        case GB_PARAM_FREQ: val = TO_INT16(m_buf[B1], m_buf[B2]); break;
        case GB_PARAM_COR_U:
            val = static_cast<int8_t>(m_buf[B1]) * 10;
            val += static_cast<int8_t>(m_buf[B2]) / 10;
            break;
        case GB_PARAM_COR_I: val = TO_INT16(m_buf[B3], m_buf[B4]); break;
        default:
            uint8_t pos = B1;

            // смещение в зависимости от номера однотипного параметра
            if (getParamType(local_param) == Param::PARAM_BITES)
            {
                pos += (local->getNumOfCurrSameParam() - 1) / 8;
            }
            else
            {
                pos += local->getNumOfCurrSameParam() - 1;
            }

            if (getSendDop(local_param) != 0)
            {
                pos += getSendDop(local_param) - 1;
            }

            // приведение к знаковому типу, в случае если возможно
            // отрицательное значение параметра
            val = (local->getMin() < 0) ? static_cast<int8_t>(m_buf[pos]) : m_buf[pos];

            break;
        }

        local->setValue(val);
    }

    return is_param;
}
