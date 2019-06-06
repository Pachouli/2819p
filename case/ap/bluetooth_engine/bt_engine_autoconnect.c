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
 * \brief    MainMenu的main函数模块
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
        //支持HFP的案子或者是测试模式时才支持连接
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
            //支持a2dp的案子或者是测试模式时才支持连接
            if (g_btengine_cfg_data.enable_a2dp_flag == 1)
            {
                BT_AUTO_RECONN_PRINTF("a2dp");
                ret = app_msg_deal_a2dp_connect(btengine_global_variable.g_cur_rmt_dev);
            }
        }
        else if ((btengine_global_variable.local_device_support_profile & (uint8) HFP_SUPPORTED) != 0)
        {
            //支持HFP的案子才支持连接
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
            //支持a2dp的案子才支持连接
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
    //回连次数限制
    //三种回连模式
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
        //回连有限次
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
    //停止自动回连
    set_auto_connect_flag(0);
    g_p_info->isneed_reconnect_flag = FALSE;
    //在接下来的20秒周期内，auto_connect_count很快就被清0，然后在bt_engine_control中销毁该定时器
    return;
}

//本函数只提供手机回连
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
            //音箱交替进行组队回连和回连手机，上一次回连手机，这次不再回连手机
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

        if (bt_test_active() == TRUE) //att 测试时，如果超时断开，则3秒内重新回连
        {
            //随机测试模式下，尽快回连
            gp_reconn_sta->reconn_interval = 3;
        }
        else
        {
            //第一次回连，需要尽快回连手机
            if (gp_reconn_sta->reconn_times_count == 0)
            {
                gp_reconn_sta->reconn_interval = 3;
            }

            //用户模式下，两次回连间间隔时间为20s
            if (gp_reconn_sta->reconn_interval != bt_auto_connect_cfg.connect_interval)
            {
                gp_reconn_sta->reconn_interval = bt_auto_connect_cfg.connect_interval;
            }
        }

        //自动回连处理
        if (gp_reconn_sta->halfsec_time_count >= (gp_reconn_sta->reconn_interval * COM_AUTOCONNECT_TICK_INTEVAL))
        {
            //libc_printf("\n waint reconnect %d", gp_reconn_sta->halfsec_time_count);
            deal_connect_mode();
            gp_reconn_sta->halfsec_time_count = 0;
            //测试模式下，清零连接超时计数值
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
