/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙免提通话中间件回声消除算法初始化接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "speech_player_global.h"
#include "sysdef.h"

int32 start_speech_decoder_aec_init(speech_player_t *speech_player, actionsaec_prms_t *p_aec_prms)
{
    int32 ret = 0;
    sv_prms_t tmp_sv_prms;

    libc_memset(&tmp_sv_prms, 0x00, sizeof(sv_prms_t));
    libc_memcpy(&tmp_sv_prms, p_aec_prms, sizeof(actionsaec_prms_t));

    //AEC收敛速度，默认为0
    tmp_sv_prms.aec_prms.nlp_speed = 0;
    tmp_sv_prms.aec_prms.nlp_echolevel = 0;

    //双讲保留程度范围扩大，进一步降低保留程度的下限
    if (p_aec_prms->dt_threshold == 0)
    {
        tmp_sv_prms.aec_prms.dt_threshold = -50;
    }
    else if (p_aec_prms->dt_threshold == 1)
    {
        tmp_sv_prms.aec_prms.dt_threshold = -40;
    }
    else if (p_aec_prms->dt_threshold == 2)
    {
        tmp_sv_prms.aec_prms.dt_threshold = -30;
    }
    else if (p_aec_prms->dt_threshold == 3)
    {
        tmp_sv_prms.aec_prms.dt_threshold = -20;
    }
    else if (p_aec_prms->dt_threshold == 4)
    {
        tmp_sv_prms.aec_prms.dt_threshold = -10;
    }
    else if (p_aec_prms->dt_threshold == 5)
    {
        tmp_sv_prms.aec_prms.dt_threshold = 0;
    }
    else
    {
        tmp_sv_prms.aec_prms.dt_threshold = p_aec_prms->dt_threshold;
    }

    ret = hfp_speech_cmd(&(speech_player->aec_handle), HS_CMD_AEC_OPEN, (uint32) &tmp_sv_prms);
    if ((ret == -1) || (speech_player->aec_handle == NULL))
    {
        libc_printf_warning("aec open fail!!\n");
        ret = -1;
    }

    return ret;
}
