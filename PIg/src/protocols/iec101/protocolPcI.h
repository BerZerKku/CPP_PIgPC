/*
 * protocolPcI.h
 *
 *  Created on: 14.05.2015
 *      Author: Shcheblykin
 */

#ifndef PROTOCOLPCI_H_
#define PROTOCOLPCI_H_

#include "CIec101.h"
#include "glbDefine.h"
#include "paramBsp.h"

#ifdef AVR
	#include <avr/pgmspace.h>
#else
	#define PROGMEM
	#define pgm_read_byte(a) (*a)
#endif

/**	\defgroup Notation ����� ����������.
 *
 *	�������:
 *	- b - ������ ����������, bError;
 *	- cl - �����, clIec101;
 *	- e - ������������, eTypeId;
 *	- i8 - ������������� ���������� �������� 8 ���, i8Error;
 *	- i16 - ������������� ���������� �������� 16 ���, i16Error;
 *	- p - ���������, pArray;
 *	- r - ������, rArray;
 *	- s - ������ � RAM, sDate;
 *	- sf - ������ � FLASH, sfDate;
 *	- st - ���������, SData;
 *	- u - ����������� union, uAsdu;
 *	- u8 - ����������� ���������� �������� 8 ���, u8Size;
 *	- u16 - ����������� ���������� �������� 8 ���, u16Size;
 *	- v - ���������� ���� void, vReserved.
 *
 *	�������:
 *	- c_ - ����������� ���������� �������, c_u8Address.
 *	- g_ - ���������� ����������, g_u8Address;
 *	- m_ - ����������-����, m_u8Address;
 *	- s_ - ����������� ����������-����, s_u8AddressMax;
 *
 *	�������:
 *	- C - �����, CIec101;
 *	- E - ������������, ECot;
 *	- S - ���������, SDate;
 *	- T - ����� ��� ������ (typedef);
 *	- U - �����������, UAsdu.
 *
 *	����� ��������� ���������� ������ ��� ������ ���������.
 */

class TProtocolPcI : public CIec101{

	// ����� ������� �������� � ������� ������� (������ ����� 1).
	static const uint16_t c_adrIe1Event1 = 700;

	// ����� ������ ������� ����������� (��) (������ ����� 1).
	static const uint16_t c_adrIe1PrdDCom1 = 740;

	// ����� ������ ������� ����������� (���) (������ ����� 1).
	static const uint16_t c_adrIe1PrdCCom1 = 772;

	// ����� ������ ������� ��������� (������ ����� 1).
	static const uint16_t c_adrIe1PrmCom1 = 804;

	// ����� ������� ������� ��� ������
	static const uint16_t c_adrIe1DefSignal1 = 836;

