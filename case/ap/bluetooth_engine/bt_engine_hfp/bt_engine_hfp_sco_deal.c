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
 * \brief    global variable模块
 * \author   xiaomaky
 * \version  1.0
 * \date  2013/4/11
 *******************************************************************************/
#include "./../bluetooth_engine.h"

static bool save_sco_data_to_cache(uint8 *sco_buf, uint16 w_len, uint8 pkg_status_flag)
{
    if (btengine_global_variable.hfp.sco.filter_package_count > 0)
    {
        btengine_global_variable.hfp.sco.filter_package_count--;
        return FALSE;
    }

    if (buffer_rw_write(&g_sco_input_buf, sco_buf, w_len) == FALSE)
    {
        HFP_PRINTF_WARNNING("SCO buf full\n");
        return FALSE;
    }

    //保存sco error report信息
    if (g_sco_err_report_info.length >= g_sco_err_report_info.count)
    {
        HFP_PRINTF_WARNNING("sco error report save error\n");
        btengine_global_variable.hfp.sco.sco_input_err_flag = 1;
        return FALSE;
    }

    g_sco_err_report_info.err_report_data[g_sco_err_report_info.write_ptr].pkg_status = pkg_status_flag;
    g_sco_err_report_info.err_report_data[g_sco_err_report_info.write_ptr].pkg_length = w_len;

    g_sco_err_report_info.write_ptr++;
    if (g_sco_err_report_info.count != 0)
    {
        g_sco_err_report_info.write_ptr %= g_sco_err_report_info.count;
    }
    g_sco_err_report_info.length++;

    return TRUE;
}

void init_sco_pkg_count_insecond(void)
{
    btengine_global_variable.hfp.sco.pncps.sco_data_in_last_1s = 0;
    btengine_global_variable.hfp.sco.pncps.pkg_count = 0;
    btengine_global_variable.hfp.sco.pncps.last_time_stamp = sys_get_ab_timer();
}

//统计1S内SCO包个数以及计算SCO是否正在传输
void sco_pkg_count_insecond(void) 
{
    if (sys_get_ab_timer() - btengine_global_variable.hfp.sco.pncps.last_time_stamp > SCO_TRANS_PKG_TIME)
    {
        if (btengine_global_variable.hfp.sco.pncps.pkg_count > SCO_TRANS_PKG_NUM_THRESHOLD)
        {
            btengine_global_variable.hfp.sco.pncps.sco_data_in_last_1s = 1;
        }
        else
        {
            btengine_global_variable.hfp.sco.pncps.sco_data_in_last_1s = 0;

        }
        btengine_global_variable.hfp.sco.pncps.pkg_count = 0;
        btengine_global_variable.hfp.sco.pncps.last_time_stamp = sys_get_ab_timer();
    }
    else if (btengine_global_variable.hfp.sco.pncps.last_time_stamp > sys_get_ab_timer()) //防止计时溢出重头数
    {
        btengine_global_variable.hfp.sco.pncps.last_time_stamp = sys_get_ab_timer();
        btengine_global_variable.hfp.sco.pncps.pkg_count = 0;
    }
}

void btengine_save_sco_data(uint8 *buf, int32 len, uint8 pkg_status_flag)
{
    uint16 package_len; //数据包长度
    uint8 *p_buf;

    p_buf = buf;

#if (SUPPORT_SCO_UNIT_TEST == 1)   
    sco_unit_test_receive_err_pkg_count(buf,len,pkg_status_flag);
#endif

#if (SUPPORT_ONLY_BT_TEST == 0)
    if (len > 0)
    {
        package_len = (uint16) len;
        //保存到SCO Cache
        save_sco_data_to_cache((uint8 *) (p_buf + HCI_SCO_HEADER_LEN), package_len, pkg_status_flag);
        btengine_global_variable.hfp.sco.pncps.pkg_count++;
    }
    else
    {
        HFP_PRINTF_WARNNING("sco_len err\n");
    }
#endif
    sco_mem_free(p_buf);

    return;
}

