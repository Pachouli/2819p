/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：本地播歌中间件  中间件提供给解码库的文件操作接口
 ********************************************************************************/

#include "music_player_global.h"

int32 mmm_mp_read(void *buf, int32 size, int32 count, storage_io_t *io)
{
    int32 ret = 0, t1 = 0, t2 = 0;

    if ((size != 1) /*|| (count != 512)*/|| (buf == NULL))
    {
        return -1;
    }

    if (music_player_global->need_print_time == TRUE)
    {
        t1 = sys_get_ab_timer_us();
    }
#if ((SUPPORT_LOOP_PLAY == 1)&&(SUPPORT_VFS_LOOP_PLAY == 1))
    if ((music_player_global->music_loopplay_info.is_loop_play == TRUE)
            && (music_player_global->format_check_flag == 0)
            && (music_player_global->music_loopplay_info.read_first_frame_data == TRUE))
    {
        count = loopplay_find_musicfile_first_frame_count(music_player_global, count);
    }

    if (music_player_global->music_loopplay_info.is_loop_play == TRUE)
    {
        count = loopplay_is_the_last_full_play_end(music_player_global , count);
    }

    if ((music_player_global->music_loopplay_info.is_loop_play == TRUE)
            && (music_player_global->format_check_flag == 0))
    {
        count = loopplay_find_musicfile_last_frame_data(music_player_global, count);
    }
#endif
    ret = (int32) vfs_file_read(music_player_global->fs_para.file_mount_id, buf, count, (uint32 ) io->handel);
    if (music_player_global->need_print_time == TRUE)
    {
        t2 = sys_get_ab_timer_us();
        music_player_global->read_card_time += (t2 - t1);
    }

    //发生错误时看看是否物理读错误，如果是则强制退出解码
    if (ret == 0)
    {
        uint8 err_id = vfs_get_err_info(music_player_global->fs_para.file_mount_id);
        if (err_id != 0)
        {
            libc_printf_warning("fs err:%d\n", err_id);
            if (err_id == ERR_PHY_R)
            {
                music_player_global->storage_read_err = TRUE;
            }
        }
    }

    return ret;
}

int32 mmm_mp_seek(storage_io_t *io, int32 offset, int32 whence)
{
    int32 ret = 0;
    /* 0从文件首往文件尾定位,1  从当前位置往文件尾定位,2从当前位置往文件首定位,3从文件尾往文件首定位 */
    ret = vfs_file_seek(music_player_global->fs_para.file_mount_id, offset, whence, (uint32 ) io->handel);
    return ret;
}

int32 mmm_mp_tell(storage_io_t *io)
{
    int32 ret = 0;
    int32 offset = 0;
    ret = vfs_file_tell(music_player_global->fs_para.file_mount_id, &offset, (uint32 ) io->handel);
    return offset;
}

int32 mmm_mp_read_sd(void *buf, int32 size, int32 count, storage_io_t *io)
{
    int32 ret = 0;

#if ((SUPPORT_LOOP_PLAY == 1)&&(SUPPORT_SDLIB_LOOP_PLAY == 1))
    if ((music_player_global->music_loopplay_info.is_loop_play == TRUE)
            && (music_player_global->format_check_flag == 0)
            && (music_player_global->music_loopplay_info.read_first_frame_data == TRUE))
    {
        count = loopplay_find_musicfile_first_frame_count(music_player_global, count);
    }

    if (music_player_global->music_loopplay_info.is_loop_play == TRUE)
    {
        count = loopplay_is_the_last_full_play_end(music_player_global , count);
    }

    if ((music_player_global->music_loopplay_info.is_loop_play == TRUE)
            && (music_player_global->format_check_flag == 0))
    {
        count = loopplay_find_musicfile_last_frame_data(music_player_global, count);
    }
#endif

    if ((music_player_global->tts_mode == 1) && (music_player_global->format_check_flag == 0))
    {
        ret = tts_read_sd(io->handel, buf, count);
    }
    else
    {
        ret = (int32) sys_sd_fread(io->handel, buf, count);
    }
    return ret;
}

int32 mmm_mp_seek_sd(storage_io_t *io, int32 offset, int32 whence)
{
    int32 ret = 0;
    /* 0从文件首往文件尾定位,1  从当前位置往文件尾定位,2从当前位置往文件首定位,3从文件尾往文件首定位 */
    ret = sys_sd_fseek(io->handel, whence, offset);
    return ret;
}

