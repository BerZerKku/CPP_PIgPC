#ifndef PARAM_H_
#define PARAM_H_

#include "avr.h"
#include "glbDefine.h"
#include <stdint.h>

struct Param
{

    /// ���������� ��������� (������� � fcDimension)
    typedef enum
    {
        DIM_NO = 0,  ///< ���
        DIM_MA,      ///< ��
        DIM_A,       ///< �
        DIM_KA,      ///< ��
        DIM_V,       ///< �
        DIM_KV,      ///< ��
        DIM_MSEC,    ///< ��
        DIM_SEC,     ///< �
        DIM_HOUR,    ///< ���
        DIM_DB,      ///< ��
        DIM_HZ,      ///< ��
        DIM_KHZ,     ///< ���
        DIM_DEG,     ///< ����
        DIM_MIN,     ///< ���
        //
        DIM_MAX
    } DIMENSION;

    /// ��� ������ ��� ����������� ��������� �������� ���������.
    typedef enum
    {
        RANGE_LIST = 0,    ///< ������.
        RANGE_ON_OFF,      ///< ������ ���./����.
        RANGE_INT,         ///< <min>..<max><dim>
        RANGE_INT_NO_DIM,  ///< <min>..<max>
        RANGE_U_COR,       ///< 0..�<max/10><dim>
        RANGE_INT_PM       ///< 0..�<max><dim>
    } RANGE_TYPE;

    /// ��� ���������, � ����� ����� / �����������.
    typedef enum
    {
        PARAM_NO = 0,  ///< ������������ ��������.
        PARAM_LIST,    ///< ������.
        PARAM_INT,     ///< ����� �������� jn -32768..32767.
        PARAM_U_COR,   ///< ��������� ���������� ���.�.
        PARAM_I_COR,   ///< ��������� ���� �XXX.
        PARAM_BITES    ///< ����� ������� ��������.
    } PARAM_TYPE;

    /// ����������� ��������� ���������.
    typedef enum
    {
        DEPEND_MAX_NO,          ///< ��� �����������.
        DEPEND_MAX_ON_COM_PRD,  ///< ������� �� ���������� ������ �� ���.
        DEPEND_MAX_ON_NUM_DEVS  ///< ������� �� ���������� ������ �� ���.
    } DEPEND_MAX;

    /// ����������� �������� ���������.
    typedef enum
    {
        DEPEND_SAME_NO = 0,  ///< ��� �����������.
        DEPEND_SAME_ON_NUM_DEVS,  ///< ������� �� ���������� ��������� � �����.
        DEPEND_SAME_ON_COM_PRD,  ///< ������� �� ���������� ������ �� ���.
        DEPEND_SAME_ON_COM_PRM,  ///< ������� �� ���������� ������ �� ���.
        //
        DEPEND_SAME_MAX  ///<  ������������ ���������� ������������.
    } DEPEND_SAME;

    /// ������� ��� ��������� ���������.
    typedef enum
    {
        CHANGE_REG_NO,  ///< ��� �������.
        CHANGE_REG_DISABLE  ///< �������� ����� ������ � ������ "�������".
    } CHANGE_REG;

    char name[NAME_PARAM_LENGHT];  ///< ��� ���������.
    eGB_COM com;  ///< ������� ���������� ��� ������������ ���������.

    PARAM_TYPE param;  ///< ��� ���������.
    RANGE_TYPE range;  ///< ��� ������ ��� ������ ��������� ��������.
    DIMENSION dim;  ///< ����������� ���������.
    PGM_P listValues;  ///< ������ ���������� ��� ��������� ����������.

    uint8_t num;  ///< ���������� �������� ����������.
    int16_t min;  ///< ����������� �������� ���������.
    int16_t max;  ///< ������������ �������� ��������� ��� ���-�� ��������� � ������
    uint8_t disc;   ///< ������������.
    uint8_t fract;  ///< ��������� ��� ������ �� ��������� � ���.

    eGB_SEND_TYPE send;  ///< ��� ��������� ��� ���������� ������ ��������.
    uint8_t sendDop;  ///< ���� �������������� ���������� ��� ����������.
    DEPEND_MAX dependMax;  ///< ����������� ��������� �������� ���������.
    DEPEND_SAME dependSame;  ///< ����������� ���������� �������� ���������.
    CHANGE_REG changeReg;  ///< ����� ��� ��������� ���������.
};

extern const char         fcNullBuf[] PROGMEM;
extern const Param* const fParams[] PROGMEM;

// ���������� ��������� �� ��������� ���������.
extern const Param* getPtrParam(eGB_PARAM pn);

// ���������� ������� �������� ���������.
extern int16_t getAbsMin(eGB_PARAM pn);
// ���������� ���������� �������� �������� ���������.
extern int16_t getAbsMax(eGB_PARAM pn);
// ���������� ���������� �������� ���������� ���������� ����������.
extern uint8_t getAbsMaxNumOfSameParams(eGB_PARAM pn);
// ���������� ���������� � ������ ��� ��������� ���������.
extern Param::CHANGE_REG getChangeReg(eGB_PARAM pn);
// ���������� ������� ������������ ��������� ��� ��������.
extern eGB_COM getCom(eGB_PARAM pn);
// ���������� ����������� ��������� ��� ���������.
extern Param::DEPEND_MAX getDependMax(eGB_PARAM pn);
// ���������� ����������� ���������� ��� ���������.
extern Param::DEPEND_SAME getDependSame(eGB_PARAM pn);
// ���������� ����������� ���������.
extern Param::DIMENSION getDim(eGB_PARAM pn);
// ���������� ������������ ��� �������� ���������.
extern uint8_t getDisc(eGB_PARAM pn);
// ���������� ��������� �������� ��� ���������.
extern uint8_t getFract(eGB_PARAM pn);
// ���������� ��������� �� ������ ������ �������� ���������.
extern PGM_P getListOfValues(eGB_PARAM pn);
// ���������� �������� �� ������
extern PGM_P getTextValue(eGB_PARAM pn, uint8_t value);
// ���������� ��������� �� ������ � ��������� ���������.
extern PGM_P getNameOfParam(eGB_PARAM pn);
// ���������� ��� ���������.
extern Param::PARAM_TYPE getParamType(eGB_PARAM pn);
// ���������� ��� ��������� �������� ��� ���������.
extern Param::RANGE_TYPE getRangeType(eGB_PARAM pn);
// ���������� �������� ����� ���. ���������� ��� ���������� ������ ��������.
extern uint8_t getSendDop(eGB_PARAM pn);
// ���������� ��� ��������� ��� ���������� ������ ��������.
extern eGB_SEND_TYPE getSendType(eGB_PARAM pn);

#endif /* PARAM_H_ */
