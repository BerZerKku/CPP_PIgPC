/*
 * control.h
 *
 *  Created on: 22 ���. 2021 �.
 *      Author: Shcheblykin
 */

#ifndef TCONTROL_H_
#define TCONTROL_H_

#include "glbDefine.h"

/**
 * @brief ����� ��� ������ � ��������� ����������, � ��� ����� � � ��.
 */
class TControl
{

public:
    /// ������� ����������.
    enum ctrl_t
    {
        CTRL_NO = -1,  ///< ��� �������
        //
        CTRL_Call = 0,       ///< �����
        CTRL_AcPusk,         ///< ���� ��
        CTRL_AcPuskSelf,     ///< ���� �� ����
        CTRL_AcRegime,       ///< ����� ������ ��
        CTRL_AcReset,        ///< ����� ��
        CTRL_IndReset,       ///< ����� ���������
        CTRL_PuskAdjOn,      ///< ���� ������ ���.
        CTRL_PuskAdjOff,     ///< ���� ������ ����.
        CTRL_PuskPrm,        ///< ���� ���������
        CTRL_RemoteAcPusk,   ///< ���� �� ����������
        CTRL_RemotePusk1,    ///< ���� ���������� 1
        CTRL_RemotePuskAll,  ///< ���� ���������
        CTRL_Reset,          ///< �����
        //
        CTRL_MAX
    };


    /**
     * @brief ���������� ����� ��� ������� ����������.
     * @param[in] ctrl ������ ����������.
     * @return �����.
     * @return "" � ����� ������.
     */
    PGM_P getText(ctrl_t ctrl) const;

    /**
     * @brief ���������� ������ ��� ������� ����������.
     * @param[in] ctrl ������ ����������.
     * @param[out] com �������.
     * @param[out] isbyte ������� ����� ������.
     * @param[out] byte ���� ������.
     * @return true ���� ������ ����������, ����� false.
     */
    bool getData(ctrl_t ctrl, eGB_COM &com, bool &isbyte, uint8_t &byte) const;

private:
    /// ��������� ������� ����������.
    struct data_t
    {
        char    text[20];  ///< �������� ������.
        eGB_COM com;     ///< ������� ������������ ��������� �����.
        bool    isbyte;  ///< ������� ����� ������ � �������.
        uint8_t byte;    ///< ���� ������.
    };

    static const data_t mData[TControl::CTRL_MAX] PROGMEM;

    /**
     * @brief ��������� ������������ ������� ����������.
     * @param[in] ctrl ������ ����������.
     * @return true ���� ������ ����������, ����� false.
     */
    bool checkCtrl(ctrl_t ctrl) const;
};

#endif /* TCONTROL_H_ */
