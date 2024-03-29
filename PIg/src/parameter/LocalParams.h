/*
 * LocalParams.h
 *
 *  Created on: 25.03.2015
 *      Author: Shcheblykin
 */

#ifndef LOCALPARAMS_H_
#define LOCALPARAMS_H_

#include "param.h"

class LocalParams
{
    /// ������������ ���������� ���������� � ������.
    static const uint8_t MAX_NUM_OF_PARAMS = 25;

    struct prop_t
    {
        eGB_PARAM param;
        bool      readOnly;
    };

public:
    /// ������� ��������� �������� ���������
    enum STATE
    {
        STATE_NO_ERROR = 0,  ///< �������� ���������.
        STATE_READ_PARAM,    ///< ���� ���������� ���������.
        STATE_ERROR          ///< �������� ���������.
    };

    /// �����������
    LocalParams();

    /** ��������� ������ �������� ��������� � ��� �������� �� ������������.
     *
     * 	@param val ����� �������� ���������.
     */
    void setValue(int16_t val);

    /**	���������� ������� �������� ���������.
     *
     *	���� ������� �������� ������� (\a Param::PARAM_BITES), �� ������������
     *	�������� 0 ��� 1 ��� �������� ���.������ \a currSameParam. �����
     *	������������ ������� �������� ���������.
     *
     * 	@return ������� �������� ���������.
     */
    int16_t getValue() const;

    /**	���������� ������� ���� ��� ������� ����������.
     *
     * 	@return ������� ���� ��� �������� ���������.
     */
    uint8_t getValueB() const;

    /**	���������� ������� ��������� ���������.
     *
     * 	@retval True ��������� ��������.
     * 	@retval False ���������� ��������.
     */
    STATE getState() const { return state; }

    /**	������� � ���������� ���������.
     *
     * 	���� �������� ����� ������, ���������� ������ �� ������ ��������.
     * 	���� ��� ����������� ������� (���-�� ���������� ������ 1), �� ����
     * 	���������� �������� ���������.
     *
     * 	���� �������� ��� �������, �� �������������� ���� ���������� \a refresh.
     */
    void nextParam();

    /**	������� � ����������� ���������.
     *
     * 	���� �������� ������ ������, ���������� ������ �� ��������� ��������.
     * 	���� ��� ����������� ������� (���-�� ���������� ������ 1), �� ����
     * 	���������� �������� ���������.
     *
     * 	���� �������� ��� �������, �� �������������� ���� ���������� \a refresh.
     */
    void prevParam();

    /** ������� � ���������� ����������� ���������.
     *
     *	���� �������� ����� ������ , ���������� ������ �� ������ ��������.
     * 	���� ��� ����������� ������� (���-�� ���������� ���������� ������ 1),
     * 	�� ���� ���������� �������� ���������.
     */
    void nextSameParam();

    /** ������� � ����������� ����������� ���������.
     *
     *	���� �������� ������ ������, ���������� ������ �� ��������� ��������.
     *	���� ��� ����������� ������� (���-�� ���������� ���������� ������ 1),
     * 	�� ���� ���������� �������� ���������.
     */
    void prevSameParam();

    /** ���������� � ������ ������ ���������.
     *
     * 	� ������ ������������ ������ ����������, ����� ���������� ������.
     *
     * 	@param[in] newParam ��������.
     * 	@param[in] readonly true - ������ ��� ������, ����� false.
     * 	@retval True �������� ��������.
     * 	@retval False ����� ����������, �������� ����� ��������.
     *
     */
    bool addParam(eGB_PARAM newParam, bool readonly = false);

    /**	��������� ������� �������� �� ����������� ������ ������.
     * 	@return true ���� �������� ����� ������ �������������, ����� false.
     */
    bool isReadOnly();

