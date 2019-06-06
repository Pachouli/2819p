    /********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：自动化测试：蓝牙功能测试，通过打电话模式测试，可以测试MIC通道。
 * 作者：zhouxl
 ********************************************************************************/

#include "ap_test_head.h"
#include "btcall_common.h"
#include "bank_include/ap_autotest_char_convert.h"
#include "bank_common/ap_autotest_common_entry.h"


btengine_info_t g_bt_status;
uint8 last_disp_status;

/*
extern const uint32* ap_main_entry[];
static void show_ap_main_entry()
{
    libc_printf("print main entry: %x---------\n", (uint32)ap_main_entry);
    libc_printhex(ap_main_entry, 16, 0); 
}*/


/******************************************************************************/
/*!
 * \par  Description:
 * \  清除后台蓝牙的错误状态
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \li
 */
/*******************************************************************************/
void bt_clear_error(void)
{
    msg_apps_t msg;

    msg.type = MSG_BTENGINE_CLEAR_ERR_SYNC;
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);
}




/******************************************************************************/
/*!
 * \par  Description:
 * \  清除后台蓝牙的错误状态
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \li
 */
/*******************************************************************************/
void bt_clear_paired_list(void)
{
    msg_apps_t msg;

    msg.type = MSG_BTENGINE_CLR_PAIRED_LIST_SYNC;
    msg.content.data[0]=CLEAR_ALL_DEV_PAIRED_INFO;
    //发送同步消息
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);
}




bool test_bstack_err_deal(void)
{
    bool loop_exit = FALSE;

    if (g_bt_status.status == STATUS_ERROR)
    {
//        libc_printf("bt err:%d", g_bt_status.err_status);
//        libc_printf("service:%d", g_bt_status.service_connect);

        //硬件出错，立即退出，并设置测试失败
        if (g_bt_status.err_status == BT_ERR_HW)
        {
            bt_clear_error();
            loop_exit = TRUE;
        }
    }

    return loop_exit;
}




void test_bt_force_unlink(void)
{
    msg_apps_t msg;

    msg.type = MSG_AUTOTEST_RELEASE_CONNECT_SYNC;
    //for type: 0xFF: clear all paired list; 0x01:clear current paired list
    msg.content.data[0] = 0xFF;

    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);
}






/******************************************************************************/
/*!
 * \par  Description:
 * \  TTS播报
 * \param[in]    bt_status      bluetooth status
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \li
 */
/*******************************************************************************/
void bt_tts_play(uint8 bt_status)
{   
    switch (bt_status)
    {   
        case STATUS_NONE:
        //语音播报进入蓝牙
        com_tts_state_play(TTS_ENTER_BT, 0);
        break;

        case STATUS_WAIT_PAIRED:
        if (last_disp_status != STATUS_NONE)
        {
            //语音播报蓝牙断开&蓝牙连接
            com_tts_state_play(TTS_BT_DISCON, 0);
        }
        break;

        /* for samsung S8500, link when play, maybe pause-status.  */
        case STATUS_LINKED:
        case STATUS_A2DP_PLAY:
        case STATUS_A2DP_PAUSE:
        if ((last_disp_status == STATUS_WAIT_PAIRED) || (last_disp_status == STATUS_NONE))
        {
            //蓝牙连接成功
            com_tts_state_play(TTS_BT_CON_OK, 0);
        }
        break;

        default:
        break;
    }
}





/******************************************************************************/
/*!
 * \par  Description:
 * \  处理按键消息
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \li
 */
