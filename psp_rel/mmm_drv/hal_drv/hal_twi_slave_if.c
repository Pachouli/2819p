/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：HAL驱动——TWI Slave 接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "hal_inner.h"

/* 示例代码

bool my_TWI_slave_read_handle(uint8 r_byte)
{
    //used r_byte

    //return ack/nack for next byte
    return TRUE;
}

uint8 my_TWI_slave_write_handle(void)
{
    uint8 w_byte;

    //set w_byte
    return w_byte;
}

twi_slave_hw_info_t tmp_hw_info;
pad_phy_para_t tmp_twi_phy;

tmp_twi_phy.pad_pu_en = FALSE; //使用TWI专用上拉电阻10K
tmp_twi_phy.pad_pd_en = FALSE;
tmp_twi_phy.smit_en = 0;
tmp_twi_phy.pad_driver = 3;

set_pad_function(PAD_GPIO3, GPIO3_MFP_I2C_SCL, &tmp_twi_phy, (void *) 0);
set_pad_function(PAD_GPIO4, GPIO4_MFP_I2C_SDA, &tmp_twi_phy, (void *) 0);

tmp_hw_info.addr = 0x20;
tmp_hw_info.r_handle = my_TWI_slave_read_handle;
tmp_hw_info.w_handle = my_TWI_slave_write_handle;
tmp_hw_info.stop_handle = NULL;
tmp_hw_info.err_handle = NULL;

twi_slave_set_hw_info(&tmp_hw_info);

*/

