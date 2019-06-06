/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙免提通话中间件AGC算法初始化接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "speech_player_global.h"
#include "sysdef.h"

int32 start_speech_decoder_agc_init(speech_player_t *speech_player, agc_para_t *p_agc_para)
{
    int32 ret = 0;
    AGCSetting agc_setting;

    agc_setting.sample_rate = 8000;
    agc_setting.channels = 1;

    agc_setting.noise_gate_enable = 1;
    agc_setting.noise_gate_type = 0;

    agc_setting.hold_time = 1000;
    agc_setting.decay_time = 50;
    agc_setting.attack_time = 240;

    agc_setting.agc_mode = p_agc_para->agc_mode;
    agc_setting.minimum_gain = p_agc_para->minimum_gain;
    agc_setting.maximum_gain = p_agc_para->maximum_gain;
    agc_setting.agc_level = p_agc_para->agc_level;
    agc_setting.noise_gate_threshold = p_agc_para->noise_gate_threshold;

    if (p_agc_para == speech_player->g_p_speech_pre_agc_para)
    {
        ret = hfp_speech_cmd(&(speech_player->agc_handle_pre), HS_CMD_AGC_PRE_OPEN, (uint32) &agc_setting);
        if ((ret == -1) || (speech_player->agc_handle_pre == NULL))
        {
            libc_printf_warning("pre agc open fail!!\n");
            ret = -1;
        }
    }
    else
    {
        ret = hfp_speech_cmd(&(speech_player->agc_handle_post), HS_CMD_AGC_POST_OPEN, (uint32) &agc_setting);
        if ((ret == -1) || (speech_player->agc_handle_post == NULL))
        {
            libc_printf_warning("post agc open fail!!\n");
            ret = -1;
        }
    }

    return ret;
}