	/// ������ ��������� ���������� ������ 2.
	typedef enum __attribute__ ((__packed__)) {
		IE2_ERROR = 0,
		IE2_WARNING,
		IE2_PRD_COM,
		IE2_PRM_COM,
		IE2_DI_PUSK_PRM,
		IE2_DI_RESET_IND,
		IE2_DI_TM_CONTROL,
		IE2_GLB_ERROR_H0001,
		IE2_GLB_ERROR_H0002,
		IE2_GLB_ERROR_H0004,
		IE2_GLB_ERROR_H0008,
		IE2_GLB_ERROR_H0010,
		IE2_GLB_ERROR_H0020,
		IE2_GLB_ERROR_H0040,
		IE2_GLB_ERROR_H0080,
		IE2_GLB_ERROR_H0100,
		IE2_GLB_ERROR_H0200,
		IE2_GLB_ERROR_H0400,
		IE2_GLB_ERROR_H0800,
		IE2_GLB_ERROR_H1000,
		IE2_GLB_ERROR_H2000,
		IE2_GLB_ERROR_H4000,
		IE2_GLB_ERROR_H8000,
		IE2_GLB_WARNING_H0001,
		IE2_GLB_WARNING_H0002,
		IE2_GLB_WARNING_H0004,
		IE2_GLB_WARNING_H0008,
		IE2_GLB_WARNING_H0010,
		IE2_GLB_WARNING_H0020,
		IE2_GLB_WARNING_H0040,
		IE2_GLB_WARNING_H0080,
		IE2_GLB_WARNING_H0100,
		IE2_GLB_WARNING_H0200,
		IE2_GLB_WARNING_H0400,
		IE2_GLB_WARNING_H0800,
		IE2_GLB_WARNING_H1000,
		IE2_GLB_WARNING_H2000,
		IE2_GLB_WARNING_H4000,
		IE2_GLB_WARNING_H8000,
		IE2_PRD_ON,
		IE2_PRD_ERROR_H0001,
		IE2_PRD_ERROR_H0002,
		IE2_PRD_ERROR_H0004,
		IE2_PRD_ERROR_H0008,
		IE2_PRD_ERROR_H0010,
		IE2_PRD_ERROR_H0020,
		IE2_PRD_ERROR_H0040,
		IE2_PRD_ERROR_H0080,
		IE2_PRD_ERROR_H0100,
		IE2_PRD_ERROR_H0200,
		IE2_PRD_ERROR_H0400,
		IE2_PRD_ERROR_H0800,
		IE2_PRD_ERROR_H1000,
		IE2_PRD_ERROR_H2000,
		IE2_PRD_ERROR_H4000,
		IE2_PRD_ERROR_H8000,
		IE2_PRD_WARNING_H0001,
		IE2_PRD_WARNING_H0002,
		IE2_PRD_WARNING_H0004,
		IE2_PRD_WARNING_H0008,
		IE2_PRD_WARNING_H0010,
		IE2_PRD_WARNING_H0020,
		IE2_PRD_WARNING_H0040,
		IE2_PRD_WARNING_H0080,
		IE2_PRD_WARNING_H0100,
		IE2_PRD_WARNING_H0200,
		IE2_PRD_WARNING_H0400,
		IE2_PRD_WARNING_H0800,
		IE2_PRD_WARNING_H1000,
		IE2_PRD_WARNING_H2000,
		IE2_PRD_WARNING_H4000,
		IE2_PRD_WARNING_H8000,
		IE2_PRD_COM_01,
		IE2_PRD_COM_02,
		IE2_PRD_COM_03,
		IE2_PRD_COM_04,
		IE2_PRD_COM_05,
		IE2_PRD_COM_06,
		IE2_PRD_COM_07,
		IE2_PRD_COM_08,
		IE2_PRD_COM_09,
		IE2_PRD_COM_10,
		IE2_PRD_COM_11,
		IE2_PRD_COM_12,
		IE2_PRD_COM_13,
		IE2_PRD_COM_14,
		IE2_PRD_COM_15,
		IE2_PRD_COM_16,
		IE2_PRD_COM_17,
		IE2_PRD_COM_18,
		IE2_PRD_COM_19,
		IE2_PRD_COM_20,
		IE2_PRD_COM_21,
		IE2_PRD_COM_22,
		IE2_PRD_COM_23,
		IE2_PRD_COM_24,
		IE2_PRD_COM_25,
		IE2_PRD_COM_26,
		IE2_PRD_COM_27,
		IE2_PRD_COM_28,
		IE2_PRD_COM_29,
		IE2_PRD_COM_30,
		IE2_PRD_COM_31,
		IE2_PRD_COM_32,
		IE2_PRD_DISABLED,
		IE2_PRD_ENABLED,
		IE2_PRD_TEST,
		IE2_PRM_ON,
		IE2_PRM_ERROR_H0001,
		IE2_PRM_ERROR_H0002,
		IE2_PRM_ERROR_H0004,
		IE2_PRM_ERROR_H0008,
		IE2_PRM_ERROR_H0010,
		IE2_PRM_ERROR_H0020,
		IE2_PRM_ERROR_H0040,
		IE2_PRM_ERROR_H0080,
		IE2_PRM_ERROR_H0100,
		IE2_PRM_ERROR_H0200,
		IE2_PRM_ERROR_H0400,
		IE2_PRM_ERROR_H0800,
		IE2_PRM_ERROR_H1000,
		IE2_PRM_ERROR_H2000,
		IE2_PRM_ERROR_H4000,
		IE2_PRM_ERROR_H8000,
		IE2_PRM_WARNING_H0001,
		IE2_PRM_WARNING_H0002,
		IE2_PRM_WARNING_H0004,
		IE2_PRM_WARNING_H0008,
		IE2_PRM_WARNING_H0010,
		IE2_PRM_WARNING_H0020,
		IE2_PRM_WARNING_H0040,
		IE2_PRM_WARNING_H0080,
		IE2_PRM_WARNING_H0100,
		IE2_PRM_WARNING_H0200,
		IE2_PRM_WARNING_H0400,
		IE2_PRM_WARNING_H0800,
		IE2_PRM_WARNING_H1000,
		IE2_PRM_WARNING_H2000,
		IE2_PRM_WARNING_H4000,
		IE2_PRM_WARNING_H8000,
		IE2_PRM_COM_01,
		IE2_PRM_COM_02,
		IE2_PRM_COM_03,
		IE2_PRM_COM_04,
		IE2_PRM_COM_05,
		IE2_PRM_COM_06,
		IE2_PRM_COM_07,
		IE2_PRM_COM_08,
		IE2_PRM_COM_09,
		IE2_PRM_COM_10,
		IE2_PRM_COM_11,
		IE2_PRM_COM_12,
		IE2_PRM_COM_13,
		IE2_PRM_COM_14,
		IE2_PRM_COM_15,
		IE2_PRM_COM_16,
		IE2_PRM_COM_17,
		IE2_PRM_COM_18,
		IE2_PRM_COM_19,
		IE2_PRM_COM_20,
		IE2_PRM_COM_21,
		IE2_PRM_COM_22,
		IE2_PRM_COM_23,
		IE2_PRM_COM_24,
		IE2_PRM_COM_25,
		IE2_PRM_COM_26,
		IE2_PRM_COM_27,
		IE2_PRM_COM_28,
		IE2_PRM_COM_29,
		IE2_PRM_COM_30,
		IE2_PRM_COM_31,
		IE2_PRM_COM_32,
		IE2_PRM_DISABLED,
		IE2_PRM_READY,
		IE2_PRM_ENABLED,
		IE2_PRM_TEST,
		IE2_DEF_ON,
		IE2_DEF_ERROR_H0001,
		IE2_DEF_ERROR_H0002,
		IE2_DEF_ERROR_H0004,
		IE2_DEF_ERROR_H0008,
		IE2_DEF_ERROR_H0010,
		IE2_DEF_ERROR_H0020,
		IE2_DEF_ERROR_H0040,
		IE2_DEF_ERROR_H0080,
		IE2_DEF_ERROR_H0100,
		IE2_DEF_ERROR_H0200,
		IE2_DEF_ERROR_H0400,
		IE2_DEF_ERROR_H0800,
		IE2_DEF_ERROR_H1000,
		IE2_DEF_ERROR_H2000,
		IE2_DEF_ERROR_H4000,
		IE2_DEF_ERROR_H8000,
		IE2_DEF_WARNING_H0001,
		IE2_DEF_WARNING_H0002,
		IE2_DEF_WARNING_H0004,
		IE2_DEF_WARNING_H0008,
		IE2_DEF_WARNING_H0010,
		IE2_DEF_WARNING_H0020,
		IE2_DEF_WARNING_H0040,
		IE2_DEF_WARNING_H0080,
		IE2_DEF_WARNING_H0100,
		IE2_DEF_WARNING_H0200,
		IE2_DEF_WARNING_H0400,
		IE2_DEF_WARNING_H0800,
		IE2_DEF_WARNING_H1000,
		IE2_DEF_WARNING_H2000,
		IE2_DEF_WARNING_H4000,
		IE2_DEF_WARNING_H8000,
		IE2_MAX
	} EInfoElement2;

public:
	TProtocolPcI(stGBparam *sParam, uint8_t *buf, uint8_t size);

