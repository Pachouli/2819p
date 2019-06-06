/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������ز�������
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "speech_player_global.h"

#if (SUPPORT_AUDIO_RESAMPLE == 1)

extern void single_to_dual_memcpy(short *dest, int sample_cnt);

//�����ز���������
short __section__(".bss.RESAMPLE_INPUT_BUFFER") RESAMPLE_INPUT_BUFFER[1][RESAMPLE_INPUT_BUFF_SIZE];
short __section__(".bss.RESAMPLE_OUTPUT_BUFFER") RESAMPLE_OUTPUT_BUFFER[1][RESAMPLE_OUTPUT_BUFF_SIZE];
int   __section__(".bss.RESAMPLE_TABLE_BUFF")   RESAMPLE_TABLE_BUFF[RESAMPLE_TABLE_ELEMENTS];
short __section__(".bss.RESAMPLE_TEMP_BUFF")    RESAMPLE_TEMP_BUFF[1][RESAMPLE_TEMP_SAMPLES];

const resample_op_map_t g_resample_op_maps[] =
{
    {8000, 48000,  32, 192},
    {16000, 48000, 32, 96},
};

//������������ʺ�ӳ���������������Ϣ
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
    �Ե��������ݽ����ز�����pcm���������������pcm�����С���ָ�
*/
/*
    ֧���ز���������Ҫ�Ժ����������ز�����֮���ز��������Ϊdac_fifo_buf
˵����
    ��������������Ҫ������ʽת������ӵ��ز������뻺�����У������ز���֮��ͨ�����ز��������������ݷ���
    dac_fifo_buff���Դﵽ���ŵ�Ŀ��.
ע�⣺
    1. ���������Ե���������һ�����ز����������Ҫ��������������Ҫע���ز���δ���������Ե�ά����
        ��һ����last_unalign_frame_scֵ���ơ�
    2. ��������������16bits���һ�ϸ�ʽ�����������ݣ����Ҫ�������봦����ܽ����ز���
*/
void resample_and_send_pcm_to_pcm_buf(speech_player_t *speech_player, short *in_data, int32 sample_cnt)
{
    int32 resample_out_cnt;
    int32 cur_post_process_sample_cnt   = sample_cnt;  //�������еĵ�ǰ�����Ų�����Եĸ���
    short * cur_post_process_data_addr  = in_data;     //�������еĵ�ǰ�����Ų�����Եĵ�ַ
    int32 remain_resample_out_cnt, resample_out_index, cur_single_to_dual_samples;
    uint32 t1 = 0, t2;

    while (speech_player->resample_info.cur_resample_input_len + cur_post_process_sample_cnt >= speech_player->resample_info.expected_input_sample_cnt)
    {
        /*
            �ز������뻺��ʣ������г��� + ������ʣ���ֵ �㹻���һ���������ز���ת��Ҫ������볤�ȣ�����Խ����ز�������
        */
        //��������ĺ������ݵ��ز������뻺��
        int32 fill_samples_cnt = speech_player->resample_info.expected_input_sample_cnt - speech_player->resample_info.cur_resample_input_len;

        //�������ز���������������
        libc_memcpy((uint8*)(&RESAMPLE_INPUT_BUFFER[0][speech_player->resample_info.cur_resample_input_len]), \
            (uint8*)cur_post_process_data_addr, fill_samples_cnt * sizeof(short));

        //���º���ʣ��������ַ�������� ������
        cur_post_process_sample_cnt -= fill_samples_cnt;
        cur_post_process_data_addr += fill_samples_cnt;

        if (speech_player->need_print_time == TRUE)
        {
            t1 = sys_get_ab_timer_us();
        }

        //�ز�������������
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

            //������ת��˫����
            single_to_dual_memcpy(RESAMPLE_INPUT_BUFFER[0], cur_single_to_dual_samples);
            send_pcm_to_pcm_buf_by_dac_fifo(RESAMPLE_INPUT_BUFFER[0], cur_single_to_dual_samples * 2);

            resample_out_index += cur_single_to_dual_samples;
            remain_resample_out_cnt -= cur_single_to_dual_samples;
        }

        //������뻺�������ԵĴ�С����׼�������µ�����
        speech_player->resample_info.cur_resample_input_len = 0;
    }

    //��ʣ�����ݿ������ز���������������
    libc_memcpy((uint8*)(&RESAMPLE_INPUT_BUFFER[0][speech_player->resample_info.cur_resample_input_len]), \
            (uint8*)cur_post_process_data_addr, cur_post_process_sample_cnt * sizeof(short));

    speech_player->resample_info.cur_resample_input_len += cur_post_process_sample_cnt;
}

#endif