/*******************************************************************************/
app_result_e deal_gui_bt(input_gui_msg_t *gui_msg)
{
    uint8 status;
    msg_apps_t msg;
    app_result_e app_ret;

    app_ret = RESULT_NULL;

    status = g_bt_status.status;
    libc_printf("key type:0x%x val:%d service:0x%x status:%d\n", gui_msg->data.kmsg.type, gui_msg->data.kmsg.val, g_bt_status.service_connect, status);
    if ((KEY_TYPE_NULL != gui_msg->data.kmsg.type) && (0 != (g_bt_status.service_connect & AVRCP_CONNECTED_MY))
        && ((status == STATUS_LINKED) || (status == STATUS_A2DP_PLAY) || (status == STATUS_A2DP_PAUSE) || (status == STATUS_CALL_HFP)))
    {   
        switch (gui_msg->data.kmsg.val)
        {
            //bt播放测试是属于语音播放，无法成功切换，所以注释掉
            /*case KEY_PLAY:
            //消息类型(即消息名称)
            msg.type = MSG_BTENGINE_PLAY_PAUSE_SYNC;
            msg.content.data[0] = 0x00;
            //发送同步消息
            send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);
            break;

            case KEY_PREV:
            //消息类型(即消息名称)
            msg.type = MSG_BTENGINE_PLAY_PREV_SYNC;
            //发送同步消息
            send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);
            break;

            case KEY_NEXT:
            //消息类型(即消息名称)
            msg.type = MSG_BTENGINE_PLAY_NEXT_SYNC;
            //发送同步消息
            send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);
            break;*/

            case KEY_NULL:
            break;

            default:
            app_ret = RESULT_APP_QUIT;
            break;
        }

        com_filter_key_hold();

        sys_os_time_dly(10);
    }

    return app_ret;
}

bool _call_play(void)
{
    msg_apps_t msg;
    btcall_speech_play_para_t tmp_btcall_para;

    tmp_btcall_para.p_asqt_config = NULL;
    tmp_btcall_para.p_asqt_dump_buf = NULL;
    tmp_btcall_para.oncall = TRUE;
    tmp_btcall_para.att_flag = TRUE;

    //开始播放
    msg.type = MSG_BTCALLEG_SPEECH_PLAY_SYNC;
    msg.content.addr = &tmp_btcall_para;
    send_sync_msg(APP_ID_BTCALL_EG, &msg, NULL, 0);

    return TRUE;
}

bool _call_stop(void)
{
    msg_apps_t msg;

    //无消息内容
    msg.content.data[0] = TRUE;

    //消息类型(即消息名称)
    msg.type = MSG_BTCALLEG_SPEECH_STOP_SYNC;
    //发送同步消息
    send_sync_msg(APP_ID_BTCALL_EG, &msg, NULL, 0);

    return TRUE;
}


uint32 test_bt_manager_loop_deal(uint8 bt_test_mode)
{
    uint32 bt_test_stamp;
    uint8 loop_exit = FALSE, loop_cnt = 0;
    input_gui_msg_t gui_msg;
    bool last_sco_created_flag = FALSE;
    uint32 call_flag = FALSE;
    uint32 ret_val = TRUE;

    //连接性测试超时时间
    uint32 timeout_val = 30 * 1000;

    bt_test_stamp = sys_get_ab_timer();

    while (1)
    {   
        com_get_btengine_status(&g_bt_status);

        if (g_bt_status.status != last_disp_status)
        {
            bt_tts_play(g_bt_status.status);
            last_disp_status = g_bt_status.status;
        }

        if ((last_sco_created_flag == FALSE) && (g_bt_status.sco_created_flag == TRUE))
        {
            _call_play();
            call_flag = TRUE;

            bt_test_stamp = sys_get_ab_timer();
            timeout_val = 5 * 1000;     //开始蓝牙播放的话，只需要播放5秒
        }
        else if ((last_sco_created_flag == TRUE) && (g_bt_status.sco_created_flag == FALSE))
        {
            libc_printf("_call_stop\n");
            break;
        }
        last_sco_created_flag = g_bt_status.sco_created_flag;

        //bt test mode: 0: bt connect test; 1:bt connect & play;
        if ((0 == bt_test_mode) && (STATUS_LINKED == last_disp_status))
        {
            break;
        }

        if (get_gui_msg(&gui_msg) == TRUE)
        {
            if(RESULT_APP_QUIT == deal_gui_bt(&gui_msg))
            {
                break;
            }
        }

        if (loop_cnt == 10)
        {
            loop_exit = test_bstack_err_deal();
            loop_cnt = 0;

            if (loop_exit == TRUE)
            {
                break;
            }
        }

        loop_cnt++;
        sys_os_time_dly(10);

        //限制测试时间,超过此事件强制退出
        if ((sys_get_ab_timer() - bt_test_stamp) > timeout_val)
        {
            break;
        }
    }

    if (call_flag == TRUE)
    {
        _call_stop();
    }

    if ((g_bt_status.status > STATUS_WAIT_PAIRED) && (g_bt_status.status < STATUS_NONE))
    {
        test_bt_force_unlink();

        if (0 == bt_test_mode)
        {
            //仅连接性测试
            ret_val = TRUE;
        }else{
            //连接性测试和播放测试
            ret_val = call_flag;
        }
    }
    else
    {
        ret_val = FALSE;
    }

    return ret_val;
}




