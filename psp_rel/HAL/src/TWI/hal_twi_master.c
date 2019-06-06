#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

//I2C CMD
#define    I2C_MODE                    (0x60)
#define    START                       (I2C_MODE|0x4)
#define    STOP                        (I2C_MODE|0x8)
#define    RESTA                       (I2C_MODE|0xc)
#define    REBUS                       (I2C_MODE|0x2)
#define    ACK                         (I2C_MODE|0x0)
#define    NACK                        (I2C_MODE|0x1)

void TWI_master_set_hw_info(uint8 idx, twi_master_hw_info_t *p_twi_master_hw_info)
{
    uint32 twix_base = (idx == 0) ? (I2C0_BASE) : (I2C1_BASE);
    uint8 clkdiv = (uint32)(26*1000*1000/16)/p_twi_master_hw_info->bitrate_bps;

    if (idx == 0)
    {
        *((REG32) MRCR) &= (~(1 << MRCR_IIC0_RESET));
        g_p_sys_hal_cbks->hal_udelay(10);
        *((REG32) MRCR) |= (1 << MRCR_IIC0_RESET);
        *((REG32) CMU_DEVCLKEN) |= (1 << CMU_DEVCLKEN_I2C0CLKEN);
    }
    else
    {
        *((REG32) MRCR) &= (~(1 << MRCR_IIC1_RESET));
        g_p_sys_hal_cbks->hal_udelay(10);
        *((REG32) MRCR) |= (1 << MRCR_IIC1_RESET);
        *((REG32) CMU_DEVCLKEN) |= (1 << CMU_DEVCLKEN_I2C1CLKEN);
    }

    //g_p_sys_hal_cbks->hal_printf("twi_set_hw_info clkdiv:%d\n", clkdiv);

    *((REG32) (twix_base + (I2C0_CLKDIV-I2C0_BASE))) = clkdiv;
    *((REG32) (twix_base + (I2C0_CTL-I2C0_BASE))) = I2C_MODE;
}

void error(void)
{
    g_p_sys_hal_cbks->hal_printf("i2c transfer failed!\n");
}

void nack(void)
{
    error();
}

void wait_byte_receive(uint8 idx)
{
    uint32 twix_base = (idx == 0) ? (I2C0_BASE) : (I2C1_BASE);
    uint32 tmp;
    do
    {
        tmp = (*((REG32) (twix_base + (I2C0_STAT-I2C0_BASE))) >> I2C0_STAT_TCB) & 0x01;//transimit ok
    } while (tmp != 1);

    tmp = (1 << I2C0_STAT_TCB);
    *((REG32) (twix_base + (I2C0_STAT-I2C0_BASE))) = tmp;
}

void wait_stop(uint8 idx)
{
    uint32 twix_base = (idx == 0) ? (I2C0_BASE) : (I2C1_BASE);
    uint32 tmp;
    do
    {
        tmp = (*((REG32) (twix_base + (I2C0_STAT-I2C0_BASE))) >> I2C0_STAT_STPD) & 0x01;
    } while (tmp != 1);

    tmp = (1 << I2C0_STAT_STPD);
    *((REG32) (twix_base + (I2C0_STAT-I2C0_BASE))) = tmp;
}

uint8 bus_busy(uint8 idx)
{
    uint32 twix_base = (idx == 0) ? (I2C0_BASE) : (I2C1_BASE);
    uint32 ret;
    ret = (*((REG32) (twix_base + (I2C0_STAT-I2C0_BASE))) >> I2C0_STAT_BBB) & 0x01;
    return ret;
}

void transmit_byte(uint8 idx)
{
    uint32 twix_base = (idx == 0) ? (I2C0_BASE) : (I2C1_BASE);
    uint32 tmp;

    do
    {
        tmp = (*((REG32) (twix_base + (I2C0_STAT-I2C0_BASE))) >> I2C0_STAT_TCB) & 0x01;
    } while (tmp != 1); //transimit ok

    tmp = *((REG32) (twix_base + (I2C0_STAT-I2C0_BASE))) & 0x01;
    if (tmp != 0x1)
    {
        nack();
    }
    else
    {
        tmp = (1 << I2C0_STAT_TCB);
        *((REG32) (twix_base + (I2C0_STAT-I2C0_BASE))) = tmp;
    }
}

uint8 i2c_start(uint8 idx, uint8 address, uint8 START_VALUE)
{
    uint32 twix_base = (idx == 0) ? (I2C0_BASE) : (I2C1_BASE);
    uint32 ret;

    *((REG32) (twix_base + (I2C0_TXDAT-I2C0_BASE))) = address; //Slave addr
    *((REG32) (twix_base + (I2C0_CTL-I2C0_BASE))) = START_VALUE | REBUS; //start bit and relese bus
    
    transmit_byte(idx);
    ret = bus_busy(idx);
    if (ret != 0x1)
    {
        error();
    }

    return ret;
}

