/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：按键音通道声音播放接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_func_inner.h"
#include "mmm_pp.h"

#if (SUPPORT_SOFT_KEYTONE == 1)
#define ADC_FREQ_KHZ  44
void *keytone_pp_handle;
mmm_pp_status_t keytone_pp_status;

static void _print_mmm_cmd_error(uint32 cmd_id)
{
    libc_printf_warning("MMM_PP cmd[%d] fail!\n", cmd_id);
}

bool com_keytone_play_inner(void *kt_para)
{
    char *mmm_name = { "mmm_pp.al" };
    int app_init_ret = 0;
    bool bret = TRUE;
    music_pcm_setting_t linein_music_setting;

    com_speaker_on_inner(SPK_SYS);

    //加载中间件驱动
    app_init_ret = (int32) sys_drv_install(MMM_GROUP, 0, mmm_name);
    if (app_init_ret == -1)
    {
        bret = FALSE;
        goto _keytone_play_err_0;
    }

    //OPEN中间件
    app_init_ret = mmm_pp_cmd(&keytone_pp_handle, MMM_PP_OPEN, PP_WORK_KEYTONE);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_OPEN);
        keytone_pp_handle = NULL;
        bret = FALSE;
        goto _keytone_play_err_1;
    }

    /*! PCM源选择，0表示AUX，1表示USB */
    linein_music_setting.pcm_source = PCM_SOURCE_AUX;
    /*! 数据输入接口 */
    linein_music_setting.adc_chan_set.data_input_if = ADC_SOURCE_AUX;
    /*! 采样率，ADC/I2SRX/MIC等 */
    linein_music_setting.adc_chan_set.default_samplerate = ADC_FREQ_KHZ;

    /*! AIN输入模式，0表示左右声道混合，1表示左声道输入，2表示右声道输入 */
    linein_music_setting.adc_chan_set.ain_input_mode = (uint8) com_get_config_default(LINEIN_INPUT_PIN_USE);
    /*! AIN OP Gain */
    linein_music_setting.adc_chan_set.ain_op_gain = (uint8) com_get_config_default(LINEIN_GAIN_INPUT);
    /*! AIN ADC Gain */
    linein_music_setting.adc_chan_set.ain_adc_gain = (uint8) com_get_config_default(LINEIN_GAIN_ADC);

    /*! LINEIN低能量检测阈值，持续一段时间可以认为AUX播放源停止，可以省电关机等；这两个参数需要同时满足才算低能量 */
    linein_music_setting.adc_chan_set.adc_lowpw_th_max = 0;/* 11=-70dB max */ /*为0表示不采用最大值判断*/
    linein_music_setting.adc_chan_set.adc_lowpw_th_rms = 5; /* 5=-72dB rms*/  /*为0表示不采用有效值判断*/

    /*! 默认采样率，单位：KHz，默认值44 */
    linein_music_setting.default_samplerate = ADC_FREQ_KHZ;
    /*! 默认通道数，默认值2 */
    linein_music_setting.default_channels = 2;

    /*! 音频输出通道，见 typestruct.h 中的定义 */
    linein_music_setting.aud_output_chan = com_get_config_default(SETTING_HARDWARE_AUDIO_OUTPUT_CHANNEL);

    /*! 淡入时间，单位毫秒 */
    linein_music_setting.fade_in_time = 3;
    /*! 淡出时间，单位毫秒 */
    linein_music_setting.fade_out_time = 3;

    app_init_ret = mmm_pp_cmd(keytone_pp_handle, MMM_PP_SET_MUSIC_SETTING, (unsigned int) &linein_music_setting);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_SET_MUSIC_SETTING);
        bret = FALSE;
        goto _keytone_play_err_2;
    }

    //发送播放命令
    app_init_ret = mmm_pp_cmd(keytone_pp_handle, MMM_PP_PLAY, (unsigned int) kt_para);

    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_PLAY);
        bret = FALSE;
        goto _keytone_play_err_2;
    }
    
    change_mmm_state_inner(MMM_STATE_KT_PLAYING);
    bret = TRUE;

    return bret;

    _keytone_play_err_2:
    //CLOSE中间件
    mmm_pp_cmd(keytone_pp_handle, MMM_PP_CLOSE, (unsigned int) NULL);
    keytone_pp_handle = NULL;

    _keytone_play_err_1:
    //卸载中间件驱动
    sys_drv_uninstall (MMM_GROUP);

    _keytone_play_err_0:

    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _stop(void)
 * \停止播放音乐
 * \param[in]    stop_mode 停止模式 para1
 * \param[in]    stop_mode_e:
 *                           \STOP_NORMAL 正常停止
 *                           \STOP_PAUSE  暂停播放
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_play_deal.c
 * \note
 */
/*******************************************************************************/
bool com_keytone_stop_inner(void)
{
    bool bret = TRUE;
    int app_init_ret = 0;
    bret = TRUE;

    //停止播放
    app_init_ret = mmm_pp_cmd(keytone_pp_handle, MMM_PP_STOP, (unsigned int) NULL);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_STOP);
    }

    //CLOSE中间件
    app_init_ret = mmm_pp_cmd(keytone_pp_handle, MMM_PP_CLOSE, (unsigned int) NULL);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_CLOSE);
    }

    //卸载中间件驱动
    sys_drv_uninstall (MMM_GROUP);

    keytone_pp_handle = NULL;

    com_speaker_off_inner(SPK_SYS);

    change_mmm_state_inner(MMM_STATE_NO_PLAY);
    bret = TRUE;

    return bret;
}

void com_keytone_check_status(void)
{
    mmm_pp_status_t mmm_status;

    if (keytone_pp_handle != NULL)
    {
        //获取当前播放状态
        if (mmm_pp_cmd(keytone_pp_handle, MMM_PP_GET_STATUS, (unsigned int) &mmm_status) == 0)
        {
            change_ktp_status_inner(mmm_status.ktp_status);
        }
        else
        {
            _print_mmm_cmd_error(MMM_PP_GET_STATUS);
            change_ktp_status_inner(MMM_KTP_ENGINE_NULL);
        }
    }
}

bool com_keytone_set_dae(void)
{
    if (keytone_pp_handle != NULL)
    {
        //获取当前播放状态
        if (mmm_pp_cmd(keytone_pp_handle, MMM_PP_SET_EQ, 0) < 0)
        {
            _print_mmm_cmd_error(MMM_PP_SET_EQ);
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }

    return FALSE;
}

#endif