void act_test_btengine_install(btplay_test_arg_t *btplay_test_arg)
{
    bool ret;
    msg_apps_t msg;

    last_disp_status = STATUS_NONE;
    bt_tts_play(STATUS_NONE);

    //创建蓝牙后台
    com_creat_bt_engine(1);

    //创建蓝牙免提引擎
    CREATE_APP_ENGINE(APP_ID_BTCALL_EG, 0);

    btengine_setinfo_t bt_info;
    libc_memset((uint8*)&bt_info, 0, sizeof(btengine_setinfo_t));
    GET_BT_INFO(&bt_info);

    libc_memcpy(bt_info.bt_PairedAddr, btplay_test_arg->bt_transmitter_addr, 6);

    //更新配对地址列表
    msg.type = MSG_AUTOTEST_SET_RECONNECT_SYNC;
    msg.content.addr = &bt_info;

    ret = send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    //蓝牙回连
    msg.type = MSG_BTENGINE_SET_CONNECT_ADDR_SYNC;
    msg.content.addr = bt_info.bt_PairedAddr;

    ret = send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);
}




void act_test_btengine_unstall(void)
{
    msg_apps_t msg;

    msg.type = MSG_BTENGINE_CLR_PAIRED_LIST_SYNC;
    msg.content.data[0]=CLEAR_ALL_DEV_PAIRED_INFO;
    //发送同步消息
    send_sync_msg(APP_ID_BTENGINE, &msg, NULL, 0);

    //先杀掉蓝牙引擎
    KILL_CURRENT_APP_ENGINE();

    //后杀蓝牙后台
    com_close_bt_engine();
}


int32 act_test_bt_test_ext(void *arg_buffer, uint32 test_mode, uint8* log_buffer, uint32* cur_log_len)
{
    bool ret_val;
    return_result_t *return_data;
    uint16 trans_bytes = 0;

    btplay_test_arg_t *btplay_test_arg = (btplay_test_arg_t *)arg_buffer;

//    sys_os_sched_unlock();

    act_test_btengine_install(btplay_test_arg);

    ret_val = test_bt_manager_loop_deal(btplay_test_arg->bt_test_mode);

    if (test_mode != TEST_MODE_CARD)
    {   
        uint8 * stub_rw_buffer = sys_malloc(STUB_ATT_RW_TEMP_BUFFER_LEN);
        if (NULL == stub_rw_buffer) {
            libc_printf("bt_test_ext: stub_rw_buffer null\n");
            while(1){}
        }

        return_data = (return_result_t *) (stub_rw_buffer);

        return_data->test_id = TESTID_BT_TEST;

        return_data->test_result = ret_val;

        bytes_to_unicode(btplay_test_arg->bt_transmitter_addr, 5, 6, return_data->return_arg, &trans_bytes);

        //添加参数分隔符','
        return_data->return_arg[trans_bytes++] = 0x002c;

        bytes_to_unicode(&(btplay_test_arg->bt_test_mode), 0, 1, &(return_data->return_arg[trans_bytes]), &trans_bytes);

        //添加结束符
        return_data->return_arg[trans_bytes++] = 0x0000;

        //如果参数未四字节对齐，要四字节对齐处理
        if ((trans_bytes % 2) != 0)
        {
            return_data->return_arg[trans_bytes++] = 0x0000;
        }

        STUB_REPORT_RESULT((uint8*)return_data, trans_bytes * 2 + 4);

        sys_free(stub_rw_buffer);
    }
    else
    {
        ATT_TEST_ADD_RESULT_TO_SD_LOG("btplay test", ret_val, log_buffer, cur_log_len);
    }

    //卸载蓝牙相关引擎, 后台,清除配对列表,防止下次自动回连
    act_test_btengine_unstall();
//    sys_os_sched_lock();

    return ret_val;
}


