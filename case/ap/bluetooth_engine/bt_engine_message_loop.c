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
#include "bt_engine_le_test.h"
#include "bt_engine_spp_test.h"
#include "bt_engine_le.h"

#define AVDTP_NO_DATA_TIME  1000 //AVDTP没有数据就关掉PA所需持续时间，单位ms

void sbc_data_transmission_hook(void)
{
    //clean idle count when get sbc packet
    btengine_global_variable.sbc_data_stop_trans_cnt = 0;
}

bool check_sbc_data_trans_stop(void) //idle:true  busy:FALSE
{
    return (btengine_global_variable.sbc_data_stop_trans_cnt >= MAX_BT_SBC_DATA_TRANS);
}

void hci_data_transmission_hook(void)
{
    //clean idle count when hci conmmand rx/tx
    btengine_global_variable.bt_idle_cnt = 0;
}

bool check_is_bt_idle(void) //idle:true  busy:FALSE
{
    return (btengine_global_variable.bt_idle_cnt >= MAX_BT_IDLE_TIME);
}

void hci_lowpower_switch_hook(uint8 low_power_mode)
{
    g_p_info->low_power_mode = low_power_mode;
    if (low_power_mode != 0)
    {
        btengine_global_variable.bt_highpw_cnt = 0;
    }
}

//最近4S是否进入过低功耗模式
bool check_is_bt_lowpw_window(void)
{
    return (btengine_global_variable.bt_highpw_cnt < MAX_BT_LOWPW_WINDOW);
}

//2Hz定时器，蓝牙引擎所有可以用2Hz定时的任务都可以挂载这里执行
void half_second_handle(void)
{
    uint8 bt_status = get_cur_device_status(btengine_global_variable.g_cur_rmt_dev);
    //auto reconnect when bt link not connected
    check_is_needed_reconnect();
    //deal profile connect after bt link connected
    auto_connect_deal();

#ifdef BT_BREDR_GET_RSSI
    //获取当前连接手机的RSSI值
    if (CURRENT_DEVICE_ACL != NULL)
    {
        g_p_info->phone_rssi.status = 0;
        bt_get_rmt_rssi(CURRENT_DEVICE_ACL);
    }
#endif

    if (check_is_bt_idle() == FALSE)
    {
        btengine_global_variable.bt_idle_cnt++;
    }

    if (check_is_bt_lowpw_window() == TRUE)
    {
        btengine_global_variable.bt_highpw_cnt++;
    }

    if ((bt_status != STATUS_WAIT_PAIRED) && (bt_status != STATUS_NONE))
    {
        if (check_sbc_data_trans_stop() == FALSE)
        {
            btengine_global_variable.sbc_data_stop_trans_cnt++;
            g_p_info->avdtp_data_pending = TRUE;
        }
        else
        {
            g_p_info->avdtp_data_pending = FALSE;
        }
    }
    else
    {
        btengine_global_variable.sbc_data_stop_trans_cnt = MAX_BT_SBC_DATA_TRANS;
        g_p_info->avdtp_data_pending = FALSE;
    }

    check_sniff_status();
    check_device_disconnect();
    con_monitor_rf_temperature();
    check_connect_timeout();
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (bt_manager_cfg_info.sp_tws_link == 1)
    {
        TWS_CheckPairedStatus();
        TWS_CheckForceUlink();

#ifdef BT_BREDR_GET_RSSI
        //获取当前连接对箱设备的RSSI值
        if (tws_status_info.acl_handle != NULL)
        {
            g_p_info->tws_rssi.status = 0;
            bt_get_rmt_rssi(tws_status_info.acl_handle);
        }
#endif        
    }
#endif

    check_hw_error_no(&(btengine_global_variable.hw_error_no));
}

//检查空闲状态，连接8秒没有数据则主动发送进入sniff信息
void check_sniff_status(void)
{
    uint8 bt_status = get_cur_device_status(btengine_global_variable.g_cur_rmt_dev);
    if ((bt_status == STATUS_WAIT_PAIRED) || (bt_status == STATUS_NONE))
    {
        return;
    }
    if (check_is_bt_idle() == TRUE)
    {
        if (bt_stack_cfg_info.sp_sniff_enable == FALSE)
        {
            return;
        }

        //某些应用不可进入sniff模式
        if (btengine_global_variable.disable_bt_sniff_by_app == TRUE)
        {
            //检测是否进入sniff，如果进入需要强制唤醒
            bt_exit_sniff(btengine_global_variable.g_cur_rmt_dev);
        }
        else
        {
            bt_enter_sniff(btengine_global_variable.g_cur_rmt_dev);
        }
    }
}

