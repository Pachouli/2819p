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
#include "./../bluetooth_engine.h"

#define ATCMD_CCLK_LEN      9

const uint8 at_bldn[8] = "AT+BLDN\r";
const uint8 at_cclk[ATCMD_CCLK_LEN] = "AT+CCLK?\r";
#ifdef ENABLE_HFP_VOLUME_SYNC
uint8 at_VGS[10] = "AT+VGS=06\r";
#endif

/* hfp 获取手机时间
 AT CMD : "AT+CCLK?\r"                  获取当前手机时间
 "+CCLK: "yy/MM/dd,hh:mm:ss""  命令应答
 */
void app_msg_deal_read_phone_clock_req(bt_read_phone_time_t *p_time_reply)
{
    if ((btengine_global_variable.g_cur_rmt_dev != NULL)
            && (btengine_global_variable.g_cur_rmt_dev->p_dev_status != NULL))
    {
        if (CURRENT_DEVICE_SNIFF_FLAG != 0)
        {
            bt_exit_sniff(btengine_global_variable.g_cur_rmt_dev);
        }
        HFP_ExtendCmd((uint8 * )btengine_global_variable.g_cur_rmt_dev->p_dev_status->g_hf_handle, (void * )at_cclk,
                ATCMD_CCLK_LEN, 0);
        //indicate at_cmd has been sent
        p_time_reply->replay = REQ_DATA_INVALID;
        HFP_PRINTF("Get phone time\n");
    }
    else
    {
        //indicate bt_status err and at_cmd hasn't send
        p_time_reply->replay = BT_NO_READY;
        HFP_PRINTF_WARNNING("no deviece or p_dev_status\n");
    }
}

//蓝牙免提接听电话
void app_msg_deal_accept_call(void)
{
    switch (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev))
    {
        case STATUS_WAIT_PAIRED:
        break;
        case STATUS_LINKED:
        break;
        case STATUS_A2DP_PLAY:
        break;
        case STATUS_A2DP_PAUSE:
        break;
        case STATUS_CALL_INCOMING:
        HFP_PRINTF("accept call\n")
        ;
        deal_device_phone_status();

#ifdef CONFIG_PHONEBOOK    //不再播报
        if (g_btengine_cfg_data.hfp_pb_mode != HFP_PB_DISABLE)
        {
            libc_memset(&pb_result, 0, sizeof(pb_search_result_t) );
            pb_result.validstatus = PB_FIND_NAME;
            btengine_global_variable.g_btengine_info.phone_status = 0;
        }
#endif
        HFCmdInRing(btengine_global_variable.g_cur_rmt_dev, HFP_EV_ANSWER_CALL_REQ);
        break;
        case STATUS_CALL_OUTGOING:
        break;
        case STATUS_CALL_HFP:
        break;
        case STATUS_CALL_PHONE:
        break;
        case STATUS_NONE:
        break;
        default:
        break;
    }
}
//蓝牙免提拒听电话
void app_msg_deal_reject_call(void)
{
    switch (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev))
    {
        case STATUS_WAIT_PAIRED:
        break;
        case STATUS_LINKED:
        break;
        case STATUS_A2DP_PLAY:
        break;
        case STATUS_A2DP_PAUSE:
        break;
        case STATUS_CALL_INCOMING:
        HFP_PRINTF("reject call\n")
        ;
        deal_device_phone_status();

#ifdef CONFIG_PHONEBOOK    //不再播报
        if (g_btengine_cfg_data.hfp_pb_mode != HFP_PB_DISABLE)
        {
            libc_memset(&pb_result, 0, sizeof(pb_search_result_t) );
            pb_result.validstatus = PB_FIND_NAME;
            btengine_global_variable.g_btengine_info.phone_status = 0;
        }
#endif
        HFCmdInRing(btengine_global_variable.g_cur_rmt_dev, HFP_EV_CANCEL_CALL_REQ);
        break;
        case STATUS_CALL_OUTGOING:
        break;
        case STATUS_CALL_HFP:
        break;
        case STATUS_CALL_PHONE:
        break;
        case STATUS_NONE:
        break;
        default:
        break;
    }
}
//蓝牙免提挂断电话
void app_msg_deal_handup_call(void)
{
    switch (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev))
    {
        case STATUS_WAIT_PAIRED:
        break;
        case STATUS_LINKED:
        break;
        case STATUS_A2DP_PLAY:
        break;
        case STATUS_A2DP_PAUSE:
        break;
        case STATUS_CALL_INCOMING:
        break;
        case STATUS_CALL_OUTGOING:
        case STATUS_CALL_HFP:
        case STATUS_CALL_PHONE:
        HFP_PRINTF("handup call\n")
        ;
        HFCmdInRing(btengine_global_variable.g_cur_rmt_dev, HFP_EV_CANCEL_CALL_REQ);
        break;
        case STATUS_NONE:
        break;
        default:
        break;
    }
}

