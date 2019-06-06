/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������������ͨ���м�������ӿڡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include  "app_btcall_engine.h"

static void _print_mmm_cmd_error(uint32 cmd_id)
{
    libc_printf_warning("MMM_SP cmd[%d] fail!\n", cmd_id);
}

bool _play(bool oncall, bool att_flag, asqt_configs_t *p_asqt_config, asqt_dump_buf_t *p_asqt_dump_buf)
{
    char *mmm_name = { "mmm_sp.al" };
    int32 app_init_ret;
    bool ret = TRUE;
    speech_setting_ext_t *p_speech_sett_ext;

    if (sp_handle != NULL)
    {
        libc_printf_warning("_play return, sp_handle NOT NULL!\n");
        return TRUE;
    }

    if (oncall == TRUE)
    {
        //�������SCO Input����
        com_btengine_set_sco_pipe_filter(PIPE_OP_UNFILTER | PIPE_OP_FLUSH);
    }

    //�����м������
    app_init_ret = (int32) sys_drv_install(MMM_GROUP, 0, mmm_name);
    if (app_init_ret == -1)
    {
        ret = FALSE;
        goto _speech_play_err_0;
    }

    //OPEN�м��
#if (SUPPORT_OK_MIC == 1)
    app_init_ret = mmm_sp_cmd(&sp_handle, MMM_SP_OPEN, SP_WORK_MODE_MIC);
#else
    app_init_ret = mmm_sp_cmd(&sp_handle, MMM_SP_OPEN, SP_WORK_MODE_NORMAL);
#endif
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_SP_OPEN);
        sp_handle = NULL;
        ret = FALSE;
        goto _speech_play_err_1;
    }

    p_speech_sett_ext = (speech_setting_ext_t *) sys_malloc(sizeof(speech_setting_ext_t));
    if (p_speech_sett_ext == NULL)
    {
        libc_printf_error("p_speech_sett_ext\n");
        while (1);
    }

    //����AUDIO����
    //NOTE!! ������MMM_SP_OPEN���������ã����ܲ���������������
    {
        if (p_asqt_config != NULL)
        {
            audio_setting_init_prms(&(p_speech_sett_ext->speech_setting), p_asqt_config);
        }
        else
        {
            audio_setting_init_prms(&(p_speech_sett_ext->speech_setting), NULL);
        }

        p_speech_sett_ext->speech_setting.use_sco_data_predeal = (oncall);
        p_speech_sett_ext->speech_setting.use_sco_data_postdeal = (bool) com_get_config_default(BTSTACK_SCO_POSTDEAL_ENABLE);

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1) && (SUPPORT_BTCALL_RECORD_FUNC == 1))
        p_speech_sett_ext->speech_setting.use_sco_data_postdeal = FALSE;
#endif
#if (SUPPORT_OK_MIC == 1)
        p_speech_sett_ext->speech_setting.use_sco_data_postdeal = FALSE;
#endif

        p_speech_sett_ext->speech_setting.sco_codec_type = 0;
    }

    //����PLC
    {
        if (p_asqt_config != NULL)
        {
            plc_init_prms(&(p_speech_sett_ext->plc_para), p_asqt_config);
        }
        else
        {
            plc_init_prms(&(p_speech_sett_ext->plc_para), NULL);
        }
    }

    //����PRE AGC
    {
        if (p_asqt_config != NULL)
        {
            preagc_init_prms(&(p_speech_sett_ext->pre_agc_para), p_asqt_config);
        }
        else
        {
            preagc_init_prms(&(p_speech_sett_ext->pre_agc_para), NULL);
        }
    }

    //����POST AGC
    {
        if (p_asqt_config != NULL)
        {
            postagc_init_prms(&(p_speech_sett_ext->post_agc_para), p_asqt_config);
        }
        else
        {
            postagc_init_prms(&(p_speech_sett_ext->post_agc_para), NULL);
        }
    }

    //����AEC
    {
        if (p_asqt_config != NULL)
        {
            actaec_init_sv_prms(&(p_speech_sett_ext->aec_prms), p_asqt_config, att_flag);
        }
        else
        {
            actaec_init_sv_prms(&(p_speech_sett_ext->aec_prms), NULL, att_flag);
        }
    }

#if (SUPPORT_OK_MIC == 1)
    //AEC&AGCֻ��ͨ��������Ҫ��OK MIC����ǿ�ƹر�
    p_speech_sett_ext->aec_prms.aec_enable = FALSE;
    p_speech_sett_ext->pre_agc_para.agc_enable = FALSE;
    p_speech_sett_ext->post_agc_para.agc_enable = FALSE;
