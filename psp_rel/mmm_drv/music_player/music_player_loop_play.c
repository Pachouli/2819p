/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：本地无缝播放中间件，文件接口实现
 ********************************************************************************/

#include "music_player_global.h"

#if (SUPPORT_LOOP_PLAY == 1)

#define MP3_FILE_IDV3V1_LEN         128
#define FILTER_FRAME_TIME           200 //单位ms
#define MUTE_FRAME_ENERGY_THRESHOLD 100

bool is_music_file_has_idv3v1(music_player_t *music_player, void *buf)
{
    int32 offset;
    int32 ret = 0;
    int32 offset_backup;

#if (SUPPORT_RES_DIR_LOOP_PLAY == 1)
    offset = sys_res_ftell((uint32 ) music_player->storage_io.handel);
    offset_backup = offset;
    sys_res_fseek((uint32 ) music_player->storage_io.handel,3,-MP3_FILE_IDV3V1_LEN);
    ret = (int32) sys_res_fread((uint32 ) music_player->storage_io.handel, buf, MP3_FILE_IDV3V1_LEN);
    sys_res_fseek((uint32 ) music_player->storage_io.handel,0, offset_backup);

#elif (SUPPORT_EXTLIB_LOOP_PLAY == 1)
    offset = lib_ftell((lib_file_t* ) music_player->storage_io.handel);
    offset_backup = offset;
    lib_fseek((lib_file_t* ) music_player->storage_io.handel,3,-MP3_FILE_IDV3V1_LEN);
    ret = (int32) lib_fread((lib_file_t* ) music_player->storage_io.handel, buf, MP3_FILE_IDV3V1_LEN);
    lib_fseek((lib_file_t* ) music_player->storage_io.handel,0, offset_backup);

#elif (SUPPORT_SDLIB_LOOP_PLAY == 1)
    offset = sys_sd_ftell((uint32 ) music_player->storage_io.handel);
    offset_backup = offset;
    sys_sd_fseek((uint32 ) music_player->storage_io.handel,3,-MP3_FILE_IDV3V1_LEN);
    ret = (int32) sys_sd_fread((uint32 ) music_player->storage_io.handel, buf, MP3_FILE_IDV3V1_LEN);
    sys_sd_fseek((lib_file_t* ) music_player->storage_io.handel,0, offset_backup);

#elif(SUPPORT_VFS_LOOP_PLAY == 1)    
    vfs_file_tell(music_player_global->fs_para.file_mount_id, &offset, (uint32 ) music_player->storage_io.handel);
    offset_backup = offset;
    vfs_file_seek(music_player_global->fs_para.file_mount_id, -MP3_FILE_IDV3V1_LEN, SEEK_END,(uint32 ) music_player->storage_io.handel);
    ret = (int32) vfs_file_read(music_player_global->fs_para.file_mount_id, buf, MP3_FILE_IDV3V1_LEN,(uint32 ) music_player->storage_io.handel);
    vfs_file_seek(music_player_global->fs_para.file_mount_id, offset_backup, SEEK_SET,(uint32 ) music_player->storage_io.handel);
#endif

    if (((uint8 *) buf)[0] == 'T' && ((uint8 *) buf)[1] == 'A' && ((uint8 *) buf)[2] == 'G') //TAG标签头
    {
        libc_printf_info("idv3v1\n");
        return TRUE;
    }
    else
    {
        libc_printf_info("not idv3v1\n");
        return FALSE;
    }
}

int32 loopplay_find_musicfile_first_frame_count(music_player_t *music_player, int32 count)
{
#if (SUPPORT_RES_DIR_LOOP_PLAY == 1)
    sys_res_fseek((uint32 ) music_player->storage_io.handel,0, music_player->music_info.file_hdr_len);
    music_player->music_loopplay_info.first_offset = sys_res_ftell((uint32 ) music_player->storage_io.handel);
#elif (SUPPORT_EXTLIB_LOOP_PLAY == 1)
    lib_fseek((lib_file_t* ) music_player->storage_io.handel,0, music_player->music_info.file_hdr_len);
    music_player->music_loopplay_info.first_offset = lib_ftell((lib_file_t* ) music_player->storage_io.handel);
#elif (SUPPORT_SDLIB_LOOP_PLAY == 1)
    sys_sd_fseek((uint32 ) music_player->storage_io.handel,0, music_player->music_info.file_hdr_len);
    music_player->music_loopplay_info.first_offset = sys_sd_ftell((uint32 ) music_player->storage_io.handel);
#elif(SUPPORT_VFS_LOOP_PLAY == 1)    
    vfs_file_seek(music_player->fs_para.file_mount_id, music_player->music_info.file_hdr_len, SEEK_SET,(uint32 ) music_player->storage_io.handel);
    vfs_file_tell(music_player->fs_para.file_mount_id, &music_player->music_loopplay_info.first_offset,(uint32 ) music_player->storage_io.handel);
#endif

    count = count - (music_player->music_loopplay_info.first_offset % count);
    music_player->music_loopplay_info.read_first_frame_data = FALSE;

    return count;

}

