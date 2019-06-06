/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙推歌引擎应用中间件控制接口。
 * 作者：cailizhen
 ********************************************************************************/

#include  "app_btplay_engine.h"

static void _print_mmm_cmd_error(uint32 cmd_id)
{
    libc_printf_warning("MMM_BP cmd[%d] fail!\n", cmd_id);
}

bool _play(void)
{
    char *mmm_name =
    { "mmm_bp.al" };
    int32 app_init_ret;
    bool ret = TRUE;
    music_bt_setting_t bt_sett;
    freq_point_config_t tmp_freq_point_cfg;

    if (bp_handle != NULL)
    {
        libc_printf_warning("_play return, bp_handle NOT NULL!\n");
        return TRUE;
    }

    //加载中间件驱动
    app_init_ret = (int32) sys_drv_install(MMM_GROUP, 0, mmm_name);
    if (app_init_ret == -1)
    {
        ret = FALSE;
        goto _bt_play_err_0;
    }

    //OPEN中间件
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (g_bt_tws_enable == TRUE)
    {
        app_init_ret = mmm_bp_cmd(&bp_handle, MMM_BP_OPEN, BP_WORK_MODE_TWS);
    }
    else
#endif
    {
#if (SUPPORT_OK_MIC == 1)
        app_init_ret = mmm_bp_cmd(&bp_handle, MMM_BP_OPEN, BP_WORK_MODE_MIC);
#else
        app_init_ret = mmm_bp_cmd(&bp_handle, MMM_BP_OPEN, BP_WORK_MODE_NORMAL);
#endif
    }

    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_BP_OPEN);
        bp_handle = NULL;
        ret = FALSE;
        goto _bt_play_err_1;
    }

    //告诉蓝牙引擎蓝牙推歌已开始播放
    com_btengine_set_sbc_play(TRUE);

    //设置播放参数
    if (g_btplay_data_source == SOURCE_BT_A2DP)
    {
        bt_sett.data_source = MMM_SOURCE_AVDTP_STREAM;

#if (SUPPORT_OK_MIC == 1)
        /*! 数据输入接口 */
        bt_sett.adc_chan_set.data_input_if = ADC_SOURCE_MIC;
        /*! AIN输入模式，0表示左右声道混合，1表示左声道输入，2表示右声道输入 */
        bt_sett.adc_chan_set.ain_input_mode = (uint8) com_get_config_default(LINEIN_INPUT_PIN_USE);
        if ((uint8) com_get_config_default(OK_MIC_MICIN_SOURCE) == 0)
        {
            bt_sett.adc_chan_set.micin_input_mode = AIN_INPUT_MIC0_L;
        }
        else
        {
            bt_sett.adc_chan_set.micin_input_mode = AIN_INPUT_MIC1_R;
        }
        bt_sett.adc_chan_set.micin_op_gain = (uint8) com_get_config_default(OK_MIC_MICIN_OP_GAIN);
        bt_sett.adc_chan_set.micin_adc_gain = (uint8) com_get_config_default(OK_MIC_MICIN_ADC_GAIN);
        /*! ADC采样率 */
        bt_sett.adc_chan_set.default_samplerate = 44;

        /*! LINEIN低能量检测阈值，持续一段时间可以认为AUX播放源停止，可以省电关机等；这两个参数需要同时满足才算低能量 */
        bt_sett.adc_chan_set.adc_lowpw_th_max = 0;/* 11=-70dB max */ /*为0表示不采用最大值判断*/
        bt_sett.adc_chan_set.adc_lowpw_th_rms = 5; /* 5=-72dB rms*/  /*为0表示不采用有效值判断*/
#endif
    }
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    else
    {
        f_send_frame p_f_send_frame;
        //从bt engine获取Send frame回调函数
        com_btengine_tws_get_send_frame_cbk(&p_f_send_frame);
        //设置Send frame回调函数
        app_init_ret = mmm_bp_cmd(bp_handle, MMM_BP_SET_SEND_FRAME_CBK, (unsigned int) p_f_send_frame);
        if (app_init_ret < 0)
        {
            _print_mmm_cmd_error(MMM_BP_SET_SEND_FRAME_CBK);
            ret = FALSE;
            goto _bt_play_err_2;
        }

        bt_sett.data_source = MMM_SOURCE_ADC_CHAN;
        /*! 数据输入接口 */
        if (g_btplay_data_sub_source == SUB_SOURCE_SPDIF)
        {
            bt_sett.adc_chan_set.data_input_if = ADC_SOURCE_SPDIFRX;
            set_pad_function(SPDIF_RX_PIN, SPDIF_RX_FUNC, NULL, 0);
        }
        else if (g_btplay_data_sub_source == SUB_SOURCE_I2SRX)
        {
            bt_sett.adc_chan_set.data_input_if = ADC_SOURCE_I2SRX;

            bt_sett.adc_chan_set.i2srx_work_mode = I2SRX_WORK_MODE;
            bt_sett.adc_chan_set.i2srx_slave_mclk_use_inner = I2SRX_SLAVE_MCLK_USE_INNER;
            bt_sett.adc_chan_set.i2srx_width = I2SRX_EFF_WIDTH;
            bt_sett.adc_chan_set.i2srx_data_mode = I2SRX_DATA_MODE;

            //I2S RX PIN config
            set_pad_function(I2SRX_BCLK_PIN, I2SRX_BCLK_FUNC, NULL, 0);
            set_pad_function(I2SRX_LRCLK_PIN, I2SRX_LRCLK_FUNC, NULL, 0);
#if ((I2SRX_WORK_MODE == I2SRX_WORK_SLAVE) && (I2SRX_SLAVE_MCLK_USE_INNER == 1))
            set_pad_function(I2SRX_MCLK_PIN, I2SRX_MCLK_FUNC, NULL, 0);
#endif
            set_pad_function(I2SRX_DI_PIN, I2SRX_DI_FUNC, NULL, 0);
        }
        else if (g_btplay_data_sub_source == SUB_SOURCE_HDMI)
        {
            bt_sett.adc_chan_set.data_input_if = ADC_SOURCE_SPDIFRX;
            set_pad_function(SPDIF_RX_HDMI_PIN, SPDIF_RX_HDMI_FUNC, NULL, 0);
        }
        else
        {
            bt_sett.adc_chan_set.data_input_if = ADC_SOURCE_AUX;
            /*! AIN输入模式，0表示左右声道混合，1表示左声道输入，2表示右声道输入 */
            bt_sett.adc_chan_set.ain_input_mode = (uint8) com_get_config_default(LINEIN_INPUT_PIN_USE);
            /*! AIN OP Gain */
            bt_sett.adc_chan_set.ain_op_gain = (uint8) com_get_config_default(LINEIN_GAIN_INPUT);
            /*! AIN ADC Gain */
            bt_sett.adc_chan_set.ain_adc_gain = (uint8) com_get_config_default(LINEIN_GAIN_ADC);
        }
        /*! ADC采样率 */
        bt_sett.adc_chan_set.default_samplerate = 44;

        /*! LINEIN低能量检测阈值，持续一段时间可以认为AUX播放源停止，可以省电关机等；这两个参数需要同时满足才算低能量 */
        bt_sett.adc_chan_set.adc_lowpw_th_max = 0;/* 11=-70dB max */ /*为0表示不采用最大值判断*/
        bt_sett.adc_chan_set.adc_lowpw_th_rms = 5; /* 5=-72dB rms*/  /*为0表示不采用有效值判断*/
    }