//建立hfp连接通道
bool app_msg_deal_hfp_connect(bt_dev_t *dev)
{
    bool ret = FALSE;
    switch (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev))
    {
        case STATUS_WAIT_PAIRED:
        if (bt_linking_flag == TRUE)
        {
            HFP_PRINTF_WARNNING("bt linking....\n");
            break;
        }
        if (dev != NULL)
        {
            if ((btengine_global_variable.connect_lock_flag < g_btengine_cfg_data.max_link_num)
                    && (!((dev->p_dev_status != NULL) && (dev->p_dev_status->g_hf_handle != 0))))
            {
                HFP_PRINTF("auto Connect hfp,device name %s\n", (uint8 * ) &dev->dev_info.name);
                bt_set_visual_mode(GAP_VM_PAIR);

                ret = HFConnectAG(dev);
            }
            else
            {
                HFP_PRINTF_WARNNING("reconnect para err %d\n", btengine_global_variable.connect_lock_flag);
            }
        }
        else
        {
            HFP_PRINTF_WARNNING("reconnect device no exist....\n");
        }
        break;

        case STATUS_LINKED:
        case STATUS_A2DP_PLAY:
        case STATUS_A2DP_PAUSE:
        /* some phone connect hfp fail but a2dp success, so continue to connect hfp. */
        if (((g_p_info->need_auto_connect & HFP_CONNECTED) != 0)
                && (((g_p_info->service_connect) & HFP_CONNECTED) == 0))
        {
            HFP_PRINTF("connect after a2dp-connect\n");
            ret = HFConnectAG(dev);
        }
        break;

        case STATUS_CALL_INCOMING:
        break;
        case STATUS_CALL_OUTGOING:
        break;
        case STATUS_CALL_HFP:
        break;
        case STATUS_CALL_PHONE:
        break;
        case STATUS_NONE:
        break;
        default:
        break;
    }
    return ret;
}

void app_msg_deal_transmit_dtmf(uint8 dtmf_code)
{
    switch (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev))
    {
        case STATUS_CALL_HFP:
        case STATUS_CALL_PHONE:
        if (((dtmf_code >= '0') && (dtmf_code <= '9')) || (dtmf_code == '*') || (dtmf_code == '#'))
        {
            HFP_HF_TxDTMF((uint8 * ) btengine_global_variable.g_cur_rmt_dev->p_dev_status->g_hf_handle, dtmf_code);
        }
        break;

        default:
        break;
    }

}

void app_msg_deal_dial_phone_number(uint8 *phone_num)
{
    uint32 phone_len;
    /* phone_num format: *phone_num='1';*(phone_num+1)='1';*(phone_num+2)='0';*(phone_num+3)=0x00; */
    //如果phone_num为NULL则拔打最后一个电话
    //否则拔打指定的电话号码
    if (phone_num == NULL)
    {
        return;
    }
    phone_len = strlen(phone_num);
    if ((phone_len >= 31) || (phone_len < 3))
    {
        return;
    }
    switch (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev))
    {
        case STATUS_LINKED:
        case STATUS_A2DP_PLAY:
        case STATUS_A2DP_PAUSE:
        HFP_HF_Dial((uint8 * ) btengine_global_variable.g_cur_rmt_dev->p_dev_status->g_hf_handle, phone_num);
        break;

        default:
        break;
    }

}