#define A2DP_FORCE_RECONN_MODE      0
#define A2DP_GENERAL_RECONN_MODE    1

//检查是否需要回连A2DP
//A2DP_FORCE_RECONN_MODE    立刻发起a2dp连接
//A2DP_GENERAL_RECONN_MODE  定时检测是否需要发起a2dp连接
void check_a2dp_isneed_recont(uint8 mode)
{
    uint16 connect_count = 0;
    if (bt_manager_cfg_info.sp_hfp == 0)
    {
        return;
    }
    if (btengine_global_variable.a2dp_linking_flag == TRUE)
    {
        BTEG_PRINTF_WARNNING("a2dp is linking..\n");
        return;
    }
    if (not_reconnect_a2dp_flag == TRUE)
    {
        return;
    }
    //如果支持HFP,并且HFP服务已经连接上了，刚需要回连A2DP
    if (((btengine_global_variable.g_cur_rmt_dev->profile & HFP_SUPPORTED) != 0)
            && ((g_p_info->service_connect & HFP_CONNECTED) != 0))
    {
        /* wait 5s, if remote not initiate a2dp-connection, we initiate.   */
        if (bt_test_active() == TRUE)
        {
            connect_count = 0;
        }
        else if (mode == A2DP_FORCE_RECONN_MODE)
        {
            BTEG_PRINTF("fast connect a2dp\n");
            A2dpConnect(0, btengine_global_variable.g_cur_rmt_dev->dev_info.bd_addr.bytes);
        }
        else
        {
            connect_count = A2DP_CONNECT_TIME_AFTER_HFP_CONNECT;
        }
        if (btengine_global_variable.connect_count > connect_count)
        {
            /* 如果设备处理等待连接状态，则发起A2DP连接 */

            BTEG_PRINTF("connect a2dp\n");
            A2dpConnect(0, btengine_global_variable.g_cur_rmt_dev->dev_info.bd_addr.bytes);
            btengine_global_variable.connect_count = 0;

        }
        else
        {
            btengine_global_variable.connect_count++;
        }
    }
}
//检查是否需要回连HID
void check_hid_isneed_recont(void)
{
    if (bt_manager_cfg_info.sp_hid == 0)
    {
        return;
    }
    if (((btengine_global_variable.g_cur_rmt_dev->profile & HID_SUPPORTED) != 0)
            && (((g_p_info->service_connect & HFP_CONNECTED) != 0)
                    || ((g_p_info->service_connect & A2DP_CONNECTED) != 0)))
    {
        if (btengine_global_variable.connect_count > CHECK_HID_CONNECT_TIME)
        {
            BTEG_PRINTF("reconnect hid\n");
            HID_connect(btengine_global_variable.g_cur_rmt_dev);
            btengine_global_variable.connect_count = 0;
        }
        else
        {
            btengine_global_variable.connect_count++;
        }
    }
}
//检查HFP服务是否已经建立，如果没有，则重建
void check_hfp_isneed_recont(void)
{
    if (bt_manager_cfg_info.sp_hfp == 0)
    {
        return;
    }
    if (btengine_global_variable.hfp.disconnect_reason == ER_HFP_RFCOMM_DISCONNECT_COMPLETE)
    {
        return;
    }
    if (((g_p_info->service_connect & HFP_CONNECTED) == 0)
            && (((g_p_info->service_connect & A2DP_CONNECTED) != 0)
                    || ((g_p_info->service_connect & HID_CONNECTED) != 0)))
    {
        //检测是否需要连接HFP
        if (btengine_global_variable.connect_count > HFP_CONNECT_TIME_AFTER_A2DP_CONNECT)
        {

            BTEG_PRINTF("reconnect HFP\n");
            HFConnectAG(btengine_global_variable.g_cur_rmt_dev);
            btengine_global_variable.connect_count = 0;
        }
        else
        {
            btengine_global_variable.connect_count++;
        }
    }
}