#endif
    bt_sett.default_samplerate = 44;
    bt_sett.default_channels = 2;
    bt_sett.aud_output_chan = com_get_config_default(SETTING_HARDWARE_AUDIO_OUTPUT_CHANNEL);
    bt_sett.fade_in_time = 40;
    bt_sett.fade_out_time = 40;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if ((get_tws_mode() != TWS_MODE_SINGLE) && (get_tws_role() != NORMAL_DEV))
    {
        if (get_tws_mode() == TWS_MODE_BT_TWS)
        {
            bt_sett.start_delay_time = com_get_config_default(BTPLAY_SBC_PLAY_DELAY_START_TWS);
        }
        else
        {
            bt_sett.start_delay_time = com_get_config_default(BTPLAY_LINEIN_PLAY_DELAY_START_TWS);
        }
    }
    else
#endif
    {
        bt_sett.start_delay_time = com_get_config_default(BTPLAY_SBC_PLAY_DELAY_START);
    }

    bt_sett.bt_lowpw_th_max = 0;/* 11=-70dB max */ /*为0表示不采用最大值判断*/
    bt_sett.bt_lowpw_th_rms = 3; /* 3=-77dB rms*/  /*为0表示不采用有效值判断*/
    app_init_ret = mmm_bp_cmd(bp_handle, MMM_BP_SET_MUSIC_SETTING, (unsigned int) &bt_sett);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_BP_SET_MUSIC_SETTING);
        ret = FALSE;
        goto _bt_play_err_2;
    }

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (get_tws_mode() != TWS_MODE_SINGLE)
    {
        app_init_ret = btplay_eg_tws_play_setting();
        if (app_init_ret < 0)
        {
            ret = FALSE;
            goto _bt_play_err_2;
        }
    }
