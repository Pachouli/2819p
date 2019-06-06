/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * �������ز�������
 ********************************************************************************/

#include "music_player_global.h"

#if (SUPPORT_AUDIO_RESAMPLE == 1)

//�����ز�����ػ�����
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

//������������ʺ�ӳ���������������Ϣ
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
    �ú��������16bits�������ݷ��뵽����������16bits��������������
����:
    left_output_buff: ���������ջ���
    right_output_buff: ���������ջ���
    mixed_input_buff: ����������뻺��
    samples:    ��ת���Ĳ����Դ�С
ע��:
    �ú������Ի����С����飬������ȷ�����岻���
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
    ר������ز������������е�һ��ת�����������16bits������������ת����16bits��������ϸ�ʽ����
����:
    samples: ��ת���Ĳ�����Ե�����
    RESAMPLE_OUTPUT_BUFFER: ����ز���������������ݣ��Ǹ���ά����ȫ�ֱ�����[0]��ʾ������,[1]�������壬[2]������
���˵��:
    ת����ɺ�[0]��[2]��Ӧ�ķ������ݽ������һ�ϸ�ʽ�����洢��[0]��[1]����������ռ䣬���samplesֵС����ֻռ��[0]�ռ�
*/
void resample_turn_seperated_to_mix_16bit_format(int32 samples)
{
    int32 i;

    if (samples > RESAMPLE_OUTPUT_BUFF_SIZE)
    {
        libc_printf("turn_seperated_16bit_to_mix_format input_samples too big:%d > max_size:%d!\n", samples, RESAMPLE_OUTPUT_BUFF_SIZE);
        while(1){}
    }

    //���������Ĳ�����ֵת�Ƶ�(��Ӧ����ֵ2��)��λ�ã���0->0,1->2,2->4����˽����ܻ���[1]����д������
    //ע�⣬����Ӻ���ǰ����
    for (i = (samples-1); i > 0; i--)
    {
        RESAMPLE_OUTPUT_BUFFER[0][2*i] = RESAMPLE_OUTPUT_BUFFER[0][i];
    }

    //��������������ֵ���Ƶ�������������(��Ӧ����ֵ2��+1)��λ�ã������ܻ���[1]����д������
    for (i = (samples-1); i >= 0; i--)
    {
        RESAMPLE_OUTPUT_BUFFER[0][2*i+1] = RESAMPLE_OUTPUT_BUFFER[2][i];
    }
}

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
bool resample_and_send_pcm_to_pcm_buf(music_player_t *music_player, short *in_data, int32 samples_cnt, \
        bool *pcm_buf_full_flag, bool ignore_dec_too_late)
{
    int32 resample_out_cnt[2];
    int32 cur_post_process_samples_cnt = samples_cnt; //�������еĵ�ǰ�����Ų�����Եĸ���
    short * cur_post_process_data_addr = in_data; //�������еĵ�ǰ�����Ų�����Եĵ�ַ
    uint32 t1 = 0, t2;

    while (music_player->resample_info.cur_resample_input_len + cur_post_process_samples_cnt >= music_player->resample_info.expected_input_sample_cnt)
    {
        /*
            �ز������뻺��ʣ������г��� + ������ʣ���ֵ �㹻���һ���������ز���ת��Ҫ������볤�ȣ�����Խ����ز�������
        */
        //��������ĺ������ݵ��ز������뻺��
        int32 fill_size = music_player->resample_info.expected_input_sample_cnt - music_player->resample_info.cur_resample_input_len;

        aout_turn_mix_to_seperated_16bit_format(&RESAMPLE_INPUT_BUFFER[0][music_player->resample_info.cur_resample_input_len], \
            &RESAMPLE_INPUT_BUFFER[1][music_player->resample_info.cur_resample_input_len], cur_post_process_data_addr, fill_size);

        //���º���ʣ��������ַ�ͳ���
        cur_post_process_samples_cnt -= fill_size;
        cur_post_process_data_addr += fill_size * 2;    //ע�⣬���������һ�ϸ�ʽ������Ҫ����2

        if (music_player->need_print_time == TRUE)
        {
            t1 = sys_get_ab_timer_us();
        }

        //�ز���������
        resample_out_cnt[0] = pp_resample_proccess(music_player->resample_info.resample_handle_left, (short*)RESAMPLE_INPUT_BUFFER[0], \
            music_player->resample_info.expected_input_sample_cnt, (short*)RESAMPLE_OUTPUT_BUFFER[0]);
        //�ز���������
        resample_out_cnt[1] = pp_resample_proccess(music_player->resample_info.resample_handle_right, (short*)RESAMPLE_INPUT_BUFFER[1], \
            music_player->resample_info.expected_input_sample_cnt, (short*)RESAMPLE_OUTPUT_BUFFER[2]);

        if (music_player->need_print_time == TRUE)
        {
            t2 = sys_get_ab_timer_us();

            music_player->resample_time += (t2 - t1);
        }

        //���ز���������и�ʽת���������ҷ���ʽת���ɻ��ʽ
        resample_turn_seperated_to_mix_16bit_format(resample_out_cnt[0]);

        //������뻺�������ԵĴ�С����׼�������µ�����
        music_player->resample_info.cur_resample_input_len = 0;

        //������������ϸ�ʽ���ز����������dac_fifo
        if (loop_send_pcm_to_pcm_buf(music_player, RESAMPLE_OUTPUT_BUFFER[0], resample_out_cnt[0], \
        	    pcm_buf_full_flag, ignore_dec_too_late) == TRUE)
        {
            return TRUE;
        }

        //������뻺�������ԵĴ�С����׼�������µ�����
        music_player->resample_info.cur_resample_input_len = 0;
    }

    //������ʣ���������䵽�ز������뻺����
    aout_turn_mix_to_seperated_16bit_format(&RESAMPLE_INPUT_BUFFER[0][music_player->resample_info.cur_resample_input_len], \
            &RESAMPLE_INPUT_BUFFER[1][music_player->resample_info.cur_resample_input_len], cur_post_process_data_addr, cur_post_process_samples_cnt);

    music_player->resample_info.cur_resample_input_len += cur_post_process_samples_cnt;

    return FALSE;
}

#endif
