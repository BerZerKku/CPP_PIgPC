/** @brief ���������������� ����.
 *
 *  ����� ������ � ���������������� ������.
 */

#ifndef UART_H_
#define UART_H_

#include "paramUart.h"

class TUart
{
public:
    /**	���������� ���������� ����������� ������ ����������.
     *
     *	��� �������� ���������� ����� �� ������, ����������� ����������
     *	�� ����������� ������.
     *
     * 	������ ������������� � ����������!!!
     */
    void isrUDR()
    {
        if (cnt_ < numTrByte_)
        {
            *udr_        = buf_[cnt_];
            buf_[cnt_++] = 0;
        }
        else
        {
            *ucsrb_ &= ~(1 << UDRIE1);
        }
    }

    /**	���������� ���������� ��������� ��������.
     *
     * 	����������� ���������� �� ��������, ����������� ���������� �� �����.
     *
     * 	������ ������������� � ����������!!!
     */
    void isrTX()
    {
        cnt_       = 0;
        numTrByte_ = 0;
        *ucsrb_ |= (1 << RXCIE);
        *ucsrb_ &= ~(1 << TXCIE);
    }

    /// ��� ����������������� �����
    enum ePORT
    {
        PORT_UART0,  ///< ���������������� ���� UART0
        PORT_UART1   ///< ���������������� ���� UART1
    };

    /** �����������
     *
     * 	����� �����-������ ������ ���� ��������� �������.
     *
     * 	@param port ��� �����
     * 	@see ePORT
     * 	@param *buf ��������� �� ����� ������
     * 	@param size ������ ������
     */
    TUart(TUart::ePORT port, uint8_t *const buf, uint8_t size);

    /**	��������� ����.
     *
     *	� ������ ��������� ������ �������� �����, ����� ��������� ���������
     *	�� ���������:
     *	- �������� 19200
     *	- ���� ������ 8
     *	- �������� �������� ���
     *	- ��� ���� �����
     *
     * 	@param baud	�������� ������ �����.
     * 	@param databits	���������� ����� ������.
     * 	@param parity �������� �������� ��������.
     * 	@param stopbits ���������� ���� �����.
     * 	@return True - � ������ ��������� ��������
     */
    bool open(uint16_t             baud,
              TDataBits::DATA_BITS databits,
              TParity::PARITY      parity,
              TStopBits::STOP_BITS stopbits);

    /** ��������� ����.
     *
     * 	����������� ������ ��������� � ����������� UART, � ���� �� ������������
     * 	��� ����������.
     */
    void close();

    /**	�������� ��������� ���-�� ������, ������� ���������� � �����.
     *
     * 	@param size ���-�� ���� ������ ��� ��������.
     * 	@return ���-�� ������������ ���� ������
     */
    void trData(uint8_t size);

private:
    /// ���-�� ��������/���������� ������
    uint8_t cnt_;
    /// ��������� �� �����
    uint8_t *const buf_;
    /// ������ ������
    const uint8_t size_;
    /// ���-�� ������ �� ��������
    uint8_t numTrByte_;

    // ������� ������
    volatile uint8_t *udr_;

    // �������� ���������
    volatile uint8_t *ubbrh_;
    volatile uint8_t *ubbrl_;
    volatile uint8_t *ucsrc_;
    volatile uint8_t *ucsrb_;
    volatile uint8_t *ucsra_;
};

#endif /* UART_H_ */
