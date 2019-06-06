/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：FM收音机引擎应用对应中间件控制接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "app_fmengine.h"

#define ADC_FREQ_KHZ  44 //48
#define ADC_RREQ_HZ   44100 //48000
#define REC_BIT_RATE  176000 //192000

static void _print_mmm_cmd_error(uint32 cmd_id)
{
    libc_printf_warning("MMM_PP cmd[%d] fail!\n", cmd_id);
}

bool _play(void)
{
    char *mmm_name = { "mmm_pp.al" };
    int app_init_ret = 0;
    bool bret = TRUE;
    music_pcm_setting_t fm_music_setting;
    freq_point_config_t tmp_freq_point_cfg;

    if (pp_handle != NULL)
    {
        libc_printf_warning("_play return, pp_handle NOT NULL!\n");
        return TRUE;
    }

    //加载中间件驱动
    app_init_ret = (int32) sys_drv_install(MMM_GROUP, 0, mmm_name);
    if (app_init_ret == -1)
    {
        bret = FALSE;
        goto _fm_play_err_0;
    }

    //OPEN中间件
#if (SUPPORT_OK_MIC == 1)
    app_init_ret = mmm_pp_cmd(&pp_handle, MMM_PP_OPEN, PP_WORK_MODE_MIC);
#else
    app_init_ret = mmm_pp_cmd(&pp_handle, MMM_PP_OPEN, PP_WORK_MODE_NORMAL);
#endif
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_OPEN);
        pp_handle = NULL;
        bret = FALSE;
        goto _fm_play_err_1;
    }

    /*! PCM源选择，0表示AUX，1表示USB */
    fm_music_setting.pcm_source = PCM_SOURCE_AUX;
#if (SUPPORT_OK_MIC == 1)
    /*! 数据输入接口 */
    fm_music_setting.adc_chan_set.data_input_if = ADC_SOURCE_MIC_AUX;
    if ((uint8) com_get_config_default(OK_MIC_MICIN_SOURCE) == 0)
    {
        fm_music_setting.adc_chan_set.micin_input_mode = AIN_INPUT_MIC0_L;
    }
    else
    {
        fm_music_setting.adc_chan_set.micin_input_mode = AIN_INPUT_MIC1_R;
    }
    fm_music_setting.adc_chan_set.micin_op_gain = (uint8) com_get_config_default(OK_MIC_MICIN_OP_GAIN);
    fm_music_setting.adc_chan_set.micin_adc_gain = (uint8) com_get_config_default(OK_MIC_MICIN_ADC_GAIN);
#else
    /*! 数据输入接口 */
    fm_music_setting.adc_chan_set.data_input_if = ADC_SOURCE_AUX;
#endif
    /*! AIN输入模式 */
    fm_music_setting.adc_chan_set.ain_input_mode = (uint8) com_get_config_default(RADIO_INPUT_PIN_USE);
    /*! AIN OP Gain */
    fm_music_setting.adc_chan_set.ain_op_gain = (uint8) com_get_config_default(RADIO_GAIN_INPUT);
    /*! AIN ADC Gain */
    fm_music_setting.adc_chan_set.ain_adc_gain = (uint8) com_get_config_default(RADIO_GAIN_ADC);
    /*! ADC采样率 */
    fm_music_setting.adc_chan_set.default_samplerate = ADC_FREQ_KHZ;

    fm_music_setting.adc_chan_set.adc_lowpw_th_max = 0;/* 11=-70dB max */ /*为0表示不采用最大值判断*/
    fm_music_setting.adc_chan_set.adc_lowpw_th_rms = 3; /* 3=-77dB rms*/  /*为0表示不采用有效值判断*/

    /*! 默认采样率，单位：KHz，默认值44 */
    fm_music_setting.default_samplerate = ADC_FREQ_KHZ;
    /*! 默认通道数，默认值2 */
    fm_music_setting.default_channels = 2;

    /*! 音频输出通道，见 typestruct.h 中的定义 */
    fm_music_setting.aud_output_chan = com_get_config_default(SETTING_HARDWARE_AUDIO_OUTPUT_CHANNEL);

    /*! 淡入时间，单位毫秒 */
    fm_music_setting.fade_in_time = 50;
    /*! 淡出时间，单位毫秒 */
    fm_music_setting.fade_out_time = 50;

    app_init_ret = mmm_pp_cmd(pp_handle, MMM_PP_SET_MUSIC_SETTING, (unsigned int) &fm_music_setting);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_SET_MUSIC_SETTING);
        bret = FALSE;
        goto _fm_play_err_2;
    }

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
    mmm_pp_cmd(pp_handle, MMM_PP_SET_FREQ_POINT, (unsigned int) &tmp_freq_point_cfg);

    //发送播放命令
    app_init_ret = mmm_pp_cmd(pp_handle, MMM_PP_PLAY, (unsigned int) NULL);

    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_PLAY);
        bret = FALSE;
        goto _fm_play_err_2;
    }

    change_mmm_state(MMM_STATE_PLAYING);
    bret = TRUE;

    return bret;


    _fm_play_err_2:
    //CLOSE中间件
    mmm_pp_cmd(pp_handle, MMM_PP_CLOSE, (unsigned int) NULL);
    pp_handle = NULL;

    _fm_play_err_1:
    //卸载中间件驱动
    sys_drv_uninstall (MMM_GROUP);

    _fm_play_err_0:

    return bret;
}