//检查a2dp服务是否已经建立，如果没有，则重建
uint8 check_a2dp_service(void)
{
    if (g_btengine_cfg_data.enable_a2dp_flag == 0)
    {
        return 0;
    }
    if (bt_test_active() == TRUE)
    {
        return 0;
    }
    if (btengine_global_variable.a2dp_linking_flag == TRUE)
    {
        return 0;
    }
    if (not_reconnect_a2dp_flag == TRUE)
    {
        return 0;
    }

    if (((g_p_info->service_connect & A2DP_CONNECTED) == 0)
            && (((g_p_info->service_connect & HFP_CONNECTED) != 0) || ((g_p_info->service_connect & HID_CONNECTED) != 0)))
    {
        //检测是否需要连A2DP
        if (btengine_global_variable.onlyone_disconnect_cnt > A2DP_CONNECT_TIME_AFTER_HFP_CONNECT)
        {
            //try to connect again, up to the reconnect another limit
            if ((bt_auto_connect_cfg.try_con_other_times == 0)
                    || (btengine_global_variable.reconnect_another_cnt < bt_auto_connect_cfg.try_con_other_times))
            {
                /* if phone is ongoing, wait.  */
                if (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev) == STATUS_LINKED)
                {
                    BTEG_PRINTF("reconnect another A2DP\n");
                    A2dpConnect(0, btengine_global_variable.g_cur_rmt_dev->dev_info.bd_addr.bytes);
                    btengine_global_variable.onlyone_disconnect_cnt = 0;
                    btengine_global_variable.reconnect_another_cnt++;
                }
            }
        }
        else
        {
            btengine_global_variable.onlyone_disconnect_cnt++;
        }
        return 1;
    }
    return 0;
}
//检查HFP服务是否已经建立，如果没有，则重建
uint8 check_hfp_service(void)
{
    if (bt_manager_cfg_info.sp_hfp == 0)
    {
        return 0;
    }
    if (btengine_global_variable.hfp.disconnect_reason == ER_HFP_RFCOMM_DISCONNECT_COMPLETE)
    {
        return 0;
    }

    if (((g_p_info->service_connect & HFP_CONNECTED) == 0)
            && (((g_p_info->service_connect & A2DP_CONNECTED) != 0)
                    || ((g_p_info->service_connect & HID_CONNECTED) != 0)))
    {
        //BTEG_PRINTF("\nhfp disconnect_cnt:%d\n", btengine_global_variable.onlyone_disconnect_cnt);
        //检测是否需要连接HFP
        if (btengine_global_variable.onlyone_disconnect_cnt > HFP_CONNECT_TIME_AFTER_A2DP_CONNECT)
        {
            //try to connect again, up to the reconnect another limit
            if ((bt_auto_connect_cfg.try_con_other_times == 0)
                    || (btengine_global_variable.reconnect_another_cnt < bt_auto_connect_cfg.try_con_other_times))
            {
                BTEG_PRINTF("reconnect another HFP\n");
                HFConnectAG(btengine_global_variable.g_cur_rmt_dev);
                btengine_global_variable.onlyone_disconnect_cnt = 0;
                btengine_global_variable.reconnect_another_cnt++;
            }
        }
        else
        {
            btengine_global_variable.onlyone_disconnect_cnt++;
        }
        return 1;
    }
    return 0;
}
//检查hid服务是否已经建立，如果没有，则重建
uint8 check_hid_service(void)
{
    if (bt_manager_cfg_info.sp_hid == 0)
    {
        return 0;
    }
    if ((((g_p_info->service_connect & HFP_CONNECTED) != 0) || ((g_p_info->service_connect & A2DP_CONNECTED) != 0))
            && ((g_p_info->service_connect & HID_CONNECTED) == 0))
    {
        //检测是否需要连接HID
        if (btengine_global_variable.onlyone_disconnect_cnt > CHECK_HID_CONNECT_TIME)
        {
            //try to connect again, up to the reconnect another limit
            if ((bt_auto_connect_cfg.try_con_other_times == 0) || (btengine_global_variable.reconnect_another_cnt < 1))
            {
                BTEG_PRINTF("reconnect hid\n");
                HID_connect(btengine_global_variable.g_cur_rmt_dev);
                btengine_global_variable.onlyone_disconnect_cnt = 0;
                btengine_global_variable.reconnect_another_cnt++;
            }
        }
        else
        {
            btengine_global_variable.onlyone_disconnect_cnt++;
        }
        return 1;
    }
    return 0;
}
//检查avrcp服务是否已经建立，如果没有，则重建
void check_avrcp_service(void)
{
    if (bt_manager_cfg_info.sp_avrcp == 0)
    {
        return;
    }
    if (((g_p_info->service_connect & AVRCP_CONNECTED_MY) == 0) && ((g_p_info->service_connect & A2DP_CONNECTED) != 0))
    //倘若A2DP已连接且AVRCP未连接
    {
        if (btengine_global_variable.avrcp_disconnect_cnt > CHECK_AVRCP_CONNECT_TIME)
        {
            Avrcp_Connect_Req(btengine_global_variable.g_cur_rmt_dev);
            btengine_global_variable.avrcp_disconnect_cnt = 0;
        }
        else
        {
            btengine_global_variable.avrcp_disconnect_cnt++;
        }
    }
    else
    {
        btengine_global_variable.avrcp_disconnect_cnt = 0;
    }
}
//检查是否已经所有的服务都断开了，但是实际上没有收到断开的消息
//如果连续20秒内都没有收到，则主动发起断开命令
void check_device_disconnect(void)
{
    uint8 bt_status = get_cur_device_status(btengine_global_variable.g_cur_rmt_dev);
    if ((bt_status == STATUS_NONE) || (bt_status == STATUS_WAIT_PAIRED))
    {
        profile_disconnect_count = 0;
        return;
    }
    if ((g_p_info->service_connect & 0x0f) == 0)
    {
        profile_disconnect_count++;
        if (profile_disconnect_count >= MAX_ALL_PROFILE_DIS_COUNT)
        {
            BTEG_PRINTF_WARNNING("time out disc acl\n");
            bt_disconnect_acl(btengine_global_variable.g_cur_rmt_dev);
            profile_disconnect_count = 0;
        }
    }
    else
    {
        profile_disconnect_count = 0;
    }
}
//回连的顺序为:HFP->A2DP->HID,如果某一个服务没有连接上，需要不断检测，然后回连
void auto_connect_deal(void)
{
    if (btengine_global_variable.g_cur_rmt_dev == NULL)
    {
        return;
    }
    if ((get_cur_device_status(btengine_global_variable.g_cur_rmt_dev) == STATUS_WAIT_PAIRED)
            || (bt_linking_flag == TRUE))
    {
        return;
    }

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (bt_manager_cfg_info.sp_tws_link == 1)
    {
        if ((TWS_GetPairedStatus() == TWS_PAIREING) || (TWS_GetPairedStatus() == TWS_SEARCH)
                || (tws_connect_flag == TRUE)
                || ((TWS_GetPairedStatus() == TWS_PAIRED) && (devs_cfg.tws_paired_list.is_source == 0)))
        {
            return;
        }
    }
#endif
    //有协议没有连接上需要回连的情况
    if ((g_p_info->need_auto_connect & A2DP_CONNECTED) != 0)
    {
        check_a2dp_isneed_recont(A2DP_GENERAL_RECONN_MODE);
    }
    else if ((g_p_info->need_auto_connect & HFP_CONNECTED) != 0)
    {
        check_hfp_isneed_recont();
    }

    else if ((g_p_info->need_auto_connect & HID_CONNECTED) != 0)
    {
        // HID BQB 测试要求HID不能自动回连
        if ((BQB_TEST_ENABLE & btengine_global_variable.bqb_test) == 0)
        {
            check_hid_isneed_recont();
        }
    }
    else if ((g_p_info->need_auto_connect & AVRCP_CONNECTED_MY) != 0)
    {
        //检测是否需要连接AVRCP
        check_avrcp_service();
    }
    else
    {
        btengine_global_variable.connect_count = 0;
    }
    //即使当前已不需要回连，也需要检测是否还有服务没有连上
    if (g_p_info->need_auto_connect == 0)
    {
        do
        {
            if (check_a2dp_service() == 1)
            {
                break;
            }
            if (check_hfp_service() == 1)
            {
                break;
            }

            // HID BQB 测试要求HID不能自动回连
            if ((BQB_TEST_ENABLE & btengine_global_variable.bqb_test) == 0)
            {
                if (check_hid_service() == 1)
                {
                    break;
                }
            }
            //检测是否需要连接AVRCP
            check_avrcp_service();
            btengine_global_variable.reconnect_another_cnt = 0;
            btengine_global_variable.onlyone_disconnect_cnt = 0;
        } while (0);

    }
    else
    {
        btengine_global_variable.reconnect_another_cnt = 0;
        btengine_global_variable.onlyone_disconnect_cnt = 0;
    }

}

