/********************************************************************************
 * 	   Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 * 						   All Rights Reserved.
 *
 * 描述：本地播歌中间件  播放文件设置
 ********************************************************************************/

#include "music_player_global.h"

//设置播放文件
int32 mmm_mp_init_file(music_player_t *music_player, uint32 args)
{
    int32 ret = 0;

    /*最近一曲的播放信息在setfile命令前可以保留*/
    //libc_memset(&music_player->music_status, 0, sizeof(mmm_mp_status_t));
    //libc_memset(&music_player->aout, 0, sizeof(audiout_pcm_t));
    //libc_memset(&music_player->music_decode_info, 0, sizeof(music_decode_info_t));
    //music_player->music_decode_info.aout = &music_player->aout;
    libc_memcpy(&music_player->fs_para, (void * ) args, sizeof(mmm_mp_fs_para_t));

    music_player->storage_io.handel = NULL;

    music_player->storage_io.file_type = music_player->fs_para.fs_type;

    if (music_player->fs_para.fs_type == FS_TYPE_VFS)
    {
        music_player->storage_io.handel = (void *) vfs_file_open(music_player->fs_para.file_mount_id,
                music_player->fs_para.file_name, R_NORMAL_SEEK);
        music_player->storage_io.read = mmm_mp_read;
        music_player->storage_io.seek = mmm_mp_seek;
        music_player->storage_io.tell = mmm_mp_tell;
    }
    else if (music_player->fs_para.fs_type == FS_TYPE_SDFILE)
    {
        music_player->storage_io.handel = (void *) sys_sd_fopen(music_player->fs_para.file_name);
        music_player->storage_io.read = mmm_mp_read_sd;
        music_player->storage_io.seek = mmm_mp_seek_sd;
        music_player->storage_io.tell = mmm_mp_tell_sd;
    }
    else if (music_player->fs_para.fs_type == FS_TYPE_RESFILE)
    {
        music_player->storage_io.handel = (void *) sys_res_fopen(music_player->fs_para.file_name);
        music_player->storage_io.read = mmm_mp_read_res;
        music_player->storage_io.seek = mmm_mp_seek_res;
        music_player->storage_io.tell = mmm_mp_tell_res;
    }
    else if (music_player->fs_para.fs_type == FS_TYPE_EXTLIB)
    {
        music_player->storage_io.handel = (void *) lib_fopen(music_player->fs_para.file_name);
        music_player->storage_io.read = mmm_mp_read_extlib;
        music_player->storage_io.seek = mmm_mp_seek_extlib;
        music_player->storage_io.tell = mmm_mp_tell_extlib;
    }
    else
    {
        libc_printf_warning("fs_type invalid!\n");
    }

#if (SUPPORT_LOOP_PLAY == 1)
    if (music_player->music_loopplay_info.is_loop_play == TRUE)
    {
        music_player->music_loopplay_info.music_file_has_endtag = is_music_file_has_idv3v1(music_player,
                FORMAT_CHECK_TEMP_BUF_ADDR);
    }
#endif

    if (music_player->storage_io.handel == NULL)
    {
        music_player->music_status.err_no = MMM_MP_ERR_OPEN_FILE;
        ret = -1;
        goto player_init_file_0;
    }

    if (music_player->fs_para.is_tts == 1)
    {
        mmm_mp_tts_para_t *p_tts_para = music_player->fs_para.p_tts_para;

        if (tts_play_init(music_player, p_tts_para->sec_ids, p_tts_para->sec_cnt) == FALSE)
        {
            music_player->music_status.err_no = MMM_MP_ERR_FILE_NOT_SUPPORT;
            ret = -1;
            goto player_init_file_1;
        }
        music_player->tts_mode = 1;
    }

    /*检测实际编码格式并且获取歌曲信息，节省非播放状态下切换歌曲的时间 */
    music_player->format_check_flag = 1;
    ret = format_check(&music_player->music_info);
    music_player->format_check_flag = 0;
    if (0 != ret)
    {
        libc_printf_warning("format_check fail!\n");
        music_player->music_status.err_no = MMM_MP_ERR_FILE_NOT_SUPPORT;
        ret = -1;
        goto player_init_file_1;
    }

    if ((music_player->music_info.total_time < 0) || (music_player->music_info.channels > 2)
            || (music_player->music_info.sample_rate <= 0))
    {
        libc_printf_warning("music_info error!\n");
        music_player->music_status.err_no = MMM_MP_ERR_FILE_NOT_SUPPORT;
        ret = -1;
        goto player_init_file_1;
    }

    if (music_player->music_info.sample_rate > 48)
    {
        libc_printf_warning("unexpect sample rate! %d\n", music_player->music_info.sample_rate);
        music_player->music_status.err_no = MMM_MP_ERR_FILE_NOT_SUPPORT;
        ret = -1;
        goto player_init_file_1;
    }

    //打开文件后状态切换为 FILESET
    music_player->music_status.status = MMM_MP_ENGINE_FILESET;

    return ret;

    player_init_file_1:
    if (music_player->fs_para.fs_type == FS_TYPE_VFS)
    {
        vfs_file_close(music_player->fs_para.file_mount_id, (uint32 ) music_player->storage_io.handel);
    }
    else if (music_player->fs_para.fs_type == FS_TYPE_SDFILE)
    {
        sys_sd_fclose(music_player->storage_io.handel);
    }
    else if (music_player->fs_para.fs_type == FS_TYPE_RESFILE)
    {
        sys_res_fclose(music_player->storage_io.handel);
    }
    else if (music_player->fs_para.fs_type == FS_TYPE_EXTLIB)
    {
        lib_fclose(music_player->storage_io.handel);
    }
    music_player->storage_io.handel = NULL;

    player_init_file_0:

    return ret;
}

/*关闭文件*/
int32 mmm_mp_clear_file(music_player_t *music_player)
{
    int32 ret = 0;

    /*最近一曲的播放信息在setfile命令前可以保留*/
    //libc_memset(&music_player->music_status, 0, sizeof(mmm_mp_status_t));
    //libc_memset(&music_player->aout, 0, sizeof(audiout_pcm_t));
    //libc_memset(&music_player->music_decode_info, 0, sizeof(music_decode_info_t));
    if (music_player->tts_mode == 1)
    {
        tts_play_exit(music_player);

        music_player->tts_mode = 0;
    }

    if (music_player->storage_io.handel != NULL)
    {
        if (music_player->fs_para.fs_type == FS_TYPE_VFS)
        {
            vfs_file_close(music_player->fs_para.file_mount_id, (uint32 ) music_player->storage_io.handel);
        }
        else if (music_player->fs_para.fs_type == FS_TYPE_SDFILE)
        {
            sys_sd_fclose(music_player->storage_io.handel);
        }
        else if (music_player->fs_para.fs_type == FS_TYPE_RESFILE)
        {
            sys_res_fclose(music_player->storage_io.handel);
        }
        else if (music_player->fs_para.fs_type == FS_TYPE_EXTLIB)
        {
            lib_fclose(music_player->storage_io.handel);
        }
        else
        {
            //fs_type 2 means ain src , do nothing
        }

        music_player->storage_io.handel = NULL;
    }

    //关闭文件后状态切换为 STOPPED
    music_player->music_status.status = MMM_MP_ENGINE_STOPPED;

    return ret;
}