    /**	������� ������ ����������.
     *
     * 	�� ��������� ��������������� ������ �������� �� ������
     * 	\a GB_PARAM_TIME_SYNCH, ��� ���� ���������� ���������� ����������.
     */
    void clearParams();

    /**	���������� ����������� �������� ���������.
     *
     * 	@return ����������� �������� ���������.
     */
    int16_t getMin() const;

    /**	���������� ������������ �������� ���������.
     *
     *	��� ��������� ����������, � ��������� ������� �������� ���-�� ���������
     *	������, ����� ���������� �������� ������������� �������� � ������
     *	������������ max = min + flash_max - 1.
     *
     * 	@return ������������ �������� ���������.
     */
    int16_t getMax() const;

    /**	���������� ����� �������� ���������.
     *
     * 	@return ����� �������� ���������.
     */
    uint8_t getNumOfCurrParam() const { return currParam + 1; }

    /**	���������� ���������� ������� ����������.
     *
     * 	@return ���������� ������� ����������.
     */
    uint8_t getNumOfParams() const { return numOfParams; }

    /**	���������� ������� ����� ����������� ��������� ������� � 1.
     *
     * 	���� ����� �������� ��������� ��������� ������������ ����������, �����
     * 	��������� ��������.
     *
     * 	@return ������� ����� ����������� ���������.
     */
    uint8_t getNumOfCurrSameParam() const;

    /**	���������� ���������� ���������� ���������.
     *
     * 	��������, ���� ��� ������ �� ������ �������� ���������� ���� ��������
     * 	"������������ �������", �� ��� ����� ���������� ������.
     *
     * 	@return ���������� ���������� ����������.
     */
    uint8_t getNumOfSameParams() const;

    /**	���������� ������� ��������.
     *
     * 	@return ������� ��������.
     */
    eGB_PARAM getParam() const { return param[currParam].param; }

    /** ��������� ���-�� ������ �� ��������.
     *
     * 	@param numComPrd ���-�� ������ �� ��������.
     */
    void setNumComPrd(uint8_t numComPrd) { this->numComPrd = numComPrd; }

    /** ��������� ���-�� ������ �� ������.
     *
     * 	@param numComPrd ���-�� ������ �� ������.
     */
    void setNumComPrm(uint8_t numComPrm) { this->numComPrm = numComPrm; }

    /**	��������� ���������� ��������� � �����
     *
     * � ������ ���������� �������� ����� ����������� 2.
     *
     * 	@param[in] numDevices ���������� ��������� � �����
     * 	@argval 2
     * 	@argval 3
     */
    void setNumDevices(uint8_t numDevices) { this->numDevices = (numDevices == 3) ? (3) : (2); }

private:
    prop_t  param[MAX_NUM_OF_PARAMS];  ///< ������ ����������.
    int16_t val;                       ///< �������� �������� ���������.
    uint8_t currParam;                 ///< ����� �������� ���������.
    uint8_t numOfParams;  ///< ���������� ���������� � ������� ������.
    uint8_t currSameParam;  ///< ����� �������� ����������� ���������.
    STATE   state;          ///< ���� ������ � ������� ��������.
    uint8_t numComPrm;      ///< ���������� ������ �� ������.
    uint8_t numComPrd;      ///< ���������� ������ �� ��������.
    uint8_t numDevices;     ///< ���������� ��������� � �����.

    /**	�������� �������������� �������� ��������� �� ������������.
     *
     * 	� ����� ���������� �������� � ��������� \a state ����� ��������
     * 	�������� STATE_ERROR, ���� ��� ����� - \a STATE_NO_ERROR.
     *
     *	@see STATE_ERROR
     *	@see STATE_NO_ERROR
     */
    void checkValue();

    /**	���������� ���������.
     *
     * 	������� ������ \a state ������������ � �������� ������ STATE_READ_PARAM,
     * 	� �������� ��������� ������������ � 0.
     *
     */
    void refreshParam();
};

#endif /* LOCALPARAMS_H_ */