#ifdef ENABLE_HFP_VOLUME_SYNC
void app_hfp_volume_update(uint8 g_volume)
{
    uint8 at_vgs_Len = 9;
    uint8 bt_status;
    bt_status = get_cur_device_status(btengine_global_variable.g_cur_rmt_dev);
    if ((bt_status == STATUS_CALL_HFP) || (bt_status == STATUS_CALL_OUTGOING))
    {

        if (g_btengine_cfg_data.g_hf_spk_vol != g_volume)
        {
            HFP_PRINTF("at+vgs %d\n", g_volume);
            if (g_volume >= 10)
            {
                at_VGS[7] = '1';
                at_VGS[8] = (g_volume % 10) + '0';
                at_vgs_Len = 10;
            }
            else
            {
                at_VGS[7] = g_volume + '0';
            }
            at_VGS[at_vgs_Len - 1] = '\r';
            HFP_ExtendCmd((uint8 * )btengine_global_variable.g_cur_rmt_dev->p_dev_status->g_hf_handle, at_VGS,
                    at_vgs_Len, 0);
            g_btengine_cfg_data.g_hf_spk_vol = g_volume;
        }
        btengine_global_variable.hfp.allow_sync_vol_to_ap_flag = TRUE;
    }

    return;
}

#endif

void APP_Create_SCO_Timer_Proc(void)
{
    //建立SCO链路失败时，2S超时后将标志清除
    btengine_global_variable.hfp.sco.sco_conn_status = SCO_CREATE_READY;
}

