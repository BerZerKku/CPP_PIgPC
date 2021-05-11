/*
 * enterParam.h
 *
 *  Created on: 26.09.2017
 *      Author: Shcheblykin
 */

#ifndef TX_COM_HPP_
#define TX_COM_HPP_

#include "src/glbDefine.h"

// ����� ��� �������� ������
class TTxCom {
    static const uint8_t BUFFER_SIZE = 16;

   public:
    TTxCom() {
        clear();
    }

    // ������� ������
    void clear();

    /** ������ ������� � ����� 1.
     *
     * 	���� num > 0, �� ���������� ��������� ������� � ������;
     * 	���� num = 0 ������� �����������.
     * 	@param com ��� �������.
     * 	@param num ������ �������� � ������.
     * 	@retval True - � ������ �������� ������.
     * 	@retval False - ���� �� ������� ��������� ������� � �����.
     */
    bool addCom1(eGB_COM com, uint8_t num = 0);

    /** ���������������� ������ ��������� � ������ 1 ������ (�� �����).
     * 	@return ��� ������� �������.
     */
    eGB_COM getCom1();

    /** ������ ������� � ����� 2.
     * 	@param com ��� �������.
     * 	@retval True - � ������ �������� ������.
     * 	@retval False - ���� �� ������� ��������� ������� � �����.
     */
    bool addCom2(eGB_COM com);

    /** ���������������� ������ ��������� � ������ 2 ������ (�� �����).
     * 	@return ��� ������� �������.
     */
    eGB_COM getCom2();

    /** ���������� ������� ���������� ������ � ������ 2.
     * @return
     */
    uint8_t getNumCom2() const;

    /**	������ ������� ������� � ����� �������.
     *
     *	��� ���������� ������� ������� ��� ���������� ������ �������� �����������.
     *
     * 	@param com ��� ������� �������
     */
    void addFastCom(eGB_COM com);

    /**	���������� ������� �������. ��� ���� ���� ����������� ������
     *
     *	��� ���������� ������� ������� � ����� �������� ���������� �����������
     *	��� ������ ������� ������.
     *
     * 	@return ��� ������� �������.
     */
    eGB_COM getFastCom();

    /**	������ ���� ������ � �����.
     * 	@param byte	���� ������.
     * 	@param num ������ �������� �������.
     */
    void setInt8(uint8_t byte, uint8_t num = 0);

    /** ���������� ����� ������.
     * 	@param num ������ �������� �������.
     * 	@return ���� ������.
     */
    uint8_t getInt8(uint8_t num = 0) const;

    /**	������ 2-� �������� ����� (uint16_t) � �����.
     * 	� ������ ������ ������������ � 1 � 2 ������� ������� uint8_t.
     * 	@param val ������.
     */
    void setInt16(uint16_t val);

    /** ���������� 2-� �������� ����� (uint16_t) �� ������.
     * 	������ �������� � 1 � 2 ��������� ������� uint8_t.
     * 	@return ������.
     */
    uint16_t getInt16() const;

    /**	���������� ��������� �� ����� ������.
     * 	@return ��������� �� ����� ������.
     */
    uint8_t* getBuferAddress();

    /**	���������� ��� ������� �� ��������.
     *
     * 	@return ��� ������� �� ��������
     */
    eGB_SEND_TYPE getSendType() const;

    /**	��������� ���� ������� �� ��������.
     *
     * 	@param sendType ��� ������� �� ��������.
     */
    void setSendType(eGB_SEND_TYPE sendType);

   private:
    // ��� ������������ �������
    eGB_SEND_TYPE sendType;
    // ������� ������� (�� ���������)
    eGB_COM comFast_[MAX_NUM_FAST_COM];
    // ����� ������� ������� �������
    uint8_t cntComFast;
    // ������ ����� ������
    eGB_COM com1_[MAX_NUM_COM_BUF1];
    // ���-�� ������ � ������ ������
    uint8_t numCom1_;
    // ����� ������� ������� � ������ ������
    uint8_t cnt1_;
    // ������ ����� ������
    eGB_COM com2_[MAX_NUM_COM_BUF2];
    // ���-�� ������ �� ������ ������
    uint8_t numCom2_;
    // ����� ������� ������� �� ������ ������
    uint8_t cnt2_;
    // ����� ������ (��� ������ ������� ������� � �������� )
    uint8_t buf_[MAX_NUM_FAST_COM + 1] [BUFFER_SIZE];
};

#endif /* TX_COM_HPP_ */
