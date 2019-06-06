/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：OK MIC空闲场景播放器实现。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_func_inner.h"
#include "mmm_pp.h"

#if (SUPPORT_OK_MIC == 1)

#define ADC_FREQ_KHZ  44 //录音也固定为该采样率，如果修改要同步修改录音采样率

void *ok_mic_pp_handle;
mmm_pp_status_t ok_mic_pp_status;

static void _print_mmm_cmd_error(uint32 cmd_id)
{
    libc_printf_warning("MMM_PP cmd[%d] fail!\n", cmd_id);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _play(void)
 * \开始播放音乐
 * \param[in]    play_mode 播放模式 para1
 * \param[in]    play_mode_e:
 *                           \PLAY_NORMAL 正常开始播放
 *                           \PLAY_RESUME 断点续播
 *                           \PLAY_FFON   快进播放
 *                           \PLAY_FBON   快退播放
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_play_deal.c
 * \note
 * \li   注意如果文件SET_FILE失败，这里并不直接关闭句柄,而是先通过状态检测获取错误
 号，然后再关闭句柄。如果直接调用CLEAR_FILE命令，会清除错误号
 */
/*******************************************************************************/
bool com_ok_mic_idle_play_inner(void)
{
    char *mmm_name = { "mmm_pp.al" };
    int app_init_ret = 0;
    bool bret = TRUE;
    music_pcm_setting_t linein_music_setting;
    freq_point_config_t tmp_freq_point_cfg;

    if (ok_mic_pp_handle != NULL)
    {
        libc_printf_warning("_play return, ok_mic_pp_handle NOT NULL!\n");
        return TRUE;
    }

    //加载中间件驱动
    app_init_ret = (int32) sys_drv_install(MMM_GROUP, 0, mmm_name);
    if (app_init_ret == -1)
    {
        bret = FALSE;
        goto _linein_play_err_0;
    }

    //OPEN中间件
    app_init_ret = mmm_pp_cmd(&ok_mic_pp_handle, MMM_PP_OPEN, PP_WORK_MODE_MIC);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_OPEN);
        ok_mic_pp_handle = NULL;
        bret = FALSE;
        goto _linein_play_err_1;
    }

    /*! PCM源选择，0表示AUX，1表示USB */
    linein_music_setting.pcm_source = PCM_SOURCE_AUX;
    /*! 数据输入接口 */
    linein_music_setting.adc_chan_set.data_input_if = ADC_SOURCE_MIC_AUX;
    if ((uint8) com_get_config_default(OK_MIC_MICIN_SOURCE) == 0)
    {
        linein_music_setting.adc_chan_set.micin_input_mode = AIN_INPUT_MIC0_L;
    }
    else
    {
        linein_music_setting.adc_chan_set.micin_input_mode = AIN_INPUT_MIC1_R;
    }
    linein_music_setting.adc_chan_set.micin_op_gain = (uint8) com_get_config_default(OK_MIC_MICIN_OP_GAIN);
    linein_music_setting.adc_chan_set.micin_adc_gain = (uint8) com_get_config_default(OK_MIC_MICIN_ADC_GAIN);

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
    linein_music_setting.fade_in_time = 10;
    /*! 淡出时间，单位毫秒 */
    linein_music_setting.fade_out_time = 10;

    app_init_ret = mmm_pp_cmd(ok_mic_pp_handle, MMM_PP_SET_MUSIC_SETTING, (unsigned int) &linein_music_setting);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_SET_MUSIC_SETTING);
        bret = FALSE;
        goto _linein_play_err_2;
    }

    mmm_pp_cmd(ok_mic_pp_handle, MMM_PP_SET_IDLE_MODE, TRUE);

    //获取多频点能量值参数
    tmp_freq_point_cfg.freq_point_enable = DEF_FREQ_POINT_ENABLE;
    tmp_freq_point_cfg.freq_point_num = DEF_FREQ_POINT_NUM;
    tmp_freq_point_cfg.freq_point[0] = DEF_FREQ_POINT_0;
    tmp_freq_point_cfg.freq_point[1] = DEF_FREQ_POINT_1;
    tmp_freq_point_cfg.freq_point[2] = DEF_FREQ_POINT_2;
    tmp_freq_point_cfg.freq_point[3] = DEF_FREQ_POINT_3;
    tmp_freq_point_cfg.freq_point[4] = DEF_FREQ_POINT_4;
    tmp_freq_point_cfg.freq_point[5] = DEF_FREQ_POINT_5;
    tmp_freq_point_cfg.freq_point[6] = DEF_FREQ_POINT_6;
    tmp_freq_point_cfg.freq_point[7] = DEF_FREQ_POINT_7;
    tmp_freq_point_cfg.freq_point[8] = DEF_FREQ_POINT_8;
    tmp_freq_point_cfg.freq_point[9] = DEF_FREQ_POINT_9;
    mmm_mp_cmd(ok_mic_pp_handle, MMM_PP_SET_FREQ_POINT, (unsigned int) &tmp_freq_point_cfg);

    //发送播放命令
    app_init_ret = mmm_pp_cmd(ok_mic_pp_handle, MMM_PP_PLAY, (unsigned int) NULL);

    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_PLAY);
        bret = FALSE;
        goto _linein_play_err_2;
    }

    change_mmm_state_inner(MMM_STATE_KT_PLAYING);
    bret = TRUE;

    return bret;


    _linein_play_err_2:
    //CLOSE中间件
    mmm_pp_cmd(ok_mic_pp_handle, MMM_PP_CLOSE, (unsigned int) NULL);
    ok_mic_pp_handle = NULL;

    _linein_play_err_1:
    //卸载中间件驱动
    sys_drv_uninstall (MMM_GROUP);

    _linein_play_err_0:

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
bool com_ok_mic_idle_stop_inner(void)
{
    bool bret = TRUE;
    int app_init_ret = 0;

    if (ok_mic_pp_handle == NULL)
    {
        libc_printf_warning("_stop return, ok_mic_pp_handle IS NULL!\n");
        return TRUE;
    }

    //停止播放
    app_init_ret = mmm_pp_cmd(ok_mic_pp_handle, MMM_PP_STOP, (unsigned int) NULL);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_STOP);
    }

    //CLOSE中间件
    app_init_ret = mmm_pp_cmd(ok_mic_pp_handle, MMM_PP_CLOSE, (unsigned int) NULL);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_CLOSE);
    }

    //卸载中间件驱动
    sys_drv_uninstall (MMM_GROUP);

    ok_mic_pp_handle = NULL;

    change_mmm_state_inner(MMM_STATE_NO_PLAY);
    bret = TRUE;

    return bret;
}

