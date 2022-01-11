/*
 * protocolBspS.cpp
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */
#include "protocolPcS.h"

clProtocolPcS::clProtocolPcS(uint8_t* buf, uint8_t size, stGBparam* sParam) :
    clProtocolS(buf, size, sParam)
{
}

bool clProtocolPcS::getData()
{

    bool    stat = false;
    eGB_COM com  = (eGB_COM) m_buf[2];

    if (com == GB_COM_GET_PASSWORD)
    {
        uint16_t tmp = sParam_->password.get();
        m_buf[3]       = 2;
        m_buf[4]       = tmp >> 8;
        m_buf[5]       = tmp;
        addCom();
        stat = true;
    }
    else if (com == GB_COM_SET_PASSWORD)
    {
        if (m_buf[3] == 2)
        {
            uint16_t tmp = ((uint16_t) m_buf[4] << 8) + m_buf[5];
            sParam_->password.set(tmp);
        }
        addCom();  // эхо
        stat = true;
    }

    return stat;
}

//
bool clProtocolPcS::update()
{
    bool    update = false;
    eGB_COM com    = static_cast<eGB_COM>(getCurrentCom());

    if (com == GB_COM_GET_VERS)
    {
        update = addVersion();
    }
    else if (com == GB_COM_GET_MEAS)
    {
        update = addTemperature();
    }

    return update;
}

// Добавляет температуру.
bool clProtocolPcS::addTemperature()
{
    const uint8_t size = 16;  // мин. количество байт данных в посылке с температурой

    bool    update = false;
    uint8_t crc    = m_buf[maxLen_ - 1];
    uint8_t len    = m_buf[3];

    if (len < size)
    {
        for (uint8_t i = len + B1; len < size; i++, len++)
        {
            m_buf[i] = 0x00;
        }

        m_buf[B16] = sParam_->measParam.getTemperature();
        crc += m_buf[B16];

        crc -= m_buf[NUM];
        m_buf[NUM] = size;
        crc += m_buf[NUM];

        maxLen_          = m_buf[NUM] + 5;
        m_buf[maxLen_ - 1] = crc;

        update = true;
    }

    return update;
}

// Добавляет версию МК ПИ.
bool clProtocolPcS::addVersion()
{
    const uint8_t size = 19;  // мин. количество байт данных в посылке с версией ПИ

    uint8_t crc = m_buf[maxLen_ - 1];
    uint8_t len = m_buf[NUM];
    if (len < size)
    {
        for (uint8_t i = len + B1; len < 19; i++, len++)
        {
            m_buf[i] = 0x00;
        }
        crc += size - m_buf[3];
        m_buf[NUM] = size;
        maxLen_  = m_buf[NUM] + 5;
    }
    else
    {
        crc -= m_buf[B18];
        crc -= m_buf[B19];
    }
    uint16_t vers = sParam_->glb.getVersProgIC(GB_IC_PI_MCU);
    crc += (m_buf[B18] = (vers >> 8));
    crc += (m_buf[B19] = (vers & 0xFF));
    m_buf[maxLen_ - 1] = crc;

    return true;
}
