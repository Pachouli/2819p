/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������OK MIC���г���������ʵ�֡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_func_inner.h"
#include "mmm_pp.h"

#if (SUPPORT_OK_MIC == 1)

#define ADC_FREQ_KHZ  44 //¼��Ҳ�̶�Ϊ�ò����ʣ�����޸�Ҫͬ���޸�¼��������

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

    //�����м������
    app_init_ret = (int32) sys_drv_install(MMM_GROUP, 0, mmm_name);
    if (app_init_ret == -1)
    {
        bret = FALSE;
        goto _linein_play_err_0;
    }

    //OPEN�м��
    app_init_ret = mmm_pp_cmd(&ok_mic_pp_handle, MMM_PP_OPEN, PP_WORK_MODE_MIC);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_OPEN);
        ok_mic_pp_handle = NULL;
        bret = FALSE;
        goto _linein_play_err_1;
    }

    /*! PCMԴѡ��0��ʾAUX��1��ʾUSB */
    linein_music_setting.pcm_source = PCM_SOURCE_AUX;
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

    /*! �����ʣ�ADC/I2SRX/MIC�� */
    linein_music_setting.adc_chan_set.default_samplerate = ADC_FREQ_KHZ;

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
    linein_music_setting.default_samplerate = ADC_FREQ_KHZ;
    /*! Ĭ��ͨ������Ĭ��ֵ2 */
    linein_music_setting.default_channels = 2;

    /*! ��Ƶ���ͨ������ typestruct.h �еĶ��� */
    linein_music_setting.aud_output_chan = com_get_config_default(SETTING_HARDWARE_AUDIO_OUTPUT_CHANNEL);

    /*! ����ʱ�䣬��λ���� */
    linein_music_setting.fade_in_time = 10;
    /*! ����ʱ�䣬��λ���� */
    linein_music_setting.fade_out_time = 10;

    app_init_ret = mmm_pp_cmd(ok_mic_pp_handle, MMM_PP_SET_MUSIC_SETTING, (unsigned int) &linein_music_setting);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_SET_MUSIC_SETTING);
        bret = FALSE;
        goto _linein_play_err_2;
    }

    mmm_pp_cmd(ok_mic_pp_handle, MMM_PP_SET_IDLE_MODE, TRUE);

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
    mmm_mp_cmd(ok_mic_pp_handle, MMM_PP_SET_FREQ_POINT, (unsigned int) &tmp_freq_point_cfg);

    //���Ͳ�������
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
    //CLOSE�м��
    mmm_pp_cmd(ok_mic_pp_handle, MMM_PP_CLOSE, (unsigned int) NULL);
    ok_mic_pp_handle = NULL;

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
bool com_ok_mic_idle_stop_inner(void)
{
    bool bret = TRUE;
    int app_init_ret = 0;

    if (ok_mic_pp_handle == NULL)
    {
        libc_printf_warning("_stop return, ok_mic_pp_handle IS NULL!\n");
        return TRUE;
    }

    //ֹͣ����
    app_init_ret = mmm_pp_cmd(ok_mic_pp_handle, MMM_PP_STOP, (unsigned int) NULL);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_STOP);
    }

    //CLOSE�м��
    app_init_ret = mmm_pp_cmd(ok_mic_pp_handle, MMM_PP_CLOSE, (unsigned int) NULL);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_PP_CLOSE);
    }

    //ж���м������
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

    //��ȡ�м��״̬
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

    //��ʼ�����̲������ļ�

    fs_para.fs_type = 0;
    fs_para.file_mount_id = g_record_param.file_sys_id;
    fs_para.file_handle = g_record_param.file_handle;
    fs_para.file_max_length = g_record_param.fs_free_space;

	//����¼����¼���ļ�����ܳ���512MB��
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

    //�����ļ���ж�ش���

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

    //����״̬����UI

    return TRUE;
}

#endif


#endif
