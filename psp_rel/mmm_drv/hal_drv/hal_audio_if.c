/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：HAL驱动——音频接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "hal_inner.h"

#define DAC_VOL_MIN_VOL    -600   //最小有效音量 -60db
//0db(vol_p1_db = 0) ~ -60db(vol_p1_db = -600)
//vol_p1_db < -600 for mute
//return: >= 0 ok, < 0 fail
int32 audio_set_audio_volume(int32 vol_p1_db)
{
    int32 result;

    if (vol_p1_db > 30) //+3db
    {
        return -1;
    }

    if (vol_p1_db < DAC_VOL_MIN_VOL)
    {
        result = 0;
    }
    else
    {
        result = libc_math_exp_fixed(vol_p1_db);
    }

    return hal_audio_set_audio_volume(result);
}
