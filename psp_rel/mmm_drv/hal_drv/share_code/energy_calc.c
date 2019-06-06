/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：能量值计算。
 * 作者：cailizhen
 ********************************************************************************/

#include <psp_includes.h>

void energy_calc_inner(short *frame_buf, uint16 samples_cnt, bool is_stereo, short *p_cur_power_max, short *p_cur_power_rms)
{
    int32 sample_total = 0;
    int32 sample_value;
    int32 sample_max = 0;
    uint16 sample_cnt;
    uint16 i;

    sample_cnt = samples_cnt;
    if (is_stereo == TRUE)
    {
        sample_cnt *= 2;
    }

    for (i = 0; i < sample_cnt; i++)
    {
        if (frame_buf[i] < 0)
        {
            sample_value = 0 - frame_buf[i];
        }
        else
        {
            sample_value = frame_buf[i];
        }

        sample_total += sample_value;
        if (sample_value > sample_max)
        {
            sample_max = sample_value;
        }
    }

    *p_cur_power_max = (sample_max > 0x7fff) ? (0x7fff) : (sample_max);
    *p_cur_power_rms = sample_total / sample_cnt;
}
