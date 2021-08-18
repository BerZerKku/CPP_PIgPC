/*
 * protocolBspM.h
 *
 *  Created on: 05.09.2014
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLPCM_H_
#define PROTOCOLPCM_H_

#include "glbDefine.h"
#include "paramBsp.h"
#include "protocolModbus.h"

class TProtocolPcM : public TProtocolModbus
{
    /// ������ ��������� � ������ ����������
    enum ADR_REG
    {
        ADR_REG_MIN = 0,
        // ���� � �����
        ADR_YEAR = 1,  ///< ���.
        ADR_MONTH,     ///< �����.
        ADR_DAY,       ///< ����.
        ADR_HOUR,      ///< ����.
        ADR_MINUTE,    ///< ������.
        ADR_SECOND,    ///< �������.
        // ������
        ADR_PASSWORD = 7,  ///< ������� ������.
        ADR_NEW_PASSWORD,  ///< ����� ������.
        // ������� ���������
        ADR_GLB_FAULT = 10,  ///< ��� ������������� �����.
        ADR_GLB_WARNING,     ///< ��� �������������� �����.
        ADR_PRM_FAULT,       ///< ��� ������������� ���������.
        ADR_PRM_WARNING,     ///< ��� �������������� ���������.
        ADR_PRM_REGIME,      ///< ����� ���������.
        ADR_PRM_STATE,       ///< ��������� ���������.
        ADR_PRM_DOP,         ///< �������������� ���� ���������.
        ADR_PRD_FAULT,       ///< ��� ������������� �����������.
        ADR_PRD_WARNING,     ///< ��� �������������� �����������.
        ADR_PRD_REGIME,      ///< ����� �����������.
        ADR_PRD_STATE,       ///< ��������� �����������.
        ADR_PRD_DOP,         ///< �������������� ���� �����������.
        ADR_DEF_FAULT,       ///< ��� ������������� ������.
        ADR_DEF_WARNING,     ///< ��� �������������� ������.
        ADR_DEF_REGIME,      ///< ����� ������.
        ADR_DEF_STATE,       ///< ��������� ������.
        ADR_DEF_DOP,         ///< �������������� ���� ������.
        // ������ �������
        ADR_JRN_EVT_CNT_PWR = 27,  ///< ���-�� ��������� ������� � ������� ���������.
        ADR_JRN_EVT_CNT,      ///< ���-�� ������� � �������.
        ADR_JRN_EVT_NUM,      ///< ����� ������� ������ � �������.
        ADR_JRN_EVT_REZ1,     ///< ������.
        ADR_JRN_EVT_REZ2,     ///< ������.
        ADR_JRN_EVT_DEV,      ///< ��� ����������.
        ADR_JRN_EVT_TYPE,     ///< ��� �������.
        ADR_JRN_EVT_REG,      ///< ����� ������.
        ADR_JRN_EVT_REZ3,     ///< ������.
        ADR_JRN_EVT_REZ4,     ///< ������.
        ADR_JRN_EVT_REZ5,     ///< ������.
        ADR_JRN_EVT_REZ6,     ///< ������.
        ADR_JRN_EVT_MSECOND,  ///< ������������.
        ADR_JRN_EVT_SECOND,   ///< �������.
        ADR_JRN_EVT_MINUTE,   ///< ������.
        ADR_JRN_EVT_HOUR,     ///< ���.
        ADR_JRN_EVT_WDAY,     ///< ���� ������.
        ADR_JRN_EVT_DAY,      ///< ����.
        ADR_JRN_EVT_MONTH,    ///< �����.
        ADR_JRN_EVT_YEAR,     ///< ���.
        // ������ ���������
        ADR_JRN_PRM_CNT_PWR = 50,  ///< ���-�� ��������� ������� � ������� ���������.
        ADR_JRN_PRM_CNT,      ///< ���-�� ������� � �������.
        ADR_JRN_PRM_NUM,      ///< ����� ������� ������ � �������.
        ADR_JRN_PRM_REZ1,     ///< ������.
        ADR_JRN_PRM_REZ2,     ///< ������.
        ADR_JRN_PRM_DEV,      ///< ��� ����������.
        ADR_JRN_PRM_NUM_COM,  ///< ����� �������.
        ADR_JRN_PRM_EVENT,    ///< �������.
        ADR_JRN_PRM_REZ3,     ///< ������.
        ADR_JRN_PRM_REZ4,     ///< ������.
        ADR_JRN_PRM_REZ5,     ///< ������.
        ADR_JRN_PRM_REZ6,     ///< ������.
        ADR_JRN_PRM_MSECOND,  ///< ������������.
        ADR_JRN_PRM_SECOND,   ///< �������.
        ADR_JRN_PRM_MINUTE,   ///< ������.
        ADR_JRN_PRM_HOUR,     ///< ���.
        ADR_JRN_PRM_WDAY,     ///< ���� ������.
        ADR_JRN_PRM_DAY,      ///< ����.
        ADR_JRN_PRM_MONTH,    ///< �����.
        ADR_JRN_PRM_YEAR,     ///< ���.
        // ������ �����������
        ADR_JRN_PRD_CNT_PWR = 74,  ///< ���-�� ��������� ������� � ������� ���������.
        ADR_JRN_PRD_CNT,      ///< ���-�� ������� � �������.
        ADR_JRN_PRD_NUM,      ///< ����� ������� ������ � �������.
        ADR_JRN_PRD_REZ1,     ///< ������.
        ADR_JRN_PRD_REZ2,     ///< ������.
        ADR_JRN_PRD_DEV,      ///< ��� ����������.
        ADR_JRN_PRD_NUM_COM,  ///< ����� �������.
        ADR_JRN_PRD_EVENT,    ///< �������.
        ADR_JRN_PRD_SOURCE,   ///< �������� �������.
        ADR_JRN_PRD_REZ4,     ///< ������.
        ADR_JRN_PRD_REZ5,     ///< ������.
        ADR_JRN_PRD_REZ6,     ///< ������.
        ADR_JRN_PRD_MSECOND,  ///< ������������.
        ADR_JRN_PRD_SECOND,   ///< �������.
        ADR_JRN_PRD_MINUTE,   ///< ������.
        ADR_JRN_PRD_HOUR,     ///< ���.
        ADR_JRN_PRD_WDAY,     ///< ���� ������.
        ADR_JRN_PRD_DAY,      ///< ����.
        ADR_JRN_PRD_MONTH,    ///< �����.
        ADR_JRN_PRD_YEAR,     ///< ���.
        // ������ ������
        ADR_JRN_DEF_CNT_PWR = 98,  ///< ���-�� ��������� ������� � ������� ���������.
        ADR_JRN_DEF_CNT,      ///< ���-�� ������� � �������.
        ADR_JRN_DEF_NUM,      ///< ����� ������� ������ � �������.
        ADR_JRN_DEF_REZ1,     ///< ������.
        ADR_JRN_DEF_REZ2,     ///< ������.
        ADR_JRN_DEF_DEV,      ///< ��� ����������.
        ADR_JRN_DEF_TYPE,     ///< ��������� ��������.
        ADR_JRN_DEF_STATE,    ///< ���������.
        ADR_JRN_DEF_REZ3,     ///< ������.
        ADR_JRN_DEF_REZ4,     ///< ������.
        ADR_JRN_DEF_REZ5,     ///< ������.
        ADR_JRN_DEF_REZ6,     ///< ������.
        ADR_JRN_DEF_MSECOND,  ///< ������������.
        ADR_JRN_DEF_SECOND,   ///< �������.
        ADR_JRN_DEF_MINUTE,   ///< ������.
        ADR_JRN_DEF_HOUR,     ///< ���.
        ADR_JRN_DEF_WDAY,     ///< ���� ������.
        ADR_JRN_DEF_DAY,      ///< ����.
        ADR_JRN_DEF_MONTH,    ///< �����.
        ADR_JRN_DEF_YEAR,     ///< ���.
        // ��������
        ADR_MEAS_U_OUT = 123,  ///< �������� ����������.
        ADR_MEAS_I_OUT,        ///< �������� ���.
        ADR_MEAS_UC1,          ///< ����� �� U� (U�1).
        ADR_MEAS_UC2,          ///< ����� �� U�2.
        ADR_MEAS_UN1,          ///< ������� ����� U� (U�1).
        ADR_MEAS_UN2,          ///< ������� ����� U�2.
        ADR_MEAS_UD1,          ///< ����� �� U� (U�1).
        ADR_MEAS_UD2,          ///< ����� �� U�2.
        ADR_MEAS_PW,  ///< ������������ ��������� �� ����������.
        // ��������� ������
        ADR_IND_COM_PRM_16 = 140,  ///< ��������� ������ ��������� 1-16.
        ADR_IND_COM_PRM_32,        ///< ��������� ������ ��������� 17-32.
        ADR_IND_COM_PRD_16,        ///< ��������� ������ ����������� 1-16.
        ADR_IND_COM_PRD_32,  ///< ��������� ������ ����������� 16-32.
        // ������ ��������
        ADR_IC_BSP_MCU = 156,  ///< ������ �������� �� ����� ���.
        ADR_IC_BSP_DSP,        ///< ������ �������� DSP ����� ���.
        ADR_IC_PI_MCU,         ///< ������ �������� �� ����� ��.
        ADR_IC_BSK_PRD1,  ///< ������ �������� ���� ��� ����� ���1.
        ADR_IC_BSK_PRM1,  ///< ������ �������� ���� ��� ����� ���1.
        ADR_IC_BSK_PRD2,  ///< ������ �������� ���� ��� ����� ���2.
        ADR_IC_BSK_PRM2,  ///< ������ �������� ���� ��� ����� ���2.
        ADR_IC_BSZ,       ///< ������ �������� ���� ����� ���.
        // ������������ ���-�� ������������ ������� ���������
        ADR_REG_MAX
    };

    enum ADR_COIL
    {
        ADR_C_MIN = 200,
        // ����� �������� ���������
        ADR_C_FAULT = 201,  ///< ������� �������������.
        ADR_C_WARNING,      ///< ������� ��������������.
        ADR_C_IND_PRD,  ///< ������� ��������� ������ �����������.
        ADR_C_IND_PRM,  ///< ������� ��������� ������ ���������.
        // ����� �������������� �����, ������� � ���� 0�0001 �� ��� 0x8000
        ADR_C_GLB_FAULT_1  = 301,  ///< ������������� ����� � ����� 0�0001.
        ADR_C_GLB_FAULT_16 = 316,  ///< ������������� ����� � ����� 0�8000.
        // ����� �������������� �����, ������� � ���� 0�0001 �� ��� 0x8000
        ADR_C_GLB_WARNING_1 = 317,  ///< �������������� ����� � ����� 0�0001.
        ADR_C_GLB_WARNING_16 = 332,  ///< �������������� ����� � ����� 0�8000.

        // ����������
        ADR_C_PRD_IS = 400,  ///< ���������� ���������.
        // ����� �������������� �����������, ������� � ���� 0�0001 �� ��� 0x8000
        ADR_C_PRD_FAULT_1 = 401,  ///< ������������� ����������� � ����� 0x0001.
        ADR_C_PRD_FAULT_16 = 416,  ///< ������������� ����������� � ����� 0x8000.
        // ����� �������������� �����������, ������� � ���� 0�0001 �� ��� 0x8000
        ADR_C_PRD_WARNING_1 = 417,  ///< �������������� ��������� � ����� 0�0001.
        ADR_C_PRD_WARNING_16 = 432,  ///< �������������� ��������� � ����� 0�8000.
        // ��������� ������ �����������, ������� � 1 �� 32
        ADR_C_PRD_IND_1  = 450,  ///< ��������� 1 ������� �����������.
        ADR_C_PRD_IND_32 = 481,  ///< ��������� 32 ������� �����������.

        // ���������
        ADR_C_PRM_IS = 500,  ///< �������� ���������.
        // ����� �������������� ���������, ������� � ���� 0�0001 �� ��� 0x8000
        ADR_C_PRM_FAULT_1 = 501,  ///< ������������� ��������� � ����� 0x0001.
        ADR_C_PRM_FAULT_16 = 516,  ///< ������������� ��������� � ����� 0x8000.
        // ����� �������������� ���������, ������� � ���� 0�0001 �� ��� 0x8000
        ADR_C_PRM_WARNING_1 = 517,  ///< �������������� ��������� � ����� 0�0001.
        ADR_C_PRM_WARNING_16 = 532,  ///< �������������� ��������� � ����� 0�8000.
        // ��������� ������ �����������, ������� � 1 �� 32
        ADR_C_PRM_IND_1  = 550,  ///< ��������� 1 ������� ���������.
        ADR_C_PRM_IND_32 = 581,  ///< ��������� 32 ������� ���������.

        // ������
        ADR_C_DEF_IS = 600,  ///< ������ ����������.
        // ����� �������������� ���������, ������� � ���� 0�0001 �� ��� 0x8000
        ADR_C_DEF_FAULT_1 = 601,  ///< ������������� ������ � ����� 0x0001.
        ADR_C_DEF_FAULT_16 = 616,  ///< ������������� ������ � ����� 0x8000.
        // ����� �������������� ���������, ������� � ���� 0�0001 �� ��� 0x8000
        ADR_C_DEF_WARNING_1 = 617,  ///< �������������� ������ � ����� 0�0001.
        ADR_C_DEF_WARNING_16 = 632,  ///< �������������� ������ � ����� 0x8000.
        // ������������ ���-�� ������������ ������
        ADR_C_MAX
    };


public:
    /**	�����������.
     *
     * 	@param sParam ��������� �� ��������� ����������.
     * 	@param buf ��������� �� ����� ������.
     * 	@param size ������ ������ ������.
     */
    TProtocolPcM(stGBparam *sParam, uint8_t *buf, uint8_t size);

    /**	����������.
     *
     */
    //	~TProtocolPcM() {};

    /**	������� �������� ���������.
     *
     * 	@return ���-�� ���� ������ ��� ��������.
     */
    uint8_t send();

