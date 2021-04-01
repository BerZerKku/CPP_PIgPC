/*
 * protocolBspS.h
 *
 *  Created on: 15.07.2013
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLBSPS_H_
#define PROTOCOLBSPS_H_

#include "protocolS.h"


class clProtocolBspS : public clProtocolS
{
public:
	clProtocolBspS(uint8_t *buf, uint8_t size, stGBparam *sParam);
	/// ��������� ��������� ���������. � ������ ������� ���������� False.
	bool getData(bool pc);

	/// �������� �������
	uint8_t sendData(eGB_COM com);

private:
	bool getDefCommand(eGB_COM com, bool pc);
	bool getPrmCommand(eGB_COM com, bool pc);
	bool getPrdCommand(eGB_COM com, bool pc);
	bool getGlbCommand(eGB_COM com, bool pc);

	uint8_t sendModifGlbCommand(eGB_COM com);

	uint8_t sendReadJrnCommand(eGB_COM com);

    /** ������������ ����� �� �������� ������� ������ "������� �����"
     *
     *  @return true ������
     */
    bool hdlrComGetNetAdr();

    /** ������������ ����� �� �������� ������� ������ "������� �����"
     *
     *  @return true ������
     */
    bool hdlrComSetNetAdr();

    /** ���������� ���������� ��� ������ GB_COM_GET_NET_ADR � SET.
     *
     *  @param pos ����� ���������.
     *  @param buf ��������� ����� ������.
     *  @param len ���������� ���������� ���� � ������.
     *  @return ���������� ��������� ���� ������.
     *  @retval len ���� � ������ �� ���������� ���� ��� ���������.
     */
    uint8_t getComNetAdr(posComNetAdr_t pos, const uint8_t *buf, uint8_t len);

#ifdef TEST_FRIENDS
    TEST_FRIENDS;
#endif
};


#endif /* PROTOCOLBSPS_H_ */
