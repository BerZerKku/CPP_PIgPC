/*
 * measure.h
 *
 *  Created on: 17 июн. 2021 г.
 *      Author: Shcheblykin
 */

#ifndef MEASURE_H_
#define MEASURE_H_

#include <stdint.h>

/**
 * @brief Класс измеряемых параметров.
 */
class TMeasure
{
public:
    TMeasure() {
        mVoltDef = 0;
        mVoltDef2 = 0;
        mVoltCf = 0;
        mVoltCf2 = 0;
        mVoltNoise = 0;
        mVoltNoise2 = 0;
        mVoltOut = 0;
        mCurOut = 0;
        mResistOut = 0;
        mPulseWidth = 0;
        mD = 0;
        mTemperature = -100;
        mFreqDev = 0;
    }

    /**
     * @brief Устанавливает запас по защите.
     * @param[in] valuе Значение [-99..99].
     * @return true если значение было установлено, иначе false.
     */
    bool setVoltageDef(int8_t value)
    {
        bool check = (value > -100) && (value < 100);

        if (check) {
            mVoltDef = value;
        }

        return check;
    }

    /**
     * @brief Возвращает запас по защите.
     * @return Запас по защите.
     */
    int8_t getVoltageDef() const { return mVoltDef; }

    /**
     * @brief Устанавливает запас по защите 2.
     * @param[in] valuе Значение [-99..99].
     * @return true если значение было установлено, иначе false.
     */
    bool setVoltageDef2(int8_t value)
    {
        bool check = (value > -100) && (value < 100);

        if (check) {
            mVoltDef2 = value;
        }

        return check;
    }

    /**
     * @brief Возвращает запас по защите 2.
     * @return Запас по защите.
     */
    int8_t getVoltageDef2() const { return mVoltDef2; }

    /**
     * @brief Устанавливает запас по КЧ.
     * @param[in] valuе Значение [-99..99].
     * @return true если значение было установлено, иначе false.
     */
    bool setVoltageCf(int8_t value)
    {
        bool check = (value > -100) && (value < 100);

        if (check) {
            mVoltCf = value;
        }

        return check;
    }

    /**
     * @brief Возвращает запас по КЧ.
     * @return Запас по защите.
     */
    int8_t getVoltageCf() const { return mVoltCf; }

    /**
     * @brief Устанавливает запас по КЧ2.
     * @param[in] valuе Значение [-99..99].
     * @return true если значение было установлено, иначе false.
     */
    bool setVoltageCf2(int8_t value)
    {
        bool check = (value > -100) && (value < 100);

        if (check) {
            mVoltCf2 = value;
        }

        return check;
    }

    /**
     * @brief Возвращает запас по КЧ2.
     * @return Запас по защите.
     */
    int8_t getVoltageCf2() const { return mVoltCf2; }

    /**
     * @brief Устанавливает напряжение выхода.
     * @param[in] valuеInt Значение целой части [0..99].
     * @param[in] valuеFract Значение дробной части [0..9].
     * @return true если значение было установлено, иначе false.
     */
    bool setVoltageOut(uint8_t valueInt, uint8_t valueFract)
    {
        bool check = (valueInt< 100) && (valueFract < 10);

        if (check) {
            mVoltOut = static_cast<uint16_t> (valueInt) * 10;
            mVoltOut += valueFract;
        }

        return check;
    }

    /**
     * @brief Возвращает напряжение выхода.
     * @return Напряжение выхода.
     */
    uint16_t getVoltageOut() const { return mVoltOut; }

    /**
     * @brief Возвращает целую часть для напряжения выхода.
     * @return Целая часть напряжения выхода
     */
    uint8_t getVoltageOutInt() const {
        return static_cast<uint8_t> (mVoltOut / 10);
    }

    /**
     * @brief Возвращает дробную часть для напряжения выхода.
     * @return Дробная часть напряжения выхода
     */
    uint8_t getVoltageOutFract() const {
        return static_cast<uint8_t> (mVoltOut % 10);
    }

    /**
     * @brief Устанавливает ток выхода.
     * @param[in] valuе Значение [0..999].
     * @return true если значение было установлено, иначе false.
     */
    bool setCurrentOut(uint16_t value) {
        bool check = value < 1000;

        if (check) {
            mCurOut = value;
        }

        return check;
    }

    /**
     * @brief Возвращает току выхода.
     * @return Ток выхода.
     */
    uint16_t getCurrentOut() const { return mCurOut; }

