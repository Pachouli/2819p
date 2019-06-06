/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙推歌前台应用 TWS 处理。
 * 作者：cailizhen
 ********************************************************************************/

#include  "ap_bluetooth.h"

#ifdef ENABLE_TRUE_WIRELESS_STEREO

uint8 last_tws_pair_status;
uint8 last_tws_dev_role;
uint8 g_tws_pos_switch_flag; //0表示未进入切换状态，1表示已播报左右箱体或切换过，等待7秒钟内按键切换
uint32 g_tws_pos_switch_time;

void tws_spk_pos_tts_state_play(bool master_is_left)
{
    if (com_get_input_msg_type() == INPUT_MSG_KEY_SLAVE)
    {
        if (master_is_left == TRUE)
        {
            com_tts_state_play_tws_slave(TTS_TWS_RIGHT_BOX, 0);
        }
        else
        {
            com_tts_state_play_tws_slave(TTS_TWS_LEFT_BOX, 0);
        }
    }
    else
    {
        if (master_is_left == TRUE)
        {
            com_tts_state_play(TTS_TWS_LEFT_BOX, 0);
        }
        else
        {
            com_tts_state_play(TTS_TWS_RIGHT_BOX, 0);
        }
    }
}

void tws_switch_tws_mode(tws_mode_e tws_mode)
{
    msg_apps_t msg;

    msg.type = MSG_BTPLAYEG_TWS_PAUSE_SYNC;
    send_sync_msg(APP_ID_BTPLAY_EG, &msg, NULL, 0);

    com_tws_m2s_switch_tws_mode_cmd(tws_mode);

    msg.type = MSG_BTPLAYEG_TWS_RESUME_SYNC;
    send_sync_msg(APP_ID_BTPLAY_EG, &msg, NULL, 0);
}

app_result_e tws_dev_spk_pos_switch(void)
{
#if (TWS_POS_MODE != TWS_POS_MODE_ALWAYS_MASTER_LEFT)
#if (SUPPORT_LEFT_RIGHT_SWITCH == 1)
    msg_apps_t msg;

    if (g_btplay_bt_info.tws_pair_status == TWS_PAIRED)
    {
        dev_spk_pos_e tmp_dev_spk_pos = get_tws_spk_pos();

        if (g_tws_pos_switch_flag == 0)
        {
            //仅播报左右箱体，然后等待6秒钟内按键切换
            if (tmp_dev_spk_pos == TWS_SPK_LEFT)
            {
                tws_spk_pos_tts_state_play(TRUE);
            }
            else if (tmp_dev_spk_pos == TWS_SPK_RIGHT)
            {
                tws_spk_pos_tts_state_play(FALSE);
            }

            g_tws_pos_switch_flag = 1;
            g_tws_pos_switch_time = sys_get_ab_timer();
        }
        else
        {
            //暂停TWS播放
            msg.type = MSG_BTPLAYEG_TWS_PAUSE_SYNC;
            send_sync_msg(APP_ID_BTPLAY_EG, &msg, NULL, 0);

            //切换并播报左右箱体，然后等待6秒钟内按键切换
            if (tmp_dev_spk_pos == TWS_SPK_LEFT)
            {
                change_tws_spk_pos(TWS_SPK_RIGHT);
                com_tws_m2s_switch_pos_cmd(TWS_SPK_LEFT);

                //切换左右箱体，然后播报，然后恢复播放
                tws_spk_pos_tts_state_play(FALSE);
            }
            else if (tmp_dev_spk_pos == TWS_SPK_RIGHT)
            {
                change_tws_spk_pos(TWS_SPK_LEFT);
                com_tws_m2s_switch_pos_cmd(TWS_SPK_RIGHT);

                //先停下播放，然后切换左右箱体，然后播报，然后恢复播放
                tws_spk_pos_tts_state_play(TRUE);
            }

            //恢复TWS播放
            msg.type = MSG_BTPLAYEG_TWS_RESUME_SYNC;
            send_sync_msg(APP_ID_BTPLAY_EG, &msg, NULL, 0);

            g_tws_pos_switch_time = sys_get_ab_timer();
        }
    }
#endif
#endif
    return RESULT_NULL;
}

void tws_tts_display_deal(void)
{
    if (g_btplay_bt_info.tws_pair_status != last_tws_pair_status)
    {
        if (g_btplay_bt_info.tws_pair_status == TWS_PAIRED)
        {
            if (g_btplay_bt_info.dev_role == TWS_MASTER)
            {
                com_tts_state_play(TTS_PAIR_SUCCEED, 0);
            }
        }
        else if (last_tws_pair_status == TWS_PAIRED)
        {
            com_tts_state_play(TTS_PAIR_DISCON, 0);
            if (last_tws_dev_role == TWS_SLAVE)
            {
                com_tts_state_play(TTS_WAIT_BT_CON, 0);
            }
            g_need_draw = TRUE;
        }

        last_tws_pair_status = g_btplay_bt_info.tws_pair_status;
        last_tws_dev_role = g_btplay_bt_info.dev_role;
    }

    if (g_tws_pos_switch_flag == 1)
    {
        if ((sys_get_ab_timer() - g_tws_pos_switch_time) >= 6000)
        {
            g_tws_pos_switch_flag = 0;
        }
    }

    //处理副箱TTS播报的请求
    if (get_tws_role() == TWS_MASTER)
    {
        com_tts_state_play_tws_slave(-1, 0); //-1表示从缓冲区读取tts_id播放
    }
}

#endif