#endif

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
    mmm_bp_cmd(bp_handle, MMM_BP_SET_FREQ_POINT, (unsigned int) &tmp_freq_point_cfg);

    //启动播放
    app_init_ret = mmm_bp_cmd(bp_handle, MMM_BP_PLAY, (unsigned int) NULL);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_BP_PLAY);
        ret = FALSE;
        goto _bt_play_err_2;
    }

    change_mmm_state(MMM_STATE_PLAYING);
    return ret;

    _bt_play_err_2:
    //CLOSE中间件
    mmm_bp_cmd(bp_handle, MMM_BP_CLOSE, (unsigned int) NULL);
    bp_handle = NULL;

    _bt_play_err_1:
    //卸载中间件驱动
    sys_drv_uninstall (MMM_GROUP);

    _bt_play_err_0:

    return ret;
}

bool _stop(bool filter_flag)
{
    int32 app_init_ret;
    bool ret = TRUE;

    if (bp_handle == NULL)
    {
        libc_printf_warning("_stop return, bp_handle IS NULL!\n");
        return TRUE;
    }

    app_init_ret = mmm_bp_cmd(bp_handle, MMM_BP_STOP, (unsigned int) NULL);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_BP_STOP);
    }

    if (filter_flag == TRUE)
    {
        //清除和过滤AVDTP数据
        com_btengine_set_play_pipe_filter(PIPE_OP_FILTER | PIPE_OP_FLUSH);
    }
    else
    {
        //仅清除AVDTP数据
        com_btengine_set_play_pipe_filter(PIPE_OP_FLUSH);
    }

    //告诉蓝牙引擎蓝牙推歌已停止
    com_btengine_set_sbc_play(FALSE);

    //CLOSE中间件
    app_init_ret = mmm_bp_cmd(bp_handle, MMM_BP_CLOSE, (unsigned int) NULL);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_BP_CLOSE);
    }

    //卸载中间件驱动
    sys_drv_uninstall (MMM_GROUP);

    bp_handle = NULL;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (g_btplay_data_sub_source == SUB_SOURCE_SPDIF)
    {
        set_pad_function(SPDIF_RX_PIN, PAD_FUNC_GPIOIN, NULL, 0);
    }
    else if (g_btplay_data_sub_source == SUB_SOURCE_HDMI)
    {
        set_pad_function(SPDIF_RX_HDMI_PIN, PAD_FUNC_GPIOIN, NULL, 0);
    }
#endif

    change_mmm_state(MMM_STATE_NO_PLAY);
    return ret;
}

//更新EQ参数
bool _eq_set(void)
{
    int32 mmm_ret;

    if (bp_handle == NULL)
    {
        return FALSE;
    }

    mmm_ret = mmm_bp_cmd(bp_handle, MMM_BP_SET_EQ, 0);
    if (mmm_ret < 0)
    {
        _print_mmm_cmd_error(MMM_BP_SET_EQ);
        return FALSE;
    }

    return TRUE;
}

bool _get_mmm_status(void)
{
    int32 mmm_ret;

    if (bp_handle == NULL)
    {
        return FALSE;
    }

    //获取中间件状态
    mmm_ret = mmm_bp_cmd(bp_handle, MMM_BP_GET_STATUS, (unsigned int) &bp_status);
    if (mmm_ret < 0)
    {
        _print_mmm_cmd_error(MMM_BP_GET_STATUS);
        return FALSE;
    }

    return TRUE;
}

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_BT_RECORD_FUNC == 1) && (SUPPORT_BTPLAY_RECORD_FUNC == 1))

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

    mmm_bp_cmd(bp_handle, MMM_BP_RECORDING_SET_FS, &fs_para);

    encode_para.encode_mode = ENCODE_ADPCM;
    encode_para.bitrate = 176000;
    encode_para.sample_rate_hz = 44100; //default, 在解码开始后跟随蓝牙推歌音源采样率
    encode_para.channel = channel;
    encode_para.record_source = record_source;
    encode_para.bps = 16;

    mmm_bp_cmd(bp_handle, MMM_BP_RECORDING_SET_ENCODE, &encode_para);

    app_init_ret = mmm_bp_cmd(bp_handle, MMM_BP_RECORDING_START, NULL);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_BP_RECORDING_START);
        bret = FALSE;
    }

    return bret;
}

bool _record_stop(void)
{
    int app_init_ret = 0;
    bool bret = TRUE;

    app_init_ret = mmm_bp_cmd(bp_handle, MMM_BP_RECORDING_STOP,  (unsigned int) NULL);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_BP_RECORDING_STOP);
        bret = FALSE;
    }

    return bret;
}

bool _get_mmm_record_status(void)
{
    int32 mr_ret;

    if (bp_handle == NULL)
    {
        return FALSE;
    }

    mr_ret = mmm_bp_cmd(bp_handle, MMM_BP_RECORDING_GET_STATUS, (unsigned int) &g_rm_status);
    if (mr_ret < 0)
    {
        _print_mmm_cmd_error(MMM_BP_RECORDING_GET_STATUS);
        return FALSE;
    }

    return TRUE;
}

#endif
