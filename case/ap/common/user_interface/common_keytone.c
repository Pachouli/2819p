/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：按键音播放功能，支持非阻塞按键音，支持FIFO，支持动态开关按键音。
 *       软按键音不支持非阻塞模式。
 * 作者：cailizhen
 ********************************************************************************/
#define USE_KEYTONE_FILE

#include "common_ui_inner.h"

extern bool com_sd_mp3_player_start_soft_kt(uint8 *kt_file);
extern bool com_sd_mp3_player_get_status(void);

static keytone_infor_t g_keytone_infor;

uint8 keytone_fifo_out(void);
bool keytone_fifo_in(uint8 kt_mode);

static void audio_kt_start(uint8 kt_mode)
{
    kt_setting_t kt_sett;
    int32 vol_exp_result;
    uint8 kt_vol;

    libc_printf_info("keytone play\n");

    if (g_keytone_infor.volmode == 1)
    {
        kt_vol = g_keytone_infor.max_volume;
    }
    else
    {
        kt_vol = com_get_sound_volume_inner();
        if (com_get_sound_volume_mode() == VOLUME_MODE_CALL)
        {
            kt_vol = kt_vol * VOLUME_VALUE_MAX / VOLUME_VALUE_CALL_MAX;
        }
        if (kt_vol > g_tts_play_infor.max_volume)
        {
            kt_vol = g_tts_play_infor.max_volume;
        }

        vol_exp_result = (int32) g_aud_vol_p1_db_table[kt_vol] - (int32) g_aud_vol_p1_db_table[g_tts_play_infor.max_volume];
        vol_exp_result = libc_math_exp_fixed(vol_exp_result);
        kt_vol = (uint8)((int32) g_keytone_infor.max_volume * vol_exp_result / 32768);

        if (kt_vol < g_keytone_infor.min_volume)
        {
            kt_vol = g_keytone_infor.min_volume;
        }
    }

    kt_sett.volume = kt_vol;
    kt_sett.freq = (kt_mode & KT_FREQ_OPT) >> KT_FREQ_SHIFT;
    kt_sett.period = (kt_mode & KT_PERIOD_OPT) >> KT_PERIOD_SHIFT;
    start_key_tone(&kt_sett);
}

#if (SUPPORT_SOFT_KEYTONE == 1)
static void mmm_kt_start(uint8 kt_mode)
{
    kt_id_e kt_id = (kt_id_e)(kt_mode&0xf);
    bool kt_block_flag = ((kt_mode&KT_BLOCK_OPT) != 0);

    if (kt_id < KT_ID_MAX)
    {
        mmm_state_e cur_mmm_state;
        bool need_kt_playing = FALSE;

        sys_os_sched_lock();

        //防止播放到一半被engine插入启动其他中间件
        cur_mmm_state = get_mmm_state_inner();
        if (cur_mmm_state == MMM_STATE_NO_PLAY)
        {
            change_mmm_state_inner(MMM_STATE_KT_PLAYING);
            need_kt_playing = TRUE;
        }

        sys_os_sched_unlock();

        if (need_kt_playing == TRUE)
        {
            if (com_keytone_play_inner(g_keytone_files[kt_id]) == FALSE)
            {
                libc_printf_warning("mmm_kt_start fail!\n");
                change_mmm_state_inner(MMM_STATE_NO_PLAY);
            }
        }
        else
        {
            if (get_mmm_state_inner() == MMM_STATE_TTS_PLAYING)
            {
                if (com_sd_mp3_player_start_soft_kt(g_keytone_files[kt_id]) == FALSE)
                {
                    libc_printf_warning("mmm_kt_start fail!\n");
                }
            }
            else if (get_mmm_state_inner() == MMM_STATE_PLAYING)
            {
                msg_apps_t msg;
                msg_reply_t temp_reply;
                msg.type = MSG_SOFT_KEYTONE_PLAY_SYNC;
                msg.content.addr = g_keytone_files[kt_id];
                //发送同步消息
                if (get_engine_type_inner() != ENGINE_NULL)
                {
                    if ((send_sync_msg_inner(APP_ID_THEENGINE, &msg, &temp_reply, 0) == FALSE)
                        || (temp_reply.type != MSG_REPLY_SUCCESS))
                    {
                        libc_printf_warning("mmm_kt_start fail!\n");
                    }
                }
            }
        }
    }
}
#endif

static void _kt_start(uint8 kt_mode)
{
    if (g_keytone_infor.kt_type == KT_TYPE_HARD)
    {
        audio_kt_start(kt_mode);
    }
#if (SUPPORT_SOFT_KEYTONE == 1)
    else
    {
        mmm_kt_start(kt_mode);
    }
#endif
}

