/*
 * menu.h
 *
 *  Created on: 01.05.2012
 *      Author: ������
 */

#ifndef MENU_H_
#define MENU_H_

#include "glbDefine.h"
#include "paramBsp.h"
#include "drivers/keyboard.h"
#include "menu/menuPunkt.h"
#include "menu/enterParam.h"
#include "parameter/LocalParams.h"


/// ����� �� ������ ������������� �������, ��
#define TIME_TO_INIT_LCD (200 / MENU_TIME_CYLCE)

/// ����� �� ����������������� �������, ��
#define TIME_TO_REINIT_LCD (10000 / MENU_TIME_CYLCE)

/// ������������ ���-�� ������������ �� ������ ����������
#define MAX_NUM_MEAS_PARAM 6

/// ������������ ���-�� ������������ ���������� � ���� "���������"
#define MAX_NUM_MEAS_PARAM_LVL 10

/// ����� ������ ����������� ��������� �� �����, ��
#define TIME_MEAS_PARAM (3000 / MENU_TIME_CYLCE)

/// ����� ������ ������ �� �����, �� (�������� ������/��� ������)
#define TIME_TEXT (1000 / MENU_TIME_CYLCE)

/// ����� �� ������ �� ���� 1 / ���� 2 � �.�. ���� ����� != ������� �����, ��
#define TIME_TEST_EXIT (1000 / MENU_TIME_CYLCE)

/// ����� �� �������� �� ��������� �������, �
#define TIME_RETURN_LVL_START (5 * 60 * (1000 / MENU_TIME_CYLCE))

/// ���������� ���������
enum eMENU_MEAS_PARAM {
	MENU_MEAS_PARAM_NO,			///< ��� ����������� ���������
	MENU_MEAS_PARAM_DATE,		///< ����
	MENU_MEAS_PARAM_TIME,		///< �����
	MENU_MEAS_PARAM_UZ,			///< ����� �� ������
	MENU_MEAS_PARAM_UZ1,		///< ����� �� ������ 1 (��� 3-� ��������)
	MENU_MEAS_PARAM_UZ2,		///< ����� �� ������ 2 (��� 3-� ��������)
	MENU_MEAS_PARAM_UC,			///< ����� �� ��
	MENU_MEAS_PARAM_UC1,		///< ����� �� �� 1 (��� 3-� ��������)
	MENU_MEAS_PARAM_UC2,		///< ����� �� �� 2 (��� 3-� ��������)
	MENU_MEAS_PARAM_UN,			///< ������� �����
	MENU_MEAS_PARAM_UN1,		///< ������� ����� 1 (��� 3-� ��������)
	MENU_MEAS_PARAM_UN2,		///< ������� ����� 2 (��� 3-� ��������)
	MENU_MEAS_PARAM_UOUT,		///< ��������� ������
	MENU_MEAS_PARAM_IOUT,		///< ��� ������
	MENU_MEAS_PARAM_ROUT,		///< ������������� �����
	MENU_MEAS_PARAM_SD,			///< �������� � �������
	MENU_MEAS_PARAM_D,			///< ����� �� ����.������� (��������) ��� ��������� ������/������ (��������)
	MENU_MEAS_PARAM_TEMPERATURE,///< ����������� �� ����� ���-��
	MENU_MEAS_PARAM_DF			///< ���������� ������� �� �� ��� (�400)
};



// ����� ����
class clMenu {
public:

	/**	�����������.
	 * 	���������������� �� ��������� �������������� ��� ��������.
	 * 	����������� ����������� ��� ������ �������.
	 * 	������������ ������� �� ��������� ������� ����.
	 */
	clMenu();

	/**	������ � ����.
	 */
	void main();

	/** ��������� ���� �������� � ��������� ���� � ��� ������.
	 * 	�� ��������� ����� ������ ����� ������ �� �������� ���������� �
	 * 	���������� ��������� ���������.
	 * 	@param device ��� ����������
	 * 	@return False � ������ ������, ����� True
	 */
	bool setDevice(eGB_TYPE_DEVICE device = AVANT_NO);

	/**	���������� ��������� ������� �� ����������.
	 *
	 * 	������� ����������� ������� �������, ���� �� ��� ���� ������� �������.
	 *
	 * 	������ ���� ��������� �� 8 ������ ������������:
	 * 	- ������� ���������;
	 * 	- �����;
	 * 	- ������� �� ������ 2
	 * 		- ������ ������ ���� ������� ��������, ���� ����
	 * 		- ����� ������� ����������� ��� ��������� ���� (������������� � �.�.)
	 * 	- ������� �� ������ 1
	 * 		������� ������ ��������� ���������� � ������ ������ ����
	 * 	- ���� ������ ���, �� ������� ����� ������������.
	 *
	 * 	@param ���
	 * 	@return �������
	 */
	eGB_COM getTxCommand();