/******************************************************************************/
/*!
 * \par  Description:
 *      处理当前应用的私有消息
 * \param[in]    PARAM_FROM_XXX
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
app_result_e btengine_deal_app_msg(void)
{
    private_msg_t cur_msg;
    app_result_e result = RESULT_NULL;

    if (get_app_msg(THIS_APP_TYPE, &cur_msg) == TRUE)
    {
        result = app_message_deal(&cur_msg);
    }

    return result;
}

/**********************************************************/
/*!功能描述:蓝牙调频接口
 *freq_level:当前的频率等级
 *注*:目前蓝牙调频分如下几档:
 1.蓝牙初始化和蓝牙配对时，调为104M
 2.蓝牙推歌和蓝牙打电话时，调为45M
 3.蓝牙空闲时，调为26M
 4.蓝牙进入sniff时，调为0M
 ***********************************************************/
void bt_adjust_clk(uint8 freq_level)
{
    if (freq_level == BT_LINK_WORK_FREQ)
    {
        bt_adjust_clk_timer = sys_get_ab_timer(); //调高频率之后，8秒之后才允许调低频率
        wait_bt_adjust_clk_down_flag = TRUE;
    }

    sys_adjust_clk(freq_level, SET_BT_CLK);
}

#ifdef ENABLE_TRUE_WIRELESS_STEREO
void tws_bt_adjust_clk(void)
{
    if (wait_bt_adjust_clk_down_flag == TRUE)
    {
        if ((sys_get_ab_timer() - bt_adjust_clk_timer) > 8000)
        {
            //仍在连接或忙状态，不降低频率
            if ((bt_linking_flag == TRUE) || (TWS_GetPairedStatus() == TWS_PAIREING)
                    || (TWS_GetPairedStatus() == TWS_SEARCH))
            {
                ; //nothing
            }
            else
            {
                bt_adjust_clk_down_do: if (g_p_info->sco_created_flag == TRUE)
                {
                    if (TWS_GetPairedStatus() == TWS_PAIRED)
                    {
                        //TWS Master 打电话场景
                        bt_adjust_clk(FREQ_48M);
                    }
                    else
                    {
                        bt_adjust_clk(FREQ_36M);
                    }
                }
                else
                {
                    if ((TWS_GetPairedStatus() == TWS_PAIRED) && (devs_cfg.tws_paired_list.is_source == 1))
                    {
                        //TWS Master 非打电话场景
                        bt_adjust_clk(FREQ_36M);
                    }
                    else
                    {
                        bt_adjust_clk(FREQ_26M);
                    }
                }
            }

            //更新Last状态
            g_last_phone_sco_created_flag = g_p_info->sco_created_flag;
            g_last_bt_linking_flag = bt_linking_flag;
            g_last_tws_pair_status = TWS_GetPairedStatus();
            wait_bt_adjust_clk_down_flag = FALSE;
        }
    }
    else
    {
        //仍在连接或忙状态，不降低频率
        if ((bt_linking_flag == TRUE) || (TWS_GetPairedStatus() == TWS_PAIREING)
                || (TWS_GetPairedStatus() == TWS_SEARCH))
        {
            ; //nothing
        }
        else
        {
            if ((g_last_bt_linking_flag == TRUE) || (g_last_tws_pair_status == TWS_PAIREING)
                    || (g_last_tws_pair_status == TWS_SEARCH))
            {
                goto bt_adjust_clk_down_do;
            }

            if ((g_last_phone_sco_created_flag == TRUE) && (g_p_info->sco_created_flag == FALSE))
            {
                if ((TWS_GetPairedStatus() == TWS_PAIRED) && (devs_cfg.tws_paired_list.is_source == 1))
                {
                    //TWS Master 非打电话场景
                    bt_adjust_clk(FREQ_36M);
                }
                else
                {
                    bt_adjust_clk(FREQ_26M);
                }
            }
            else if (g_p_info->sco_created_flag == FALSE)
            {
                if ((g_last_tws_pair_status != TWS_IDLE) && (TWS_GetPairedStatus() == TWS_IDLE))
                {
                    bt_adjust_clk(FREQ_26M);
                }
            }
        }

        //更新Last状态
        g_last_phone_sco_created_flag = g_p_info->sco_created_flag;
        g_last_bt_linking_flag = bt_linking_flag;
        g_last_tws_pair_status = TWS_GetPairedStatus();
    }
}
#endif