#endif

    if (btcall_bt_info.status == STATUS_SIRI)
    {
        //AEC&AGCֻ��ͨ��������Ҫ��SIRI�������ֳ���ǿ�ƹر�
        p_speech_sett_ext->aec_prms.aec_enable = FALSE;
        p_speech_sett_ext->pre_agc_para.agc_enable = FALSE;
        p_speech_sett_ext->post_agc_para.agc_enable = FALSE;
        libc_printf("SIRI MODE disable AEC/AGC!\n");
    }

    //����SPEECH���в���
    mmm_sp_cmd(sp_handle, MMM_SP_SET_SPEECH_SETTING_EXT, (unsigned int) p_speech_sett_ext);

    sys_free(p_speech_sett_ext);

    //����ASQT DUMP BUFFER
    if (p_asqt_dump_buf != NULL)
    {
        mmm_sp_cmd(sp_handle, MMM_SP_SET_ASQT_DUMP_BUF, (unsigned int) p_asqt_dump_buf);
    }

    app_init_ret = mmm_sp_cmd(sp_handle, MMM_SP_PLAY, (unsigned int) NULL);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_SP_PLAY);
        ret = FALSE;
        goto _speech_play_err_2;
    }

    change_mmm_state(MMM_STATE_PLAYING);
    return ret;


    _speech_play_err_2:
    //CLOSE�м��
    mmm_sp_cmd(sp_handle, MMM_SP_CLOSE, (unsigned int) NULL);
    sp_handle = NULL;

    _speech_play_err_1:
    //ж���м��
    sys_drv_uninstall (MMM_GROUP);

    _speech_play_err_0:

    return ret;
}

bool _stop(bool oncall)
{
    int32 app_init_ret;
    bool ret = TRUE;

    if (sp_handle == NULL)
    {
        libc_printf_warning("_stop return, sp_handle IS NULL!\n");
        return TRUE;
    }

    app_init_ret = mmm_sp_cmd(sp_handle, MMM_SP_STOP, (unsigned int) NULL);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_SP_STOP);
    }

    if (oncall == TRUE)
    {
        //����SCO Input����
        com_btengine_set_sco_pipe_filter(PIPE_OP_FILTER | PIPE_OP_FLUSH);
    }

    //CLOSE�м��
    app_init_ret = mmm_sp_cmd(sp_handle, MMM_SP_CLOSE, (unsigned int) NULL);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_SP_CLOSE);
    }

    //ж���м������
    sys_drv_uninstall (MMM_GROUP);

    sp_handle = NULL;

    change_mmm_state(MMM_STATE_NO_PLAY);
    return ret;
}

bool _get_mmm_status(void)
{
    int32 app_init_ret;

    if (sp_handle == NULL)
    {
        return FALSE;
    }

    //��ȡ�м��״̬
    app_init_ret = mmm_sp_cmd(sp_handle, MMM_SP_GET_STATUS, (unsigned int) &sp_status);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_SP_GET_STATUS);
        return FALSE;
    }

    return TRUE;
}

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1) && (SUPPORT_BTCALL_RECORD_FUNC == 1))

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

    //����¼����¼���ļ�����ܳ���512MB��
    if(fs_para.file_max_length>(1024*1024))
    {
        fs_para.file_max_length = 1024*1024;
    }

    mmm_sp_cmd(sp_handle, MMM_SP_RECORDING_SET_FS, &fs_para);

    encode_para.encode_mode = ENCODE_ADPCM;
    encode_para.bitrate = 32000;
    encode_para.sample_rate_hz = 8000; //default, ��������������Դ������
    encode_para.channel = channel;
    encode_para.record_source = record_source;
    encode_para.bps = 16;

    mmm_sp_cmd(sp_handle, MMM_SP_RECORDING_SET_ENCODE, &encode_para);

    app_init_ret = mmm_sp_cmd(sp_handle, MMM_SP_RECORDING_START, NULL);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_SP_RECORDING_START);
        bret = FALSE;
    }

    return bret;
}

bool _record_stop(void)
{
    int app_init_ret = 0;
    bool bret = TRUE;

    app_init_ret = mmm_sp_cmd(sp_handle, MMM_SP_RECORDING_STOP,  (unsigned int) NULL);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_SP_RECORDING_STOP);
        bret = FALSE;
    }

    return bret;
}

bool _get_mmm_record_status(void)
{
    int32 mr_ret;

    if (sp_handle == NULL)
    {
        return FALSE;
    }

    mr_ret = mmm_sp_cmd(sp_handle, MMM_SP_RECORDING_GET_STATUS, (unsigned int) &g_rm_status);
    if (mr_ret < 0)
    {
        _print_mmm_cmd_error(MMM_SP_RECORDING_GET_STATUS);
        return FALSE;
    }

    return TRUE;
}

#endif
