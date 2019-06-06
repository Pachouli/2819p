/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：TWS 副箱 TTS 播报处理。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

#ifdef ENABLE_TRUE_WIRELESS_STEREO

extern tts_play_infor_t g_tts_play_infor;
extern void *tts_play_handle;

static bool com_tts_state_sync(uint8 mode);

void com_tws_tts_play_slave(uint8 tts_id, uint16 mode)
{
    uint8 tts_list[1];
    uint8 section_cnt;
    uint8 dacclk_bk;
    bool tts_stop_flag;
    tws_sync_cmd_t tmp_tws_cmd;

    tts_list[0] = tts_id;
    section_cnt = 1;

    //请求播放TTS
    if (com_tts_state_sync(1) == FALSE)
    {
        return;
    }

    libc_printf_info("slave tts play:%d\n", tts_list[0]);

    g_tts_play_infor.tts_playing = 1;

    com_speaker_on_inner(SPK_SYS);

    dacclk_bk = get_dac_rate();

    //开始播放TTS
    tts_play_handle = com_sd_mp3_player_play_tts_inner(tts_list, section_cnt);
    if (tts_play_handle == NULL)
    {
        libc_printf_warning("tts play fail!\n");
        goto _tts_state_play_list_fail_ret;
    }

    //等待tts播报完毕
    while (1)
    {
        tts_stop_flag = com_sd_mp3_player_is_stop_inner(tts_play_handle);

        if (read_tws_cmd_from_buffer_rw_inner(g_p_tws_ui_m2s_pipe, &tmp_tws_cmd, sizeof(tws_sync_cmd_t)) == TRUE)
        {
            if (tmp_tws_cmd.cmd_id == TWS_SYNC_M2S_UI_TTS_STA_REQ)
            {
                tws_slave_tts_state_e tts_sta;

                if (tts_stop_flag == TRUE)
                {
                    tts_sta = TWS_SLAVE_STA_STOP;
                }
                else
                {
                    tts_sta = TWS_SLAVE_STA_PLAYING;
                }
                com_tws_s2m_cmd_sync_reply(TWS_SYNC_REPLY_ACK, &tts_sta, 1);
            }
        }

        if (tts_stop_flag == TRUE)
        {
            break;
        }

        sys_os_time_dly(1);
    }

    com_sd_mp3_player_stop_inner(tts_play_handle);
    tts_play_handle = NULL;

    _tts_state_play_list_fail_ret:

    g_tts_play_infor.tts_playing = 0;

    set_dac_rate(dacclk_bk);

    com_speaker_off_inner(SPK_SYS);

    //TTS播报完成
    com_tts_state_sync(0);

    libc_printf_info("slave tts play over\n");
}

//mode为1表示开始播报，返回TRUE表示可以播报，返回FALSE表示不能播报；
//mode为0表示播报完成，返回值无意义
static bool com_tts_state_sync(uint8 mode)
{
    msg_apps_t msg;
    msg_reply_t reply;
    bool bret = TRUE;

    switch (mode)
    {
        //普通TTS播报完成
        case 0:
        msg.type = MSG_TTS_STOP_SYNC;
        break;

        //普通TTS播报开始
        case 1:
        msg.type = MSG_TTS_START_SYNC;
        break;

        default:
        break;
    }

    //发送同步消息
    if (get_engine_type_inner() != ENGINE_NULL)
    {
        bret = send_sync_msg_inner(APP_ID_THEENGINE, &msg, &reply, 0);
        if ((mode == 1) && (reply.type != MSG_REPLY_SUCCESS))
        {
            bret = FALSE;
        }
    }

    return bret;
}

#endif