void normal_bt_adjust_clk(void)
{
    if (wait_bt_adjust_clk_down_flag == TRUE)
    {
        if ((sys_get_ab_timer() - bt_adjust_clk_timer) > 8000)
        {
            //仍在连接或忙状态，不降低频率
            if (bt_linking_flag == TRUE)
            {
                ; //nothing
            }
            else
            {
                bt_adjust_clk_down_do: if (g_p_info->sco_created_flag == TRUE)
                {
                    bt_adjust_clk(FREQ_36M);
                }
                else
                {
                    bt_adjust_clk(FREQ_26M);
                }
            }

            //更新Last状态
            g_last_phone_sco_created_flag = g_p_info->sco_created_flag;
            g_last_bt_linking_flag = bt_linking_flag;
            wait_bt_adjust_clk_down_flag = FALSE;
        }
    }
    else
    {
        //仍在连接或忙状态，不降低频率
        if (bt_linking_flag == TRUE)
        {
            ; //nothing
        }
        else
        {
            if (g_last_bt_linking_flag == TRUE)
            {
                goto bt_adjust_clk_down_do;
            }

            if ((g_last_phone_sco_created_flag == TRUE) && (g_p_info->sco_created_flag == FALSE))
            {
                bt_adjust_clk(FREQ_26M);
            }
        }

        //更新Last状态
        g_last_phone_sco_created_flag = g_p_info->sco_created_flag;
        g_last_bt_linking_flag = bt_linking_flag;
    }
}

