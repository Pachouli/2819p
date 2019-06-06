/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙播歌中间件SBC解码相关接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "music_bt_player_global.h"

//从Input Buffer读取一笔数据
uint16 read_bt_input_buf(buffer_rw_t *input_buf, uint8 *read_buf, uint16 data_len)
{
    uint16 read_len, rem_len;
    uint32 temp_flag = sys_local_intc_save(0x01);

    if (input_buf->length == 0)
    {
        sys_local_intc_restore(temp_flag);

        return 0;
    }

    if (input_buf->length < data_len)
    {
        data_len = input_buf->length;
    }

    if ((input_buf->read_ptr + data_len) > input_buf->capacity)
    {
        read_len = input_buf->capacity - input_buf->read_ptr;
        rem_len = data_len - read_len;
    }
    else
    {
        read_len = data_len;
        rem_len = 0;
    }
    libc_memcpy(read_buf, input_buf->raw_data + input_buf->read_ptr, read_len);

    if (rem_len != 0)
    {
        libc_memcpy(read_buf + read_len, input_buf->raw_data, rem_len);
    }

    sys_local_intc_restore(temp_flag);
    return data_len;
}

//更新临界区变量
void update_bt_input_buf(buffer_rw_t *input_buf, uint16 read_len)
{
    buffer_rw_update(input_buf, read_len);
}

int sbc_unpack_frame(const uint8 *data, uint32 len, uint32* freq)
{
    if (len < 3)
    {
        return -1;
    }
    if ((data[0] != SBC_SYNCWORD) || (data[2] > SBC_MAX_BITPOOL))
    {
        return -2;
    }

    *freq = (data[1] >> 6) & 0x03;

    return 0;
}

//-1表示无效，16, 32, 44, 48 有效
int sbc_sample_check(music_bt_player_t *music_player, uint8* bitstream_ptr, uint32 src_len, uint32 *drop_size)
{
    music_frame_info_t sbc_frame_info;
    uint32 get_freq, i = 0;
    int framelen;
    int frequency = 44;
    int32 ret = 0;

    *drop_size = 0;
    while (i < (src_len - 2))
    {
        framelen = sbc_unpack_frame(bitstream_ptr + i, src_len - i, &get_freq);
        if (framelen == 0)
        {
            sbc_frame_info.input = bitstream_ptr;
            sbc_frame_info.input_len = src_len;
            sbc_frame_info.output = (void *) SBC_DEC_OUTPUT_BUFFER_ADDR;

            ret = sbc_decoder_cmd(music_player->audiodec_handle, AD_CMD_FRAME_DECODE, (uint32) &sbc_frame_info);
            if (ret == 0)
            {
                music_player->g_sbc_frame_size = sbc_frame_info.frame_len;
                music_player->g_sbc_channel_mode = (bitstream_ptr[1]>>2)&0x03; //channel mode
                if (music_player->g_sbc_frame_size > SBC_MAX_FRAME_LEN)
                {
                    music_player->g_sbc_frame_size = SBC_MAX_FRAME_LEN;
                }
                switch (get_freq)
                {
                    case SBC_FREQ_16000:
                    frequency = 16;
                    break;

                    case SBC_FREQ_32000:
                    frequency = 32;
                    break;

                    case SBC_FREQ_44100:
                    frequency = 44;
                    break;

                    case SBC_FREQ_48000:
                    frequency = 48;
                    break;

                    default:
                    frequency = 44;
                    break;
                }
                music_player->g_sbc_sample_rate = frequency;
                break;
            }

            *drop_size = i;
        }
        i++;
    }

    if (i == (src_len - 2))
    {
        *drop_size = i;
        //第一批数据严重异常，需丢弃
        libc_printf_warning("sbc data err!!\n");
        return -1;
    }

    return frequency;
}

//play sbc check deal
int sbc_check_sample_rate(music_bt_player_t *music_player)
{
    int check_ret = 0;
    uint32 drop_size;
    uint8 *sbc_dec_temp_buf = (uint8 *) SBC_DEC_TEMP_BUFFER_ADDR;

    read_bt_input_buf(music_player->input_buf, sbc_dec_temp_buf, SBC_DEC_TEMP_BUFFER_LEN);

    check_ret = sbc_sample_check(music_player, sbc_dec_temp_buf, SBC_DEC_TEMP_BUFFER_LEN, &drop_size);

    if (drop_size != 0)
    {
        libc_printf_warning("sbc check drop:%d\n", drop_size);
        update_bt_input_buf(music_player->input_buf, drop_size);
    }

    return check_ret;
}

//根据帧长来计算，假设每帧长度一致，并且最大帧长为119字节
int sbc_calc_frame_count(music_bt_player_t *music_player)
{
    uint32 temp_flag;

    temp_flag = sys_local_intc_save(0x01);

    music_player->frame_count = music_player->input_buf->length / music_player->g_sbc_frame_size;
    if ((music_player->input_buf->length % music_player->g_sbc_frame_size) != 0)
    {
        music_player->frame_count++;
    }

    sys_local_intc_restore(temp_flag);

    return 0;
}

