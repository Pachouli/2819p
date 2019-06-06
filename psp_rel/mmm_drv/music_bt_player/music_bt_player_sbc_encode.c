/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙播歌中间件SBC编码，结果输出到SBC Input Buffer。
 * 作者：cailizhen
 ********************************************************************************/

#include "music_bt_player_global.h"

#ifdef DEC_ENABLE_TRUE_WIRELESS_STEREO

int32 sbc_encoder_frame_deal(music_bt_player_t *music_player)
{
    music_encode_info_t sbc_encode_info;
    short *encode_input = SBC_DEC_TEMP_BUFFER_ADDR;
    uint8 *encode_ouput = SBC_ENC_OUTPUT_BUFFER_ADDR;
    uint32 t1, t2;

    while (buffer_rw_read(music_player->p_input_buf_r, encode_input, 512) == TRUE)
    {
        sbc_encode_info.ain = encode_input;
        sbc_encode_info.aout = encode_ouput;

        if (music_player->need_print_time == TRUE)
        {
            t1 = sys_get_ab_timer_us();
        }
        if (sbc_encoder_cmd(music_player->sbc_encoder_handle, AE_CMD_FRAME_ENCODE, (uint32) &sbc_encode_info) != 0)
        {
            return -1;
        }
        if (music_player->need_print_time == TRUE)
        {
            t2 = sys_get_ab_timer_us();
            music_player->encoder_time += (t2 - t1);
        }

        if (sbc_encode_info.bytes_enced <= 119)
        {
            if (music_player->p_f_send_frame != NULL)
            {
                music_player->p_f_send_frame(encode_ouput, sbc_encode_info.bytes_enced);
            }
            else
            {
                libc_printf_error("p_f_send_frame cbk is NULL\n");
                while (1);
            }
        }
        else
        {
            libc_printf_warning("sbc encoder output len invalid:%d\n", sbc_encode_info.bytes_enced);
        }
    }

    return 0;
}
#endif
