/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：本地播歌中间件  对音乐资源文件做format check处理
 ********************************************************************************/

#include "music_player_global.h"

int32 read_n_bytes(music_info_t *music_info, int32 count, uint8 *header)
{
    int32 ret = count;
    int32 i, bytes_read;
    uint8 *bs_ptr, *hdr_ptr;
    storage_io_t *storage_io = music_info->storage_io;

    bs_ptr = (uint8 *) music_info->data_buf_addr + 512 - music_info->data_buf_len;
    hdr_ptr = header;
    i = 0;
    while (i < count)
    {
        if (music_info->data_buf_len > 0)
        {
            hdr_ptr[i] = bs_ptr[i];
            i++;
            music_info->data_buf_len--;
        }
        else
        {
            bytes_read = storage_io->read(music_info->data_buf_addr, 1, 512, storage_io);
            if (bytes_read <= 0)
            {
                ret = -1;
                goto read_n_bytes_exit_0;
            }
            music_info->data_buf_len += bytes_read;
            bs_ptr = (uint8 *) music_info->data_buf_addr - i;
        }
    }
    read_n_bytes_exit_0: return ret;
}

int32 skip_n_bytes(music_info_t *music_info, int32 count)
{
    int32 ret = 0;
    int32 bytes_read, page_skip;
    storage_io_t *storage_io = music_info->storage_io;

    if (music_info->data_buf_len > count)
    {
        music_info->data_buf_len -= count;
    }
    else
    {
        page_skip = (int32) ((uint32) (count - music_info->data_buf_len) & 0xfffffe00);
        if (page_skip != 0)
        {
            storage_io->seek(storage_io, page_skip, SEEK_CUR);
        }
        bytes_read = storage_io->read(music_info->data_buf_addr, 1, 512, storage_io);
        if (bytes_read <= 0)
        {
            ret = -1;
            goto skip_n_bytes_exit_0;
        }
        music_info->data_buf_len = bytes_read - ((count - music_info->data_buf_len) & 0X1FF);
    }
    skip_n_bytes_exit_0: return ret;
}

int32 set_n_bytes(music_info_t *music_info, int32 count)
{
    int32 ret = 0;
    int32 bytes_read;
    storage_io_t *storage_io = music_info->storage_io;

    storage_io->seek(storage_io, (int32) ((uint32) count & 0xfffffe00), SEEK_SET);
    bytes_read = storage_io->read(music_info->data_buf_addr, 1, 512, storage_io);
    if (bytes_read <= 0)
    {
        ret = -1;
        goto set_n_bytes_exit_0;
    }
    music_info->data_buf_len = bytes_read - (count & 0X1FF);

    set_n_bytes_exit_0: return ret;
}


