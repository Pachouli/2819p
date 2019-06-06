/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������SD��MP3�ļ����ſ��ƽӿڣ����ڲ���TTS�����������ȼ������������
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

static void *sd_mp3_play_hd;

static void _print_mmm_cmd_error(uint32 cmd_id)
{
    libc_printf_warning("MMM_MP cmd[%d] fail!\n", cmd_id);
}

void * _play_file(bool is_tts, uint8 * file_name, uint16 * sec_ids, uint8 sec_cnt);

//���ز����������NULL��ʾ����������ʧ��
void * com_sd_mp3_player_play_tts_do_inner(uint16 * sec_ids, uint8 sec_cnt)
{
    char *tts_rc_name =
        { "tts_rc.lib" };

    sd_mp3_play_hd = _play_file(TRUE, tts_rc_name, sec_ids, sec_cnt);
    return sd_mp3_play_hd;
}

void * com_sd_mp3_player_play_mp3_inner(uint8 * mp3_name)
{
    sd_mp3_play_hd = _play_file(FALSE, mp3_name, NULL, 0);
    return sd_mp3_play_hd;
}

void * _play_file(bool is_tts, uint8 * file_name, uint16 * sec_ids, uint8 sec_cnt)
{
    void * player_handle;
    int32 ret = 0;
    char *mmm_name =
        { "mmm_mp.al" };
    mmm_mp_fs_para_t set_file_param;
    mmm_mp_tts_para_t set_tts_param;
    music_setting_t music_sett;
    freq_point_config_t tmp_freq_point_cfg;

    //�����м������
    ret = (int32) sys_drv_install(MMM_GROUP, 0, mmm_name);
    if (ret == -1)
    {
        goto _play_file_fail_ret_0;
    }

    //��ʼ���м����
    ret = mmm_mp_cmd(&player_handle, MMM_MP_OPEN, MP_WORK_MODE_NORMAL);
    if (ret < 0)
    {
        player_handle = NULL;
        _print_mmm_cmd_error(MMM_MP_OPEN);
        goto _play_file_fail_ret_1;
    }

    //set file
#if(SUPPORT_EXTLIB_TTS_PLAY == 1)
    set_file_param.fs_type = FS_TYPE_EXTLIB;
#else
    set_file_param.fs_type = FS_TYPE_SDFILE;
#endif
    set_file_param.is_tts = is_tts;
    set_file_param.only_mono = TRUE; //NOTE:һ��TTS���������������������������ǵ�������
    set_file_param.file_name = file_name;
    set_file_param.p_tts_para = NULL;
    if (is_tts == TRUE)
    {
        set_file_param.p_tts_para = &set_tts_param;

        set_tts_param.sec_ids = sec_ids;
        set_tts_param.sec_cnt = sec_cnt;
    }

    //����set file����
    ret = mmm_mp_cmd(player_handle, MMM_MP_SET_FILE, (unsigned int ) &set_file_param);
    if (ret < 0)
    {
        _print_mmm_cmd_error(MMM_MP_SET_FILE);
        goto _play_file_fail_ret_2;
    }

    //���ò��Ų���
    music_sett.aout_setting.default_samplerate = 44;
    music_sett.aout_setting.default_channels = 2;
    music_sett.aud_output_chan = com_get_config_default_inner(SETTING_HARDWARE_AUDIO_OUTPUT_CHANNEL);
    music_sett.fade_in_time = 50;
    music_sett.fade_out_time = 50;
    music_sett.ffb_step = 1000;
    music_sett.ffb_delay = 100;
    music_sett.ffb_play_time = 0;
    music_sett.music_lowpw_th_max = 0;/* 11=-70dB max *//*Ϊ0��ʾ���������ֵ�ж�*/
    music_sett.music_lowpw_th_rms = 0; /* 3=-77dB rms*//*Ϊ0��ʾ��������Чֵ�ж�*/
    music_sett.is_uhost = FALSE;

#if (SUPPORT_OK_MIC == 1)
    /*! ��������ӿ� */
    music_sett.adc_chan_set.data_input_if = ADC_SOURCE_MIC;
    /*! AIN����ģʽ��0��ʾ����������ϣ�1��ʾ���������룬2��ʾ���������� */
    music_sett.adc_chan_set.ain_input_mode = (uint8) com_get_config_default(LINEIN_INPUT_PIN_USE);
    if ((uint8) com_get_config_default(OK_MIC_MICIN_SOURCE) == 0)
    {
        music_sett.adc_chan_set.micin_input_mode = AIN_INPUT_MIC0_L;
    }
    else
    {
        music_sett.adc_chan_set.micin_input_mode = AIN_INPUT_MIC1_R;
    }
    music_sett.adc_chan_set.micin_op_gain = (uint8) com_get_config_default(OK_MIC_MICIN_OP_GAIN);
    music_sett.adc_chan_set.micin_adc_gain = (uint8) com_get_config_default(OK_MIC_MICIN_ADC_GAIN);
    /*! ADC������ */
    music_sett.adc_chan_set.default_samplerate = 44;

    /*! LINEIN�����������ֵ������һ��ʱ�������ΪAUX����Դֹͣ������ʡ��ػ��ȣ�������������Ҫͬʱ������������ */
    music_sett.adc_chan_set.adc_lowpw_th_max = 0;/* 11=-70dB max */ /*Ϊ0��ʾ���������ֵ�ж�*/
    music_sett.adc_chan_set.adc_lowpw_th_rms = 5; /* 5=-72dB rms*/  /*Ϊ0��ʾ��������Чֵ�ж�*/
#endif

    ret = mmm_mp_cmd(player_handle, MMM_MP_SET_MUSIC_SETTING, (unsigned int ) &music_sett);
    if (ret < 0)
    {
        _print_mmm_cmd_error(MMM_MP_SET_MUSIC_SETTING);
        goto _play_file_fail_ret_3;
    }

    //��ȡ��Ƶ������ֵ����
    tmp_freq_point_cfg.freq_point_enable = 0;
    tmp_freq_point_cfg.freq_point_num = 0;
    mmm_mp_cmd(player_handle, MMM_MP_SET_FREQ_POINT, (unsigned int ) &tmp_freq_point_cfg);

    //���Ͳ�������
    ret = mmm_mp_cmd(player_handle, MMM_MP_PLAY, (unsigned int) NULL);
    if (ret < 0)
    {
        _print_mmm_cmd_error(MMM_MP_PLAY);
        goto _play_file_fail_ret_3;
    }

    change_mmm_state_inner(MMM_STATE_TTS_PLAYING);
    return player_handle;

    _play_file_fail_ret_3:
    //ֹͣ�ر��ļ�
    mmm_mp_cmd(player_handle, MMM_MP_CLEAR_FILE, (unsigned int) NULL);

    _play_file_fail_ret_2:
    //�ر��м��
    mmm_mp_cmd(player_handle, MMM_MP_CLOSE, (unsigned int) NULL);
    player_handle = NULL;

    _play_file_fail_ret_1:
    //ж���м������
    sys_drv_uninstall(MMM_GROUP);

    _play_file_fail_ret_0:

    return NULL;
}

