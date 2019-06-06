/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：重采样处理
 ********************************************************************************/

#include "music_player_global.h"

#if (SUPPORT_AUDIO_RESAMPLE == 1)

//定义重采样相关缓冲区
short __section__(".bss.RESAMPLE_INPUT_BUFFER") RESAMPLE_INPUT_BUFFER[2][RESAMPLE_INPUT_BUFF_SIZE];
short __section__(".bss.RESAMPLE_OUTPUT_BUFFER") RESAMPLE_OUTPUT_BUFFER[3][RESAMPLE_OUTPUT_BUFF_SIZE];
int   __section__(".bss.RESAMPLE_TABLE_BUFF")   RESAMPLE_TABLE_BUFF[RESAMPLE_TABLE_ELEMENTS];
short __section__(".bss.RESAMPLE_TEMP_BUFF")    RESAMPLE_TEMP_BUFF[2][RESAMPLE_TEMP_SAMPLES];

const resample_op_map_t g_resample_op_maps[] =
{
    {8000, 48000,  32, 192},
    {16000, 48000, 64, 192},
    {11025, 44100, 64,  256},
    {22050, 44100, 128, 256},
    {12000, 48000, 64,  256},
    {24000, 48000, 128, 256},
    {32000, 48000, 128, 192},
};

//根据输入采样率和映射表更新输入采样信息
void set_resample_info(audio_resample_info_t *resample_info, uint32 input_rate_khz)
{
    int32 i;
    for(i = 0; i < (sizeof(g_resample_op_maps)/sizeof(resample_op_map_t)); i++)
    {
        if (input_rate_khz == g_resample_op_maps[i].input_rate_hz / 1000)
        {
            if (g_resample_op_maps[i].input_samples > RESAMPLE_INPUT_BUFF_SIZE)
            {
                libc_printf("set_resample_attribute error, input_samples:%d bigger than buffer_size:%d!\n", \
                    g_resample_op_maps[i].input_samples, RESAMPLE_INPUT_BUFF_SIZE);
                while(1){}
            }
            if (g_resample_op_maps[i].output_samples > RESAMPLE_OUTPUT_BUFF_SIZE)
            {
                libc_printf("set_resample_attribute error, output_samples:%d bigger than buffer_size:%d!\n", \
                    g_resample_op_maps[i].output_samples, RESAMPLE_OUTPUT_BUFF_SIZE);
                while(1){}
            }

            resample_info->enable_resample = TRUE;
            resample_info->convert_output_rate_hz = g_resample_op_maps[i].output_rate_hz;
            resample_info->convert_input_rate_hz = g_resample_op_maps[i].input_rate_hz;
            resample_info->expected_input_sample_cnt = g_resample_op_maps[i].input_samples;
            resample_info->expected_output_sample_cnt = g_resample_op_maps[i].output_samples;
            resample_info->cur_resample_input_len = 0;
            libc_printf("support resample process. input_rate:%d outout_rate:%d input_samples:%d out_samples:%d\n",\
                g_resample_op_maps[i].input_rate_hz, g_resample_op_maps[i].output_rate_hz, \
                g_resample_op_maps[i].input_samples, g_resample_op_maps[i].output_samples);
            return;
        }
    }

    resample_info->enable_resample = FALSE;
}

/*
    该函数将混合16bits声道数据分离到两个独立的16bits左右声道缓冲中
参数:
    left_output_buff: 左声道接收缓冲
    right_output_buff: 右声道接收缓冲
    mixed_input_buff: 混合声道输入缓冲
    samples:    待转换的采样对大小
注意:
    该函数不对缓冲大小做检查，调用者确保缓冲不溢出
*/
void aout_turn_mix_to_seperated_16bit_format(short * left_output_buff, short * right_output_buff, short * mixed_input_buff, int32 samples)
{
    int i;

    if (NULL == left_output_buff || NULL == right_output_buff || NULL == mixed_input_buff)
    {
        libc_printf("aout_turn_to_seperated_16bit_format param null!\n");
        while(1) ;
    }

    for(i = 0; i < samples; i++)
    {
        left_output_buff[i]  = mixed_input_buff[2*i];
        right_output_buff[i] = mixed_input_buff[2*i+1];
    }
}

/*
    专门针对重采样输出缓冲进行的一种转换，将分离的16bits左右声道数据转换成16bits的声道混合格式数据
参数:
    samples: 待转换的采样点对的数量
    RESAMPLE_OUTPUT_BUFFER: 存放重采样输出采样点数据，是个三维数组全局变量，[0]表示左声道,[1]辅助缓冲，[2]右声道
结果说明:
    转换完成后，[0]和[2]对应的分离数据将以左右混合格式连续存储在[0]和[1]这两个缓冲空间，如果samples值小，则只占用[0]空间
*/
void resample_turn_seperated_to_mix_16bit_format(int32 samples)
{
    int32 i;

    if (samples > RESAMPLE_OUTPUT_BUFF_SIZE)
    {
        libc_printf("turn_seperated_16bit_to_mix_format input_samples too big:%d > max_size:%d!\n", samples, RESAMPLE_OUTPUT_BUFF_SIZE);
        while(1){}
    }

    //将左声道的采样点值转移到(对应索引值2倍)的位置，例0->0,1->2,2->4，因此将可能会在[1]缓冲写入数据
    //注意，必须从后往前遍历
    for (i = (samples-1); i > 0; i--)
    {
        RESAMPLE_OUTPUT_BUFFER[0][2*i] = RESAMPLE_OUTPUT_BUFFER[0][i];
    }

    //将右声道采样点值复制到左声道缓冲区(对应索引值2倍+1)的位置，将可能会在[1]缓冲写入数据
    for (i = (samples-1); i >= 0; i--)
    {
        RESAMPLE_OUTPUT_BUFFER[0][2*i+1] = RESAMPLE_OUTPUT_BUFFER[2][i];
    }
}