private:
    stGBparam *const sParam_;  ///< ��������� ����������

    // ������ ������.
    TProtocolModbus::CHECK_ERR readCoil(uint16_t adr, bool &val);

    // ������ ������.
    TProtocolModbus::CHECK_ERR writeCoil(uint16_t adr, bool val);

    // ������ ���������.
    TProtocolModbus::CHECK_ERR readRegister(uint16_t adr, uint16_t &val);

    // ������ ���������
    TProtocolModbus::CHECK_ERR writeRegister(uint16_t adr, uint16_t val);

    // ��������� �������� ������� ������ ID.
    TProtocolModbus::CHECK_ERR readID(char *buf, uint8_t &size);

    // ���������� ������ �����
    bool readCoilGlb(uint16_t adr);

    // ���������� ������ ������
    bool readCoilDef(uint16_t adr);

    // ���������� ������ ���������
    bool readCoilPrm(uint16_t adr);

    // ���������� ������ �����������
    bool readCoilPrd(uint16_t adr);

    // ���������� �� ��������� ���� � �������.
    uint16_t readRegDateTime(uint16_t adr);

    // ���������� �� ��������� ���������.
    uint16_t readRegState(uint16_t adr);

    // C��������� �� ��������� ���������� ����������.
    uint16_t readRegMeasure(uint16_t adr);

    // C��������� �� ��������� ������ ��������.
    uint16_t readRegVersionIC(uint16_t adr);

    // ���������� ������� �������.
    uint16_t readJournalEvent(uint16_t adr);

    // ���������� ������� ���������.
    uint16_t readJournalPrm(uint16_t adr);

    // ���������� ������� �����������.
    uint16_t readJournalPrd(uint16_t adr);

    // ���������� ������� ������.
    uint16_t readJournalDef(uint16_t adr);
};


#endif /* PROTOCOLPM_H_ */
