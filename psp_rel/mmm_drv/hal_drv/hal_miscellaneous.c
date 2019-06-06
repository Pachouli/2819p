/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：HAL驱动——其他硬件功能接口，比如设置晶振内部匹配电容。
 * 作者：cailizhen
 ********************************************************************************/

#include "hal_inner.h"

uint8 card_pw_cfg = 0,charging_current = 0;

int32 hal_hardware_set(hw_set_op_e set_op, void *para)
{
    int32 ret = 0;

    switch (set_op)
    {
        case HAL_SET_CARD_PW_CTRL_CFG:
        {
            card_pw_cfg = (uint8) (uint32) para;
            libc_printf("card power ctrl cfg:0x%x", card_pw_cfg);
        }
        break;

        case HAL_SET_PHONE_CHARGING_CURRENT:
        {
            charging_current = (uint8) (uint32) para;
        }
        break;

        default:
        break;
    }

    return ret;
}

int32 hal_hardware_get(hw_get_op_e get_op, void *para)
{
    int32 ret = 0;

    switch (get_op)
    {
        case HAL_GET_CARD_PW_CTRL_CFG:
        {
            return card_pw_cfg;
        }
        break;

        case HAL_GET_CHARGING_CURRENT:
        {
            return charging_current;
        }
        break;

        default:
        break;
    }

    return ret;
}

//USB检测设置，返回0表示设置成功，-1表示设置失败
int32 hal_usb_device_uhost_detect_switch_inner(uint8 det_mode)
{
    int32 ret = -1;

    if (det_mode == USB_DET_FOR_DEVICE)
    {
        ret = hal_usb_detect_init_inner();
    }
    else if (det_mode == USB_DET_FOR_UHOST)
    {
        ret = hal_uhost_detect_init_inner();
    }
    else
    {
        ret = -1;
    }

    return ret;
}

int hal_sdmmc_app_detect_with_mutex(void)
{
    int det_ret;

    if (libc_pthread_mutex_lock(p_sys_global_info->card_mutex) < 0)
    {
        libc_printf_error("card_mutex lock err!\n");
        while (1);
    }
    det_ret = hal_sdmmc_app_detect();
    if (libc_pthread_mutex_unlock(p_sys_global_info->card_mutex) < 0)
    {
        libc_printf_error("card_mutex unlock err!\n");
        while (1);
    }

    return det_ret;
}
