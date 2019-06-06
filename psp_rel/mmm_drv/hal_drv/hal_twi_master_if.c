/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：HAL驱动——模拟 TWI Master 接口。
 * 作者：cailizhen
 ********************************************************************************/
#include "hal_inner.h"

/*
 ******************************************************************************
 *             uint8 hal_TWI_write_bytes(uint8 *buf, uint8 length)
 *
 * Description : 将存放到*buf地址里长度为length的数据发送出去
 *
 * Arguments   : unsigned char *buf: 数据存放地址
 unsigned char length: 数据长度, 以byte为单位
 *
 * Returns     : FALSE: 发送数据接收到nack
 TRUE: 发送数据接收到ack
 *
 * Notes       : 由主控向I2C 设备发送数据，接收来自设备的响应
 *                  驱动内部函数，不向外提供接口
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
 * Description : 读取长度length
 *
 * Arguments   : unsigned char *buf: 数据存放地址
 unsigned char address: slave地址
 *               unsigned char length: 数据长度,以byte为单位
 *
 * Returns     : FALSE: slave不响应
 TRUE: 收到数据
 *
 * Notes       :  驱动内部接口，不向外提供
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