/*
    支持重采样，则需要对后处理结果进行重采样，之后将重采样结果作为dac_fifo_buf
说明：
    后处理缓冲区的数据要经过格式转换并添加到重采样输入缓冲区中，经过重采样之后，通过将重采样输出缓冲的数据发到
    dac_fifo_buff中以达到播放的目的.
注意：
    1. 后处理采样点对的数量并不一定是重采样输入参数要求的整数倍，因此要注意重采样未对齐采样点对的维护，
        这一点与last_unalign_frame_sc值类似。
    2. 后处理采样点对是以16bits左右混合格式保存声道数据，因此要经过分离处理才能进行重采样
*/
bool resample_and_send_pcm_to_pcm_buf(music_player_t *music_player, short *in_data, int32 samples_cnt, \
        bool *pcm_buf_full_flag, bool ignore_dec_too_late)
{
    int32 resample_out_cnt[2];
    int32 cur_post_process_samples_cnt = samples_cnt; //后处理缓冲中的当前待播放采样点对的个数
    short * cur_post_process_data_addr = in_data; //后处理缓冲中的当前待播放采样点对的地址
    uint32 t1 = 0, t2;

    while (music_player->resample_info.cur_resample_input_len + cur_post_process_samples_cnt >= music_player->resample_info.expected_input_sample_cnt)
    {
        /*
            重采样输入缓冲剩余的现有长度 + 后处理缓冲剩余的值 足够填充一个完整的重采样转换要求的输入长度，则可以进行重采样处理
        */
        //填充足量的后处理数据到重采样输入缓冲
        int32 fill_size = music_player->resample_info.expected_input_sample_cnt - music_player->resample_info.cur_resample_input_len;

        aout_turn_mix_to_seperated_16bit_format(&RESAMPLE_INPUT_BUFFER[0][music_player->resample_info.cur_resample_input_len], \
            &RESAMPLE_INPUT_BUFFER[1][music_player->resample_info.cur_resample_input_len], cur_post_process_data_addr, fill_size);

        //更新后处理剩余采样点地址和长度
        cur_post_process_samples_cnt -= fill_size;
        cur_post_process_data_addr += fill_size * 2;    //注意，这里是左右混合格式，所以要乘以2

        if (music_player->need_print_time == TRUE)
        {
            t1 = sys_get_ab_timer_us();
        }

        //重采样左声道
        resample_out_cnt[0] = pp_resample_proccess(music_player->resample_info.resample_handle_left, (short*)RESAMPLE_INPUT_BUFFER[0], \
            music_player->resample_info.expected_input_sample_cnt, (short*)RESAMPLE_OUTPUT_BUFFER[0]);
        //重采样右声道
        resample_out_cnt[1] = pp_resample_proccess(music_player->resample_info.resample_handle_right, (short*)RESAMPLE_INPUT_BUFFER[1], \
            music_player->resample_info.expected_input_sample_cnt, (short*)RESAMPLE_OUTPUT_BUFFER[2]);

        if (music_player->need_print_time == TRUE)
        {
            t2 = sys_get_ab_timer_us();

            music_player->resample_time += (t2 - t1);
        }

        //对重采样输出进行格式转换，从左右分离式转换成混合式
        resample_turn_seperated_to_mix_16bit_format(resample_out_cnt[0]);

        //清空输入缓冲采样点对的大小，以准备接受新的数据
        music_player->resample_info.cur_resample_input_len = 0;

        //将左右声道混合格式的重采样输出发到dac_fifo
        if (loop_send_pcm_to_pcm_buf(music_player, RESAMPLE_OUTPUT_BUFFER[0], resample_out_cnt[0], \
        	    pcm_buf_full_flag, ignore_dec_too_late) == TRUE)
        {
            return TRUE;
        }

        //清空输入缓冲采样点对的大小，以准备接受新的数据
        music_player->resample_info.cur_resample_input_len = 0;
    }

    //将后处理剩余采样点填充到重采样输入缓冲中
    aout_turn_mix_to_seperated_16bit_format(&RESAMPLE_INPUT_BUFFER[0][music_player->resample_info.cur_resample_input_len], \
            &RESAMPLE_INPUT_BUFFER[1][music_player->resample_info.cur_resample_input_len], cur_post_process_data_addr, cur_post_process_samples_cnt);

    music_player->resample_info.cur_resample_input_len += cur_post_process_samples_cnt;

    return FALSE;
}

#endif
