/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     mainmenu_main.c
 * \brief    MainMenu��main����ģ��
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "bluetooth_engine.h"

bool deal_connect_profile(void)
{
    bool ret = FALSE;
    if (btengine_global_variable.g_cur_rmt_dev == NULL)
    {
        BT_AUTO_RECONN_PRINTF_WARNNING("dev is null\n");
        return ret;
    }

    if ((btengine_global_variable.g_cur_rmt_dev->profile & (uint8) HFP_SUPPORTED) != 0)
    {
        //֧��HFP�İ��ӻ����ǲ���ģʽʱ��֧������
        if (bt_manager_cfg_info.sp_hfp == 1)
        {
            BT_AUTO_RECONN_PRINTF("hfp");
            ret = app_msg_deal_hfp_connect(btengine_global_variable.g_cur_rmt_dev);
        }
    }
    else
    {
        if ((btengine_global_variable.local_device_support_profile & (uint8) A2DP_SUPPORTED) != 0)
        {
            //֧��a2dp�İ��ӻ����ǲ���ģʽʱ��֧������
            if (g_btengine_cfg_data.enable_a2dp_flag == 1)
            {
                BT_AUTO_RECONN_PRINTF("a2dp");
                ret = app_msg_deal_a2dp_connect(btengine_global_variable.g_cur_rmt_dev);
            }
        }
        else if ((btengine_global_variable.local_device_support_profile & (uint8) HFP_SUPPORTED) != 0)
        {
            //֧��HFP�İ��Ӳ�֧������
            if (bt_manager_cfg_info.sp_hfp == 1)
            {
                BT_AUTO_RECONN_PRINTF("hfp");
                ret = app_msg_deal_hfp_connect(btengine_global_variable.g_cur_rmt_dev);
            }
        }
        else if ((btengine_global_variable.local_device_support_profile & (uint8) HID_SUPPORTED) != 0)
        {
            if (bt_manager_cfg_info.sp_hid == 1)
            {
                BT_AUTO_RECONN_PRINTF("hid");
                ret = HID_connect(btengine_global_variable.g_cur_rmt_dev);
            }
        }
        else
        {
            //֧��a2dp�İ��Ӳ�֧������
            if (g_btengine_cfg_data.enable_a2dp_flag == 1)
            {
                BT_AUTO_RECONN_PRINTF("a2dp");
                ret = app_msg_deal_a2dp_connect(btengine_global_variable.g_cur_rmt_dev);
            }
        }
    }
    return ret;
}
void deal_connect_mode(void)
{
    //������������
    //���ֻ���ģʽ
    if ((bt_auto_connect_cfg.connect_type == AUTO_CONNECT_TYPE_TIMEOUT)
            && (bt_auto_connect_cfg.connect_count_timeout != 0))
    {
        if ((btengine_global_variable.reconnect_status.reconn_times_count >= bt_auto_connect_cfg.connect_count_timeout))
        {
            goto NEED_TO_STOP_AUTOCONNECT;
        }
    }
    else if ((bt_auto_connect_cfg.connect_type == AUTO_CONNECT_TYPE_MODE)
            && (bt_auto_connect_cfg.connect_count_mode != 0))
    {
        //�������޴�
        if ((btengine_global_variable.reconnect_status.reconn_times_count >= bt_auto_connect_cfg.connect_count_mode))
        {
            goto NEED_TO_STOP_AUTOCONNECT;
        }
    }
    else if ((bt_auto_connect_cfg.connect_type == AUTO_CONNECT_TYPE_STARTUP)
            && (bt_auto_connect_cfg.connect_count_startup != 0))
    {
        if ((btengine_global_variable.reconnect_status.reconn_times_count >= bt_auto_connect_cfg.connect_count_startup))
        {
            goto NEED_TO_STOP_AUTOCONNECT;
        }
    }
    else
    {
        //do nothing for QAC
    }

    if (g_p_info->isneed_reconnect_flag == TRUE)
    {
        bool ret;
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (bt_manager_cfg_info.sp_tws_link == 1)
        {
            tws_status_info.cur_reconnect_dev = NO_TWS_DEV;
            phone_connect_flag = TRUE;
        }
#endif

        ret = deal_connect_profile();

#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (ret == FALSE)
        {
            phone_connect_flag = FALSE;
        }
#endif

        if (bt_linking_flag == FALSE)
        {
            btengine_global_variable.reconnect_status.reconn_times_count++;
        }
    }

    return;

    NEED_TO_STOP_AUTOCONNECT:
    //ֹͣ�Զ�����
    set_auto_connect_flag(0);
    g_p_info->isneed_reconnect_flag = FALSE;
    //�ڽ�������20�������ڣ�auto_connect_count�ܿ�ͱ���0��Ȼ����bt_engine_control�����ٸö�ʱ��
    return;
}

