/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * ���������ز�������Ӧ�� ���š���ͣ����ȡ�ϵ���Ϣ������ˡ�EQ���õȴ���
 ********************************************************************************/
#include "app_mengine.h"

static void _print_mmm_cmd_error(uint32 cmd_id)
{
    libc_printf_warning("MMM_MP cmd[%d] fail!\n", cmd_id);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int _mengine_check_disk_in(uint8 disk_type)
 * \���Ϊ���̲����ļ�����⿨�Ƿ����,�������u�̼��u���Ƿ����
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval       1 ����
 * \retval       0 ������
 * \ingroup      mengine_play_deal.c
 * \note
 */
/*******************************************************************************/
bool _mengine_check_disk_in(uint8 disk_type)
{
    bool bret = TRUE;

    if ((disk_type == DISK_H) && (com_get_config_default(SETTING_APP_SUPPORT_CARDPLAY) == 1))
    {
        bret = sys_card_detect();
    }
    else
    {
        if ((disk_type == DISK_U) && (com_get_config_default(SETTING_APP_SUPPORT_UHOSTPLAY) == 1))
        {
            bret = sys_uhost_detect();
        }
        else
        {
            if (disk_type != DISK_C)
            {
                bret = FALSE;
            }
        }
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _set_file(void)
 * \���ò����ļ���������
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      mengine_play_deal.c
 * \note
 */
/*******************************************************************************/
bool _set_file(void)
{
    int result;

    mmm_mp_fs_para_t set_file_param;

    if ((g_eg_cfg_p->fsel_type != FSEL_TYPE_SDFILE) && (g_eg_cfg_p->fsel_type != FSEL_TYPE_RESFILE))
    {
        //���ȼ������Ƿ����
        if (FALSE == _mengine_check_disk_in(g_eg_cfg_p->location.dirlocation.disk))
        {
            //�������״̬
            g_eg_status_p->err_status = EG_ERR_DISK_OUT;
            return FALSE;
        }
    }

    //�ļ�ϵͳ����id
    if (g_eg_cfg_p->fsel_type == FSEL_TYPE_SDFILE)
    {
        //sd file
        set_file_param.fs_type = FS_TYPE_SDFILE;
        set_file_param.is_tts = 0;
        set_file_param.only_mono = FALSE;
        set_file_param.file_name = (char *) g_file_name;
        set_file_param.p_tts_para = NULL;
    }
    else if (g_eg_cfg_p->fsel_type == FSEL_TYPE_COMMONDIR)
    {
        //normal file
        set_file_param.fs_type = FS_TYPE_VFS;
        set_file_param.is_tts = 0;
        set_file_param.only_mono = FALSE;
        set_file_param.file_name = NULL;
        set_file_param.p_tts_para = NULL;
        //��������
        if (g_file_sys_id == 0)
        {
            return FALSE;
        }
        vfs_get_name(g_file_sys_id, g_file_name, 16);

        //music engineֻ�����м��play֮ǰ�����ļ�ϵͳ�ӿڣ���������м�������̶߳����ݷ����ļ�ϵͳ�������
        libc_memcpy(g_eg_cfg_p->name_buf, g_file_name, 16);
    }
    else if (g_eg_cfg_p->fsel_type == FSEL_TYPE_RESFILE)
    {
        if (g_eg_cfg_p->location.dirlocation.file_num >= 1)
        {
            play_file_num = g_eg_cfg_p->location.dirlocation.file_num;
        }
#if (SUPPORT_RES_DIR == 1)
        if (TRUE != get_current_file_name(play_file_num, g_file_name))
        {
            return FALSE;
        }
        //res file
        set_file_param.fs_type = FS_TYPE_RESFILE;
#elif (SUPPORT_EXTLIB_PLAY == 1)
        if(TRUE != extlib_get_current_file_name(play_file_num, g_file_name))
        {
            return FALSE;
        }
        //extlib file
        set_file_param.fs_type = FS_TYPE_EXTLIB;
#endif
        set_file_param.is_tts = 0;
        set_file_param.only_mono = FALSE;
        set_file_param.file_name = (char *) g_file_name;
        set_file_param.p_tts_para = NULL;

        g_eg_cfg_p->location.dirlocation.disk = DISK_C;
#if (SUPPORT_RES_DIR == 1)
        g_eg_cfg_p->location.dirlocation.file_total = get_disk_total_num();
#elif (SUPPORT_EXTLIB_PLAY == 1)
        g_eg_cfg_p->location.dirlocation.file_total = extlib_get_disk_total_num();
#endif
        g_eg_cfg_p->location.dirlocation.file_num = play_file_num;
    }
    else
    {
        ;
    }

    //�̷�
    set_file_param.file_mount_id = (uint32) g_file_sys_id;

#ifdef PRINT_FILE_NAME
    {
        uint8 name_buf[32], i, char_len;

        //�򵥵�UNICODEת���룬ֻ֧��Ӣ������
        if ((g_file_name[0] == 0xff) && (g_file_name[1] == 0xfe))
        {
            char_len = 0;
            for (i = 2; i < 32; i++)
            {
                if (g_file_name[i] < 0x80)
                {
                    name_buf[char_len] = g_file_name[i];
                    char_len++;
                    i++;
                }
                else
                {
                    break;
                }
            }
            if (char_len > 0)
            {
                name_buf[char_len] = 0;
            }

            libc_printf_info("FILE:%s\n", name_buf);
        }
        else
        {
            libc_printf_info("FILE:%s\n", g_file_name);
        }
        libc_printhex(g_file_name, MAX_FILE_NAME, 0);
    }
#endif

    //����set file����
    result = mmm_mp_cmd(mp_handle, MMM_MP_SET_FILE, (unsigned int ) &set_file_param);
    if (result < 0)
    {
        _get_errno_no();

        _print_mmm_cmd_error(MMM_MP_SET_FILE);

        return FALSE;
    }
    else
    {
        //��ȡ�ļ���ʽ����ʱ�䡢�����ʵ���Ϣ
        result = mmm_mp_cmd(mp_handle, MMM_MP_MEDIA_INFO, (unsigned int ) &g_mengine_info.eg_file_info);
        if (result < 0)
        {
            _get_errno_no();

            //�ر��ļ�
            result = mmm_mp_cmd(mp_handle, MMM_MP_CLEAR_FILE, (unsigned int) NULL);
            if (result < 0)
            {
                _print_mmm_cmd_error(MMM_MP_CLEAR_FILE);
            }

            return FALSE;
        }
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _play(play_mode_e play_mode)
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
bool _play(play_mode_e play_mode)
{
    int result;
    bool bret = TRUE;
    int app_init_ret = 0;
    char *mmm_name =
        { "mmm_mp.al" };
    music_setting_t music_sett;
    freq_point_config_t tmp_freq_point_cfg;

    //�Ѿ�����������ֱ�ӷ���
    if ((g_eg_status_p->play_status != StopSta) && (g_eg_status_p->play_status != PauseSta))
    {
        libc_printf_warning("_play return, It already Played!\n");
        return TRUE;
    }

    if (mp_handle != NULL)
    {
        libc_printf_warning("_play return, mp_handle NOT NULL!\n");
        return TRUE;
    }

    //�����м������
    app_init_ret = (int32) sys_drv_install(MMM_GROUP, 0, mmm_name);
    if (app_init_ret == -1)
    {
        bret = FALSE;
        goto _play_err_ret_0;
    }

    //��ʼ���м����
#if (SUPPORT_OK_MIC == 1)
    app_init_ret = mmm_mp_cmd(&mp_handle, MMM_MP_OPEN, MP_WORK_MODE_MIC);
#else
    app_init_ret = mmm_mp_cmd(&mp_handle, MMM_MP_OPEN, MP_WORK_MODE_NORMAL);
#endif
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_MP_OPEN);
        mp_handle = NULL;
        bret = FALSE;
        goto _play_err_ret_1;
    }

    //�����޷첥��������
#if 0
    app_init_ret = mmm_mp_cmd(mp_handle, MMM_MP_SET_LOOP_PLAY, TRUE);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_MP_SET_LOOP_PLAY);
        bret = FALSE;
        goto _play_err_ret_1;
    }
#endif

    //���ò����ļ�
    bret = _set_file();
    if (bret == FALSE)
    {
        goto _play_err_ret_2;
    }

    //���ò��Ų���
    music_sett.aout_setting.default_samplerate = 44;
    music_sett.aout_setting.default_channels = 2;
    music_sett.aud_output_chan = com_get_config_default(SETTING_HARDWARE_AUDIO_OUTPUT_CHANNEL);
    music_sett.fade_in_time = 100;
    music_sett.fade_out_time = 100;
    music_sett.ffb_step = MUSIC_FFB_STEP;
    music_sett.ffb_delay = MUSIC_FFB_DELAY_TIME;
    music_sett.ffb_play_time = MUSIC_FFB_PLAY_TIME;
    music_sett.music_lowpw_th_max = 0;/* 11=-70dB max *//*Ϊ0��ʾ���������ֵ�ж�*/
    music_sett.music_lowpw_th_rms = 3; /* 3=-77dB rms*//*Ϊ0��ʾ��������Чֵ�ж�*/

    if ((g_open_disk == DISK_H) || (g_open_disk == DISK_C))
    {
        music_sett.is_uhost = FALSE;
    }
    else
    {
        music_sett.is_uhost = TRUE;
    }

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

    app_init_ret = mmm_mp_cmd(mp_handle, MMM_MP_SET_MUSIC_SETTING, (unsigned int ) &music_sett);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_MP_SET_MUSIC_SETTING);
        bret = FALSE;
        goto _play_err_ret_3;
    }

    //����ģʽ����,�ϵ�����
    if (play_mode == PLAY_FFON)
    {
        //�������
        result = mmm_mp_cmd(mp_handle, MMM_MP_SET_FFB, (unsigned int ) 4);
    }
    else if (play_mode == PLAY_FBON)
    {
        //���˲���
        g_eg_cfg_p->bk_infor.bp_time_offset = -5000;
        g_eg_cfg_p->bk_infor.bp_file_offset = 0;
        g_eg_cfg_p->bk_infor.bp_info_ext = 0;
        mmm_mp_cmd(mp_handle, MMM_MP_SET_BREAK_POINT, (unsigned int ) &g_eg_cfg_p->bk_infor);

        result = mmm_mp_cmd(mp_handle, MMM_MP_SET_FFB, (unsigned int ) -4);
    }
    else if (play_mode == PLAY_RESUME)
    {
        //�ϵ�����,���öϵ�
        if (g_eg_cfg_p->bk_infor.bp_time_offset != 0)
        {
            mmm_mp_cmd(mp_handle, MMM_MP_SET_BREAK_POINT, (unsigned int ) &g_eg_cfg_p->bk_infor);
        }
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
    mmm_mp_cmd(mp_handle, MMM_MP_SET_FREQ_POINT, (unsigned int ) &tmp_freq_point_cfg);

    //���Ͳ�������
    result = mmm_mp_cmd(mp_handle, MMM_MP_PLAY, (unsigned int) NULL);
    if (result != 0)
    {
        _get_errno_no();
        bret = FALSE;
        _print_mmm_cmd_error(MMM_MP_PLAY);
        goto _play_err_ret_3;
    }

    //���ò���״̬
    if (play_mode == PLAY_FFON)
    {
        //�������
        g_eg_status_p->play_status = PlayFast;
    }
    else if (play_mode == PLAY_FBON)
    {
        //���˲���
        g_eg_status_p->play_status = PlayFast;
    }
    else if (play_mode == PLAY_RESUME)
    {
        //�ϵ�����
        g_eg_status_p->play_status = PlaySta;
    }
    else
    {
        //��������
        g_eg_status_p->play_status = PlaySta;
    }

    g_eg_playinfo_p->cur_file_switch |= 0x02; //�ָ����ţ����Ի�ȡ��ʱ�����Ϣ
    change_mmm_state(MMM_STATE_PLAYING);
    bret = TRUE;
    return bret;

    _play_err_ret_3:
    //�ر��ļ�
    result = mmm_mp_cmd(mp_handle, MMM_MP_CLEAR_FILE, (unsigned int) NULL);
    if (result < 0)
    {
        _print_mmm_cmd_error(MMM_MP_CLEAR_FILE);
    }

    _play_err_ret_2:
    //�ر��м��
    mmm_mp_cmd(mp_handle, MMM_MP_CLOSE, (unsigned int) NULL);
    mp_handle = NULL;

    _play_err_ret_1:
    //ж���м������
    sys_drv_uninstall(MMM_GROUP);

    _play_err_ret_0:

    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _stop(stop_mode_e stop_mode)
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
bool _stop(stop_mode_e stop_mode)
{
    bool bret = TRUE;
    int result = 0;

    //�Ѿ�����������ֱ�ӷ���
    if ((g_eg_status_p->play_status == StopSta) || (g_eg_status_p->play_status == PauseSta))
    {
        libc_printf_warning("_stop return, It already Stopped!\n");
        return TRUE;
    }

    if (mp_handle == NULL)
    {
        libc_printf_warning("_stop return, mp_handle IS NULL!\n");
        return TRUE;
    }

    //ȡ�������
    if (g_eg_status_p->play_status == PlayFast)
    {
        mmm_mp_cmd(mp_handle, MMM_MP_SET_FFB, (unsigned int ) 0);
        g_eg_status_p->play_status = PlaySta;
    }

    //���ԣ������������PlaySta
    if (g_eg_status_p->play_status != PlaySta)
    {
        libc_printf_warning("MUST BT PlaySta!\n");
        while (1)
            ;
    }

    //����ϵ�
    _get_breakpoint();

    //ֹͣ����
    result = mmm_mp_cmd(mp_handle, MMM_MP_STOP, (unsigned int) NULL);
    if (result < 0)
    {
        _get_errno_no();
        _print_mmm_cmd_error(MMM_MP_STOP);
    }

    //ֹͣ�ر��ļ�
    result = mmm_mp_cmd(mp_handle, MMM_MP_CLEAR_FILE, (unsigned int) NULL);
    if (result < 0)
    {
        _get_errno_no();
        _print_mmm_cmd_error(MMM_MP_CLEAR_FILE);
    }

    //�ر��м���豸
    mmm_mp_cmd(mp_handle, MMM_MP_CLOSE, (unsigned int) NULL);

    //ж���м������
    sys_drv_uninstall(MMM_GROUP);

    mp_handle = NULL;

    //���ò���״̬
    if (stop_mode == STOP_PAUSE)
    {
        //��ͣ����
        g_eg_status_p->play_status = PauseSta;
    }
    else
    {
        //����ֹͣ
        g_eg_status_p->play_status = StopSta;
    }

    change_mmm_state(MMM_STATE_NO_PLAY);
    bret = TRUE;

    //����ѭ��ģʽ TODO Ϊʲô��
    fsel_set_mode((uint8) (g_eg_cfg_p->repeat_mode | g_eg_cfg_p->shuffle_flag));

    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void engine_get_status(void)
 * \����״̬����,��ȡ��ǰ���ֲ���״̬
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      mengine_control.c
 * \note
 */
/*******************************************************************************/
bool _get_mmm_status(void)
{
    int32 mmm_ret;

    if (mp_handle == NULL)
    {
        return FALSE;
    }

    //��ȡ�м��״̬
    mmm_ret = mmm_mp_cmd(mp_handle, MMM_MP_GET_STATUS, (unsigned int ) &g_mp_status);
    if (mmm_ret < 0)
    {
        _print_mmm_cmd_error(MMM_MP_GET_STATUS);
        return FALSE;
    }

    return TRUE;
}

bool _get_breakpoint(void)
{
    int32 mmm_ret;
    mmm_mp_bp_info_t *p_bk_infor = (mmm_mp_bp_info_t *) (g_p_esd_bk_info->bk_info);

    if (mp_handle == NULL)
    {
        return FALSE;
    }

    //��ȡ�м��״̬
    mmm_ret = mmm_mp_cmd(mp_handle, MMM_MP_GET_BREAK_POINT, (unsigned int ) &(g_eg_cfg_p->bk_infor));
    if (mmm_ret < 0)
    {
        _print_mmm_cmd_error(MMM_MP_GET_BREAK_POINT);
        return FALSE;
    }

    libc_memcpy(p_bk_infor, &g_eg_cfg_p->bk_infor, sizeof(mmm_mp_bp_info_t));

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void user_set_ffb(void)
 * \����״̬����,���������Ϳ����������м��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      mengine_control.c
 * \note
 */
/*******************************************************************************/
bool _set_ffb(set_ffb_mode mode)
{
    if (mp_handle == NULL)
    {
        return FALSE;
    }

    switch (mode)
    {
        case FAST_FORWARD:
        {
            mmm_mp_cmd(mp_handle, MMM_MP_SET_FFB, (unsigned int ) 4);
        }
        break;

        case FAST_BACKWARD:
        {
            mmm_mp_cmd(mp_handle, MMM_MP_SET_FFB, (unsigned int ) -4);
        }
        break;

        case CANCLE_FAST_BACKWARD_FORWARD:
        {
            mmm_mp_cmd(mp_handle, MMM_MP_SET_FFB, (unsigned int ) 0);
        }
        break;

        default:
        break;
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void get_file_info(void)
 * \����״̬����,��ȡ�ļ�������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      mengine_control.c
 * \note
 */
/*******************************************************************************/

bool _get_file_info(void)
{
    if (mp_handle == NULL)
    {
        return FALSE;
    }

    mmm_mp_cmd(mp_handle, MMM_MP_MEDIA_INFO, (unsigned int ) &g_mengine_info.eg_file_info);

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_play_eq(void)
 * \����eq
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      mengine_control.c
 * \note
 */
/*******************************************************************************/
bool _set_eq(void)
{
    int32 mmm_ret;

    if (mp_handle == NULL)
    {
        return FALSE;
    }

    mmm_ret = mmm_mp_cmd(mp_handle, MMM_MP_SET_EQ, 0);
    if (mmm_ret < 0)
    {
        _print_mmm_cmd_error(MMM_MP_SET_EQ);
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _get_errno_no(void)
 * \������������
 * \param[in]    void  para1
 * \param[out]   none
 * \return       none
 * \ingroup      mengine_mesasge_done.c
 * \note
 */
/*******************************************************************************/
bool _get_errno_no(void)
{
    eg_err_e err_status = EG_ERR_NONE;
    int32 mp_err_no;

    if (mp_handle == NULL)
    {
        return FALSE;
    }

    //��ȡ�����
    mmm_mp_cmd(mp_handle, MMM_MP_GET_ERRORNO, (uint32 )&mp_err_no);

    //��ӡ�м�������������
    libc_printf_info("MP_ERRORNO:0x%x\n", mp_err_no);

    //ת�������
    switch (mp_err_no)
    {
        case MMM_MP_ERR_OPEN_FILE:
        err_status = EG_ERR_OPEN_FILE; //���ļ�ʧ��
        break;

        case MMM_MP_ERR_FILE_NOT_SUPPORT: //�ļ���ʽ��֧��
        case MMM_MP_ERR_FILE_DAMAGED: //�ļ����
        err_status = EG_ERR_NOT_SUPPORT;
        break;

        case MMM_MP_ERR_DECODER_ERROR: //�������
        err_status = EG_ERR_DECODER_ERROR;
        break;

        case MMM_MP_ERR_NO_LICENSE: //��license
        err_status = EG_ERR_NO_LICENSE;
        break;

        case MMM_MP_ERR_SECURE_CLOCK: //DRMʱ�Ӵ���
        err_status = EG_ERR_SECURE_CLOCK;
        break;

        case MMM_MP_ERR_LICENSE_INFO: //���Ŵ����������ǲ���ʱ���Ѿ�����
        err_status = EG_ERR_DRM_INFO;
        break;

        case MMM_MP_ERR_DISK_ERROR:
        err_status = EG_ERR_DISK_OUT;
        break;

        default:
        err_status = EG_ERR_DECODER_ERROR; //�������
        break;
    }

    //�������״̬
    g_eg_status_p->err_status = err_status;

    if ((g_eg_status_p->err_status != EG_ERR_NONE) && (err_status != EG_ERR_DISK_OUT))
    {
        if (g_mengine_enter_mode == ENTER_MUSIC_START)
        {
            g_eg_cfg_p->location.dirlocation.file_total_disk = fsel_get_disk_total();
        }
        else
        {
            g_eg_cfg_p->location.dirlocation.file_total_disk = fsel_get_total();
        }
    }

    return TRUE;
}