int32 format_check(music_info_t *music_info)
{
    storage_io_t *storage_io = music_info->storage_io;
    music_player_t *music_player = music_player_global;
    uint32 file_len = 0, id3_len = 0;
    int32 ret = 0;
    int32 getbytes = 0;
    uint8 *header;
    int32 tagsize;
    uint32 tmp, tmp1;

    music_info->music_type = UNSUPPORT_TYPE;
    music_info->decode_mode = 0;
    /*默认为2声道，避免6声道文件报错以后，mp3歌曲不更新通道数而报错*/
    music_info->channels = 2;
    music_info->buf = 0;

    if (music_player->fs_para.fs_type == 0)
    {
        /*vfs*/
        /*0 for handle*/
        vfs_file_get_size(music_player->fs_para.file_mount_id, &file_len, music_player->storage_io.handel, 0);
    }
    else
    {
        if (music_player->tts_mode == 1)
        {
            file_len = 0x100000;
        }
        else
        {
            /*not vfs, such as sd file system*/
            storage_io->seek(storage_io, 0, SEEK_END);
            file_len = storage_io->tell(storage_io);
            storage_io->seek(storage_io, 0, SEEK_SET);
        }
    }

    music_info->file_len = file_len;
    if (file_len < 512)
    {
        ret = -1;
        goto format_check_exit_0;
    }

    music_info->data_buf_addr = (int8 *) format_check_temp_buf; //buf;
    music_info->data_buf_len = 0;
    header = (uint8 *) music_info->data_buf_addr + 512;

    //tts seek到第一帧位置，直接分析
    if (music_player->tts_mode == 1)
    {
        tts_section_t tmp_tts_sec;
#if(SUPPORT_EXTLIB_TTS_PLAY == 1)  
        if (lib_fseek(music_player->storage_io.handel, 0, music_player->p_tts_info->tts_header_len) == -1)
#else
        if (sys_sd_fseek(music_player->storage_io.handel, 0, music_player->p_tts_info->tts_header_len) == -1)
#endif
        {
            goto format_check_exit_0;
        }
#if(SUPPORT_EXTLIB_TTS_PLAY == 1)  
        if (lib_fread(music_player->storage_io.handel, &tmp_tts_sec, sizeof(tts_section_t)) == -1)
#else
        if (sys_sd_fread(music_player->storage_io.handel, &tmp_tts_sec, sizeof(tts_section_t)) == -1)
#endif
        {
            goto format_check_exit_0;
        }

        music_info->file_hdr_len = tmp_tts_sec.offset;

        //mp3 check
        ret = mp3check(music_info, music_player->fs_para.only_mono);
        if (ret != 0)
        {
            goto format_check_exit_0;
        }
        music_info->music_type = MP3_TYPE;
        music_info->decode_mode = 0xabcd; //表示为TTS，会在加载数据时做特殊处理
    }
    else
    {
        getbytes = read_n_bytes(music_info, 10, header);
        if (getbytes < 0)
        {
            ret = -1;
            goto format_check_exit_0;
        }

        /* 跳过多个ID3信息 */
        while (1)
        {
            if (!((header[0] == 0x49) && (header[1] == 0x44) && (header[2] == 0x33)))
            {
                break;
            }
            /* check high bit is not used */
            if (((header[6] | header[7] | header[8] | header[9]) & 0x80) == 0x80)
            {
                /*Bad ID3, skip ID3 */
                break;
            }

            tagsize = (int32)(((uint32) header[6] << 21) | ((uint32) header[7] << 14) | ((uint32) header[8] << 7)
                    | ((uint32) header[9]));

            id3_len += (tagsize + 10);
            if (file_len < id3_len)
            {
                ret = -1;
                goto format_check_exit_0;
            }

            skip_n_bytes(music_info, id3_len - 10);
            /* get another 10bytes for format decision */
            getbytes = read_n_bytes(music_info, 10, header);
            if (getbytes < 10)
            {
                ret = -1;
                goto format_check_exit_0;
            }
        }

        music_info->file_hdr_len = id3_len;

        getbytes = read_n_bytes(music_info, 2, header + 10); //extend to 12 bytes

        tmp = (((uint32) header[0] << 24) | ((uint32) header[1] << 16) | ((uint32) header[2] << 8)
                | ((uint32) header[3]));

        //兼容act工具制作的文件
        tmp1 = (((uint32) header[8] << 24) | ((uint32) header[9] << 16) | ((uint32) header[10] << 8)
                | ((uint32) header[11]));
        if (tmp1 == 0x57415645)
        {
            tmp = 0x52494646;
        }

        switch (tmp)
        {
            //ASF header 0x30,0x26,0xB2,0x75"
            case 0x3026B275:
            if (music_info->file_hdr_len != 0)
            {
                ret = -1; //special wma include id3, not support
                goto format_check_exit_0;
            }
            ret = wmacheck(music_info);
            if (ret != 0)
            {
                goto format_check_exit_0;
            }
            music_info->music_type = WMA_TYPE;
            break;

            //"RIFF"?" 0x52,0x49,0x46,0x46"
            case 0x52494646:
            ret = wavcheck(music_info);
            if (ret < 0)
            {
                goto format_check_exit_0;
            }
            else if (ret == 0)
            {
                music_info->music_type = WAV_TYPE;
            }
            else
            {
                ret = -1;
                goto format_check_exit_0;
            }
            if (music_info->sub_format == 0x55)
            {
                /*mp3 in wav container*/
                music_info->file_hdr_len = 0;
                //mp3 check
                ret = mp3check(music_info, music_player->fs_para.only_mono);
                if (ret != 0)
                {
                    goto format_check_exit_0;
                }
                music_info->music_type = MP3_TYPE;
#if (SUPPORT_LOOP_PLAY == 1)
                if(music_player->music_loopplay_info.is_loop_play == TRUE)
                {
                    music_info->decode_mode = 0xabcd; //表示为TTS，会在加载数据时做特殊处理
                }
#endif
            }
            break;

            //"oggs"?" 0x4f,0x67,0x67,0x53"
            case 0x4f676753:
            ret = -1;
            goto format_check_exit_0;
            break;

#if (SUPPORT_APE_TYPE == 1)
            //APE:"MAC "?" 0x4D,0x41,0x43,0x20"
            case 0x4D414320:
            ret = apecheck(music_info);
            if (ret != 0)
            {
                goto format_check_exit_0;
            }
            music_info->music_type = APE_TYPE;
            //hifi_mode = 1; //?? 282
            //time_end_stamp = 1; //?? 282
            break;
#else
            //APE:"MAC "?" 0x4D,0x41,0x43,0x20"
            case 0x4D414320:
            ret = -1;
            goto format_check_exit_0;
            break;
#endif

#if (SUPPORT_FLAC_TYPE == 1)
            //fLaC"?" 0x66,0x4c,0x61,0x43"
            case 0x664c6143:
            ret = flaccheck(music_info);
            if (ret != 0)
            {
                goto format_check_exit_0;
            }
            music_info->music_type = FLAC_TYPE;
            break;
#else
            //fLaC"?" 0x66,0x4c,0x61,0x43"
            case 0x664c6143:
            ret = -1;
            goto format_check_exit_0;
            break;
#endif

            //xls:"新建Microsoft Excel工作表的.wma"
            case 0xd0cf11e0:
            ret = -1;
            goto format_check_exit_0;
            break;

            default:

            tmp = (((uint32) header[4] << 24) | ((uint32) header[5] << 16) | ((uint32) header[6] << 8)
                    | ((uint32) header[7]));
            //ftyp"?" 0x66,0x74,0x79,0x70"
            if (tmp == 0x66747970 || tmp == 0x57907536)
            {
                ret = -1;
                goto format_check_exit_0;
            }
            else
            {
                //mp3 check
                ret = mp3check(music_info, music_player->fs_para.only_mono);
                if (ret != 0)
                {
                    goto format_check_exit_0;
                }
                music_info->music_type = MP3_TYPE;
#if (SUPPORT_LOOP_PLAY == 1)
                if (music_player->music_loopplay_info.is_loop_play == TRUE)
                {
                    music_info->decode_mode = 0xabcd; //表示为TTS，会在加载数据时做特殊处理
                }
#endif
            }
            break;
        }
    }

    music_info->sample_rate = music_info->sample_rate_hz / 1000;

    format_check_exit_0:

    return ret;
}