static bool _is_kt_playing(void)
{
    bool ret = FALSE;

    if (g_keytone_infor.kt_type == KT_TYPE_HARD)
    {
        ret = get_key_tone_status();
    }
#if (SUPPORT_SOFT_KEYTONE == 1)
    else
    {
        if (get_mmm_state_inner() == MMM_STATE_KT_PLAYING)
        {
            com_keytone_check_status();
            if (get_ktp_status_inner() != MMM_KTP_ENGINE_PLAYING)
            {
                com_keytone_stop_inner();
                return ret;
            }

            ret = TRUE;
        }
        else if (get_mmm_state_inner() == MMM_STATE_TTS_PLAYING)
        {
            //获取当前播放状态
            if (com_sd_mp3_player_get_status() == FALSE)
            {
                return ret;
            }

            if (get_mmm_state_inner() == MMM_STATE_NO_PLAY)
            {
                change_ktp_status_inner(MMM_KTP_ENGINE_NULL);
                return ret;
            }

            if (get_ktp_status_inner() != MMM_KTP_ENGINE_PLAYING)
            {
                return ret;
            }

            ret = TRUE;
        }
        else if (get_mmm_state_inner() == MMM_STATE_PLAYING)
        {
            if (get_mmm_state_inner() == MMM_STATE_NO_PLAY)
            {
                change_ktp_status_inner(MMM_KTP_ENGINE_NULL);
                return ret;
            }

            if (get_ktp_status_inner() != MMM_KTP_ENGINE_PLAYING)
            {
                return ret;
            }

            ret = TRUE;
        }
    }
#endif

    return ret;
}

void keytone_var_init(void)
{
#if (SUPPORT_SOFT_KEYTONE == 1)
    g_keytone_infor.kt_type = KT_TYPE_SOFT;
#else
    g_keytone_infor.kt_type = KT_TYPE_HARD;
#endif
    g_keytone_infor.volmode = (uint8) com_get_config_default_inner(SETTING_SOUND_KEYTONE_VOLMODE);
    if (com_get_config_default_inner(SETTING_SOUND_TTS_VOLMODE) == 1)
    {
        g_keytone_infor.volmode = 1;
    }
    g_keytone_infor.min_volume = (uint8) com_get_config_default_inner(SETTING_SOUND_KEYTONE_MINVOL);
    g_keytone_infor.max_volume = (uint8) com_get_config_default_inner(SETTING_SOUND_KEYTONE_MAXVOL);
    g_keytone_infor.enable = TRUE;
    g_keytone_infor.playing = FALSE;
    g_keytone_infor.count = 0;
}

//按键音开关有2个：全局开关和场景开关，这里就是场景开关
void keytone_set_on_off_inner(bool on_off)
{
    if (on_off == g_keytone_infor.enable)
    {
        return;
    }

    //等待按键音结束后才能切换关掉
    while (_is_kt_playing() == TRUE)
    {
        sys_os_time_dly(1);
    }

    g_keytone_infor.enable = on_off;
}

void keytone_play_deal(void)
{
    uint8 kt_mode;

    if (g_keytone_infor.count == 0)
    {
        return;
    }

    if (_is_kt_playing() == TRUE)
    {
        return;
    }

    kt_mode = keytone_fifo_out();
    if (kt_mode == 0xff)
    {
        libc_printf_warning("kt fifo out err!!\n");
        return;
    }

    if (g_keytone_infor.enable == TRUE)
    {
        _kt_start(kt_mode);
    }
}

void keytone_play_deal_wait(void)
{
    uint32 kt_play_overtime = sys_get_ab_timer();

    //等待按键音结束
    while (_is_kt_playing() == TRUE)
    {
        sys_os_time_dly(1);

        if ((sys_get_ab_timer() - kt_play_overtime) > 2000)
        {
            libc_printf_error("keytone play overtime!\n");
            break;
        }
    }
}

bool keytone_play_inner(uint8 kt_mode)
{
    if (g_keytone_infor.enable == FALSE)
    { //已禁用按键音
        return FALSE;
    }

    //外部PA使能静音时不支持非阻塞模式按键音
    if ((com_get_config_default_inner(SETTING_HARDWARE_EXTERN_PA_MUTE_ENABLE) == TRUE)
        || (g_keytone_infor.kt_type == KT_TYPE_SOFT))
    {
        kt_mode = kt_mode | KT_BLOCK_YES;
    }

    if ((kt_mode & KT_BLOCK_OPT) == KT_BLOCK_NO)
    {
        keytone_fifo_in(kt_mode);

        keytone_play_deal();
    }
    else
    {
        keytone_play_deal_wait();

        keytone_fifo_in(kt_mode);

        while (g_keytone_infor.count > 0)
        {
            kt_mode = keytone_fifo_out();
            if (kt_mode == 0xff)
            {
                break;
            }

            if (g_keytone_infor.enable == TRUE)
            {
                com_speaker_on_inner(SPK_SYS);

                _kt_start(kt_mode);

                keytone_play_deal_wait();

                com_speaker_off_inner(SPK_SYS);
            }
        }
    }

    return TRUE;
}

uint8 keytone_fifo_out(void)
{
    uint8 kt_mode, i;

    if (g_keytone_infor.count == 0)
    {
        return 0xff;
    }

    kt_mode = g_keytone_infor.kt_fifo[0];
    g_keytone_infor.count--;

    for (i = 0; i < g_keytone_infor.count; i++)
    {
        g_keytone_infor.kt_fifo[i] = g_keytone_infor.kt_fifo[i + 1];
    }

    return kt_mode;
}

bool keytone_fifo_in(uint8 kt_mode)
{
    if (g_keytone_infor.count >= KT_FIFO_DEPTH)
    {
        libc_printf_warning("kt fifo in full!\n");
        return FALSE;
    }

    g_keytone_infor.kt_fifo[g_keytone_infor.count] = kt_mode;
    g_keytone_infor.count++;

    return TRUE;
}

