/*
 * measure.h
 *
 *  Created on: 17 ���. 2021 �.
 *      Author: Shcheblykin
 */

#ifndef MEASURE_H_
#define MEASURE_H_

#include <stdint.h>

/**
 * @brief ����� ���������� ����������.
 */
class TMeasure
{
public:
    TMeasure()
    {
        mVoltDef     = 0;
        mVoltDef2    = 0;
        mVoltCf      = 0;
        mVoltCf2     = 0;
        mVoltNoise   = 0;
        mVoltNoise2  = 0;
        mVoltOut     = 0;
        mCurOut      = 0;
        mResistOut   = 0;
        mPulseWidth  = 0;
        mD           = 0;
        mTemperature = -100;
        mFreqDev     = 0;
    }

    /**
     * @brief ������������� ����� �� ������.
     * @param[in] valu� �������� [-99..99].
     * @return true ���� �������� ���� �����������, ����� false.
     */
    bool setVoltageDef(int8_t value)
    {
        bool check = (value > -100) && (value < 100);

        if (check)
        {
            mVoltDef = value;
        }

        return check;
    }

    /**
     * @brief ���������� ����� �� ������.
     * @return ����� �� ������.
     */
    int8_t getVoltageDef() const { return mVoltDef; }

    /**
     * @brief ������������� ����� �� ������ 2.
     * @param[in] valu� �������� [-99..99].
     * @return true ���� �������� ���� �����������, ����� false.
     */
    bool setVoltageDef2(int8_t value)
    {
        bool check = (value > -100) && (value < 100);

        if (check)
        {
            mVoltDef2 = value;
        }

        return check;
    }

    /**
     * @brief ���������� ����� �� ������ 2.
     * @return ����� �� ������.
     */
    int8_t getVoltageDef2() const { return mVoltDef2; }

    /**
     * @brief ������������� ����� �� ��.
     * @param[in] valu� �������� [-99..99].
     * @return true ���� �������� ���� �����������, ����� false.
     */
    bool setVoltageCf(int8_t value)
    {
        bool check = (value > -100) && (value < 100);

        if (check)
        {
            mVoltCf = value;
        }

        return check;
    }

    /**
     * @brief ���������� ����� �� ��.
     * @return ����� �� ������.
     */
    int8_t getVoltageCf() const { return mVoltCf; }

    /**
     * @brief ������������� ����� �� ��2.
     * @param[in] valu� �������� [-99..99].
     * @return true ���� �������� ���� �����������, ����� false.
     */
    bool setVoltageCf2(int8_t value)
    {
        bool check = (value > -100) && (value < 100);

        if (check)
        {
            mVoltCf2 = value;
        }

        return check;
    }

    /**
     * @brief ���������� ����� �� ��2.
     * @return ����� �� ������.
     */
    int8_t getVoltageCf2() const { return mVoltCf2; }

    /**
     * @brief ������������� ���������� ������.
     * @param[in] valu�Int �������� ����� ����� [0..99].
     * @param[in] valu�Fract �������� ������� ����� [0..9].
     * @return true ���� �������� ���� �����������, ����� false.
     */
    bool setVoltageOut(uint8_t valueInt, uint8_t valueFract)
    {
        bool check = (valueInt < 100) && (valueFract < 10);

        if (check)
        {
            mVoltOut = static_cast<uint16_t>(valueInt) * 10;
            mVoltOut += valueFract;
        }

        return check;
    }

    /**
     * @brief ���������� ���������� ������.
     * @return ���������� ������.
     */
    uint16_t getVoltageOut() const { return mVoltOut; }

    /**
     * @brief ���������� ����� ����� ��� ���������� ������.
     * @return ����� ����� ���������� ������
     */
    uint8_t getVoltageOutInt() const { return static_cast<uint8_t>(mVoltOut / 10); }

    /**
     * @brief ���������� ������� ����� ��� ���������� ������.
     * @return ������� ����� ���������� ������
     */
    uint8_t getVoltageOutFract() const { return static_cast<uint8_t>(mVoltOut % 10); }

    /**
     * @brief ������������� ��� ������.
     * @param[in] valu� �������� [0..999].
     * @return true ���� �������� ���� �����������, ����� false.
     */
    bool setCurrentOut(uint16_t value)
    {
        bool check = value < 1000;

        if (check)
        {
            mCurOut = value;
        }

        return check;
    }