	/** ���������� ������� ��������� ����� � ���.
	 * 	@retval True - ����� � ��� ����.
	 * 	@retval False - ����� � ��� ���.
	 */
	bool isConnectionBsp() const {
		return connectionBsp_;
	}

	/** ��������� ����� ������� ����� � ���.
	 * 	@retval True - ����� ����.
	 * 	@retval False - ����� ���.
	 */
	void setConnectionBsp(bool f) {
		connectionBsp_ = f;
	}

	/** ���������
	 */
	stGBparam sParam;

private:
	// ��� ������
	eKEY key_;

	// ���� ������� ��������
	bool blink_;

	// ���� ����� ���������� ����������
	bool blinkMeasParam_;

	// ���� �������� ���������� ����� � ���, True - ����
	bool connectionBsp_;

	// true - ���������� ������� �� ����� ������
	bool cursorEnable_;

	// ������� ��������� ������� (����� ������)
	uint8_t cursorLine_;

	// ���-�� ������������ ����������
	uint8_t lineParam_;

	// ������� �������� (�������� ����� ������� ������� ���������, ������� � 1)
	uint8_t curCom_;

	// ���� true - ���������� ������� ������� ����
	bool lvlCreate_;

	// ����� ������ ���.��������� �� ����� (�������� ��������� �� ������)
	uint8_t delay_;

	// ���������� ���������
	eMENU_MEAS_PARAM measParam[MAX_NUM_MEAS_PARAM * 2];

	// ���������� ��������� ��� ������ ���� "���������"
	eMENU_MEAS_PARAM measParamLvl[MAX_NUM_MEAS_PARAM_LVL];

	// ������� ������ ����
	TMenuPunkt Punkts_;

	// ��������� ��� ����� ����� ��������
	TEnterParam EnterParam;

	//  ��������� ��� ��������������� ���������
	bool setDeviceK400();
	bool setDeviceRZSK();
	bool setDeviceR400M();
	bool setDeviceOPTO();

	// ������� ���������� ������
	void clearTextBuf();

	// ������� ������
	void clearLine(uint8_t line);

	// ����� ��������� �� �����
	void printMessage() {
		delay_ = 0;
	}

	// ���������� true - � ������ ������������� ������ ���������
	bool isMessage() const {
		return (delay_ < TIME_MESSAGE);
	}

	// ����� �� ����� ����������� ���������
	void printMeasParam(uint8_t poz, eMENU_MEAS_PARAM par);

	// ����� �� ����� �������� ��������� ���������
	void printDevicesStatus(uint8_t poz, TDeviceStatus *device);

	// ����� � ������ ���� "�����" �������� ������ ���������
	void printDevicesRegime(uint8_t poz, TDeviceStatus *device);

	// ������ ����
	void lvlError();
	void lvlStart();
	void lvlFirst();
	void lvlInfo();
	void lvlJournal();
	void lvlJournalEvent();
	void lvlJournalDef();
	void lvlJournalPrm();
	void lvlJournalPrd();
	void lvlControl();
	void lvlSetup();
	void lvlRegime();
	void lvlRegimeAc();
	void lvlSetupParam();
	void lvlSetupParamDef();
	void lvlSetupParamPrm();
	void lvlSetupParamPrd();
	void lvlSetupParamGlb();
	void lvlSetupParamRing();
	void lvlSetupDT();
	void lvlMeasure();
	void lvlTest();
	void lvlTest1();
	void lvlTest2();
	void lvlSetupInterface();

	// ���� ���������
	eMENU_ENTER_PARAM enterValue();

	eMENU_ENTER_PARAM enterPassword();

	// ����������� ������� �����
	void cursorLineUp() {
		cursorLine_=(cursorLine_>1)? cursorLine_-1 : Punkts_.getMaxNumPunkts();
	}

	// ���������� ������� ����
	void cursorLineDown() {
		cursorLine_=(cursorLine_<Punkts_.getMaxNumPunkts())? cursorLine_+1 : 1;
	}

	// ����� �� ����� ������� ������� ���� � �������
	void printPunkts();

	/// ����� �� ����� �������� ���������.
	void printParam();

