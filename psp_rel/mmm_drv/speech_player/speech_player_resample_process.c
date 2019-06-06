/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：重采样处理。
 * 作者：cailizhen
 ********************************************************************************/

#include "speech_player_global.h"

#if (SUPPORT_AUDIO_RESAMPLE == 1)

extern void single_to_dual_memcpy(short *dest, int sample_cnt);

//定义重采样缓冲区
short __section__(".bss.RESAMPLE_INPUT_BUFFER") RESAMPLE_INPUT_BUFFER[1][RESAMPLE_INPUT_BUFF_SIZE];
short __section__(".bss.RESAMPLE_OUTPUT_BUFFER") RESAMPLE_OUTPUT_BUFFER[1][RESAMPLE_OUTPUT_BUFF_SIZE];
int   __section__(".bss.RESAMPLE_TABLE_BUFF")   RESAMPLE_TABLE_BUFF[RESAMPLE_TABLE_ELEMENTS];
short __section__(".bss.RESAMPLE_TEMP_BUFF")    RESAMPLE_TEMP_BUFF[1][RESAMPLE_TEMP_SAMPLES];

const resample_op_map_t g_resample_op_maps[] =
{
    {8000, 48000,  32, 192},
    {16000, 48000, 32, 96},
};

//根据输入采样率和映射表更新输入采样信息
void set_resample_info(audio_resample_info_t *resample_info, uint32 input_rate_khz)
{
    int32 i;

    for (i = 0; i < (sizeof(g_resample_op_maps)/sizeof(resample_op_map_t)); i++)
    {
        if (input_rate_khz == g_resample_op_maps[i].input_rate_hz / 1000)
        {
            if (g_resample_op_maps[i].input_samples > RESAMPLE_INPUT_BUFF_SIZE)
            {
                libc_printf("set_resample_attribute error, input_samples:%d bigger than buffer_size:%d!\n", g_resample_op_maps[i].input_samples, RESAMPLE_INPUT_BUFF_SIZE);
                while(1){}
            }
            if (g_resample_op_maps[i].output_samples > RESAMPLE_OUTPUT_BUFF_SIZE)
            {
                libc_printf("set_resample_attribute error, output_samples:%d bigger than buffer_size:%d!\n", g_resample_op_maps[i].output_samples, RESAMPLE_OUTPUT_BUFF_SIZE);
                while(1){}
            }

            resample_info->enable_resample = TRUE;
            resample_info->convert_output_rate_hz = g_resample_op_maps[i].output_rate_hz;
            resample_info->convert_input_rate_hz = g_resample_op_maps[i].input_rate_hz;
            resample_info->expected_input_sample_cnt = g_resample_op_maps[i].input_samples;
            resample_info->expected_output_sample_cnt = g_resample_op_maps[i].output_samples;
            resample_info->cur_resample_input_len = 0;
            libc_printf("enable resample process. input_rate:%d outout_rate:%d input_samples:%d out_samples:%d\n",\
                g_resample_op_maps[i].input_rate_hz, g_resample_op_maps[i].output_rate_hz, \
                g_resample_op_maps[i].input_samples, g_resample_op_maps[i].output_samples);
            return;
        }
    }

    resample_info->enable_resample = FALSE;
}

/*
    对单声道数据进行重采样和pcm输出，本函数不对pcm输出大小做分割
*/
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
void resample_and_send_pcm_to_pcm_buf(speech_player_t *speech_player, short *in_data, int32 sample_cnt)
{
    int32 resample_out_cnt;
    int32 cur_post_process_sample_cnt   = sample_cnt;  //后处理缓冲中的当前待播放采样点对的个数
    short * cur_post_process_data_addr  = in_data;     //后处理缓冲中的当前待播放采样点对的地址
    int32 remain_resample_out_cnt, resample_out_index, cur_single_to_dual_samples;
    uint32 t1 = 0, t2;

    while (speech_player->resample_info.cur_resample_input_len + cur_post_process_sample_cnt >= speech_player->resample_info.expected_input_sample_cnt)
    {
        /*
            重采样输入缓冲剩余的现有长度 + 后处理缓冲剩余的值 足够填充一个完整的重采样转换要求的输入长度，则可以进行重采样处理
        */
        //填充足量的后处理数据到重采样输入缓冲
        int32 fill_samples_cnt = speech_player->resample_info.expected_input_sample_cnt - speech_player->resample_info.cur_resample_input_len;

        //拷贝到重采样左声道缓冲区
        libc_memcpy((uint8*)(&RESAMPLE_INPUT_BUFFER[0][speech_player->resample_info.cur_resample_input_len]), \
            (uint8*)cur_post_process_data_addr, fill_samples_cnt * sizeof(short));

        //更新后处理剩余采样点地址和数量； 单声道
        cur_post_process_sample_cnt -= fill_samples_cnt;
        cur_post_process_data_addr += fill_samples_cnt;

        if (speech_player->need_print_time == TRUE)
        {
            t1 = sys_get_ab_timer_us();
        }

        //重采样左声道数据
        resample_out_cnt = pp_resample_proccess(speech_player->resample_info.resample_handle_left, RESAMPLE_INPUT_BUFFER[0], \
                                                speech_player->resample_info.expected_input_sample_cnt, RESAMPLE_OUTPUT_BUFFER[0]);
        if (resample_out_cnt != speech_player->resample_info.expected_output_sample_cnt)
        {
            libc_printf("pp_resample_proccess input_len:%d actual_resample_out_cnt:%d expected_output_sample_cnt:%d\n", \
                speech_player->resample_info.expected_input_sample_cnt, resample_out_cnt, speech_player->resample_info.expected_output_sample_cnt);
            while(1){}
        }

        if (speech_player->need_print_time == TRUE)
        {
            t2 = sys_get_ab_timer_us();

            speech_player->resample_time += (t2 - t1);
        }

        resample_out_index = 0;
        remain_resample_out_cnt = resample_out_cnt;
        while (remain_resample_out_cnt > 0)
        {
            if (remain_resample_out_cnt >= RESAMPLE_SINGLE_TO_DUAL_SAMPLES)
            {
                cur_single_to_dual_samples = RESAMPLE_SINGLE_TO_DUAL_SAMPLES;
            }
            else
            {
                cur_single_to_dual_samples = remain_resample_out_cnt;
            }

            libc_memcpy((uint8 *)&RESAMPLE_INPUT_BUFFER[0][cur_single_to_dual_samples], \
                        (uint8 *)&RESAMPLE_OUTPUT_BUFFER[0][resample_out_index], cur_single_to_dual_samples*sizeof(short));

            //单声道转成双声道
            single_to_dual_memcpy(RESAMPLE_INPUT_BUFFER[0], cur_single_to_dual_samples);
            send_pcm_to_pcm_buf_by_dac_fifo(RESAMPLE_INPUT_BUFFER[0], cur_single_to_dual_samples * 2);

            resample_out_index += cur_single_to_dual_samples;
            remain_resample_out_cnt -= cur_single_to_dual_samples;
        }

        //清空输入缓冲采样点对的大小，以准备接受新的数据
        speech_player->resample_info.cur_resample_input_len = 0;
    }

    //将剩余数据拷贝到重采样左声道缓冲区
    libc_memcpy((uint8*)(&RESAMPLE_INPUT_BUFFER[0][speech_player->resample_info.cur_resample_input_len]), \
            (uint8*)cur_post_process_data_addr, cur_post_process_sample_cnt * sizeof(short));

    speech_player->resample_info.cur_resample_input_len += cur_post_process_sample_cnt;
}

#endif
