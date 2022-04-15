/*
 * protocolS.cpp
 *
 *  Created on: 28.06.2012
 *      Author: Shcheblykin
 */
#include "protocolS.h"

/**
 * *****************************************************************************
 *
 * @brief �����������.
 * @param[in] buf ����� ������.
 * @param[in] size ������ ������ ������.
 *
 * *****************************************************************************
 */
clProtocolS::clProtocolS(uint8_t* buf, uint8_t size) : m_buf(buf), size_(size)
{
    cnt_      = 0;
    maxLen_   = 0;
    old_      = PRTS_STATUS_OFF;
    cntCycle_ = 0;
    stat_     = PRTS_STATUS_OFF;
    statDef_  = PRTS_STATUS_OFF;
}

/**
 * *****************************************************************************
 *
 * @brief ��������� ������� ��������� ��������� �����.
 *
 * ����������� ����������� �����.
 *
 * @return True ���� ���� �������� ����, ����� false.
 *
 * *****************************************************************************
 */
bool clProtocolS::checkReadData()
{
    bool check = false;

    if (stat_ == PRTS_STATUS_READ_OK)
    {

        check = checkCRC();
        if (!check)
            setCurrentStatus(PRTS_STATUS_NO);
    }

    return check;
}


/**
 * *****************************************************************************
 *
 * @brief �������� ������ �� �������� �������.
 * @param[out] data ������.
 * @param[in] size ������ ������.
 * @return
 *
 * *****************************************************************************
 */
bool clProtocolS::getData(uint8_t* data, uint8_t size)
{
    stat_ = statDef_;

    if (m_buf[COM] == 0x11)
    {
        Q_ASSERT(m_buf[NUM] == 128);

        if (m_buf[NUM] == size)
        {
            for (uint8_t i = 0; i < size; i++)
            {
                data[i] = m_buf[B1 + i];
            }
        }

        setCurrentStatus(PRTS_STATUS_WRITE_READY);
    }

    return stat_ == PRTS_STATUS_WRITE_READY;
}


/**
 * *****************************************************************************
 *
 * @brief ��������� ���� ��� ��������.
 * @param[in] com �������.
 * @param[in] data ������.
 * @param[in] size ���������� ������ ��� ��������.
 * @return ���������� ���� � �����.
 *
 * *****************************************************************************
 */
uint8_t clProtocolS::sendData(uint8_t com, const uint8_t* data, uint8_t size)
{
    uint8_t num = 0;

    if (stat_ == PRTS_STATUS_WRITE_READY)
    {
        num = addCom(com, size, data);
    }

    return num;
}

/**
 * *****************************************************************************
 *
 * @brief ��������� ������� ���������.
 *
 * ���������� ��������� � �������� �� ���������, ���� ����� ����� ������.
 *
 * *****************************************************************************
 */
void clProtocolS::checkStat()
{
    if (old_ == stat_)
    {
        if (stat_ != statDef_)
        {
            cntCycle_++;
            if (cntCycle_ >= MAX_CYCLE_TO_REST_SOST)
            {
                setCurrentStatus(statDef_);
                old_      = statDef_;
                cntCycle_ = 0;
            }
        }
    }
    else
    {
        old_      = stat_;
        cntCycle_ = 0;
    }
}


/**
 * *****************************************************************************
 *
 * @brief ��������� ���� ��� ��������.
 * @note ������������� ������ �������� ������.
 * @param[in] com �������.
 * @param[in] size ���������� ������ ��� ��������.
 * @param[in] data ������.
 * @return ���������� ���� � �����.
 *
 * *****************************************************************************
 */
uint8_t clProtocolS::addCom(uint8_t com, uint8_t size, const uint8_t* data)
{
    uint8_t cnt = 0;

    if (size < (this->size_ - 5))
    {
        m_buf[cnt++] = 0x55;
        m_buf[cnt++] = 0xAA;
        m_buf[cnt++] = com;
        m_buf[cnt++] = size;

        for (uint8_t i = 0; i < size; i++, cnt++)
            m_buf[cnt] = data[i];
        m_buf[cnt++] = getCRC();

        maxLen_ = cnt;
        setCurrentStatus(PRTS_STATUS_WRITE);
    }

    return cnt;
}


/**
 * *****************************************************************************
 *
 * @brief ��������� ����������� �����.
 * @return True ���� ������ ����������� �����, ����� false.
 *
 * *****************************************************************************
 */
bool clProtocolS::checkCRC() const
{
    bool    stat = false;
    uint8_t crc  = 0;
    uint8_t len  = maxLen_ - 1;

    for (uint8_t i = 2; i < len; i++)
        crc += m_buf[i];

    if (crc == m_buf[len])
        stat = true;

    return stat;
}


/**
 * *****************************************************************************
 *
 * @brief ���������� ����������� ����� ��� ����������� ������
 * @return ����������� �����.
 *
 * *****************************************************************************
 */
uint8_t clProtocolS::getCRC() const
{
    uint8_t crc = 0;
    uint8_t len = m_buf[3] + 5;
    uint8_t i   = 2;

    if (len > size_)
        return 0;

    for (; i < (len - 1); i++)
        crc += m_buf[i];

    return crc;
}
