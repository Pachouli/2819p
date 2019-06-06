/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：
 ********************************************************************************/
#include "ap_usound.h"

const key_map_t usound_key_map_list[] =
{
    { { KEY_NEXT, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_NEXTSONG },
    /*! 短按按prev切换上一曲事件 */
    { { KEY_PREV, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_PREVSONG },
    /*!短按play播放/暂停事件 */
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_PAUSE },

    /*! 结束标志 */
    { { KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};


static void key_vol_sub_deal(void)
{
    usound_volume_sub();
}

static void key_vol_add_deal(void)
{
    usound_volume_add();
}

static void key_next_deal(void)
{
    usound_play_next();
    com_tts_state_play(TTS_NEXT_SONG, TTS_MODE_CONTROL_TTS_YES);
}

static void key_prev_deal(void)
{
    usound_play_prev();
    com_tts_state_play(TTS_LAST_SONG, TTS_MODE_CONTROL_TTS_YES);
}

static void key_play_pause_deal(void)
{
    if ((g_cur_play_status.play_status == PauseSta) || (g_cur_play_status.play_status == StopSta))
    {
        com_tts_state_play(TTS_PLAY_SONG, TTS_MODE_CONTROL_TTS_YES);
        if (mannul_set_play() == FALSE)
        {
            libc_printf_warning("mannul_set_play fail!\n");
        }
    }
    else
    {
        if (mannul_set_pause() == FALSE)
        {
            libc_printf_warning("mannul_set_pause fail!\n");
        }
        com_tts_state_play(TTS_PAUSE_SONG, TTS_MODE_CONTROL_TTS_YES);
    }
}

app_result_e usound_msg_deal(input_gui_msg_t* ptr_gui_msg)
{
    app_result_e result = RESULT_NULL;
    input_gui_msg_t* gui_msg = ptr_gui_msg;
    msg_apps_type_e cur_event;

    if (com_key_mapping(gui_msg, &cur_event, usound_key_map_list) == TRUE)
    {
        switch (cur_event)
        {
            case EVENT_PLAYING_NEXTSONG:
            key_next_deal();
            break;
            case EVENT_PLAYING_PREVSONG:
            key_prev_deal();
            break;
            case EVENT_PLAYING_PAUSE:
            key_play_pause_deal();
            break;

            /*! 按下 VOL+ /VOL- /NEXT LONG /PREV LONG进入音量调节界面事件 */
            case EVENT_VOLUME_ADD:
            if (g_volume_sync == USOUND_VOL_SYNC_ONLY_DEVICE)
            {
                com_volume_add(SET_VOL_BY_KEYSHORT);
            }
            if (g_volume_sync == USOUND_VOL_SYNC_PC_FIRST)
            {
                key_vol_add_deal();
            }
            break;

            case EVENT_VOLUME_ADD_HOLD:
            case EVENT_VOLUME_ADD_NEXT:
            if (g_volume_sync == USOUND_VOL_SYNC_ONLY_DEVICE)
            {
                com_volume_add(SET_VOL_BY_KEYHOLD);
            }
            if (g_volume_sync == USOUND_VOL_SYNC_PC_FIRST)
            {
                key_vol_add_deal();
            }
            com_filter_key_up();
            break;

            case EVENT_VOLUME_SUB:
            if (g_volume_sync == USOUND_VOL_SYNC_ONLY_DEVICE)
            {
                com_volume_sub(SET_VOL_BY_KEYSHORT);
            }
            if (g_volume_sync == USOUND_VOL_SYNC_PC_FIRST)
            {
                key_vol_sub_deal();
            }
            break;

            case EVENT_VOLUME_SUB_HOLD:
            case EVENT_VOLUME_SUB_PREV:
            if (g_volume_sync == USOUND_VOL_SYNC_ONLY_DEVICE)
            {
                com_volume_sub(SET_VOL_BY_KEYHOLD);
            }
            if (g_volume_sync == USOUND_VOL_SYNC_PC_FIRST)
            {
                key_vol_sub_deal();
            }
            com_filter_key_up();
            break;

            default:
            result = com_message_box(cur_event);
            break;
        }
    }

    return result;
}

