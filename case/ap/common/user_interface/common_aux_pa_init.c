/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：LINEIN 走AUX->PA直通通路，可以支持真正双声道。初始化阶段会有杂音，需要处理掉。
 * 作者：cailizhen
 ********************************************************************************/

#include  "common_ui_inner.h"
#include  "linein_common.h"

void com_aux_pa_init_inner(void)
{
#if (SUPPORT_AUX_AA_STEREO == 1)

    ain_setting_t ain_sett;
    bool is_spk_on_flag;

#if (SUPPORT_EXTERN_PA == 1)
    is_spk_on_flag = is_speaker_on_inner();
    if (is_spk_on_flag == TRUE)
    {
        com_speaker_off_do();
    }
#endif

    ain_sett.op_gain.ain_op_gain = com_get_config_default_inner(LINEIN_GAIN_INPUT);
    ain_sett.ain_pa_mode = TRUE;
    ain_sett.ain_input_mode = com_get_config_default(LINEIN_INPUT_PIN_USE);
    enable_ain(AIN_SOURCE_AUX, &ain_sett);

    //延时400MS, 避免出现较大杂音
    sys_os_time_dly(40);

    disable_ain(AIN_SOURCE_AUX);

#if (SUPPORT_EXTERN_PA == 1)
    if (is_spk_on_flag == TRUE)
    {
        com_speaker_on_do();
    }
#endif

#endif
}