int32 loopplay_find_musicfile_last_frame_data(music_player_t *music_player, int32 count)
{
    int32 offset = 0;
    int32 remain_data_len = 0;
    int32 file_len = music_player->music_loopplay_info.musicfile_actual_len;

#if (SUPPORT_RES_DIR_LOOP_PLAY == 1)
    offset = sys_res_ftell((uint32 ) music_player->storage_io.handel);
#elif (SUPPORT_EXTLIB_LOOP_PLAY == 1)
    offset = lib_ftell((lib_file_t* ) music_player->storage_io.handel);
#elif (SUPPORT_SDLIB_LOOP_PLAY == 1)
    offset = sys_sd_ftell((uint32 ) music_player->storage_io.handel);
#elif(SUPPORT_VFS_LOOP_PLAY == 1)    
    vfs_file_tell(music_player->fs_para.file_mount_id, &offset,(uint32 ) music_player->storage_io.handel);
#endif

    if (music_player->music_loopplay_info.music_file_has_endtag == TRUE)
    {
        remain_data_len = file_len - offset - MP3_FILE_IDV3V1_LEN;
    }
    else
    {
        remain_data_len = file_len - offset;
    }

    if (remain_data_len <= count)
    {
        count = remain_data_len;
        music_player->music_loopplay_info.read_first_frame_data = TRUE;
    }

    return count;
}

static bool find_mute_frame(music_player_t *music_player)
{
    int32 offest = 0;
    int32 filter_frame_data = music_player->music_info.avg_bitrate / 8 * FILTER_FRAME_TIME;
    int32 file_len = music_player->music_loopplay_info.musicfile_actual_len;

    if (music_player->music_loopplay_info.music_file_has_endtag == TRUE)
    {
        file_len = music_player->music_loopplay_info.musicfile_actual_len - MP3_FILE_IDV3V1_LEN;
    }

#if (SUPPORT_RES_DIR_LOOP_PLAY == 1)
    offest = sys_res_ftell((uint32 ) music_player->storage_io.handel);
#elif (SUPPORT_EXTLIB_LOOP_PLAY == 1)
    offest = lib_ftell((lib_file_t* ) music_player->storage_io.handel);
#elif (SUPPORT_SDLIB_LOOP_PLAY == 1)
    offest = sys_sd_ftell((uint32 ) music_player->storage_io.handel);
#elif(SUPPORT_VFS_LOOP_PLAY == 1)    
    vfs_file_tell(music_player->fs_para.file_mount_id,&offest,(uint32 ) music_player->storage_io.handel);
#endif

    if (music_player->music_loopplay_info.filter_head_mute_frame_flag == TRUE)
    {
        if ((offest - music_player->music_info.file_hdr_len <= filter_frame_data)
                || (file_len - offest <= filter_frame_data))
        {
            music_player->music_loopplay_info.filter_mute_frame = TRUE;
        }
        else
        {
            music_player->music_loopplay_info.filter_mute_frame = FALSE;
        }
    }
    else
    {
        if (file_len - offest <= filter_frame_data)
        {
            music_player->music_loopplay_info.filter_mute_frame = TRUE;
            music_player->music_loopplay_info.filter_head_mute_frame_flag = TRUE;
        }
        else
        {
            music_player->music_loopplay_info.filter_mute_frame = FALSE;
        }
    }

    return music_player->music_loopplay_info.filter_mute_frame;
}

int32 loopplay_is_the_last_full_play_end(music_player_t *music_player , int32 count)
{
    int32 bp_file_offset;

    bp_file_offset = music_player_global->music_decode_info.music_bp_info.bp_file_offset;
    if((music_player->music_info.file_len - bp_file_offset < 20*1024)
            &&(music_player->music_loopplay_info.read_first_frame_data == TRUE))
    {
        count = 0;
    }
    return count;
}

bool filter_mute_frame(music_player_t *music_player)
{
    if (find_mute_frame(music_player) == TRUE)
    {
        if (music_player->music_status.music_cur_energy_rms <= MUTE_FRAME_ENERGY_THRESHOLD)
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }
    else
    {
        return FALSE;
    }
}

#endif