bool _stop(void)
{
    bool bret = TRUE;
    int app_init_ret = 0;

    if (pp_handle == NULL)
    {
        libc_printf_warning("_stop return, pp_handle IS NULL!\n");
        return TRUE;
    }

    //停止播放
    app_init_ret = mmm_pp_cmd(pp_handle, MMM_PP_STOP, (unsigned int) NULL);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_STOP);
    }

    //CLOSE中间件
    app_init_ret = mmm_pp_cmd(pp_handle, MMM_PP_CLOSE, (unsigned int) NULL);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_CLOSE);
    }

    //卸载中间件驱动
    sys_drv_uninstall (MMM_GROUP);

    pp_handle = NULL;

    change_mmm_state(MMM_STATE_NO_PLAY);
    bret = TRUE;

    return bret;
}

bool _eq_set(void)
{
    int32 mmm_ret;

    if (pp_handle == NULL)
    {
        return FALSE;
    }

    mmm_ret = mmm_pp_cmd(pp_handle, MMM_PP_SET_EQ, 0);
    if (mmm_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_SET_EQ);
        return FALSE;
    }

    return TRUE;
}

bool _get_mmm_status(void)
{
    int32 mmm_ret;

    if (pp_handle == NULL)
    {
        return FALSE;
    }

    //获取中间件状态
    mmm_ret = mmm_pp_cmd(pp_handle, MMM_PP_GET_STATUS, (unsigned int) &g_pp_status);
    if (mmm_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_GET_STATUS);
        return FALSE;
    }

    return TRUE;
}

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1) && (SUPPORT_RADIO_RECORD_FUNC == 1))

bool _record_start(uint8 channel, uint8 record_source)
{
    int app_init_ret = 0;
    bool bret = TRUE;
    record_module_encode_para_t encode_para;
    record_module_fs_para_t fs_para;

    fs_para.fs_type = 0;
    fs_para.file_mount_id = g_record_param.file_sys_id;
    fs_para.file_handle = g_record_param.file_handle;
    fs_para.file_max_length = g_record_param.fs_free_space;

    //单次录音，录音文件最大不能超过512MB。
    if(fs_para.file_max_length>(1024*1024))
    {
        fs_para.file_max_length = 1024*1024;
    }

    mmm_pp_cmd(pp_handle, MMM_PP_RECORDING_SET_FS, &fs_para);

    encode_para.encode_mode = ENCODE_ADPCM;
    encode_para.bitrate = REC_BIT_RATE;
    encode_para.sample_rate_hz = ADC_RREQ_HZ;
    encode_para.channel = channel;
    encode_para.record_source = record_source;
    encode_para.bps = 16;

    mmm_pp_cmd(pp_handle, MMM_PP_RECORDING_SET_ENCODE, &encode_para);

    app_init_ret = mmm_pp_cmd(pp_handle, MMM_PP_RECORDING_START, NULL);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_RECORDING_START);
        bret = FALSE;
    }

    return bret;
}

bool _record_stop(void)
{
    int app_init_ret = 0;
    bool bret = TRUE;

    app_init_ret = mmm_pp_cmd(pp_handle, MMM_PP_RECORDING_STOP,  (unsigned int) NULL);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_RECORDING_STOP);
        bret = FALSE;
    }

    return bret;
}

bool _get_mmm_record_status(void)
{
    int32 mr_ret;

    if (pp_handle == NULL)
    {
        return FALSE;
    }

    mr_ret = mmm_pp_cmd(pp_handle, MMM_PP_RECORDING_GET_STATUS, (unsigned int) &g_rm_status);
    if (mr_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_RECORDING_GET_STATUS);
        return FALSE;
    }

    return TRUE;
}

#endif