	/**	������� �������� ���������.
	 *
	 * 	@return ���-�� ���� ������ ��� ��������.
	 */
	uint8_t send();

	/**	�������� ������� ������ ������ 1 �� ��������.
	 *
	 *	@param adr[out] �����.
	 *	@param val[out] ��������.
	 *	@param time[out] ���� � �����.
	 * 	@retval False ��� ������ �� ��������.
	 * 	@retval True ���� ������ �� ��������.
	 */
	virtual bool checkEventClass1(uint16_t &adr, bool &val, SCp56Time2a &time);

	/**	�������� ������� ������ ������ 2 �� ��������.
	 *
	 *	@param adr[out] �����.
	 *	@param val[out] ��������.
	 *	@param time[out] ���� � �����.
	 * 	@retval False ��� ������ �� ��������.
	 * 	@retval True ���� ������ �� ��������.
	 */
	virtual bool checkEventClass2(uint16_t &adr, bool &val, SCp56Time2a &time);

private:

	/// ��������� ����������.
	stGBparam * const sParam_;

	/// ��������� ��������� ���������� (EInfoElement2).
	bool m_flags[IE2_MAX];

	/// ������ ��������� ���������� (EInfoElement2).
	static const uint16_t c_adrIE2[];

	/**	��������� ������ �� ������� ������.
	 *
	 *	��� ���������� ������ ���������������� ���� C_IC_NA_1 � ������������ \a
	 *	True, ����� False.
	 *
	 *	@param adr[out] ����� �����.
	 *	@param val[out] �������� �����.
	 *	@retval True ������������ ������ ��� ��������.
	 *	@retval False ������ ���.
	 */
	virtual bool procInterrog(uint16_t &adr, bool &val);