uint8 i2c_stop(uint8 idx)
{
    uint32 twix_base = (idx == 0) ? (I2C0_BASE) : (I2C1_BASE);
    uint32 ret = 0;
    *((REG32) (twix_base + (I2C0_CTL-I2C0_BASE))) = STOP | REBUS; //generate stop bit
    wait_stop(idx);
    ret = bus_busy(idx);
    if (ret != 0x0)
    {
        error();
    }

    g_p_sys_hal_cbks->hal_udelay(10);
    return ret;
}

/*
 Byte write functions
 */
uint8 i2c_bytewrite(uint8 idx, uint8* dat)
{
    uint32 twix_base = (idx == 0) ? (I2C0_BASE) : (I2C1_BASE);
    uint32 ret = 0;

    *((REG32) (twix_base + (I2C0_TXDAT-I2C0_BASE))) = *dat; //release I2C
    *((REG32) (twix_base + (I2C0_CTL-I2C0_BASE))) = REBUS; //release I2C
    transmit_byte(idx);
    ret = bus_busy(idx);
    if (ret != 0x1)
    {
        error();
    }

    return ret;
}

/*
 Byte read functions
 */
uint8 i2c_byteread(uint8 idx, uint8 ACK_FLAG)
{
    uint32 twix_base = (idx == 0) ? (I2C0_BASE) : (I2C1_BASE);
    uint32 ret = 0;
    uint8 dat;

    *((REG32) (twix_base + (I2C0_CTL-I2C0_BASE))) = REBUS | ACK_FLAG;
    wait_byte_receive(idx);
    dat = *((REG32) (twix_base + (I2C0_RXDAT-I2C0_BASE)));
    ret = bus_busy(idx);
    if (ret != 0x1)
    {
        error();
    }

    return dat;
}

/*
 ******************************************************************************
 *             uint8 hal_TWI_write_bytes(uint8 *buf, uint8 length)
 *
 * Description : ����ŵ�*buf��ַ�ﳤ��Ϊlength�����ݷ��ͳ�ȥ
 *
 * Arguments   : unsigned char *buf: ���ݴ�ŵ�ַ
 unsigned char length: ���ݳ���, ��byteΪ��λ
 *
 * Returns     : FALSE: �������ݽ��յ�nack
 TRUE: �������ݽ��յ�ack
 *
 * Notes       : ��������I2C �豸�������ݣ����������豸����Ӧ
 *                  �����ڲ��������������ṩ�ӿ�
 ********************************************************************************
 */
uint8 TWI_master_write_bytes(uint8 idx, uint8 *buf, uint8 address, uint8 length, uint8 read_mode)
{
    uint8 ret = 0, len = 0, i = 0;

    ret = i2c_start(idx,address,START);

    len = length;
    for (i = 0; i < len; i++)
    {
        i2c_bytewrite(idx, (buf + i));
    }

    ret = 1;

    //��ȡģʽ������ŷ��Ͷ��������STOP
    if ((ret == 0) || (read_mode == TWI_READ_MODE_NO))
    {
        i2c_stop(idx);
    }

    return ret;
}

/*
 ******************************************************************************
 *             uint8 hal_TWI_read_bytes(uint8 *buf, uint8 address, uint8 length)
 *
 * Description : ��ȡ����length
 *
 * Arguments   : unsigned char *buf: ���ݴ�ŵ�ַ
 unsigned char address: slave��ַ
 *               unsigned char length: ���ݳ���,��byteΪ��λ
 *
 * Returns     : FALSE: slave����Ӧ
 TRUE: �յ�����
 *
 * Notes       :  �����ڲ��ӿڣ��������ṩ
 *
 ********************************************************************************
 */
uint8 TWI_master_read_bytes(uint8 idx, uint8 *buf, uint8 address, uint8 length)
{   
    if(length == 0) return 1;

    uint8 ret = 0;
    uint8 len = 0, i = 0;
    i2c_start(idx,address,RESTA);

    len = length;
    for (i = 0; i < len - 1; i++)
    {
        *(buf + i) = i2c_byteread(idx, ACK);
    }

    //���һ���ֽ��е����⣬��Ҫ�ظ�NACK
    *(buf + i) = i2c_byteread(idx, NACK);

    i2c_stop(idx);

    ret = 1;

    return ret;
}

