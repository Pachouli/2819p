/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：本地播歌中间件  中间提供给应用层的接口实现
 ********************************************************************************/

#include "music_player_global.h"

music_player_t* music_player_global;

freq_point_config_t g_freq_point_cfg;

uint8 __section__(".bss.ktd_input_buffer") KTD_INPUT_BUFFER_ADDR[KTD_INPUT_BUFFER_LEN];
uint8 __section__(".bss.dae_frame_align_buf") DAE_128_ALIGN_BUF_ADDR[DAE_128_ALIGN_BUF_LEN];

uint8 __section__(".bss.FORMAT_CHECK_TEMP_BUF") FORMAT_CHECK_TEMP_BUF_ADDR[FORMAT_CHECK_TEMP_BUF_LEN];

const uint32 __section__(".rodata.entry") mmm_mp_cmd[] =
{
    (uint32) audio_mmm_mp_cmd,
};


/*! \cond MMM_MP_API */
int32 audio_mmm_mp_cmd(void *handel, mmm_mp_cmd_t cmd, uint32 args)
{
    music_player_t *music_player = NULL;
    int32 ret = 0;

    if (cmd != MMM_MP_OPEN)
    {
        music_player = (music_player_t *) handel;
    }

    if ((music_player_global->music_status.status == MMM_MP_ENGINE_NULL) && (cmd != MMM_MP_OPEN))
    {
        ret = -1;
        return ret;
    }

    sys_os_sched_lock();

    switch (cmd)
    {
        case MMM_MP_GET_STATUS:
        music_player->music_status.cur_time = music_player->music_decode_info.cur_time;
        music_player->music_status.cur_bitrate = music_player->music_decode_info.cur_bitrate;
        music_player->music_status.cur_energy = music_player->music_decode_info.cur_energy;
        libc_memcpy((void *) args, &music_player->music_status, sizeof(mmm_mp_status_t));
        break;

        case MMM_MP_GET_ERRORNO:
        *((mmm_mp_err_t *) args) = music_player->music_status.err_no;
        break;

        case MMM_MP_OPEN:
        music_player = mmm_mp_open(args);
        if (music_player == NULL)
        {
            *((music_player_t **) handel) = NULL;
            ret = -1;
            break;
        }
        *((music_player_t **) handel) = music_player;
        break;

        case MMM_MP_CLOSE:
        //非法状态
        if (music_player->music_status.status != MMM_MP_ENGINE_STOPPED)
        {
            ret = -1;
            break;
        }

        ret = mmm_mp_close(handel, NULL);
        break;

#if (SUPPORT_LOOP_PLAY == 1)
		case MMM_MP_SET_LOOP_PLAY:
        //非法状态
        if (music_player->music_status.status != MMM_MP_ENGINE_STOPPED)
        {
            ret = -1;
            break;
        }
		music_player->music_loopplay_info.is_loop_play = (bool)(uint32)args;
		break;
#endif

        case MMM_MP_SET_FILE:
        //非法状态
        if (music_player->music_status.status != MMM_MP_ENGINE_STOPPED)
        {
            ret = -1;
            break;
        }

        ret = mmm_mp_init_file(music_player, args);
        break;

        case MMM_MP_MEDIA_INFO:
        ret = get_media_info(music_player, args);
        break;

        case MMM_MP_CLEAR_FILE:
        //非法状态
        if (music_player->music_status.status != MMM_MP_ENGINE_FILESET)
        {
            ret = -1;
            break;
        }

        ret = mmm_mp_clear_file(music_player);
        break;

        case MMM_MP_PLAY:
        //非法状态
        if (music_player->music_status.status != MMM_MP_ENGINE_FILESET)
        {
            ret = -1;
            break;
        }

        ret = start_music_decoder(music_player);
        break;

        case MMM_MP_STOP:
        //非法状态
        if (music_player->music_status.status == MMM_MP_ENGINE_STOPPED)
        {
            ret = -1;
            break;
        }

        //如果当前处于淡入/淡出状态，必须先等淡入/淡出结束
        while ((music_player->music_status.status == MMM_MP_ENGINE_FADEIN)
            || (music_player->music_status.status == MMM_MP_ENGINE_FADEOUT))
        {
#ifdef MUSIC_MMM_DEBUG_INFO
            libc_printf("STOP CMD wait fade over\n");
#endif
            sys_os_sched_unlock();
            sys_os_time_dly(1);
            sys_os_sched_lock();
        }

        ret = stop_music_decoder(music_player);
        break;

        case MMM_MP_SET_MUSIC_SETTING:
        //非法状态
        if ((music_player->music_status.status != MMM_MP_ENGINE_FILESET)
            && (music_player->music_status.status != MMM_MP_ENGINE_STOPPED))
        {
            ret = -1;
            break;
        }

        libc_memcpy(&(music_player->music_setting), (music_setting_t *) args, sizeof(music_setting_t));
        break;

        case MMM_MP_GET_BREAK_POINT:
        //非法状态
        if (music_player->music_status.status == MMM_MP_ENGINE_STOPPED)
        {
            ret = -1;
            break;
        }

        libc_memcpy((void *) args, &(music_player->music_decode_info.music_bp_info), sizeof(mmm_mp_bp_info_t));
        break;

        case MMM_MP_SET_BREAK_POINT:
        //非法状态
        if (music_player->music_status.status != MMM_MP_ENGINE_FILESET)
        {
            ret = -1;
            break;
        }

        libc_memcpy(&(music_player->music_decode_info.music_bp_info), (void *) args, sizeof(mmm_mp_bp_info_t));
        music_player->music_decode_info.cur_time = music_player->music_decode_info.music_bp_info.bp_time_offset;
        break;

        case MMM_MP_SET_FFB:
        update_ffrev_setting(music_player, args);
        break;
        case MMM_MP_FFB_STEP:
        music_player->music_setting.ffb_step = (int32) args;
        break;
        case MMM_MP_FFB_PLAY_TIME:
        music_player->music_setting.ffb_play_time = (int32) args;
        break;
        case MMM_MP_FFB_WAIT_TIME:
        music_player->music_setting.ffb_delay = (int32) args;
        break;

        case MMM_MP_SET_EQ:
        music_player->dae_update_flag = 1;
        update_PP_para_info(music_player);
        set_music_decoder_clk(music_player);
        break;

        case MMM_MP_SET_FREQ_POINT:
        libc_memcpy(&g_freq_point_cfg, (freq_point_config_t *) args, sizeof(freq_point_config_t));
        music_player->dae_update_flag = 1;
        update_PP_para_info(music_player);
        set_music_decoder_clk(music_player);
        break;

#if (SUPPORT_SOFT_KEYTONE == 1)
        case MMM_MP_KEYTONE_PLAY:
        //非法状态
        if ((music_player->music_status.status != MMM_MP_ENGINE_FADEIN)
            && (music_player->music_status.status != MMM_MP_ENGINE_PLAYING))
        {
            ret = -1;
            break;
        }
        //如果当前还有软按键音在播放，就忽略本次软按键音播放请求
        if (music_player->music_status.ktp_status == MMM_KTP_ENGINE_PLAYING)
        {
            libc_printf_warning("keytone is playing, ignore!\n");
            ret = -1;
            break;
        }

        ret = keytone_start(music_player, args);
        break;
#endif

        default:
        libc_printf_warning("MMM_MP cmd not support:%d\n", cmd);
        ret = -1;
        music_player->music_status.err_no = MMM_MP_ERR_CMD_NOTEXIST;
        break;
    }

    sys_os_sched_unlock();

    return ret;
}

int32 audio_rcode_init(void)
{
    music_player_global = sys_malloc(sizeof(music_player_t));
    if (music_player_global == NULL)
    {
        libc_printf("mmm_player\n");
        return -1;
    }
    else
    {
        libc_memset(music_player_global, 0, sizeof(music_player_t));
        music_player_global->music_status.status = MMM_MP_ENGINE_NULL;
        return 0;
    }
}

int32 audio_rcode_exit(void)
{
    sys_free(music_player_global);
    music_player_global = NULL;
    return 0;
}

#ifndef PC
module_init (audio_rcode_init)
module_exit(audio_rcode_exit)
#endif

/*! \endcond */

