/*
 * twi.h
 *
 *  Created on: 15.01.2016
 *      Author: Shcheblykin
 */

#ifndef TWI_H_
#define TWI_H_

class TTmp75
{
    static const uint8_t s_u8AdrBits = 1;

    static const uint8_t s_u8ReadBit = 0;

    static const int8_t s_i8TempError = -100;

    /// ������ ��� ������ "������� ����������"
    typedef enum __attribute__((__packed__))
    {
        // General TWI Master status codes
        I2C_STATE_START     = 0x08,  ///< START has been transmitted.
        I2C_STATE_REP_START = 0x10,  ///< Repeated START has been transmitted.
                                     //		STATE_ARB_LOST				= 0x38,	///< Arbitration lost.

        // TWI Master Transmitter staus codes
        I2C_STATE_MTX_ADR_ACK = 0x18,   ///< SLA+W has been tramsmitted and ACK received.
                                        //		STATE_MTX_ADR_NACK			= 0x20,	///< SLA+W has been
                                        //tramsmitted and NACK received.
        I2C_STATE_MTX_DATA_ACK = 0x28,  ///< Data byte has been tramsmitted and ACK received.
                                        //		STATE_MTX_DATA_NACK			= 0x30,	///< Data byte has been
                                        //tramsmitted and NACK received.

        // TWI Master Receiver staus codes
        I2C_STATE_MRX_ADR_ACK = 0x40,  ///< SLA+R has been tramsmitted and ACK received.
        //		STATE_MRX_ADR_NACK			= 0x48, ///< SLA+R has been tramsmitted
        //and NACK received.
        I2C_STATE_MRX_DATA_ACK  = 0x50,  ///< Data byte has been received and ACK tramsmitted.
        I2C_STATE_MRX_DATA_NACK = 0x58,  ///< Data byte has been received and NACK tramsmitted.

        // TWI Slave Transmitter staus codes
        //		STATE_STX_ADR_ACK			= 0xA8, ///< Own SLA+R has been received;
        //ACK has been returned
        //		STATE_STX_ADR_ACK_M_ARB_LOST= 0xB0, ///< Arbitration lost in SLA+R/W as
        //Master; own SLA+R has been received; ACK has been returned
        //		STATE_STX_DATA_ACK			= 0xB8, ///< Data byte in TWDR has been
        //transmitted; ACK has been received
        //		STATE_STX_DATA_NACK			= 0xC0, ///< Data byte in TWDR has been
        //transmitted; NOT ACK has been received
        //		STATE_STX_DATA_ACK_LAST_BYTE= 0xC8, ///< Last data byte in TWDR has been
        //transmitted (TWEA = �0�); ACK has been received

        // TWI Slave Receiver staus codes
        //		STATE_SRX_ADR_ACK			= 0x60, ///< Own SLA+W has been received
        //ACK has been returned
        //		STATE_SRX_ADR_ACK_M_ARB_LOST= 0x68, ///< Arbitration lost in SLA+R/W as
        //Master; own SLA+W has been received; ACK has been returned
        //		STATE_SRX_GEN_ACK			= 0x70, ///< General call address has been
        //received; ACK has been returned
        //		STATE_SRX_GEN_ACK_M_ARB_LOST= 0x78, ///< Arbitration lost in SLA+R/W as
        //Master; General call address has been received; ACK has been returned
        //		STATE_SRX_ADR_DATA_ACK		= 0x80, ///< Previously addressed with own
        //SLA+W; data has been received; ACK has been returned
        //		STATE_SRX_ADR_DATA_NACK		= 0x88, ///< Previously addressed with own
        //SLA+W; data has been received; NOT ACK has been returned
        //		STATE_SRX_GEN_DATA_ACK		= 0x90, ///< Previously addressed with
        //general call; data has been received; ACK has been returned
        //		STATE_SRX_GEN_DATA_NACK		= 0x98, ///< Previously addressed with
        //general call; data has been received; NOT ACK has been returned
        //		STATE_SRX_STOP_RESTART		= 0xA0, ///< A STOP condition or repeated
        //START condition has been received while still addressed as Slave

        // TWI Miscellaneous status codes
        //		STATE_NO_STATE				= 0xF8, ///< No relevant state
        //information available; TWINT = �0�
        //		STATE_BUS_ERROR				= 0x00	///< Bus error due to an
        //illegal START or STOP condition
    } EI2C_STATE;