    /**
     * @brief Устанавливает сопротивление выхода.
     * @param[in] valuе Значение [0..999].
     * @return true если значение было установлено, иначе false.
     */
    bool setResistOut(uint16_t value)
    {
        bool check = value < 1000;

        if (check) {
            mResistOut = value;
        }

        return check;
    }

    /**
     * @brief Возвращает сопротивление выхода.
     * @return Сопротивление выхода.
     */
    uint16_t getResistOut() const { return mResistOut; }

    /**
     * @brief Устанавливает уровень шума.
     * @param[in] valuе Значение [-99..99].
     * @return true если значение было установлено, иначе false.
     */
    bool setVoltageNoise(int8_t value)
    {
        bool check = (value > -100) && (value < 100);

        if (check) {
            mVoltNoise = value;
        }

        return check;
    }

    /**
     * @brief Возвращает уровень шума.
     * @return Уровень шума.
     */
    int8_t getVoltageNoise() const { return mVoltNoise; }

    /**
     * @brief Устанавливает уровень шума 2.
     * @param[in] valuе Значение [-99..99].
     * @return true если значение было установлено, иначе false.
     */
    bool setVoltageNoise2(int8_t value)
    {
        bool check = (value > -100) && (value < 100);

        if (check) {
            mVoltNoise2 = value;
        }

        return check;
    }

    /**
     * @brief Возвращает уровень шума 2.
     * @return Уровень шума.
     */
    int8_t getVoltageNoise2() const { return mVoltNoise2; }

    /**
     * @brief Устанавливает длительность импульсов ВЧ блокировки на выходе применика.
     * @param[in] valuе Значение [0..360].
     * @return true если значение было установлено, иначе false.
     */
    bool setPulseWidth(uint16_t value)
    {
        bool check = (value <= 360);

        if (check) {
            mPulseWidth = value;
        }

        return check;
    }

    /**
     * @brief Возвращает длительность импульсов ВЧ блокировки на выходе применика.
     * @return Длительность импульсов ВЧ блокировки на выходе применика.
     */
    uint16_t getPulseWidth() const { return mPulseWidth; }


    /**
     * @brief Устанавливает отношение сигнал/помеха (одночаст).
     *
     * !!! В двухчастотном варианте это "Запас по тестовой команде".
     *
     * @param[in] valuе Значение [-64..64].
     * @return true если значение было установлено, иначе false.
     */
    bool setD(int8_t value)
    {
        bool check = (value >= -64) && (value <= 64);

        if (check) {
            mD = value;
        }

        return check;
    }

    /**
     * @brief Возвращает отношение сигнал/помеха (одночаст).
     *
     * !!! В двухчастотном варианте это "Запас по тестовой команде".
     *
     * @return Отношение сигнал/помеха (одночаст)
     */
    int8_t getD() const { return mD; }

    /**
     * @brief Устанавливает температуру.
     * @param[in] valuе Значение [-100..125].
     * @return true если значение было установлено, иначе false.
     */
    bool setTemperature(int8_t value)
    {
        bool check = (value >= -100) && (value <= 125);

        if (check) {
            mTemperature = value;
        }

        return check;
    }

    /**
     * @brief Возвращает температуру.
     * @return Температура.
     */
    int8_t getTemperature() const { return mTemperature; }

    /**
     * @brief Устанавливает отклонение частоты КС на ПРМ.
     * @param[in] valuе Значение [0..100].
     * @return true если значение было установлено, иначе false.
     */
    bool setFreqDev(uint8_t value)
    {
        bool check = (value <= 100);

        if (check) {
            mFreqDev = value;
        }

        return check;
    }

    /**
     * @brief Возвращает отклонение частоты КС на ПРМ.
     * @return Отклонение частоты КС на ПРМ.
     */
    uint8_t getFreqDev() const { return mFreqDev; }

private:
    int8_t mVoltDef;
    int8_t mVoltDef2;
    int8_t mVoltCf;
    int8_t mVoltCf2;
    int8_t mVoltNoise;
    int8_t mVoltNoise2;
    uint16_t mVoltOut;
    uint16_t mCurOut;
    uint16_t mResistOut;
    uint16_t mPulseWidth;
    int8_t mD;
    int8_t mTemperature;
    uint8_t mFreqDev;
};

#endif /* MEASURED_PARAMETERS_H_ */
