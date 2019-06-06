/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xiaomaky   2013-4-11 14:26:35     1.0             build this file
 *******************************************************************************/
/*!
 * \file     bt_engine_lib.c
 * \brief    global variableģ��
 * \author   xiaomaky
 * \version  1.0
 * \date  2013/4/11
 *******************************************************************************/
#include "bluetooth_engine.h"

#ifdef ENABLE_BATTERY_REPORT
#define AT_XAPL_LEN (25)
#define AT_BATTERY_REPORT_LEN (21)
#define AT_BATTERY_VAL_INDEX (19)
#define BATTERY_REPORT_MAX (9)
#define BATTERY_REPORT_MIN (0)
#endif

#ifdef ENABLE_BATTERY_REPORT
//*************************************************
//ƻ�������豸����Э�����

//ʹ���Զ���AT����
int8 BT_HfpEnableAppleAT(void)
{
    uint8 temp_buf[AT_XAPL_LEN];
    if ((btengine_global_variable.g_cur_rmt_dev != NULL)
            && (btengine_global_variable.g_cur_rmt_dev->p_dev_status != NULL))
    {
        libc_memcpy(temp_buf, "AT+XAPL=0000-0000-0100,2\r", AT_XAPL_LEN);
        HFP_ExtendCmd((uint8 * )btengine_global_variable.g_cur_rmt_dev->p_dev_status->g_hf_handle, temp_buf,
                AT_XAPL_LEN, 0);
        //BTEG_PRINTF("\n battery init");
    }
    else
    {
        HFP_PRINTF_WARNNING("\nbattery report init failed\n");

    }
    return 0;
}

//��ص�����ʾ
int8 BT_HfpBatteryReport(uint8 bat_val)
{
    uint8 temp_buf[AT_BATTERY_REPORT_LEN];
    if ((btengine_global_variable.g_cur_rmt_dev != NULL)
            && (btengine_global_variable.g_cur_rmt_dev->p_dev_status != NULL))
    {
        libc_memcpy(temp_buf, "AT+IPHONEACCEV=1,1,4\r", AT_BATTERY_REPORT_LEN);
        temp_buf[AT_BATTERY_VAL_INDEX] = bat_val + '0';
        if (CURRENT_DEVICE_SNIFF_FLAG != 0)
        {
            bt_exit_sniff(btengine_global_variable.g_cur_rmt_dev);
        }
        HFP_ExtendCmd((uint8 * )btengine_global_variable.g_cur_rmt_dev->p_dev_status->g_hf_handle, temp_buf,
                AT_BATTERY_REPORT_LEN, 0);

        HFP_PRINTF("bat level %d\n", bat_val, 2);
    }
    else
    {
        HFP_PRINTF_WARNNING("\nbattery report failed\n");

    }
    return 0;
}

//�ϱ�����˵ĵ�ص�����Զ���豸
/*
 mode:0-ֻʹ��;1-�ϱ�����
 bat_val:��ǰ�ϱ��ĵ���ֵ(0-9)
 */
void report_battery_to_rmtdevice(uint8 mode, uint8 bat_val)
{
    uint8 bt_status;
    bt_status = get_cur_device_status(btengine_global_variable.g_cur_rmt_dev);
    if (bt_manager_cfg_info.sp_hfp_bat_report == 0)
    {
        return;
    }
    //��绰ʱ�����ϱ�,������и���
    if ((bt_status == STATUS_CALL_HFP) || (bt_status == STATUS_CALL_PHONE))
    {
        //return;
    }
    if (mode == 0)
    {
        BT_HfpEnableAppleAT(); //��ʹ��
    }
    else
    {
        BT_HfpBatteryReport(bat_val);
    }
}
#endif