//������ֻ�ṩ�ֻ�����
void check_is_needed_reconnect(void)
{
    btengine_autoconnect_count_t *gp_reconn_sta = &btengine_global_variable.reconnect_status;
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (bt_manager_cfg_info.sp_tws_link == 1)
    {
        //it is the TWS device turn of reconnect
        if ((tws_status_info.cur_reconnect_dev == TWS_DEV) && (tws_connect_flag == TRUE))
        {
            //BT_AUTO_RECONN_DBG_PRINTF("TWS is pairing\n");
            return;
        }

        //tws is pairing
        if (TWS_GetPairedStatus() == TWS_PAIREING)
        {
            return;
        }

        //it's sink of tws
        if ((tws_status_info.have_paired_dev == TRUE) && (devs_cfg.tws_paired_list.is_source == 0))
        {
            if (TWS_GetPairedStatus() == TWS_PAIRED)
            {
                return;
            }

            //if sink cannot reconnect phone
            if (slave_connect_phone_flag == FALSE)
            {
                return;
            }
            else
            {
                //repaired_timer is a timer for tws repaired when tws disconnect with reason 0x0d
                if (repaired_timer != -1)
                {
                    return;
                }

                //this is not device need repaired
                if ((tws_status_info.cur_reconnect_dev == DEV_TYPE_NONE) && (tws_status_info.need_repaired == TRUE))
                {
                    return;
                }
            }

        }

        if ((TWS_GetPairedStatus() == TWS_PAIRED) && (devs_cfg.tws_paired_list.is_source == 1))
        {
            if (tws_status_info.tws_paired_src_reconn_phone_enable == 0)
            {
                return;
            }
        }

        //note. execute when bt not in test mode
        if (bt_test_active() == FALSE)
        {
            if (already_start_connect_phone_flag != TRUE)
            {
                bt_adjust_clk(BT_LINK_WORK_FREQ);

                bt_auto_connect_cfg.first_reconnect_after_btinit_flag = 1;
                reconnect_last_device(btengine_global_variable.g_cur_rmt_dev);
                tws_status_info.cur_reconnect_dev = NO_TWS_DEV;
                return;
            }
        }
    }
#endif

    if (g_p_info->isneed_reconnect_flag == TRUE)
    {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (bt_manager_cfg_info.sp_tws_link == 1)
        {
            //���佻�������ӻ����ͻ����ֻ�����һ�λ����ֻ�����β��ٻ����ֻ�
            if (tws_status_info.cur_reconnect_dev == NO_TWS_DEV)
            {
                if (tws_status_info.need_repaired == TRUE)
                {
                    gp_reconn_sta->reconn_times_count = 0;
                    return;
                }
            }
        }
#endif

        if (btengine_global_variable.g_cur_rmt_dev == NULL)
        {
            BT_AUTO_RECONN_PRINTF_WARNNING("dev is null\n");
        }
        gp_reconn_sta->halfsec_time_count++;

        if (bt_test_active() == TRUE) //att ����ʱ�������ʱ�Ͽ�����3�������»���
        {
            //�������ģʽ�£��������
            gp_reconn_sta->reconn_interval = 3;
        }
        else
        {
            //��һ�λ�������Ҫ��������ֻ�
            if (gp_reconn_sta->reconn_times_count == 0)
            {
                gp_reconn_sta->reconn_interval = 3;
            }

            //�û�ģʽ�£����λ�������ʱ��Ϊ20s
            if (gp_reconn_sta->reconn_interval != bt_auto_connect_cfg.connect_interval)
            {
                gp_reconn_sta->reconn_interval = bt_auto_connect_cfg.connect_interval;
            }
        }

        //�Զ���������
        if (gp_reconn_sta->halfsec_time_count >= (gp_reconn_sta->reconn_interval * COM_AUTOCONNECT_TICK_INTEVAL))
        {
            //libc_printf("\n waint reconnect %d", gp_reconn_sta->halfsec_time_count);
            deal_connect_mode();
            gp_reconn_sta->halfsec_time_count = 0;
            //����ģʽ�£��������ӳ�ʱ����ֵ
            if (bt_test_active() == TRUE)
            {
                time_out_count = 0;
            }
        }
    }
    else
    {
        gp_reconn_sta->reconn_times_count = 0;
    }
}
