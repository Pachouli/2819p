/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙推歌前台应用主循环，查询引擎状态并响应状态变化，获取按键和应用消息并响应。
 * 作者：cailizhen
 ********************************************************************************/

#include  "ap_bluetooth.h"

void update_volume_syn(void);

void btplay_get_engine_status(void)
{
    msg_apps_t msg;

    msg.type = MSG_BTPLAYEG_GET_STATUS_SYNC;
    msg.content.addr = &g_btplay_info;
    send_sync_msg(APP_ID_BTPLAY_EG, &msg, NULL, 0);
}

void btplay_tws_conform_mode(void)
{
    msg_apps_t msg;

    msg.type = MSG_BTPLAYEG_TWS_CONFORM_MODE_SYNC;
    msg.content.data[0] = FALSE;
    send_sync_msg(APP_ID_BTPLAY_EG, &msg, NULL, 0);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  根据后台蓝牙的状态进行处理
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \li
 */
/*******************************************************************************/
app_result_e bt_status_deal(void)
{
    app_result_e result = RESULT_NULL;

    com_get_btengine_status(&g_btplay_bt_info);
    g_disp_status = g_btplay_bt_info.status;

    //g_test_mode = g_btplay_bt_info.test_mode;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (g_bt_tws_enable == TRUE)
    {
        //如果蓝牙发生不可恢复的未知异常，需要RESET蓝牙模块
        if (g_btplay_info.status == BTPLAY_EXCPT)
        {
            //Step 1:卸载蓝牙播歌引擎
            _kill_conflict_engine();
            //Step 2:卸载蓝牙驱动
            com_close_bt_engine();
            //Step 3:重新装载蓝牙驱动
            com_creat_bt_engine(0);
            //Step 4:获取一次蓝牙状态
            com_get_btengine_status(&g_btplay_bt_info);
            g_disp_status = g_btplay_bt_info.status;
            //Step 5:重新装载蓝牙播歌引擎
            _create_btplay_engine();
            //Step 6:获取一次蓝牙播歌引擎状态
            btplay_get_engine_status();
            //需要恢复播歌
            g_esd_reset_need_play_flag = TRUE;
            //不播报与手机的连接TTS
            g_esd_reset_ignore_wait_tts_flag = TRUE;
            g_esd_reset_ignore_link_tts_flag = TRUE;
        }
    }
#endif

    //如果引擎出错
    if (g_disp_status == STATUS_ERROR)
    {
        result = bt_error_deal();
    }
    update_volume_syn();
    return result;
}


//同步更新远端的音量到音箱端
void update_volume_syn(void)
{
    msg_apps_t msg;
    msg_reply_t reply;
    uint8 tmp_volume;
    uint8 return_vol=0;
    if (g_btplay_bt_info.vol_syn_flag == 0)
    {
        return;
    }
    if (g_btplay_bt_info.vol_syn_flag == 1)
    {
        libc_printf_info("remote_volume:%d\n", g_btplay_bt_info.remote_volume);
        return_vol=g_btplay_bt_info.remote_volume;
        tmp_volume = (g_btplay_bt_info.remote_volume * VOLUME_VALUE_MAX) / AVRCP_VOLUME_MAX;
        tmp_volume = (tmp_volume > VOLUME_VALUE_MAX) ? VOLUME_VALUE_MAX : tmp_volume;

        com_volume_set(SET_VOL_BY_REMOTE, tmp_volume);
    }
    else if (g_btplay_bt_info.vol_syn_flag == 2)
    {
        com_volume_add(SET_VOL_BY_REMOTE);
    }
    else
    {
        com_volume_sub(SET_VOL_BY_REMOTE);
    }

    //音量同步更新完后，告知bt engine更新完成
    msg.type = MSG_BTENGINE_VOL_UPDATE_FINISH_SYNC;
    msg.content.data[0]=return_vol;
    //发送同步消息
    send_sync_msg(APP_ID_BTENGINE, &msg, &reply, 0);
}

//自动播放250ms定时器
void btplay_tick_250ms(void)
{
    g_btplay_tick_250ms = 1;
}

//A2DP和AVRCP连接成功后自动播放
void btplay_autoplay_deal(void)
{
    //断开A2DP或AVRCP后不再自动播放
    if (((g_btplay_bt_info.service_connect & A2DP_CONNECTED) == 0)
        || ((g_btplay_bt_info.service_connect & AVRCP_CONNECTED_MY) == 0))
    {
        g_avrcp_autoplay_flag = 0;
        g_avrcp_autoplay_cnt = 0;
    }

    if ((g_avrcp_autoplay_flag == 1) && (g_btplay_tick_250ms == 1))
    {
        g_btplay_tick_250ms = 0;

        //A2DP已连接但未播放
        if (g_btplay_info.status == BTPLAY_STOP)
        {
            g_send_avrcp_play_tick++;

            if (g_send_avrcp_play_tick >= (AVRCP_PLAY_START_TIME / 250)) //连上后等AVRCP_PLAY_START_TIME ms
            {
                btplay_send_avrcp_play_pause();

                g_avrcp_autoplay_try++;
                g_send_avrcp_play_tick = 0;

                libc_printf("btplay auto play!%d\n", g_avrcp_autoplay_try);

                if (g_avrcp_autoplay_try >= g_avrcp_autoplay_cnt) //发送次数限制
                {
                    g_send_avrcp_play_tick = 0;

                    g_avrcp_autoplay_flag = 0;
                    g_avrcp_autoplay_cnt = 0;
                }
            }

            g_fake_play_tick = 0;
        }
        else
        {
            g_send_avrcp_play_tick = 0;
            g_fake_play_tick++;
            //如果是播歌状态，延时3S才放弃后续发命令,过滤回连成功后的假start状态
            if (g_fake_play_tick >= (AVRCP_FAKE_START_TIME / 250))
            {
                g_avrcp_autoplay_flag = 0;
                g_avrcp_autoplay_cnt = 0;
            }
        }
    }

    if (g_avrcp_autoplay_cnt == 0)
    {
        libc_printf("btplay auto play cancel!\n");
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    MainMenu 应用的入口函数和场景管理器模块
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
    input_gui_msg_t gui_msg;
    private_msg_t pri_msg;
    btplay_status_e btplay_last_status;
    bool need_deal_status = TRUE;
    uint8 last_role = ROLE_NULL;
    uint8 drc_mode_backup;
    uint8 slave_last_drc_mode = 0; //0-NORMAL MODE, 1-AUX MODE

    //获取并更新一次状态，确保 g_btplay_info 有效
    btplay_get_engine_status();
    btplay_last_status = g_btplay_info.status;

#ifdef bt_auto_test
    get_statis_timer_id = set_app_timer(THIS_APP_TIMER_GROUP, APP_TIMER_TAG_MAIN, APP_TIMER_ATTRB_CONTROL, 12000, (timer_proc) deal_random_test);
#endif
    tick_timer_id = set_app_timer(THIS_APP_TIMER_GROUP, APP_TIMER_TAG_MAIN, APP_TIMER_ATTRB_CONTROL, 250, (timer_proc) btplay_tick_250ms);

    //注册应用消息分发函数
    set_app_msg_dispatch_handle(bt_message_deal);

    while (1)
    {
        result = bt_status_deal();
        if (result != RESULT_NULL)
        {
            break;
        }

        btplay_get_engine_status();

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_BTPLAY_RECORD_FUNC == 1))
        com_record_update_record_status(g_btplay_info.record_sta);
#endif

#ifdef ENABLE_TRUE_WIRELESS_STEREO
        //TWS连接或断开时，副箱需要redraw
        if ((g_btplay_bt_info.tws_pair_status == TWS_PAIRED) && (g_btplay_bt_info.dev_role == TWS_SLAVE))
        {
            if (!((last_tws_pair_status == TWS_PAIRED) && (last_tws_dev_role == TWS_SLAVE)))
            {
                g_need_draw = TRUE;
            }
        }
        else
        {
            if ((last_tws_pair_status == TWS_PAIRED) && (last_tws_dev_role == TWS_SLAVE))
            {
                g_need_draw = TRUE;
            }
        }

        if ((g_btplay_bt_info.tws_pair_status == TWS_PAIRED) && (g_btplay_bt_info.dev_role == TWS_SLAVE)
            && (get_tws_mode() == TWS_MODE_AUX_TWS))
        {
            if (slave_last_drc_mode == 0)
            {
                //切换DRC模式为AUX MODE
                drc_mode_backup = com_switch_drc_mode(DRC_MODE_AUX);
                libc_printf_info("drc switch to %d\n", DRC_MODE_AUX);
                slave_last_drc_mode = 1;
            }
        }
        else
        {
            if (slave_last_drc_mode == 1)
            {
                //还原DRC模式
                com_switch_drc_mode(drc_mode_backup);
                libc_printf_info("drc switch to %d\n", drc_mode_backup);
                slave_last_drc_mode = 0;
            }
        }

        //TWS 副箱LED显示仅依赖于BTPLAY ENGINE
        if ((g_btplay_bt_info.tws_pair_status == TWS_PAIRED) && (g_btplay_bt_info.dev_role == TWS_SLAVE))
        {
            if ((g_need_draw == TRUE) || (btplay_last_status != g_btplay_info.status))
            {
                bt_ui_display_tws(g_need_draw);
            }
            g_need_draw = FALSE;
        }
        //单箱或TWS 主箱LED显示依赖于手机连接状态
        else
#endif
        {
            if ((g_need_draw == TRUE) || (g_disp_status != last_disp_status))
            {
                if (g_need_draw == TRUE)
                {
                    led_clear_screen(CLR_MAIN);
                    last_role = ROLE_NULL;
                }
                bt_ui_display(g_need_draw);
                g_need_draw = FALSE;
            }
        }

        if (g_disp_status != last_disp_status)
        {
            bt_tts_play();
            last_disp_status = g_disp_status;
        }

        if ((need_deal_status == TRUE) || (g_btplay_info.status != btplay_last_status))
        {
            if (g_btplay_info.status == BTPLAY_PLAY)
            {
                com_speaker_on(SPK_PLAY);
            }
            else
            {
                com_speaker_off(SPK_PLAY);
            }
            btplay_last_status = g_btplay_info.status;
            need_deal_status = FALSE;
        }

#if 0 //设备主从角色提示，Master在Number1显示“-”符号
        if ((g_btplay_bt_info.cur_role == ROLE_MASTER) && (last_role != ROLE_MASTER))
        {
            led_display(NUMBER1, '-', TRUE);
        }
        else if ((g_btplay_bt_info.cur_role != ROLE_MASTER) && (last_role == ROLE_MASTER))
        {
            led_display(NUMBER1, 0, FALSE);
        }
        last_role = g_btplay_bt_info.cur_role;
#endif

#ifdef ENABLE_TRUE_WIRELESS_STEREO
        //主箱处理对箱应用模式和单箱应用模式切换
        if (g_btplay_bt_info.dev_role == TWS_MASTER)
        {
            if (get_tws_mode() == TWS_MODE_SINGLE)
            {
                if (g_p_global_var->slave_btplay_flag == TRUE)
                {
                    tws_switch_tws_mode(TWS_MODE_BT_TWS);
                }
            }
            else if (get_tws_mode() == TWS_MODE_BT_TWS)
            {
                if (g_p_global_var->slave_btplay_flag == FALSE)
                {
                    tws_switch_tws_mode(TWS_MODE_SINGLE);
                }
            }
        }

        if (g_bt_tws_enable == TRUE)
        {
            tws_tts_display_deal();
        }
#endif

        //ESD RESET AUTO PLAY
        if (((g_btplay_bt_info.service_connect & A2DP_CONNECTED) != 0)
            && ((g_btplay_bt_info.service_connect & AVRCP_CONNECTED_MY) != 0))
        {
            if (g_esd_reset_need_play_flag == TRUE)
            {
                g_avrcp_autoplay_cnt = 3;

                g_send_avrcp_play_tick = 0;
                g_fake_play_tick = 0;
                g_avrcp_autoplay_flag = 1;
                g_avrcp_autoplay_try = 0;
            }
            g_esd_reset_need_play_flag = FALSE;
        }

        if (g_avrcp_autoplay_cnt > 0)
        {
            btplay_autoplay_deal();
        }

        if (get_gui_msg(&gui_msg) == TRUE)
        {
            result = deal_gui_bt(&gui_msg);
        }
        else
        {
            if (get_app_msg(THIS_APP_TYPE, &pri_msg) == TRUE)
            {
                result = bt_message_deal(&pri_msg);
            }
        }

        if (result == RESULT_REDRAW)
        {
            result = RESULT_NULL;
            g_need_draw = TRUE;
        }

        if (result != RESULT_NULL)
        {
            break;
        }

#if (SUPPORT_ASET_TEST == 1)
        if (com_get_stub_tools_type() == STUB_PC_TOOL_ASET_EQ_MODE)
        {
            aset_test_loop_deal();
        }
#endif

        //挂起10ms，多任务调度
        sys_os_time_dly(1);
    }

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (slave_last_drc_mode == 1)
    {
        //还原DRC模式
        com_switch_drc_mode(drc_mode_backup);
        libc_printf_info("drc switch to %d\n", drc_mode_backup);
        slave_last_drc_mode = 0;
    }
#endif

#ifdef bt_auto_test
    if (get_statis_timer_id != -1)
    {
        kill_app_timer(THIS_APP_TIMER_GROUP, get_statis_timer_id);
        get_statis_timer_id = -1;
    }
#endif

    kill_app_timer(THIS_APP_TIMER_GROUP, tick_timer_id);
    tick_timer_id = -1;

    if (display_timer_id != -1)
    {
        kill_app_timer(THIS_APP_TIMER_GROUP, display_timer_id);
        display_timer_id = -1;
    }

    return result;
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
void bt_clear_error(void)
{
    msg_apps_t msg;
    msg_reply_t temp_reply;
    msg.type = MSG_BTENGINE_CLEAR_ERR_SYNC;
    send_sync_msg(APP_ID_BTENGINE, &msg, &temp_reply, 0);
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  蓝牙错误状态处理
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \li
 */
/*******************************************************************************/
app_result_e bt_error_deal(void)
{
    app_result_e result = RESULT_NULL;

    //硬件出错，立即退出，并设置测试失败
    if (g_btplay_bt_info.err_status == BT_ERR_HW)
    {
        result = RESULT_NEXT_FUNCTION;
    }
    else if (g_btplay_bt_info.err_status == BT_ERR_PAGE_TIMEOUT)
    {
        bt_clear_error();
    }
    else
    {
        ; //nothing for QAC
    }

    return result;
}