    /// �������� ������� ����������� TMP75
    typedef enum __attribute__((__packed__))
    {
        REG_TEMP_R = 0x00,  ///< ������� ����������� (������ ������)
        REG_CONFIG_RW = 0x01,  ///< ������� ��������� (������/������)
        REG_T_LOW_RW  = 0x02,  ///< ������� T-LOW (������/������)
        REF_T_HIGH_RW = 0x03   ///< ������� T-HIGH (������/������)
    } EREG;

public:
    /**	�����������.
     *
     * 	������������� TWI.
     * 	������� ������ 10���, �������� �������� �� ������� SCL, SDA.
     *
     *	@param[in] adr ����� ������� �����������.
     */
    TTmp75(uint8_t adr);

    /**	������ ��������� ���������� �����������.
     *
     *	������ ��������� ���������� ����������� ���������� ��� ���������� �����
     *	���������� �� ������ TWI ��� ������ ���������� � ���������� ����.
     *
     *	��� ���������� ��������� ������ � ������ TWI, ������ ������� �����������
     *	(��� ���� ������������ ���� ����������), � ����� ��� ����������� �����.
     *
     *	��� �� ��� ���������� ������ �� ����� �������� ��������� ��������� ����
     *	(�� ���������):
     *	1. ���������� TWI.
     *	2. ���������� ����� SLC � 0, SDA - �� �������.
     *	3. SCL � 0.
     *	4. SCL � 1 �� 6 ��.
     *	5. SCL � 0 �� 6 ��.
     *	6. ��������� 9 ��� � ���� 4.
     *	7. ������� SCL � ������� ���������.
     *	8. ��������� TWI.
     *	9. ������� ����.
     *
     */
    void readTemp();

    /**	���������� �����������.
     *
     *	��� ������ ���������� ����� ���������� �������� \a #s_i8TempError.
     *
     *	@return ����������� ��������� ��� ��������� �������
     */
    int8_t getTemperature();

    /**	���������� ���������� TWI;
     *
     *	@param[in] twsr �������� �������� TWSR.
     */
    void isr(uint8_t twsr)
    {
        // �� ��������� ������� ������ TWI, ���������� � ������� ��� ����������
        uint8_t twcr = (1 << TWEN) | (1 << TWIE) | (1 << TWINT);

        switch ((EI2C_STATE) (twsr & 0xF8))
        {
        case I2C_STATE_START:  // ����������� �����
            // ���������� ����� ���������� � ����������� ��� ������
            TWDR = adrIC | (0 << s_u8ReadBit);
            break;

        case I2C_STATE_MTX_ADR_ACK:  // ������� ����� IC � ACK �������
            // ���������� ����� �������� �����������
            TWDR = REG_TEMP_R;
            break;

        case I2C_STATE_MTX_DATA_ACK:  // �������� ������ � ACK
            // ����������� ��������
            twcr |= (1 << TWSTA);
            break;

        case I2C_STATE_REP_START:  // ����������� ��������
            // ���������� ����� ���������� � ����������� ��� ������
            TWDR = adrIC | (1 << s_u8ReadBit);
            break;

        case I2C_STATE_MRX_ADR_ACK:  // ������� ����� IC � ACK �������
            // ���������� ������������ ������, � ������������� ACK
            twcr |= (1 << TWEA);
            break;

        case I2C_STATE_MRX_DATA_ACK:  // �������� ������ � ������� ACK
            // ������ �������� ����� ����������, ������������ NACK
            temperature = (int8_t) TWDR;
            break;

        case I2C_STATE_MRX_DATA_NACK:  // �������� ������ � ������� NACK
            // ������ �������� ����� ����������� � ������������� ����
            twcr |= (1 << TWSTO);
            twcr &= ~(1 << TWIE);
            break;

        default:
            // ���������� ������, ������ ����������
            temperature = s_i8TempError;
            twcr &= ~(1 << TWIE);
        }

        TWCR = twcr;
    }

private:
    /// ����� ������� �����������
    uint8_t adrIC;

    /// ������� �����������
    int8_t temperature;
};

#endif /* TWI_H_ */