void update_bt_info(void)
{
    uint32 temp_flag;
    uint32 ab_timer_diff;

    g_p_info->error_no = btengine_global_variable.hw_error_no;
    if (btengine_global_variable.hw_error_no != BT_HW_NO_ERR)
    {
        g_p_info->status = STATUS_ERROR;
    }
    else
    {
        g_p_info->status = get_cur_device_status(btengine_global_variable.g_cur_rmt_dev);
    }

    g_p_info->service_connect = (uint8) CURRENT_DEVICE_SERVICE_CONNECT;
    g_p_info->need_auto_connect = (uint8) CURRENT_DEVICE_NEED_AUTO_CONNECT;
    g_p_info->sniff_flag = (uint8) CURRENT_DEVICE_SNIFF_FLAG;
    g_p_info->bt_in_idle = (check_is_bt_idle() && check_is_bt_lowpw_window());
    g_p_info->sco_info.sco_trans_status = btengine_global_variable.hfp.sco.pncps.sco_data_in_last_1s;

#ifdef BT_SUPPORT_BLE
    if ((g_bteg_le_env.rmt_client.serv_conn & BLE_CONNECTED) != 0)
    {
    	g_p_info->ble_connect_flag = TRUE;
    }
    else
#endif
    {
    	g_p_info->ble_connect_flag = FALSE;
    }

    //超过5S没有收到就将 avdtp_data_flag 置为FALSE
    temp_flag = sys_local_irq_save();
    ab_timer_diff = sys_get_ab_timer() - btengine_global_variable.last_avdtp_data_timer;
    sys_local_irq_restore(temp_flag);

    if ((g_p_info->avdtp_data_flag == TRUE) && (ab_timer_diff > AVDTP_NO_DATA_TIME))
    {
        g_p_info->avdtp_data_flag = FALSE;
    }

    //临时调高，等待8秒钟之后允许调低频率
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (bt_manager_cfg_info.sp_tws_link == 1)
    {
        tws_bt_adjust_clk();
    }
    else
#endif
    {
        normal_bt_adjust_clk();
    }
}

