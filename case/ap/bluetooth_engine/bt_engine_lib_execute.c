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
#include "bluetooth_engine.h"
#include "bt_engine_device_control.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    read hci data to BTSRC-Lib-HCI.
 * \param[in]    none.
 * \param[in]    none.
 * \param[out]   none
 * \return       none.
 * \retval       1 sucess
 * \retval       0 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *  2013年8月30日 添加内存快满时主动丢掉sbc数据功能;
 *  hci_overflow: 1 表明HCI BUFFER快要满，需要判断是否要丢数
 *                0 表明HCI BUFFER正常，
 *******************************************************************************/
#ifdef ENABLE_SBC_DROP_LOG
uint32 drop_data_cnt = 0;
uint32 acl_pb_err = 0;
#endif

bt_dev_t *BT_FindDevByAclHdl(uint16 acl_hdl)
{
    uint8 idx;
    bt_dev_t *p_dev;

    p_dev = NULL;

    for (idx = 0; idx < g_btengine_cfg_data.max_link_num; idx++)
    {
        if ((btengine_global_variable.linked_dev_list[idx] != NULL)
                && (btengine_global_variable.linked_dev_list[idx]->p_dev_status != NULL)
                && (btengine_global_variable.linked_dev_list[idx]->p_dev_status->acl_handle == acl_hdl))
        {
            p_dev = btengine_global_variable.linked_dev_list[idx];
            break;
        }
    }
    return p_dev;
}

bt_dev_t *BTFindDevByAclHdl(uint16 acl_hdl)
{
    bt_dev_t *p_dev;

    p_dev = NULL;
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (bt_manager_cfg_info.sp_tws_link == 1)
    {
        p_dev = TWS_FindByAclHdl(acl_hdl);
        if (p_dev != NULL)
        {
            return p_dev;
        }
    }
#endif
    p_dev = BT_FindDevByAclHdl(acl_hdl);
    return p_dev;

}

void read_phone_info(void)
{
#ifdef CONFIG_PHONEBOOK
    static uint16 sendcount = 0;
    if((g_p_info->service_connect & HFP_CONNECTED) && (g_btengine_cfg_data.hfp_pb_mode == HFP_PB_ALL))
    {
        sendcount++;
        //延迟2.6S，再启动读取电话本流程
        if(sendcount >= 0x10a)
        {
            //pb的执行周期为100ms
            if(pb_info.pb_enable == 1)
            {
                sendcount = 0x100;
                pb_loop_deal_req();
            }
        }
    }
#endif
}
uint8 get_ignore_hci_data_flag(uint8 mode, uint32 para)
{
    uint8 ignore_flag = 0;
    uint8 bt_status;

    bt_status = get_cur_device_status(btengine_global_variable.g_cur_rmt_dev);

    if (mode == SCO_EVENT)
    {
        uint16 sco_hdl = (uint16) para;

        //主动过滤sco数据
        if (btengine_global_variable.hfp.sco.filter_sco_input_flag == DATA_FILTER)
        {
            ignore_flag = 1;
        }

        //未完全进入免提状态，过滤sco数据
        if (g_p_info->enter_hfp_flag == 0)
        {
            ignore_flag = 1;
        }

        //SCO句柄错误，丢弃当前SCO包
        if (sco_hdl != CURRENT_DEVICE_SCOHDL)
        {
            ignore_flag = 1;
        }

        //需要过滤非通话状态时接收到的数据时
        if (btengine_param.ignore_uncall_sco == 1)
        {
            //如果处于非通话状态，那么需要过滤
            if ((bt_status != STATUS_CALL_HFP) && (bt_status != STATUS_CALL_INCOMING))
            {
                ignore_flag = 1;
            }
        }
    }
    else if (mode == SBC_EVENT)
    {
        bt_dev_t *p_bt_dev = (bt_dev_t *) para;

        sbc_data_transmission_hook();

        //主动过滤avdtp数据
        if ((btengine_global_variable.filter_avdtp_sound_flag == 1)
                || (btengine_global_variable.avdtp_data_source != AVDTP_SOURCE_BT_A2DP))
        {
            ignore_flag = 1;
        }

        //已进入蓝牙通话，此时avdtp数据将被过滤
        if (g_p_info->enter_hfp_flag == 1)
        {
            ignore_flag = 1;
        }

        //组包接收中出错时，丢弃余下该组余下部分包
        if ((p_bt_dev->p_dev_status != NULL) && (p_bt_dev->p_dev_status->acl_drop_next_flag != 0))
        {
            ignore_flag = 1;
        }

        //按下暂停后一段时间后，过滤其avdtp数据
        if (ignore_sbc_after_pause == 1)
        {
            if ((sys_get_ab_timer() - ignore_sbc_after_pause_timer) < TIMER_IGNORE_SBC_AFTER_PAUSE)
            {
                ignore_flag = 1;
                ignore_sbc_after_pause = 0;
            }
        }

        //p_bt_dev不是当前激活设备时，丢弃接收到的包
        if (p_bt_dev != btengine_global_variable.g_cur_rmt_dev)
        {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
            //tws设备不属于激活设备，副箱收到主箱数据不能过滤
            if ((bt_manager_cfg_info.sp_tws_link == 1)
                    && (g_p_info->dev_type == TWS_DEV) /*&& (tws_status_info.is_src_flag == TWS_SLAVE)*/) //TODO 需要完善判断条件，加上是否主箱发送给副箱？ cailizhen
            {

            }
            else
#endif
            {
                //过滤非激活设备数据
                ignore_flag = 1;
            }
        }
    }
    else
    {

    }
    return ignore_flag;
}

