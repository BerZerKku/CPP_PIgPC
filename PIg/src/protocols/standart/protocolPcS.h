/*
 * protocolBspS.h
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLPCS_H_
#define PROTOCOLPCS_H_

#include "protocolS.h"

class clProtocolPcS : public clProtocolS
{
    eGB_COM lastCom;

public:
	clProtocolPcS(uint8_t *buf, uint8_t size, stGBparam *sParam);

	/** ��������� ��������� ���������.
	 *
	 * 	!!! override �� �������� WinAVR.
	 *
	 * 	@return true ���� ��������� ����������, ����� false.
	 */
    bool getData();

    /**	��������� ������� ��� �������� �� �� ��.
     *
     *  - GB_COM_GET_VERS
     *      � ������� ����������� ������ �������� ���-��.
     *      ���� ���-�� ���� ������ ������, ��� ����������, �.�. ��� �������
     *      ��� ��������, ���������� ���������� ���-�� ���� ������ ��
     *      ������������. ��� ����� ����� ������, �� ����������� �����
     *      ��������, ����� 0�00.
     *  - GB_COM_SET_NET_ADR
     *      � ������ ���� "������������" ������� ��������� � �� ����
     *      GB_COM_SET_PASSWORD � � ���� ������� ��������� ������,
     *      �� ������� ����� ������������� � ����� �� �������
     *      B_COM_SET_PASSWORD.
	 *
     *	@return true ���� ������� ���� ��������, ����� false.
	 */
    bool modifyCommand();

    /**
     * @brief ��������� ������.
     * @param[in] com �������.
     * @return true ���� ������� ����������, false ���� ���� �������� ������.
     */
    bool procCommand(eGB_COM com);

    /**
     * @brief ��������� ������� ������ ������.
     * @return true ���� ������� ����������, false ���� ���� �������� ������.
     */
    bool hdlrGetPassword();

    /**
     * @brief ��������� ������� ������ ������.
     * ���� � ������� ��� �����, �� ������� ����� �������� �� ������� ������
     * ������ � ���.
     * ���� � ������� �� ��� �����, �� ������� ����� ���������� �������.
     * @returntrue ���� ������� ����������, false ���� ���� �������� ������..
     */
    bool hdlrSetPassword();

    /**
     * @brief ��������� ������� ������ �������� ��������� ����.
     * @return true ���� ������� ����������, false ���� ���� �������� ������.
     */
    bool hdlrGetNetAdr();
};


#endif /* PROTOCOLPC_H_ */
