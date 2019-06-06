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

void bt_engine_hfp_sco_connect_ind_handle(void)
{
    bt_dev_t *dev;
    dev = btengine_global_variable.g_cur_rmt_dev;
    if (dev != NULL)
    {
        btengine_global_variable.hfp.sco.sco_conn_status = SCO_CREATE_READY;
        btengine_global_variable.hfp.sco.filter_package_count = 15;
        HFP_PRINTF("sco connect\n");
        //建立一条SCO数据流，临时提高频率
        bt_adjust_clk(BT_LINK_WORK_FREQ);

        g_p_info->sco_created_flag = TRUE;

#ifdef BT_SUPPORT_BLE
        if (bt_manager_cfg_info.sp_ble == 1)
        {
            le_update_sco_connection_param();
        }
#endif
        //建立sco链接时需要清零sco包计算器
        init_sco_pkg_count_insecond();
        load_micout_timer();
#ifdef BT_SP_GETCALLINFO
        unload_call_timer();
        load_call_timer();
#endif

#if (SUPPORT_SCO_UNIT_TEST == 1)
        if (sco_unit_test_get_status() == STATUS_THREAD_NO_INIT)
        {
            sco_unit_test_init();
            btengine_global_variable.hfp.sco.filter_sco_input_flag = DATA_UNFILTER;
        }
#endif
        if (get_cur_device_status(dev) == STATUS_CALL_PHONE)
        {
            set_cur_device_status(dev, STATUS_CALL_HFP);
        }
    }
}

void bt_engine_hfp_sco_disconnect_ind_handle(void)
{
    bt_dev_t *dev;
    dev = btengine_global_variable.g_cur_rmt_dev;
    if (dev != NULL)
    {
        //SCO断开连接，中间件要退出，临时将频率提升至104M，以维持HFP 30M以上的频率预估
        bt_adjust_clk(BT_LINK_WORK_FREQ);

        switch (get_cur_device_status(dev))
        {
            case STATUS_CALL_HFP:
            set_cur_device_status(dev, STATUS_CALL_PHONE);
            break;
            default:
            break;
        }
        HFP_PRINTF("sco release\n");

#ifdef BT_SUPPORT_BLE
        if (bt_manager_cfg_info.sp_ble == 1)
        {
            le_update_default_connection_param();
        }
#endif
        unload_micout_timer();
        clear_sco_buf();
        //退出sco时需要复位sco包计算器
        init_sco_pkg_count_insecond();
        g_p_info->sco_created_flag = FALSE;
        sco_connect_request_flag = 0;
#ifdef BT_SP_GETCALLINFO
        if (dev->p_dev_status != NULL)
        {
            HFP_HF_GetCurrentCalls((uint8 * )dev->p_dev_status->g_hf_handle);
        }
#endif
#if (SUPPORT_SCO_UNIT_TEST == 1)
        if (sco_unit_test_get_status() == STATUS_THREAD_READY)
        {
            sco_unit_test_deinit();
            btengine_global_variable.filter_sco_input_flag = 1;
        }
#endif

        if (btengine_global_variable.hfp.voice_recognition_status == ACTIVATE)
        {
            HFP_PRINTF("siri end\n");
            hfp_standby_deal();
            btengine_global_variable.hfp.voice_recognition_status = INACTIVE;
        }
    }
}

void load_micout_timer(void)
{
    HFP_PRINTF("load micout timer\n");

    unload_micout_timer();

#if (SUPPORT_SCO_UNIT_TEST_SEND_SINE_WAVE == 1)        
    btengine_global_variable.hfp.sco.data_deal_irq_timer_id = sys_set_irq_timer1(sco_unit_test_send_micout_data, 0x01);
#else
    btengine_global_variable.hfp.sco.data_deal_irq_timer_id = sys_set_irq_timer1(deal_micout_sco_data, 0x01);
#endif
}
void unload_micout_timer(void)
{
    if (btengine_global_variable.hfp.sco.data_deal_irq_timer_id != -1)
    {
        HFP_PRINTF("unload micout timer\n");
        sys_del_irq_timer1((uint32 ) btengine_global_variable.hfp.sco.data_deal_irq_timer_id);
        btengine_global_variable.hfp.sco.data_deal_irq_timer_id = -1;
    }
}
#ifdef BT_SP_GETCALLINFO
void unload_call_timer(void)
{
    if (btengine_global_variable.hfp.get_call_status_timer_id != -1)
    {
        HFP_PRINTF("unload call timer\n");
        sys_del_irq_timer1((uint32 )btengine_global_variable.hfp.get_call_status_timer_id);
        btengine_global_variable.hfp.get_call_status_timer_id = -1;
    }
}

void HFP_GetCall_info(void)
{
    bt_dev_t *dev;
    dev = btengine_global_variable.g_cur_rmt_dev;
    if ((dev != NULL) && (dev->p_dev_status != NULL))
    {
        HFP_HF_GetCurrentCalls((uint8 * )dev->p_dev_status->g_hf_handle);
    }
    if (g_btengine_cfg_data.hfp_clcc_info.status == 0)
    {
        unload_call_timer();
    }

}
void load_call_timer(void)
{
    if (btengine_global_variable.hfp.get_call_status_timer_id == -1)
    {
        HFP_PRINTF("load call timer\n");
        btengine_global_variable.hfp.get_call_status_timer_id = sys_set_irq_timer1(HFP_GetCall_info, 300);
    }
}
#endif

void hfp_thread_exit(void)
{
    clear_sco_buf();
    btengine_global_variable.hfp.allow_sync_vol_to_ap_flag = FALSE;
    if (bt_stack_cfg_info.sp_sniff_enable == TRUE)
    {
        bt_set_link_policy(btengine_global_variable.g_cur_rmt_dev,
                (uint8) (HCI_LINK_POLICY_SETTINGS_ENABLE_ROLE_SWITCH | HCI_LINK_POLICY_SETTINGS_ENABLE_SNIFF_MODE));
    }
}

void hfp_thread_init(void)
{

    bt_set_link_policy(btengine_global_variable.g_cur_rmt_dev, (uint8) (HCI_LINK_POLICY_SETTINGS_ENABLE_ROLE_SWITCH));
    btengine_global_variable.hfp.allow_sync_vol_to_ap_flag = FALSE;
    HFP_DBG_PRINTF("hfp init\n");
}

void clear_sco_buf(void)
{
    uint32 temp_flag;

    if (g_p_info->enter_hfp_flag == 1)
    {
        temp_flag = sys_local_irq_save();

        g_sco_input_buf.length = 0;
        g_sco_input_buf.read_ptr = 0;
        g_sco_input_buf.write_ptr = 0;

        g_sco_err_report_info.length = 0;
        g_sco_err_report_info.read_ptr = 0;
        g_sco_err_report_info.write_ptr = 0;

        g_sco_output_buf.length = 0;
        g_sco_output_buf.read_ptr = 0;
        g_sco_output_buf.write_ptr = 0;

        sys_local_irq_restore(temp_flag);

        HFP_DBG_PRINTF("hfp sco buf clr\n");
    }
}