uint16 get_frame_data(void *data_addr, uint16 data_len)
{
    uint8* media_payload;
    uint16 media_payload_len;

    if (btengine_global_variable.media_codec_type == A2DP_CODEC_AAC)
    {
        media_payload = data_addr; //get_aac_frame_data(data_addr);

    }
    else
    {
        //get sbc media payload
        media_payload = get_sbc_frame_data(data_addr);
    }

    media_payload_len = data_len - ((uint32) media_payload - (uint32) data_addr);

#if (SUPPORT_A2DP_UNIT_TEST == 1)
    a2dp_unit_test_get_package(media_payload, data_len);
#endif

#if (SUPPORT_ONLY_BT_TEST == 0)
    if (buffer_rw_write(&g_a2dp_input_buf, media_payload, media_payload_len) == FALSE)
    {
        A2DP_PRINTF_WARNNING("input buf full\n");
    }
#endif

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (bt_manager_cfg_info.sp_tws_link == 1)
    {
        if (TWS_GetPairedStatus() != TWS_PAIRED)
        {
            return 0;
        }

        if (tws_status_info.acl_handle == (uint16) NULL)
        {
            return 0;
        }

        if (get_tws_mode() == TWS_MODE_SINGLE)
        {
            return 0;
        }

        //只有进入对箱应用模式，主箱才会转发数据给副箱
        if (tws_status_info.is_src_flag == TRUE)
        {
            TWS_SourceSendData(media_payload, media_payload_len);
        }
    }
#endif
    return 0;
}
uint8 deal_linkkey_miss_reject(uint8 *p_buf)
{
    bt_dev_t *dev;
    if ((bt_stack_cfg_info.sp_linkkey_miss_reject != 0) && (g_p_info->need_auto_connect != 0))
    {
        dev = RmtFindDevByBD(p_buf + 2);
        if (dev != NULL)
        {
            BTEG_DBG_PRINTF("miss linkkey reject!\n");
            bt_disconnect_acl(dev);
            set_auto_connect_flag(0);
            BT_SetReconnectFlag(dev->dev_type, FALSE);
            //g_p_info->isneed_reconnect_flag = FALSE;
        }
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}

void hci_role_discover_complete_handle(uint16 connect_hdl, uint8 role)
{
    bt_dev_t *dev;

    dev = BTFindDevByAclHdl(connect_hdl);
    if (dev == NULL)
    {
        return;
    }

    //only report the role of connect between phone and audio box
    if (dev->dev_type == TWS_DEV)
    {
        if ((dev->is_source == 1) && (role == ROLE_SLAVER))
        {
            bt_set_cur_role_with_retry(dev, ROLE_MASTER, ROLE_SWITCH_RETRY_INVL, ROLE_SWITCH_RETRY_TIMES);
        }
        return;
    }

    //only report the role that dev is current active dev
    if (dev == btengine_global_variable.g_cur_rmt_dev)
    {
        btengine_global_variable.g_btengine_info.cur_role = role;
    }
}

void hci_role_already_change_ind(uint8 status, struct HCI_Switch_RoleStru *role_change)
{
    bt_role_change_ind(status, role_change);
}

#ifdef BT_BREDR_GET_RSSI
void hci_get_rssi_complete(bt_bredr_rssi_st *rssi)
{
    if (rssi->stauts == HCI_STATUS_OK)
    {
        if (rssi->acl_hdl == CURRENT_DEVICE_ACL)
        {
            g_p_info->phone_rssi.rssi = rssi->rssi;
            g_p_info->phone_rssi.status = 1;
        }

#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (rssi->acl_hdl == tws_status_info.acl_handle)
        {
            g_p_info->tws_rssi.rssi = rssi->rssi;
            g_p_info->tws_rssi.status = 1;
        }
#endif        
    }
    else
    {
        if (rssi->acl_hdl == CURRENT_DEVICE_ACL)
        {
            g_p_info->phone_rssi.rssi = 0;
            g_p_info->phone_rssi.status = 0;
        }

#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (rssi->acl_hdl == tws_status_info.acl_handle)
        {
            g_p_info->tws_rssi.rssi = 0;
            g_p_info->tws_rssi.status = 0;
        }
#endif  
    }
}
#endif

void init_callback_func(void)
{
    regiter_func.FindDevByAclHdl = BTFindDevByAclHdl;
    regiter_func.get_ignore_flag = (func_get_ignore_flag) get_ignore_hci_data_flag;
    regiter_func.get_frame_data = get_frame_data;
    regiter_func.deal_acl_status = read_avdtp_sound_hook;
    regiter_func.save_sco_data = btengine_save_sco_data;
    regiter_func.deal_linkkey_miss = deal_linkkey_miss_reject;
    regiter_func.hci_data_transmission_hook = hci_data_transmission_hook;
    regiter_func.hci_lowpower_switch_hook = hci_lowpower_switch_hook;
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    regiter_func.tws_rx_cmd_deal = TWS_RxCmdDeal;
    regiter_func.tws_GetCmdData_Send = TWS_Get_Cmd_Data_ToSend;
    regiter_func.tws_deal_acl_handle = tws_deal_acl_handle;
    regiter_func.tws_get_aclhandle_sendcount = tws_get_aclhandle_sendcount;
#endif
    regiter_func.hci_role_change_ind = hci_role_discover_complete_handle;
    regiter_func.stack_printf_log = btengine_global_variable.printf.log;
    regiter_func.stack_printf_debug = btengine_global_variable.printf.log_dbg;
    regiter_func.hci_role_already_change_ind = hci_role_already_change_ind;
#ifdef BT_BREDR_GET_RSSI    
    regiter_func.hci_get_rssi_complete = hci_get_rssi_complete;
#endif
    regiter_callback_func(&regiter_func);
}