uint8 btengine_deal_deal_quit(void)
{
    /* for quit app  */
    if ((btengine_global_variable.recv_quit_flag & FORCE_QUIT_APP_FLAG) == FORCE_QUIT_APP_FLAG)
    {
        /* wait 2s for complete disconnection.   */
        if (bt_linking_flag == TRUE)
        {
            btengine_global_variable.quit_count++;
            if (btengine_global_variable.quit_count > QUIT_APP_TIME_AFTER_HFP_DISCONNECT)
            {
                btengine_global_variable.quit_count = 0;
                //app强制退出，需要重置此标志位
                bt_linking_flag = FALSE;
                AppStop();
                BTEG_PRINTF_WARNNING("bt linking err..\n");
                return TRUE;
            }

        }
        else if (((btengine_global_variable.recv_quit_flag == FORCE_QUIT_APP_FLAG) && (CURRENT_DEVICE_ACL == 0))
                || (btengine_global_variable.quit_count > QUIT_APP_TIME_AFTER_HFP_DISCONNECT))
        {
            btengine_global_variable.quit_count = 0;
            AppStop();
            BTEG_PRINTF("force quit..\n");
            return TRUE;
        }
        else
        {
            btengine_global_variable.quit_count++;
        }
    }
    else
    {
        btengine_global_variable.quit_count = 0;
    }
    return FALSE;
}
/******************************************************************************/
/*!
 * \par  Description:
 *      MainMenu 应用的入口函数和场景管理器模块
 * \param[in]    PARAM_FROM_XXX
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
app_result_e get_message_loop(void)
{
    app_result_e result = RESULT_NULL;

    while (1)
    {
        BtDev_LibExc();
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (bt_manager_cfg_info.sp_tws_link == 1)
        {
            TWS_CheckPairedState();
        }
#endif

#ifdef BT_SUPPORT_BLE
        if (bt_manager_cfg_info.sp_ble == 1)
        {
#if (BT_ENGINE_BLE_TEST_ENABLE == 1)
            le_test_loop();
#endif
#if (SPP_PASSTHROUGH_TEST_ENABLE == 1)
            spp_pass_test_loop();
#endif
        }
#endif

        sco_pkg_count_insecond();

        //实时更新状态信息
        update_bt_info();

#if (ROLE_SWITCH_RETRY_ENABLE == 1)
        bt_role_switch_retry_deal();
#endif

        //蓝牙频偏校准事件处理循环
        bt_engine_freq_offset_config_loop();

        //音箱回连手机时，默认只发起hfp连接，之后的profile随手机检测连接，这样连接速度较慢
        //在hfp确立连接后，音箱立刻发起a2dp连接，可以加快a2dp的连接速度
        if (btengine_global_variable.need_connect_a2dp_right_now == TRUE)
        {
            check_a2dp_isneed_recont(A2DP_FORCE_RECONN_MODE);
            btengine_global_variable.need_connect_a2dp_right_now = FALSE;
        }

        //异步清除配对列表信息
        clear_plist_info_async_deal(&btengine_global_variable.clpl_async_info);

        //处理当前应用的私有消息
        result = btengine_deal_app_msg();

        if (result != RESULT_NULL)
        {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
            if (bt_manager_cfg_info.sp_tws_link == 1)
            {
                tws_wait_quitflag = TRUE;
            }
            else
            {
                break;
            }
#else
            break;
#endif
        }

        //退出处理
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (bt_manager_cfg_info.sp_tws_link == 1)
        {
            if (tws_wait_quitflag == FALSE)
            {
                if ((btengine_deal_deal_quit() == TRUE) && (tws_deal_tws_quit() == TRUE))
                {
                    break;
                }
            }
            else
            {
                if (tws_deal_tws_quit() == TRUE)
                {
                    break;
                }
            }
        }
        else
        {
            if (btengine_deal_deal_quit() == TRUE)
            {
                break;
            }
        }
#else
        if (btengine_deal_deal_quit() == TRUE)
        {
            break;
        }
#endif

        //挂起10ms，多任务调度
        sys_os_time_dly(1);
    }

    btengine_global_variable.recv_quit_flag = 0;
    return result;
}
