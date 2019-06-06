/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������HAL��������ģ�� TWI Master �ӿڡ�
 * ���ߣ�cailizhen
 ********************************************************************************/
#include "hal_inner.h"

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
uint8 hal_TWI_write_bytes(uint8 mode, uint8 idx, uint8 *buf, uint8 address, uint8 length, uint8 read_mode)
{
    uint8 ret = 0;

    if (mode == TWI_MASTER_SIM)
    {
        ret = TWI_write_bytes_sim(idx,buf,address,length,read_mode);
    }
    else
    {
        ret = TWI_master_write_bytes(idx,buf,address,length,read_mode);
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
uint8 hal_TWI_read_bytes(uint8 mode, uint8 idx, uint8 *buf, uint8 address, uint8 length)
{
    uint8 ret = 0;

    if (mode == TWI_MASTER_SIM)
    {
        ret = TWI_read_bytes_sim(idx,buf,address,length);
    }
    else
    {
        ret = TWI_master_read_bytes(idx,buf,address,length);
    }

    return ret;
}

