/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙推歌前台应用TTS播报处理。
 * 作者：cailizhen
 ********************************************************************************/

#include  "ap_bluetooth.h"

void enter_bt_tts_play(tts_play_background_hook hook)
{
    //播报“进入蓝牙”
    if (g_esd_reset_ignore_enter_tts_flag == FALSE)
    {
        //语音播报进入蓝牙
        g_tts_enter_ret = com_tts_state_play_ext(TTS_ENTER_BT, 0, hook);
    }
    g_esd_reset_ignore_enter_tts_flag = FALSE;
}

void bt_tts_play(void)
{
    switch (g_disp_status)
    {
        case STATUS_WAIT_PAIRED:
        if (g_esd_reset_ignore_wait_tts_flag != FALSE)
        {
            g_esd_reset_ignore_wait_tts_flag = FALSE;
            break;
        }
        if (last_disp_status == STATUS_NONE)
        {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
            //已组队后，副箱无需播报等待连接
            if (g_btplay_bt_info.dev_role == TWS_SLAVE)
            {
                break;
            }
#endif

            //语音播报等待连接
            if ((g_tts_enter_ret == TTS_PLAY_RET_BY_KEY) || (g_tts_enter_ret == TTS_PLAY_RET_BY_SYS))
            {
                //如果播报“进入蓝牙时”被终止了，那么等待连接就不再播报了
                break;
            }
            com_tts_state_play(TTS_WAIT_BT_CON, 0);
        }
        else
        {
            //语音播报蓝牙断开&等待连接
            tts_play_ret_e temp_tts_ret;
            temp_tts_ret = com_tts_state_play(TTS_BT_DISCON, 0);
            if ((temp_tts_ret == TTS_PLAY_RET_BY_KEY) || (temp_tts_ret == TTS_PLAY_RET_BY_SYS))
            {
                //如果播报“蓝牙断开”被终止了，那么等待连接就不再播报了
                break;
            }
            com_tts_state_play(TTS_WAIT_BT_CON, 0);
        }
        break;

        /* for samsung S8500, link when play, maybe pause-status.  */
        case STATUS_LINKED:
        case STATUS_A2DP_PLAY:
        case STATUS_A2DP_PAUSE:
        if (g_esd_reset_ignore_link_tts_flag != FALSE)
        {
            g_esd_reset_ignore_link_tts_flag = FALSE;
            break;
        }
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