	/// ����� �� ����� �������� ���������.
	void printName();

	///	����� �� ����� �������� ������ � �� ���������� ��� ���������� ���-��.
	void printSameNumber(uint8_t pos);

	///	����� �� ����� ��������� �������� ���������.
	void printRange(uint8_t pos);

	/**	����� �� ����� �������� �������� ���������.
	 *
	 * 	���� �������� ��������� �������� � �������, ����� ���������� ���������
	 * 	��������������� ������� � ������� �������� ���������.
	 *
	 * 	��� ��������� ���������� ����������� ����������� ��������. �.�. ��
	 * 	�������� �������� ��������� ���������� ����������� ��������, � �����
	 * 	�������������� ����� �� ������� ����� �������� ���������.
	 */
	void printValue(uint8_t pos);

	/** ����� �� ����� ������ �� � ������� �� ��������� ��������.
	 *
	 * 	����� ��������� ���� ����������� �������:
	 * 	- �� �� ��������
	 *	- ����� "������"
	 *	- ��������� "��������"
	 *
	 *	� �400� ������������ ������� �������� ������� ��:
	 *	- � ������������� ������: ��-���� �������� �� ��-���;
	 *	- � ������������� ����-80 � ���-90: ��-���� �������� �� ��-����.
	 *
	 *	@param[in] pos ��������� ������� � ������.
	 */
	void printAc(uint8_t pos);

	/**	��������� ���������� ��� ����� �������� � ����������.
	 *
	 *	��� ���������� ���� \a Param::PARAM_NO ������� ����� �������� ��
	 *	����������.
	 *
	 * 	��� ���������� ���� \a Param::PARAM_LIST ���������� ������� ������
	 * 	�������� �� ������.
	 *
	 * 	��� ���������� ���� \a Param::PARAM_INT ���������� ������� �����
	 * 	������ �����.
	 *
	 * 	��� ���������� ���� \a Param::PARAM_U_COR ���������� ������� �����
	 * 	��������� ����������, � ����� ����������� �������� ����������� �� 0.
	 *
	 * 	��� ���������� ���� \a Param::PARAM_I_COR ���������� ������� ����� ������
	 * 	�����, ��� ���� ����������� �������� ����������� �� 0.
	 *
	 * 	��� ���������� ���� \a Param::PARAM_BITES ���������� ������� ������
	 * 	�������� �� ������.
	 */
	void enterParameter();

	/// ������������ ������ ��������� ���������� ��� ������.
	void crtListOfParamsDef();

	/// ������������ ������ ��������� ���������� ��� ���������.
	void crtListOfParamsPrm();

	/// ������������ ������ ��������� ���������� ��� �����������.
	void crtListOfParamsPrd();

	/// ������������ ������ ��������� ���������� ��� �����.
	void crtListOfParamsGlb();

	// ������� ������� �����
	eMENU_ENTER_PARAM (clMenu::*enterFunc)();

	// ������� ������� ����
	void (clMenu::*lvlMenu)();

	/** ������ � ���� ��������� ����������.
	 *
	 * 	���� ������� ���������� ���������� ����� 0, �� ����� ����������� �������
	 * 	� ����������� ������.
	 */
	void setupParam();

	/**	�������� ������������� � ���������.
	 *
	 *	������������� ��������� ��������� ���������� ��� ������� ����������
	 *	����������.
	 *
	 *	���:
	 *	- ����� �� "������";
	 *	- ������� ������������� ��� ��������������.
	 *	���:
	 *	- ��������� �� "��������";
	 *	- ������� ������������� ��� ��������������.
	 *	���:
	 *	- ��������� �� "��� ��";
	 *	- ������� ������������� ��� ��������������.
	 *	���:
	 *	- ��������� �� "��� ��";
	 *	- ������� ������������� ��� ��������������.
	 *
	 * 	@retval True �������� ���������.
	 * 	@retval False ������������� � ��������� ���.
	 */
	bool checkLedOn();

	/**	�������� �������� �������� ���� � ������������� �����.
	 *
	 * 	@return true ���� ���� � ������������� �����, ����� false.
	 */
	bool isRzskM() const;

	/** ���������� �������������� ������� ������ ����������.
	 *
	 *	@param[in] key ������� ������ ��� �������.
	 *	@return ��� ������ ���� ��� �� ���� ����������, ����� \a KEY_NO.
	 */
	eKEY onFnButton(eKEY key);
};

#endif /* MENU_H_ */
