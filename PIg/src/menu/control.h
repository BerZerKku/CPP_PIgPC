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
 * *****************************************************************************
 *
 * @brief ����� ��� ������ � ��������� ����������, � ��� ����� � � ��.
 *
 * *****************************************************************************
 */
class TControl
{
public:
    /// ������� ����������.
    enum ctrl_t
    {
        CTRL_NO = -1,  ///< ��� �������
        //
        CTRL_Call = 0,        ///< �����
        CTRL_AcAuto,          ///< �� ��������������
        CTRL_AcAccelerated,   ///< �� ����������
        CTRL_AcCtrlCheck,     ///< �� ����������� ��������
        CTRL_AcNormal,        ///< �� ����������
        CTRL_AcOff,           ///< �� ��������
        CTRL_AcOn,            ///< �� �������
        CTRL_AcPusk,          ///< ���� ��
        CTRL_AcPuskSelf,      ///< ���� �� ����
        CTRL_AcQuick,         ///< �� ������
        CTRL_AcRegime,        ///< ����� ������ ��
        CTRL_AcRequest,       ///< �� ������
        CTRL_AcReset,         ///< ����� ��
        CTRL_AcTest,          ///< �� ����
        CTRL_IndReset,        ///< ����� ���������
        CTRL_PuskAdjOn,       ///< ���� ���������� ���.
        CTRL_PuskAdjOff,      ///< ���� ���������� ����.
        CTRL_PuskPrd,         ///< ���� �����������
        CTRL_PuskPrm,         ///< ���� ���������
        CTRL_RemoteAcPusk,    ///< ���� �� ����������
        CTRL_RemoteMan,       ///< ���� ��������� �����������
        CTRL_RemoteMan1,      ///< ���� ��������� ����������� 1
        CTRL_RemoteMan2,      ///< ���� ��������� ����������� 2
        CTRL_RemoteMan3,      ///< ���� ��������� ����������� 3
        CTRL_RemoteManAll,    ///< ���� ��������� �����������
        CTRL_RemotePusk,      ///< ���� ����������
        CTRL_RemotePusk1,     ///< ���� ���������� 1
        CTRL_RemotePusk2,     ///< ���� ���������� 2
        CTRL_RemotePusk3,     ///< ���� ���������� 3
        CTRL_RemotePuskAll,   ///< ���� ���������
        CTRL_RemoteReset,     ///< ����� ����������
        CTRL_RemoteReset1,    ///< ����� ���������� 1
        CTRL_RemoteReset2,    ///< ����� ���������� 2
        CTRL_RemoteReset3,    ///< ����� ���������� 3
        CTRL_RemoteResetAll,  ///< ����� ���������
        CTRL_Reset,           ///< ����� ������
        CTRL_ResetAll,        ///< ����� ����
        CTRL_SingleOff,       ///< ������������� ����� ����.
        CTRL_SingleOn,        ///< ������������� ����� ���.
        //
        CTRL_MAX
    };

    PGM_P getText(ctrl_t ctrl) const;
    bool  getData(ctrl_t ctrl, eGB_COM &com, bool &isbyte, uint8_t &byte) const;

private:
    /// ��������� ������� ����������.
    struct data_t
    {
        PGM_P   text;    ///< �������� ������, �� ����� 16 ��������.
        eGB_COM com;     ///< ������� ������������ ��������� �����.
        bool    isbyte;  ///< ������� ����� ������ � �������.
        uint8_t byte;    ///< ���� ������.
    };

    /// ������ ������.
    static const data_t m_data[TControl::CTRL_MAX] PROGMEM;

    bool checkCtrl(ctrl_t ctrl) const;
};

#endif /* TCONTROL_H_ */
