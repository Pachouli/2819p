/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：本地播歌中间件  TTS播放处理
 ********************************************************************************/

#include "music_player_global.h"

#if (SUPPORT_EXTLIB_TTS_PLAY == 1)
int32 tts_seek_ext(lib_file_t * tts_rc_handle, int32 offset)
{
    tts_info_t *p_tts_info = music_player_global->p_tts_info;
    tts_section_t tmp_tts_sec;
    tts_section_v2_t tmp_tts_sec_v2;
    int32 ret = 0;
    int32 cur_offset = 0;
    uint8 i;

    if (p_tts_info->tts_all_offset >= p_tts_info->tts_all_length)
    {
        return -1;
    }

    for (i = 0; i < p_tts_info->tts_section_cnt; i++)
    {
        uint32 t_offset;

        //读取tts_section_t头
        t_offset = p_tts_info->tts_header_len + sizeof(tts_section_t) * p_tts_info->p_tts_section_ids[i];
        if (lib_fseek(tts_rc_handle, 0, t_offset) == -1)
        {
            return -1;
        }

        if (p_tts_info->tts_version == 2)
        {
            if (lib_fread(tts_rc_handle, &tmp_tts_sec_v2, sizeof(tts_section_v2_t)) == -1)
            {
                return -1;
            }

            if ((cur_offset + tmp_tts_sec_v2.length) > offset)
            {
                ret = lib_fseek(tts_rc_handle, 0, tmp_tts_sec_v2.offset + (offset - cur_offset));

                if (ret == 0)
                {
                    p_tts_info->tts_cur_length = tmp_tts_sec_v2.length;
                    p_tts_info->tts_cur_offset = offset - cur_offset;
                    p_tts_info->tts_cur_zero_offset = p_tts_info->tts_cur_length - tmp_tts_sec_v2.tail_zero;
                }

                return ret;
            }

            cur_offset += tmp_tts_sec_v2.length;
        }
        else
        {
            if (lib_fread(tts_rc_handle, &tmp_tts_sec, sizeof(tts_section_t)) == -1)
            {
                return -1;
            }

            if ((cur_offset + tmp_tts_sec.length) > offset)
            {
                ret = lib_fseek(tts_rc_handle, 0, tmp_tts_sec.offset + (offset - cur_offset));

                if (ret == 0)
                {
                    p_tts_info->tts_cur_length = tmp_tts_sec.length;
                    p_tts_info->tts_cur_offset = offset - cur_offset;
                }

                return ret;
            }

            cur_offset += tmp_tts_sec.length;
        }
    }

    return -1;
}

//tts v2支持压缩帧末尾0数据
//在解压缩后给到解码库时，逐帧解压缩
int32 tts_read_ext(lib_file_t * tts_rc_handle, void *buf, int32 count)
{
    tts_info_t *p_tts_info = music_player_global->p_tts_info;
    int32 ret = 0;
    int32 count_remain = count;
    int32 read_bytes = 0;
    int32 cur_read;
    int32 cur_decompression_bytes; //压缩0数据帧实际长度，后者第一帧压缩0数据之前的不足512字节数据

    while (count_remain > 0)
    {
        if (tts_seek_ext(tts_rc_handle, p_tts_info->tts_all_offset) == -1)
        {
            break;
        }

        cur_decompression_bytes = 0;

        //未压缩先送给解码库，之后是逐帧解压缩0数据
        if (p_tts_info->tts_version == 2)
        {
            if (p_tts_info->tts_cur_offset < p_tts_info->tts_cur_zero_offset)
            {
                int32 non_zero_frame_remain_bytes;
                non_zero_frame_remain_bytes = p_tts_info->tts_cur_zero_offset - p_tts_info->tts_cur_offset;
                if (non_zero_frame_remain_bytes >= count_remain)
                {
                    cur_read = count_remain;
                    cur_decompression_bytes = count_remain;
                }
                else
                {
                    cur_read = non_zero_frame_remain_bytes;
                    cur_decompression_bytes = non_zero_frame_remain_bytes;
                }

                ret = (int32) lib_fread(tts_rc_handle, (uint8 *) buf + read_bytes, cur_read);
            }
            else if (p_tts_info->tts_cur_offset < p_tts_info->tts_cur_length)
            {
                uint8 zero_frame_header[6];

                //解压缩0数据帧
                ret = (int32) lib_fread(tts_rc_handle, zero_frame_header, 6);
                libc_memcpy((uint8 *) buf, zero_frame_header, 4);
                ret = (int32) lib_fread(tts_rc_handle, (uint8 *) buf + 4, zero_frame_header[4]-4);
                libc_memset((uint8 *) buf + zero_frame_header[4], 0x0, zero_frame_header[5]);

                cur_read = zero_frame_header[4] + 2;
                cur_decompression_bytes = zero_frame_header[4] + zero_frame_header[5];
            }
        }
        else
        {
            if ((p_tts_info->tts_cur_length - p_tts_info->tts_cur_offset) >= count_remain)
            {
                cur_read = count_remain;
            }
            else
            {
                cur_read = p_tts_info->tts_cur_length - p_tts_info->tts_cur_offset;
            }

            ret = (int32) lib_fread(tts_rc_handle, (uint8 *) buf + read_bytes, cur_read);
        }

        if (p_tts_info->tts_all_offset == 0)
        {
            libc_memcpy(p_tts_info->tts_mp3_head, (uint8 *) buf + read_bytes, 4);
        }

        p_tts_info->tts_all_offset += cur_read;
        if ((p_tts_info->tts_version == 2) && (cur_decompression_bytes > 0))
        {
            read_bytes += cur_decompression_bytes;
            count_remain -= cur_decompression_bytes;
            break;
        }
        else
        {
            read_bytes += cur_read;
            count_remain -= cur_read;
        }
    }

    if ((read_bytes == 0) && (p_tts_info->tts_mp3_head_read_bytes == 16))
    {
        return -1;
    }
    else
    {
        //末尾添加帧头以便最后一帧正确解码
        if ((p_tts_info->tts_all_offset >= p_tts_info->tts_all_length) && (read_bytes < count))
        {
            uint8 cur_len;

            if ((read_bytes + (16 - p_tts_info->tts_mp3_head_read_bytes)) <= count)
            {
                cur_len = (16 - p_tts_info->tts_mp3_head_read_bytes);
            }
            else
            {
                cur_len = count - read_bytes;
            }

            libc_memcpy((uint8 *) buf + read_bytes, p_tts_info->tts_mp3_head + p_tts_info->tts_mp3_head_read_bytes,
                    cur_len);
            p_tts_info->tts_mp3_head_read_bytes += cur_len;

            read_bytes += cur_len;
        }

        return read_bytes;
    }
}
#endif