int32 mmm_mp_tell_sd(storage_io_t *io)
{
    int32 ret = 0;
    ret = (int32) sys_sd_ftell(io->handel);
    return ret;
}

int32 mmm_mp_read_res(void *buf, int32 size, int32 count, storage_io_t *io)
{
    int32 ret = 0, t1 = 0, t2 = 0;
    if (music_player_global->need_print_time == TRUE)
    {
        t1 = sys_get_ab_timer_us();
    }

#if ((SUPPORT_LOOP_PLAY == 1)&&(SUPPORT_RES_DIR_LOOP_PLAY == 1))
    if ((music_player_global->music_loopplay_info.is_loop_play == TRUE)
            && (music_player_global->format_check_flag == 0)
            && (music_player_global->music_loopplay_info.read_first_frame_data == TRUE))
    {
        count = loopplay_find_musicfile_first_frame_count(music_player_global, count);
    }

    if (music_player_global->music_loopplay_info.is_loop_play == TRUE)
    {
        count = loopplay_is_the_last_full_play_end(music_player_global , count);
    }

    if ((music_player_global->music_loopplay_info.is_loop_play == TRUE)
            && (music_player_global->format_check_flag == 0))
    {
        count = loopplay_find_musicfile_last_frame_data(music_player_global, count);
    }
#endif

    ret = sys_res_fread(io->handel, buf, count);
    if (music_player_global->need_print_time == TRUE)
    {
        t2 = sys_get_ab_timer_us();
        music_player_global->read_card_time += (t2 - t1);
    }
    return ret;
}

int32 mmm_mp_seek_res(storage_io_t *io, int32 offset, int32 whence)
{
    int32 ret = 0;
    /* 0从文件首往文件尾定位,1  从当前位置往文件尾定位,2从当前位置往文件首定位,3从文件尾往文件首定位 */
    ret = sys_res_fseek(io->handel, whence, offset);
    return ret;
}

int32 mmm_mp_tell_res(storage_io_t *io)
{
    int32 ret = 0;
    ret = (int32) sys_res_ftell(io->handel);
    return ret;
}

int32 mmm_mp_read_extlib(void *buf, int32 size, int32 count, storage_io_t *io)
{
#if ((SUPPORT_LOOP_PLAY == 1) && (SUPPORT_EXTLIB_LOOP_PLAY == 1))
    if ((music_player_global->music_loopplay_info.is_loop_play == TRUE)
            && (music_player_global->format_check_flag == 0)
            && (music_player_global->music_loopplay_info.read_first_frame_data == TRUE))
    {
        count = loopplay_find_musicfile_first_frame_count(music_player_global, count);
    }

    if (music_player_global->music_loopplay_info.is_loop_play == TRUE)
    {
        count = loopplay_is_the_last_full_play_end(music_player_global , count);
    }

    if ((music_player_global->music_loopplay_info.is_loop_play == TRUE)
            && (music_player_global->format_check_flag == 0))
    {
        count = loopplay_find_musicfile_last_frame_data(music_player_global, count);
    }
#endif

#if(SUPPORT_EXTLIB_TTS_PLAY == 1) 
    int32 ret = 0;
    if ((music_player_global->tts_mode == 1) && (music_player_global->format_check_flag == 0))
    {
        ret = tts_read_ext(io->handel, buf, count);
    }
    else
    {
        ret = (int32) lib_fread(io->handel, buf, count);
    }
#else
    int32 ret = 0, t1 = 0, t2 = 0;
    if (music_player_global->need_print_time == TRUE)
    {
        t1 = sys_get_ab_timer_us();
    }
    ret = lib_fread(io->handel, buf, count);
    if (music_player_global->need_print_time == TRUE)
    {
        t2 = sys_get_ab_timer_us();
        music_player_global->read_card_time += (t2 - t1);
    }
#endif
    return ret;
}

int32 mmm_mp_seek_extlib(storage_io_t *io, int32 offset, int32 whence)
{
    int32 ret = 0;
    /* 0从文件首往文件尾定位,1  从当前位置往文件尾定位,2从当前位置往文件首定位,3从文件尾往文件首定位 */
    ret = lib_fseek(io->handel, whence, offset);
    return ret;
}

int32 mmm_mp_tell_extlib(storage_io_t *io)
{
    int32 ret = 0;
    ret = (int32) lib_ftell(io->handel);
    return ret;
}

