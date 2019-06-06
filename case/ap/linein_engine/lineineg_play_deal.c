/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������Ƶ��������Ӧ�ö�Ӧ�м�����ƽӿڡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "app_lineineg.h"

#define ADC_FREQ_KHZ  44

static const uint8 rec_sample_rate_khz[RATE_MAX] =
{ 8, 11, 12, 16, 22, 24, 32, 44, 48};

#if (SUPPORT_AUX_AA_STEREO == 1)
bool _play_aa(void)
{
    ain_setting_t ain_sett;

    ain_sett.op_gain.ain_op_gain = com_get_config_default(LINEIN_GAIN_INPUT);
    ain_sett.ain_pa_mode = TRUE;
    ain_sett.ain_input_mode = com_get_config_default(LINEIN_INPUT_PIN_USE);
    enable_ain(AIN_SOURCE_AUX, &ain_sett);

    return TRUE;
}

bool _stop_aa(void)
{
    disable_ain(AIN_SOURCE_AUX);

    return TRUE;
}
#endif

static void _print_mmm_cmd_error(uint32 cmd_id)
{
    libc_printf_warning("MMM_PP cmd[%d] fail!\n", cmd_id);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _play(void)
 * \��ʼ��������
 * \param[in]    play_mode ����ģʽ para1
 * \param[in]    play_mode_e:
 *                           \PLAY_NORMAL ������ʼ����
 *                           \PLAY_RESUME �ϵ�����
 *                           \PLAY_FFON   �������
 *                           \PLAY_FBON   ���˲���
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_play_deal.c
 * \note
 * \li   ע������ļ�SET_FILEʧ�ܣ����ﲢ��ֱ�ӹرվ��,������ͨ��״̬����ȡ����
 �ţ�Ȼ���ٹرվ�������ֱ�ӵ���CLEAR_FILE�������������
 */
/*******************************************************************************/
bool _play(void)
{
    char *mmm_name = { "mmm_pp.al" };
    int app_init_ret = 0;
    bool bret = TRUE;
    music_pcm_setting_t linein_music_setting;
    freq_point_config_t tmp_freq_point_cfg;
    uint8 adc_samplerate = ADC_FREQ_KHZ;

#if (SUPPORT_AUX_AA_STEREO == 1)
    if ((uint8) com_get_config_default(LINEIN_CHANNEL_SEL) == AUX_MODE_AA_STEREO)
    {
        _play_aa();
        return TRUE;
    }
#endif

    if (pp_handle != NULL)
    {
        libc_printf_warning("_play return, pp_handle NOT NULL!\n");
        return TRUE;
    }

    //�����м������
    app_init_ret = (int32) sys_drv_install(MMM_GROUP, 0, mmm_name);
    if (app_init_ret == -1)
    {
        bret = FALSE;
        goto _linein_play_err_0;
    }

    //OPEN�м��
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
        goto _linein_play_err_1;
    }

    /*! PCMԴѡ��0��ʾAUX��1��ʾUSB */
    linein_music_setting.pcm_source = PCM_SOURCE_AUX;
    /*! ��������ӿ� */
    if (g_linein_play_chan == ENTER_AUX_CHAN)
    {
#if (SUPPORT_OK_MIC == 1)
        /*! ��������ӿ� */
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
#else
        linein_music_setting.adc_chan_set.data_input_if = ADC_SOURCE_AUX;
#endif
    }
    else if ((g_linein_play_chan == ENTER_MIC_CHAN_U) || (g_linein_play_chan == ENTER_MIC_CHAN_C))
    {
        linein_music_setting.adc_chan_set.data_input_if = ADC_SOURCE_MIC;
        linein_music_setting.adc_chan_set.micin_input_mode = AIN_INPUT_MIC0_L;
        linein_music_setting.adc_chan_set.micin_op_gain = (uint8) com_get_config_default(RECORD_GAIN_MIC_INPUT);
        linein_music_setting.adc_chan_set.micin_adc_gain = (uint8) com_get_config_default(RECORD_GAIN_MIC_ADC);
    }
    else if (g_linein_play_chan == ENTER_I2SRX_CHAN)
    {
        linein_music_setting.adc_chan_set.data_input_if = ADC_SOURCE_I2SRX;

        linein_music_setting.adc_chan_set.i2srx_work_mode = I2SRX_WORK_MODE;
        linein_music_setting.adc_chan_set.i2srx_slave_mclk_use_inner = I2SRX_SLAVE_MCLK_USE_INNER;
        linein_music_setting.adc_chan_set.i2srx_width = I2SRX_EFF_WIDTH;
        linein_music_setting.adc_chan_set.i2srx_data_mode = I2SRX_DATA_MODE;

        //I2S RX PIN config
        set_pad_function(I2SRX_BCLK_PIN, I2SRX_BCLK_FUNC, NULL, 0);
        set_pad_function(I2SRX_LRCLK_PIN, I2SRX_LRCLK_FUNC, NULL, 0);
#if ((I2SRX_WORK_MODE == I2SRX_WORK_SLAVE) && (I2SRX_SLAVE_MCLK_USE_INNER == 1))
        set_pad_function(I2SRX_MCLK_PIN, I2SRX_MCLK_FUNC, NULL, 0);
#endif
        set_pad_function(I2SRX_DI_PIN, I2SRX_DI_FUNC, NULL, 0);
    }
    else if (g_linein_play_chan == ENTER_SPDIF_CHAN)
    {
        linein_music_setting.adc_chan_set.data_input_if = ADC_SOURCE_SPDIFRX;
        set_pad_function(SPDIF_RX_PIN, SPDIF_RX_FUNC, NULL, 0);
    }
    else if (g_linein_play_chan == ENTER_HDMI_CHAN)
    {
        linein_music_setting.adc_chan_set.data_input_if = ADC_SOURCE_SPDIFRX;
        set_pad_function(SPDIF_RX_HDMI_PIN, SPDIF_RX_HDMI_FUNC, NULL, 0);
    }
    else if (g_linein_play_chan == ENTER_ARC_CHAN)
    {
        linein_music_setting.adc_chan_set.data_input_if = ADC_SOURCE_HDMIARC;
        linein_music_setting.start_delay_time = 20;
        set_pad_function(SPDIF_RX_ARC_PIN, SPDIF_RX_ARC_FUNC, NULL, 0);
    }

    if ((g_linein_play_chan == ENTER_MIC_CHAN_U) || (g_linein_play_chan == ENTER_MIC_CHAN_C))
    {
        adc_samplerate = rec_sample_rate_khz[(uint8) com_get_config_default(RECORD_SAMPLE_RATE)];
    }

    /*! �����ʣ�ADC/I2SRX/MIC�� */
    linein_music_setting.adc_chan_set.default_samplerate = adc_samplerate;

    /*! AIN����ģʽ��0��ʾ����������ϣ�1��ʾ���������룬2��ʾ���������� */
    linein_music_setting.adc_chan_set.ain_input_mode = (uint8) com_get_config_default(LINEIN_INPUT_PIN_USE);
    /*! AIN OP Gain */
    linein_music_setting.adc_chan_set.ain_op_gain = (uint8) com_get_config_default(LINEIN_GAIN_INPUT);
    /*! AIN ADC Gain */
    linein_music_setting.adc_chan_set.ain_adc_gain = (uint8) com_get_config_default(LINEIN_GAIN_ADC);

    /*! LINEIN�����������ֵ������һ��ʱ�������ΪAUX����Դֹͣ������ʡ��ػ��ȣ�������������Ҫͬʱ������������ */
    linein_music_setting.adc_chan_set.adc_lowpw_th_max = 0;/* 11=-70dB max */ /*Ϊ0��ʾ���������ֵ�ж�*/
    linein_music_setting.adc_chan_set.adc_lowpw_th_rms = 5; /* 5=-72dB rms*/  /*Ϊ0��ʾ��������Чֵ�ж�*/

    /*! Ĭ�ϲ����ʣ���λ��KHz��Ĭ��ֵ44 */
    linein_music_setting.default_samplerate = adc_samplerate;
    if ((g_linein_play_chan == ENTER_MIC_CHAN_U) || (g_linein_play_chan == ENTER_MIC_CHAN_C))
    {
        /*! ��MIC¼�� */
        linein_music_setting.default_channels = 1;
    }
    else
    {
        /*! Ĭ��ͨ������Ĭ��ֵ2 */
        linein_music_setting.default_channels = 2;
    }

    /*! ��Ƶ���ͨ������ typestruct.h �еĶ��� */
    linein_music_setting.aud_output_chan = com_get_config_default(SETTING_HARDWARE_AUDIO_OUTPUT_CHANNEL);

    /*! ����ʱ�䣬��λ���� */
    linein_music_setting.fade_in_time = 200;
    /*! ����ʱ�䣬��λ���� */
    linein_music_setting.fade_out_time = 100;

    app_init_ret = mmm_pp_cmd(pp_handle, MMM_PP_SET_MUSIC_SETTING, (unsigned int) &linein_music_setting);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_SET_MUSIC_SETTING);
        bret = FALSE;
        goto _linein_play_err_2;
    }

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1))
    if ((g_linein_play_chan == ENTER_MIC_CHAN_U) || (g_linein_play_chan == ENTER_MIC_CHAN_C))
    {
        if ((g_eg_status.record_sta == RECORD_STA_STOP) || (g_eg_status.record_sta == RECORD_STA_PAUSE))
        {
            app_init_ret = _record_start(1, RECORD_SOURCE_L_CH);
            if (app_init_ret == TRUE)
            {
                g_eg_status.record_sta = RECORD_STA_RECORDING;
            }
            else
            {
                g_eg_status.record_sta = RECORD_STA_ERROR;
                bret = FALSE;
                goto _linein_play_err_2;
            }
        }
    }