bool com_ok_mic_idle_eq_set(void)
{
    int32 mmm_ret;

    if (ok_mic_pp_handle == NULL)
    {
        return FALSE;
    }

    mmm_ret = mmm_pp_cmd(ok_mic_pp_handle, MMM_PP_SET_EQ, 0);
    if (mmm_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_SET_EQ);
        return FALSE;
    }

    return TRUE;
}

bool com_ok_mic_idle_get_mmm_status(void)
{
    int32 mmm_ret;

    if (ok_mic_pp_handle == NULL)
    {
        return FALSE;
    }

    //获取中间件状态
    mmm_ret = mmm_pp_cmd(ok_mic_pp_handle, MMM_PP_GET_STATUS, (unsigned int) &ok_mic_pp_status);
    if (mmm_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_GET_STATUS);
        return FALSE;
    }

    return TRUE;
}

//#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1))
#if (0)

bool com_ok_mic_idle_record_start_inner(void)
{
    int app_init_ret = 0;
    bool bret = TRUE;
    record_module_encode_para_t encode_para;
    record_module_fs_para_t fs_para;

    //初始化磁盘并创建文件

    fs_para.fs_type = 0;
    fs_para.file_mount_id = g_record_param.file_sys_id;
    fs_para.file_handle = g_record_param.file_handle;
    fs_para.file_max_length = g_record_param.fs_free_space;

	//单次录音，录音文件最大不能超过512MB。
	if(fs_para.file_max_length>(1024*1024))
	{
		fs_para.file_max_length = 1024*1024;
	}

    mmm_pp_cmd(ok_mic_pp_handle, MMM_PP_RECORDING_SET_FS, &fs_para);

    encode_para.encode_mode = ENCODE_ADPCM;
    encode_para.bitrate = 176000;
    encode_para.sample_rate_hz = 44100;
    encode_para.channel = 1;
    encode_para.record_source = RECORD_SOURCE_L_CH;
    encode_para.bps = 16;

    mmm_pp_cmd(ok_mic_pp_handle, MMM_PP_RECORDING_SET_ENCODE, &encode_para);

    app_init_ret = mmm_pp_cmd(ok_mic_pp_handle, MMM_PP_RECORDING_START, NULL);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_RECORDING_START);
        bret = FALSE;
    }

    return bret;
}

bool com_ok_mic_idle_record_stop_inner(void)
{
    int app_init_ret = 0;
    bool bret = TRUE;

    app_init_ret = mmm_pp_cmd(ok_mic_pp_handle, MMM_PP_RECORDING_STOP,  (unsigned int) NULL);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_RECORDING_STOP);
        bret = FALSE;
    }

    //保存文件并卸载磁盘

    return bret;
}

bool com_ok_mic_idle_get_mmm_record_status_inner(void)
{
    int32 mr_ret;

    if (ok_mic_pp_handle == NULL)
    {
        return FALSE;
    }

    mr_ret = mmm_pp_cmd(ok_mic_pp_handle, MMM_PP_RECORDING_GET_STATUS, (unsigned int) &g_rm_status);
    if (mr_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_RECORDING_GET_STATUS);
        return FALSE;
    }

    //根据状态更新UI

    return TRUE;
}

#endif


#endif