	/**	��������� �������.
	 *
	 *	���������� �������������, ��� ��������� ����� ������������� �������.
	 *
	 *	���������������� ������� ��� �������� ������� � ���.
	 *
	 * 	@retval True ����� �����������.
	 * 	@retval False ����� �� �����������.
	 */
	virtual bool procSetTime();

	/**	��������� �������, ��������� �� ���������.
	 *
	 * 	���������� ������������� ��� �������� ������� ������������ �����, ������
	 * 	����� ������������� �������.
	 *
	 * 	���� �� ����� ���� ������� ���� ������������� �������, ����� �����������
	 * 	���� ��������� �� ���������.
	 *
	 * 	@retval True ���� ��� ������� ����� �� ��� � �������� �� ����� �������������.
	 * 	@retval False ���� ������ ������.
	 */
	virtual bool procSetTimeEnd();

	/**	���������� ��������� �������� ����������.
	 *
	 * 	@param[in] ei ������� ����������.
	 * 	@retval True ���� ����������.
	 * 	@retval False ���� �������.
	 *
	 */
	bool getValue(EInfoElement2 ei) const;

	/**	���������� ��������� ����� ���������� ��������.
	 *
	 * 	@param[in] ei ������� ����������.
	 *	@retval True ���� ����������.
	 *	@retval False ���� �������.
	 */
	bool getDevice(EInfoElement2 ei) const;

	/**	���������� ��������� ����� ����� ����������.
	 *
	 * 	@param[in] ei ����� �����.
	 *	@retval True ���� ����������.
	 *	@retval False ���� �������.
	 */
	bool getGlb(EInfoElement2 ei) const;

	/**	���������� ��������� ����� �����������.
	 *
	 * 	@param[in] ei ����� �����.
	 *	@retval True ���� ����������.
	 *	@retval False ���� �������.
	 */
	bool getPrd(EInfoElement2 ei) const;

	/**	���������� ��������� ����� ���������.
	 *
	 * 	@param[in] ei ����� �����.
	 *	@retval True ���� ����������.
	 *	@retval False ���� �������.
	 */
	bool getPrm(EInfoElement2 ei) const;

	/**	���������� ��������� ����� ������.
	 *
	 * 	@param[in] ei ����� �����.
	 *	@retval True ���� ����������.
	 *	@retval False ���� �������.
	 */
	bool getDef(EInfoElement2 ei) const;
};

#endif /* PROTOCOLPCI_H_ */