    /**
     * @brief ���������� ���� ������.
     * @return ��� ������.
     */
    uint16_t getCurrentOut() const { return mCurOut; }

    /**
     * @brief ������������� ������������� ������.
     * @param[in] valu� �������� [0..999].
     * @return true ���� �������� ���� �����������, ����� false.
     */
    bool setResistOut(uint16_t value)
    {
        bool check = value < 1000;

        if (check)
        {
            mResistOut = value;
        }

        return check;
    }

    /**
     * @brief ���������� ������������� ������.
     * @return ������������� ������.
     */
    uint16_t getResistOut() const { return mResistOut; }

    /**
     * @brief ������������� ������� ����.
     * @param[in] valu� �������� [-99..99].
     * @return true ���� �������� ���� �����������, ����� false.
     */
    bool setVoltageNoise(int8_t value)
    {
        bool check = (value > -100) && (value < 100);

        if (check)
        {
            mVoltNoise = value;
        }

        return check;
    }

    /**
     * @brief ���������� ������� ����.
     * @return ������� ����.
     */
    int8_t getVoltageNoise() const { return mVoltNoise; }

    /**
     * @brief ������������� ������� ���� 2.
     * @param[in] valu� �������� [-99..99].
     * @return true ���� �������� ���� �����������, ����� false.
     */
    bool setVoltageNoise2(int8_t value)
    {
        bool check = (value > -100) && (value < 100);

        if (check)
        {
            mVoltNoise2 = value;
        }

        return check;
    }

    /**
     * @brief ���������� ������� ���� 2.
     * @return ������� ����.
     */
    int8_t getVoltageNoise2() const { return mVoltNoise2; }

    /**
     * @brief ������������� ������������ ��������� �� ���������� �� ������ ���������.
     * @param[in] valu� �������� [0..360].
     * @return true ���� �������� ���� �����������, ����� false.
     */
    bool setPulseWidth(uint16_t value)
    {
        bool check = (value <= 360);

        if (check)
        {
            mPulseWidth = value;
        }

        return check;
    }

    /**
     * @brief ���������� ������������ ��������� �� ���������� �� ������ ���������.
     * @return ������������ ��������� �� ���������� �� ������ ���������.
     */
    uint16_t getPulseWidth() const { return mPulseWidth; }


    /**
     * @brief ������������� ��������� ������/������ (��������).
     *
     * !!! � ������������� �������� ��� "����� �� �������� �������".
     *
     * @param[in] valu� �������� [-64..64].
     * @return true ���� �������� ���� �����������, ����� false.
     */
    bool setD(int8_t value)
    {
        bool check = (value >= -64) && (value <= 64);

        if (check)
        {
            mD = value;
        }

        return check;
    }

    /**
     * @brief ���������� ��������� ������/������ (��������).
     *
     * !!! � ������������� �������� ��� "����� �� �������� �������".
     *
     * @return ��������� ������/������ (��������)
     */
    int8_t getD() const { return mD; }

    /**
     * @brief ������������� �����������.
     * @param[in] valu� �������� [-100..125].
     * @return true ���� �������� ���� �����������, ����� false.
     */
    bool setTemperature(int8_t value)
    {
        bool check = (value >= -100) && (value <= 125);

        if (check)
        {
            mTemperature = value;
        }

        return check;
    }

    /**
     * @brief ���������� �����������.
     * @return �����������.
     */
    int8_t getTemperature() const { return mTemperature; }

    /**
     * @brief ������������� ���������� ������� �� �� ���.
     * @param[in] valu� �������� [0..100].
     * @return true ���� �������� ���� �����������, ����� false.
     */
    bool setFreqDev(uint8_t value)
    {
        bool check = (value <= 100);

        if (check)
        {
            mFreqDev = value;
        }

        return check;
    }

    /**
     * @brief ���������� ���������� ������� �� �� ���.
     * @return ���������� ������� �� �� ���.
     */
    uint8_t getFreqDev() const { return mFreqDev; }

private:
    int8_t   mVoltDef;
    int8_t   mVoltDef2;
    int8_t   mVoltCf;
    int8_t   mVoltCf2;
    int8_t   mVoltNoise;
    int8_t   mVoltNoise2;
    uint16_t mVoltOut;
    uint16_t mCurOut;
    uint16_t mResistOut;
    uint16_t mPulseWidth;
    int8_t   mD;
    int8_t   mTemperature;
    uint8_t  mFreqDev;
};

#endif /* MEASURED_PARAMETERS_H_ */