#endif

    //�����Ƿ�����������Ĭ�ϼ���
    if ((g_linein_play_chan == ENTER_MIC_CHAN_U) || (g_linein_play_chan == ENTER_MIC_CHAN_C))
    {
        bool monitor_enable = com_get_config_default(RECORD_MONITOR_ENABLE);
        mmm_pp_cmd(pp_handle, MMM_PP_CLOSE_MONITORING, (unsigned int) (!monitor_enable));
    }

    //��ȡ��Ƶ������ֵ����
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

    //���Ͳ�������
    app_init_ret = mmm_pp_cmd(pp_handle, MMM_PP_PLAY, (unsigned int) NULL);

    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_PLAY);
        bret = FALSE;
        goto _linein_play_err_2;
    }

    change_mmm_state(MMM_STATE_PLAYING);
    bret = TRUE;

    return bret;


    _linein_play_err_2:
    //CLOSE�м��
    mmm_pp_cmd(pp_handle, MMM_PP_CLOSE, (unsigned int) NULL);
    pp_handle = NULL;

    _linein_play_err_1:
    //ж���м������
    sys_drv_uninstall (MMM_GROUP);

    _linein_play_err_0:

    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _stop(void)
 * \ֹͣ��������
 * \param[in]    stop_mode ֹͣģʽ para1
 * \param[in]    stop_mode_e:
 *                           \STOP_NORMAL ����ֹͣ
 *                           \STOP_PAUSE  ��ͣ����
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_play_deal.c
 * \note
 */
