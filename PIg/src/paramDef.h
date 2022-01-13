/*
 * deviceDef.h
 *
 *  Created on: 11.06.2014
 *      Author: Shcheblykin
 */

#ifndef PARAMDEF_H_
#define PARAMDEF_H_

#include "glbDefine.h"

/// �����������, ������������ �������� ���������� ��������� � �� ������������
/// ������

/// ��� ������������
enum eGB_TYPE_AC
{
    GB_TYPE_AC_MIN         = 1,  // ����������� ��������
    GB_TYPE_AC_AUTO_NORM   = 1,  // �� ���������� / �� �������
    GB_TYPE_AC_AUTO_REPEAT = 2,  // �� ���������
    GB_TYPE_AC_ACCEL       = 3,  // �� ����������
    GB_TYPE_AC_OFF         = 4,  // �� ��������
    GB_TYPE_AC_CHECK       = 5,  // �� ������
    GB_TYPE_AC_PUSK_SELF   = 6,  // ���� �� ����/���������/�����.����.
    GB_TYPE_AC_PUSK        = 7,  // �� ����
    GB_TYPE_AC_ONE_SIDE    = 8,  // �� �������������
    GB_TYPE_AC_MAX,              // ������������ ����������� ��������
    GB_TYPE_AC_AUTO,             // �������  GB_TYPE_AC_AUTO_NORM � �400�->������
    GB_TYPE_AC_CHECK_1  // ������� GB_TYPE_AC_CHECK � �400�->����-80 � ���-90
};

/// ����� ��� ���������� � �������� ������
class TDeviceDef
{
public:
    TDeviceDef()
    {
        numDevices_     = GB_NUM_DEVICES_MAX;
        typeAc_         = GB_TYPE_AC_AUTO_NORM;
        timeToAc_       = 0;
        numJrnEntry_    = 0;
        maxNumJrnEntry_ = 0;
        overflow_       = false;
    }

    TDeviceStatus status;

    // ��� ����� (���-�� ��������� � �����)
    bool setNumDevices(eGB_NUM_DEVICES val)
    {
        bool stat = false;
        if (val >= GB_NUM_DEVICES_MIN)
        {
            if (val < GB_NUM_DEVICES_MAX)
            {
                numDevices_ = val;
                stat        = true;
            }
        }

        if (!stat)
            val = GB_NUM_DEVICES_MAX;
        numDevices_ = val;
        return stat;
    }
    eGB_NUM_DEVICES getNumDevices() const { return numDevices_; }

    // ��� ������������
    bool setTypeAC(eGB_TYPE_AC val)
    {
        bool stat = false;
        if ((val >= GB_TYPE_AC_MIN) && (val <= GB_TYPE_AC_MAX))
        {
            typeAc_ = val;
            stat    = true;
        }

        return stat;
    }
    eGB_TYPE_AC getTypeAC() const { return typeAc_; }

    // ����� �� ������������
    bool setTimeToAC(uint32_t val)
    {
        bool stat = false;

        val /= 1000;

        if (val <= 60000)
        {
            timeToAc_ = val;
            stat      = true;
        }

        return stat;
    }
    uint64_t getTimeToAC() const { return timeToAc_; }

    // ���������� ������� � �������
    // ���������� ������� � �������
    bool setNumJrnEntry(uint16_t val)
    {
        bool stat = false;

        overflow_ = (val & 0xC000) != 0;
        val &= 0x3FFF;

        if (val <= maxNumJrnEntry_)
        {
            numJrnEntry_ = val;
            stat         = true;
        }
        return stat;
    }
    uint16_t getNumJrnEntry() const { return numJrnEntry_; }

    // ������������ ���-�� ������� � �������
    bool setMaxNumJrnEntry(uint16_t max)
    {
        bool stat = false;
        if (max <= 1024)
        {
            stat            = true;
            maxNumJrnEntry_ = max;
        }
        return stat;
    }
    uint16_t getMaxNumJrnEntry() const { return maxNumJrnEntry_; }

private:
    // ��� ����� (���-�� ���������)
    eGB_NUM_DEVICES numDevices_;

    // ��� ������������
    eGB_TYPE_AC typeAc_;

    // ����� �� ������������ � ��������
    uint16_t timeToAc_;

    // ���-�� ������� � �������
    uint16_t numJrnEntry_;

    // ������������ ���-�� ������� � �������
    uint16_t maxNumJrnEntry_;

    // ���� ������������ �������
    bool overflow_;
};


#endif /* PARAMDEF_H_ */
