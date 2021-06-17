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
public:
	clProtocolPcS(uint8_t *buf, uint8_t size, stGBparam *sParam);

	/// ��������� ��������� ���������. � ������ ������� ���������� False.
	bool getData();

	/**	��������� ������� ������ �������� ������������ �� ��� �� ��.
	 *
	 *	� ������� ����������� ������ �������� ���-��. ���� ���-�� ���� ������
	 *	������, ��� ����������, �.�. ��� ������� ��� ��������, ����������
	 *	���������� ���-�� ���� ������ �� ������������. ��� ����� ����� ������,
	 *	�� ����������� ����� ��������, ����� 0�00.
	 *
	 *	@retval True - ������� GB_COM_GET_VERS, ��������� �������.
	 *	@retval False - ������ ������� ��� ��������� �� �������.
	 */
	bool update();

private:

	/** ��������� �����������.
	 *
	 * 	���� � ������ �� ������ ��������� ��� �����������, ��� ����� ���������.
	 *
	 * 	@return true ���� ����������� ���������, ����� false..
	 *
	 */
	bool addTemperature();

	/**	��������� ������ �� ��.
	 *
	 *	� ����� �� ����� ������ �������� ����������� ������ �������� ���-��.
	 *	���� ���-�� ���� ������ ������ ��� ����������, �.�. ��� ������� ���
	 *	��������, ���������� ���������� ���-�� ���� ������ �� ������������.
	 *	��� ����� ����� ������,	�� ����������� ����� ��������, ����� 0�00.
	 *
	 *	@return true ���� �������� ���� ���������, ����� false.
	 */
	bool addVersion();

};


#endif /* PROTOCOLPC_H_ */