/*******************************************************************************/
bool _stop(void)
{
    bool bret = TRUE;
    int app_init_ret = 0;

#if (SUPPORT_AUX_AA_STEREO == 1)
    if ((uint8) com_get_config_default(LINEIN_CHANNEL_SEL) == AUX_MODE_AA_STEREO)
    {
        _stop_aa();
        return TRUE;
    }
#endif

    if (pp_handle == NULL)
    {
        libc_printf_warning("_stop return, pp_handle IS NULL!\n");
        return TRUE;
    }

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1))
    if ((g_linein_play_chan == ENTER_MIC_CHAN_U) || (g_linein_play_chan == ENTER_MIC_CHAN_C))
    {
        _record_stop();
        g_eg_status.record_sta = RECORD_STA_PAUSE;
    }
#endif

    //ֹͣ����
    app_init_ret = mmm_pp_cmd(pp_handle, MMM_PP_STOP, (unsigned int) NULL);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_STOP);
    }

    //CLOSE�м��
    app_init_ret = mmm_pp_cmd(pp_handle, MMM_PP_CLOSE, (unsigned int) NULL);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_CLOSE);
    }

    //ж���м������
    sys_drv_uninstall (MMM_GROUP);

    pp_handle = NULL;

    if (g_linein_play_chan == ENTER_SPDIF_CHAN)
    {
        set_pad_function(SPDIF_RX_PIN, PAD_FUNC_GPIOIN, NULL, 0);
    }
    else if (g_linein_play_chan == ENTER_HDMI_CHAN)
    {
        set_pad_function(SPDIF_RX_HDMI_PIN, PAD_FUNC_GPIOIN, NULL, 0);
    }
    else if (g_linein_play_chan == ENTER_ARC_CHAN)
    {
        set_pad_function(SPDIF_RX_ARC_PIN, PAD_FUNC_GPIOIN, NULL, 0);
    }

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

    //��ȡ�м��״̬
    mmm_ret = mmm_pp_cmd(pp_handle, MMM_PP_GET_STATUS, (unsigned int) &g_pp_status);
    if (mmm_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_GET_STATUS);
        return FALSE;
    }

    return TRUE;
}

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1))