bool com_sd_mp3_player_stop_inner(void *player_handle)
{
    int32 ret;

    //ֹͣ����
    ret = mmm_mp_cmd(player_handle, MMM_MP_STOP, (unsigned int) NULL);
    if (ret < 0)
    {
        _print_mmm_cmd_error(MMM_MP_STOP);
    }

    //ֹͣ�ر��ļ�
    ret = mmm_mp_cmd(player_handle, MMM_MP_CLEAR_FILE, (unsigned int) NULL);
    if (ret < 0)
    {
        _print_mmm_cmd_error(MMM_MP_CLEAR_FILE);
    }

    //�ر��м���豸
    ret = mmm_mp_cmd(player_handle, MMM_MP_CLOSE, (unsigned int) NULL);
    if (ret < 0)
    {
        _print_mmm_cmd_error(MMM_MP_CLOSE);
    }

    //ж���м������
    sys_drv_uninstall(MMM_GROUP);

    change_mmm_state_inner(MMM_STATE_NO_PLAY);
    sd_mp3_play_hd = NULL;
    return TRUE;
}

bool com_sd_mp3_player_is_stop_inner(void *player_handle)
{
    mmm_mp_status_t mmm_status;

    if (player_handle != NULL)
    {
        //��ȡ��ǰ����״̬
        if (mmm_mp_cmd(player_handle, MMM_MP_GET_STATUS, (unsigned int) &mmm_status) == 0)
        {
            change_ktp_status_inner(mmm_status.ktp_status);

            if ((mmm_status.status == MMM_MP_ENGINE_PLAYEND) || (mmm_status.status == MMM_MP_ENGINE_ERROR))
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        else
        {
            return TRUE;
        }
    }

    return TRUE;
}

bool com_sd_mp3_player_start_soft_kt(uint8 *kt_file)
{
    if (sd_mp3_play_hd != NULL)
    {
        if (mmm_mp_cmd(sd_mp3_play_hd, MMM_MP_KEYTONE_PLAY, kt_file) == 0)
        {
            change_ktp_status_inner(MMM_KTP_ENGINE_PLAYING);
            return TRUE;
        }
        return FALSE;
    }

    return FALSE;
}

bool com_sd_mp3_player_get_status(void)
{
    if (sd_mp3_play_hd != NULL)
    {
        mmm_mp_status_t mmm_status;

        //��ȡ��ǰ����״̬
        if (mmm_mp_cmd(sd_mp3_play_hd, MMM_MP_GET_STATUS, (unsigned int) &mmm_status) == 0)
        {
            change_ktp_status_inner(mmm_status.ktp_status);
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    return FALSE;
}
