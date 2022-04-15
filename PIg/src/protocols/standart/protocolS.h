/*
 * protocolS.h
 *
 *  Created on: 28.06.2012
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLS_H_
#define PROTOCOLS_H_

#include "glbDefine.h"
#include <stdint.h>

// ��������� ���������
enum ePRTS_STATUS
{
    PRTS_STATUS_OFF = 0,  ///< ��������
    PRTS_STATUS_NO,       ///< ��������� �������� ���������
    PRTS_STATUS_READ,     ///< ���� ���������� ���������
    PRTS_STATUS_READ_OK,  ///< ���������� ������� ���������, �� �� �� ���������
    PRTS_STATUS_WAIT_ANSWER,  ///< �������� ������
    PRTS_STATUS_WRITE_READY,  ///< ���� �������� ��������
    PRTS_STATUS_WRITE         ///< ���������� ���������
};

// ������ ���� ������ � ���������
enum ePRTS_DATA_BYTE_NAME
{
    COM = 2,
    NUM,  // 3
    B1,   // 4
    B2,   // 5
    B3,   // 6
    B4,   // 7
    B5,   // 8
    B6,   // 9
    B7,   // 10
    B8,   // 11
    B9,   // 12
    B10,  // 13
    B11,  // 14
    B12,  // 15
    B13,  // 16
    B14,  // 17
    B15,  // 18
    B16,  // 19
    B17,  // 20
    B18,  // 21
    B19,  // 22
    B20,  // 23
    B21,  // 24
    B22   // 25
};

class clProtocolS
{
    /** ���-�� ������, ����� �������� �������� ������� ��������� ����� �������� � ���������
     * ��-���������
     */
    static const uint8_t MAX_CYCLE_TO_REST_SOST = 5;

public:
    clProtocolS(uint8_t *buf, uint8_t size);

    // ����� ��������/������������ ������
    uint8_t *const m_buf;

    /// �������� ����� ������� �������� �������
    bool checkReadData();

    /// ������ ������ ������� ���������
    void setEnable(ePRTS_STATUS stat)
    {
        statDef_ = stat;
        setCurrentStatus(stat);
        cntCycle_ = 0;
    }

    /// ��������� ������ ������� ���������
    void setDisable() { setCurrentStatus(PRTS_STATUS_OFF); }

    /// �������� �������� ��������� ���������
    bool isEnable() const { return (stat_ != PRTS_STATUS_OFF); }

    /// ������� ������� � ������
    uint8_t getCurrentCom() const { return m_buf[2]; }

    /// ������� ������ ������ ���������
    ePRTS_STATUS getCurrentStatus() const { return stat_; }

    /// ����� �������� ������� ������ ���������
    /// � ������ ��������� ��������� ��������, ����������� ������� ����
    void setCurrentStatus(ePRTS_STATUS stat)
    {
        if (stat == PRTS_STATUS_NO)
        {
            cnt_ = 0;
        }

        stat_ = stat;
    }

    bool    getData(uint8_t *data, uint8_t size);
    uint8_t sendData(uint8_t com, const uint8_t *data, uint8_t size);
    void    checkStat();

    /**	�������� ��������� ����� �� ������������ ���������
     *
     * 	!! ���������� � ���������� �� ������.
     * 	���� ����������� ������ � ��������� \a PRTS_STATUS_READ.
     *
     *	@see PRTS_STATUS_READ
     *
     * 	@param byte ���� ������.
     * 	@return ������� ������� � �������� ���������.
     * 	@retval �������� ������� �����������.
     */
    uint8_t checkByte(uint8_t byte)
    {
        uint8_t cnt = cnt_;

        if ((cnt == 0) && (byte == 0x55))
        {
            setCurrentStatus(PRTS_STATUS_READ);
        }

        if (stat_ == PRTS_STATUS_READ)
        {
            m_buf[cnt] = byte;
            switch (cnt)
            {
            case 0:
                // ������ ����������
                if (byte == 0x55)
                    cnt++;
                break;
            case 1:
                // ������ ����������
                cnt = (byte == 0xAA) ? 2 : 0;
                break;
            case 2:
                // ������ ���� �������
                cnt++;
                break;
            case 3:
                // �������� �� ������� � ������ ������������ ����� ��� �������
                cnt = (byte < (size_ - 5)) ? (maxLen_ = byte + 5, cnt + 1) : 0;
                break;
            default:
                // �������� ������ ����������� ���������� ���� ������
                cnt++;
                if (cnt >= maxLen_)
                {
                    setCurrentStatus(PRTS_STATUS_READ_OK);
                }
                break;
            }
            cnt_ = cnt;
        }
        return cnt;
    }

protected:
    // ������� ������ ������ ���������
    ePRTS_STATUS stat_;

    // ������� ��������� ����� �� ���������
    uint8_t cnt_;

    // ���������� ���-�� ���� ������ � �������
    uint8_t maxLen_;

    // ������ ������
    const uint8_t size_;

    // ��������� ������ ��������� ��-���������
    ePRTS_STATUS statDef_;

    // ���������� ��������� ���������
    ePRTS_STATUS old_;

    // ������� ���������� ���������
    uint8_t cntCycle_;

    // ���������� � �������� ������� � ������� (���������� ������)
    uint8_t addCom(uint8_t com, uint8_t size, const uint8_t *buf);

    // �������� �������� ����������� �����
    bool checkCRC() const;

    // ���������� ����������� ����� ����������� ������
    uint8_t getCRC() const;
};

#endif /* PROTOCOLS_H_ */