static const int rec_bit_rate[BIT_MAX] =
{ 32000, 44000, 48000, 64000, 88000, 96000, 128000, 176000, 192000 };

static const int rec_sample_rate[RATE_MAX] =
{ 8000, 11025, 12000, 16000, 22050, 24000, 32000, 44100, 48000};

bool _record_start(uint8 channel, uint8 record_source)
{
    int app_init_ret = 0;
    bool bret = TRUE;
    record_module_encode_para_t encode_para;
    record_module_fs_para_t fs_para;
    uint8 sr_index = RATE_44KHZ, i;

    if ((g_linein_play_chan == ENTER_MIC_CHAN_U) || (g_linein_play_chan == ENTER_MIC_CHAN_C))
    {
        sr_index = com_get_config_default(RECORD_SAMPLE_RATE);
    }
    else
    {
        for (i = 0; i < RATE_MAX; i++)
        {
            if (rec_sample_rate_khz[i] == ADC_FREQ_KHZ)
            {
                sr_index = i;
                break;
            }
        }
    }

    fs_para.fs_type = 0;
    fs_para.file_mount_id = g_record_param.file_sys_id;
    fs_para.file_handle = g_record_param.file_handle;
    fs_para.file_max_length = g_record_param.fs_free_space;

    //����¼����¼���ļ�����ܳ���512MB��
    if(fs_para.file_max_length>(1024*1024))
    {
        fs_para.file_max_length = 1024*1024;
    }

    mmm_pp_cmd(pp_handle, MMM_PP_RECORDING_SET_FS, &fs_para);

    encode_para.encode_mode = ENCODE_ADPCM;
    encode_para.bitrate = rec_bit_rate[sr_index];
    encode_para.sample_rate_hz = rec_sample_rate[sr_index];
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
    g_eg_status.rec_time = g_rm_status.time;

    return TRUE;
}

#endif
