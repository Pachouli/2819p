/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：ADC通道SPDIF光纤输入处理。
 * 作者：cailizhen
 ********************************************************************************/

#include "adc_channel.h"

#define ZERO_DET_FRAMES 50  //全0帧检测title与content切换

void spdif_dvd_title_frame_deal(adc_channel_info_t *p_ac_info, short *p_data_adc_read, uint16 pcm_adc_buf_block)
{
    bool is_zero_frame;
    uint16 i;
    uint16 zero_frame_count_th;

    zero_frame_count_th = ZERO_DET_FRAMES;
    if ((p_ac_info->need_resample == 1) && (p_ac_info->resample_src_khz >= 88))
    {
        zero_frame_count_th *= 2;
    }

    is_zero_frame = TRUE;
    for (i = 0; i < pcm_adc_buf_block/sizeof(short); i++)
    {
        if (*(p_data_adc_read+i) != 0)
        {
            is_zero_frame = FALSE;
        }
    }

    if (p_ac_info->maybe_spdif_title_flag == TRUE)
    {
        libc_memset((uint8 *)p_data_adc_read, 0x0, pcm_adc_buf_block);

        if (is_zero_frame == FALSE)
        {
            p_ac_info->zero_frame_count++;
            if (p_ac_info->zero_frame_count >= zero_frame_count_th)
            {
                p_ac_info->maybe_spdif_title_flag = FALSE;
                p_ac_info->zero_frame_count = 0;
            }
        }
        else
        {
            p_ac_info->zero_frame_count = 0;
        }
    }
    else
    {
        if ((p_ac_info->need_fadein_flag == TRUE) && (p_ac_info->fadein_point_count > 0))
        {
            int32 temp_value;
            for (i = 0; i < pcm_adc_buf_block/sizeof(short); i += 2)
            {
                temp_value = *(p_data_adc_read+i*2);
                temp_value *= (FADEIN_STEP - p_ac_info->fadein_point_count);
                temp_value /= FADEIN_STEP;
                *(p_data_adc_read+i*2) = temp_value;

                temp_value = *(p_data_adc_read+i*2+1);
                temp_value *= (FADEIN_STEP - p_ac_info->fadein_point_count);
                temp_value /= FADEIN_STEP;
                *(p_data_adc_read+i*2+1) = temp_value;

                p_ac_info->fadein_point_count--;
                if (p_ac_info->fadein_point_count == 0)
                {
                    p_ac_info->need_fadein_flag = FALSE;
                    break;
                }
            }
        }

        if (is_zero_frame == TRUE)
        {
            p_ac_info->zero_frame_count++;
            if (p_ac_info->zero_frame_count >= zero_frame_count_th)
            {
                p_ac_info->maybe_spdif_title_flag = TRUE;
                p_ac_info->zero_frame_count = 0;

                p_ac_info->need_fadein_flag = TRUE;
                p_ac_info->fadein_point_count = FADEIN_STEP;
            }
        }
        else
        {
            p_ac_info->zero_frame_count = 0;
        }
    }
}

void spdif_sample_rate_deal(adc_channel_info_t *p_ac_info)
{
    //待确定采样率
    uint32 sr_val, pd;

    sr_val = get_spdifrx_rate(0x03, &pd);
    //libc_printf("sr:%d\n", sr_val);

    p_ac_info->sample_rate_acc += sr_val;
    p_ac_info->sample_rate_count++;
    if (p_ac_info->sample_rate_count >= 20)
    {
        uint8 temp_khz, cur_sample_rate_khz;
        bool now_need_resample = FALSE;

        p_ac_info->sample_rate_acc /= p_ac_info->sample_rate_count;
        //libc_printf("sample rate:%d\n", p_ac_info->sample_rate_acc);
        temp_khz = p_ac_info->sample_rate_acc/1000;

        //转换为khz
        if ((temp_khz >= 43) && (temp_khz <= 45))
        {
            cur_sample_rate_khz = 44;
        }
        else if ((temp_khz >= 47) && (temp_khz <= 49))
        {
            cur_sample_rate_khz = 48;
        }
        else if ((temp_khz >= 31) && (temp_khz <= 33))
        {
            cur_sample_rate_khz = 32;
        }
        else if ((temp_khz >= 87) && (temp_khz <= 89))
        {
            cur_sample_rate_khz = 44;

            now_need_resample = 1;
            p_ac_info->resample_src_khz = 88;
        }
        else if ((temp_khz >= 95) && (temp_khz <= 97))
        {
            cur_sample_rate_khz = 48;

            now_need_resample = 1;
            p_ac_info->resample_src_khz = 96;
        }
        else
        {
            cur_sample_rate_khz = 44;
        }

        if (p_ac_info->sample_rate_ok_flag == TRUE)
        {
            if ((cur_sample_rate_khz != p_ac_info->sample_rate_khz)
                || (now_need_resample != p_ac_info->need_resample))
            {
                libc_printf_warning("spdif sample rate %d -> %d\n", p_ac_info->sample_rate_khz, cur_sample_rate_khz);
                p_ac_info->sample_rate_need_update = TRUE;
            }
        }
        p_ac_info->need_resample = now_need_resample;
        p_ac_info->sample_rate_khz = cur_sample_rate_khz;

        //确定采样率
        p_ac_info->adc_chan_set.default_samplerate = p_ac_info->sample_rate_khz;

        p_ac_info->sample_rate_ok_flag = TRUE;
        p_ac_info->sample_rate_acc = 0;
        p_ac_info->sample_rate_count = 0;
    }
}

void spdif_resample_predeal(adc_channel_info_t *p_ac_info, short *p_data_adc_read, uint16 pcm_adc_buf_block)
{
    int i;
    short *p_data_right_chan = (short *) p_ac_info->resample_split_buff;

    for (i = 0; i < pcm_adc_buf_block/sizeof(short)/2; i++)
    {
        *(p_data_adc_read+i) = *(p_data_adc_read + i*2); //left
        *(p_data_right_chan+i) = *(p_data_adc_read + i*2 + 1);
    }

    if (buffer_rw_write((buffer_rw_t *)&(p_ac_info->resample_left_chan_buf), (uint8 *)p_data_adc_read, pcm_adc_buf_block/2) == FALSE)
    {
        libc_printf_warning("resample left chan buffer full!\n");
    }
    if (buffer_rw_write((buffer_rw_t *)&(p_ac_info->resample_right_chan_buf), (uint8 *)p_data_right_chan, pcm_adc_buf_block/2) == FALSE)
    {
        libc_printf_warning("resample right chan buffer full!\n");
    }
}