uint8 app_message_deal_hfp(private_msg_t *pri_msg)
{
    uint8 rst;
    uint8 bt_status;
    bool is_call_status;
    bt_status = get_cur_device_status(btengine_global_variable.g_cur_rmt_dev);

    is_call_status = FALSE;
    if ((bt_status == STATUS_CALL_INCOMING) || (bt_status == STATUS_CALL_OUTGOING) || (bt_status == STATUS_CALL_HFP)
            || (bt_status == STATUS_CALL_PHONE))
    {
        is_call_status = TRUE;
    }

    rst = 0;
    switch (pri_msg->msg.type)
    {
        case MSG_BTENGINE_HFP_CONNECT_SYNC: //连接hfp
        //支持HFP的案子才支持连接
        if (bt_manager_cfg_info.sp_hfp == 1)
        {
            app_msg_deal_hfp_connect(btengine_global_variable.g_cur_rmt_dev);
        }
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_MGR_SETSYN_SYNC: //创建sco连接
        //libc_printf("\n sco_handle:0x%x\n", (uint8) CURRENT_DEVICE_SCOHDL);
        //libc_printf("\n sco_connect_status:0x%d\n", btengine_global_variable.hfp.sco.sco_conn_status);
        if ((CURRENT_DEVICE_SCOHDL == 0) && (btengine_global_variable.hfp.sco.sco_conn_status == SCO_CREATE_READY))
        {
            load_micout_timer();

            if (btengine_global_variable.hfp.sco.create_sco_timer_id != -1)
            {
                kill_app_timer(THIS_APP_TIMER_GROUP, btengine_global_variable.hfp.sco.create_sco_timer_id);
                btengine_global_variable.hfp.sco.create_sco_timer_id = -1;
            }

            btengine_global_variable.hfp.sco.create_sco_timer_id = set_single_shot_app_timer(THIS_APP_TIMER_GROUP,
                    APP_TIMER_TAG_MAIN, APP_TIMER_ATTRB_CONTROL, 2000, APP_Create_SCO_Timer_Proc);

            btengine_global_variable.hfp.sco.sco_conn_status = SCO_CREATING;
            HFP_HF_AudioConnTrans((uint8 * ) btengine_global_variable.g_cur_rmt_dev->p_dev_status->g_hf_handle);
            HFP_PRINTF("AG -> HF\n");
            btengine_reply_msg(pri_msg, TRUE);
        }
        else
        {
            btengine_reply_msg(pri_msg, FALSE);
        }
        break;

        case MSG_BTENGINE_MGR_RELEASESYN_SYNC: //断开sco连接
        if (CURRENT_DEVICE_SCOHDL != 0)
        {
            //HFP_DBG_PRINTF("switch SCO disconnect\n");
            unload_micout_timer();
            HFP_HF_AudioConnTrans((uint8 * ) btengine_global_variable.g_cur_rmt_dev->p_dev_status->g_hf_handle);
            HFP_PRINTF("HF -> AG\n");
            btengine_reply_msg(pri_msg, TRUE);
        }
        else
        {
            btengine_reply_msg(pri_msg, FALSE);
        }
        break;

        case MSG_BTENGINE_ACCEPT_CALL_SYNC: //接听
        app_msg_deal_accept_call();
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_REJECT_CALL_SYNC: //拒接
        app_msg_deal_reject_call();
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_HANDUP_CALL_SYNC: //挂断
        app_msg_deal_handup_call();
        btengine_reply_msg(pri_msg, TRUE);
        break;

        //外拨电话
        case MSG_BTENGINE_OUTGOING_CALL_SYNC:
        if (((g_p_info->service_connect & HFP_CONNECTED) != 0) && (is_call_status == FALSE))
        {
            HFP_ExtendCmd((uint8 * ) btengine_global_variable.g_cur_rmt_dev->p_dev_status->g_hf_handle,
                    (void * ) at_bldn, 8, 0);
        }
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_TRANSMIT_DTMF_SYNC: //transmit a specific DTMF code
        app_msg_deal_transmit_dtmf((uint8) pri_msg->msg.content.data[0]);
        btengine_reply_msg(pri_msg, TRUE);
        break;

        case MSG_BTENGINE_DIAL_PHONE_NUMBER_SYNC: //Place a Call with the Phone Number Supplied by the HF
        app_msg_deal_dial_phone_number(pri_msg->msg.content.addr);
        btengine_reply_msg(pri_msg, TRUE);
        break;

#ifdef ENABLE_SIRI
        case MSG_BTENGINE_SET_SIRI_SYNC:
        if (((g_p_info->service_connect & HFP_CONNECTED) != 0) && (is_call_status == FALSE))
        {
            //            uint8 temp_buf[10];
            if (pri_msg->msg.content.data[0] == BTENGINE_MSG_SIRI_START)
            {
                if (bt_status != STATUS_SIRI)
                {
                    HFP_PRINTF("SIRIstart!\n");
                    HFP_HF_VoiceRecognitionReq(
                            (uint8 * )btengine_global_variable.g_cur_rmt_dev->p_dev_status->g_hf_handle, 1);
                    btengine_global_variable.hfp.check_siri_answer = 1;
                }
            }
            else
            {
                if (bt_status == STATUS_SIRI)
                {
                    HFP_PRINTF("SIRIstop!\n");
                    HFP_HF_VoiceRecognitionReq(
                            (uint8 * )btengine_global_variable.g_cur_rmt_dev->p_dev_status->g_hf_handle, 0);
                    //btengine_global_variable.g_check_siri_answer = 2;
                    btengine_global_variable.hfp.check_siri_answer = 0;
                    set_cur_device_status(btengine_global_variable.g_cur_rmt_dev, STATUS_LINKED);
                    btengine_global_variable.hfp.voice_recognition_status = ACTIVATE;
                }
            }
        }
        btengine_reply_msg(pri_msg, TRUE);
        break;
#endif

#ifdef SUPPORT_3_WAY_CALL
        case MSG_BTENGINE_3WAY_HANDLE_SYNC:
        {
            if (bt_manager_cfg_info.sp_hfp_3way_call == 1)
            {
                hfp_3way_handle_e hd_mode = pri_msg->msg.content.data[0];

                if (hd_mode == HFP_3WAY_RELEASE_HELD)
                {
                    HFP_PRINTF("release held call\n");
                }
                else if (hd_mode == HFP_3WAY_RELEASE_ACTIVE)
                {
                    HFP_PRINTF("release active call\n");
                }
                else if (hd_mode == HFP_3WAY_SWAP_CALL)
                {
                    HFP_PRINTF("held active swap\n");
                }
                else
                {
                    HFP_PRINTF("transform\n");
                }
                HFP_HF_3WayCallingHandler((uint8 * )btengine_global_variable.g_cur_rmt_dev->p_dev_status->g_hf_handle,
                        hd_mode, 0);
            }
        }
        btengine_reply_msg(pri_msg, TRUE);
        break;
#endif

        /*! 蓝牙免提数据过滤控制 */
        case MSG_BTENGINE_SCO_PIPE_CTRL_SYNC:
        {
            uint8 pipe_op = (uint8) pri_msg->msg.content.data[0];
            uint32 temp_flag;

            temp_flag = sys_local_irq_save();

            if ((pipe_op & PIPE_OP_FILTER) != 0)
            {
                btengine_global_variable.hfp.sco.filter_sco_input_flag = DATA_FILTER;
            }
            if ((pipe_op & PIPE_OP_UNFILTER) != 0)
            {
                btengine_global_variable.hfp.sco.filter_sco_input_flag = DATA_UNFILTER;
            }
            if ((pipe_op & PIPE_OP_FLUSH) != 0)
            {
                //将 sco input pipe 清空，以免有杂音
                g_sco_input_buf.length = 0;
                g_sco_input_buf.read_ptr = 0;
                g_sco_input_buf.write_ptr = 0;

                g_sco_err_report_info.length = 0;
                g_sco_err_report_info.read_ptr = 0;
                g_sco_err_report_info.write_ptr = 0;

                g_sco_output_buf.length = 0;
                g_sco_output_buf.read_ptr = 0;
                g_sco_output_buf.write_ptr = 0;
            }
            sys_local_irq_restore(temp_flag);
        }
        btengine_reply_msg(pri_msg, TRUE);
        break;

        /*! 进入蓝牙免提应用需要做的事情 */
        case MSG_BTENGINE_ENTER_HFP_CALL_SYNC:
        g_p_info->enter_hfp_flag = TRUE;
        hfp_thread_init();
        btengine_reply_msg(pri_msg, TRUE);
        break;

        /*! 退出蓝牙免提应用需要做的事情 */
        case MSG_BTENGINE_EXIT_HFP_CALL_SYNC:
        hfp_thread_exit();
        g_p_info->enter_hfp_flag = FALSE;
        btengine_reply_msg(pri_msg, TRUE);
        break;
        /*!向远端上报电池电量*/
        case MSG_BTENGINE_REPORT_BAT_SYNC:
        {
            uint8 mode, bat_val;
            mode = (uint8) pri_msg->msg.content.data[0];
            bat_val = (uint8) pri_msg->msg.content.data[1];
            report_battery_to_rmtdevice(mode, bat_val);
            btengine_reply_msg(pri_msg, TRUE);
        }
        break;
        case MSG_BTENGINE_READ_PHONE_CLOCK_REQ_SYNC:
        {
            bt_read_phone_time_t *p_time_reply = hfp_get_bt_read_phone_time();
            app_msg_deal_read_phone_clock_req(p_time_reply);
            btengine_reply_msg(pri_msg, TRUE);
        }
        break;
        case MSG_BTENGINE_READ_PHONE_CLOCK_SYNC:
        {
            bt_read_phone_time_t *p_time_reply = hfp_get_bt_read_phone_time();
            libc_memcpy(pri_msg->msg.content.addr, p_time_reply, sizeof(bt_read_phone_time_t));
            btengine_reply_msg(pri_msg, TRUE);
        }
        break;
        default:
        rst = 1;
        break;
    }

    return rst;
}

