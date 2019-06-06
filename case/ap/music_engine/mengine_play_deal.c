/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：本地播歌引擎应用 播放、暂停、获取断点信息、快进退、EQ设置等处理
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
 * \如果为卡盘播放文件，检测卡是否存在,如果播放u盘检测u盘是否存在
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval       1 存在
 * \retval       0 不存在
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
 * \设置播放文件到解码器
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
        //首先检测磁盘是否存在
        if (FALSE == _mengine_check_disk_in(g_eg_cfg_p->location.dirlocation.disk))
        {
            //保存错误状态
            g_eg_status_p->err_status = EG_ERR_DISK_OUT;
            return FALSE;
        }
    }

    //文件系统挂载id
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
        //参数设置
        if (g_file_sys_id == 0)
        {
            return FALSE;
        }
        vfs_get_name(g_file_sys_id, g_file_name, 16);

        //music engine只能在中间件play之前调用文件系统接口，否则会与中间件解码线程读数据访问文件系统重入风险
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

    //盘符
    set_file_param.file_mount_id = (uint32) g_file_sys_id;

#ifdef PRINT_FILE_NAME
    {
        uint8 name_buf[32], i, char_len;

        //简单的UNICODE转内码，只支持英文名字
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

    //发送set file命令
    result = mmm_mp_cmd(mp_handle, MMM_MP_SET_FILE, (unsigned int ) &set_file_param);
    if (result < 0)
    {
        _get_errno_no();

        _print_mmm_cmd_error(MMM_MP_SET_FILE);

        return FALSE;
    }
    else
    {
        //获取文件格式、总时间、比特率等信息
        result = mmm_mp_cmd(mp_handle, MMM_MP_MEDIA_INFO, (unsigned int ) &g_mengine_info.eg_file_info);
        if (result < 0)
        {
            _get_errno_no();

            //关闭文件
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
bool _play(play_mode_e play_mode)
{
    int result;
    bool bret = TRUE;
    int app_init_ret = 0;
    char *mmm_name =
        { "mmm_mp.al" };
    music_setting_t music_sett;
    freq_point_config_t tmp_freq_point_cfg;

    //已经播放起来，直接返回
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

    //加载中间件驱动
    app_init_ret = (int32) sys_drv_install(MMM_GROUP, 0, mmm_name);
    if (app_init_ret == -1)
    {
        bret = FALSE;
        goto _play_err_ret_0;
    }

    //初始化中间件库
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

    //设置无缝播放明命令
#if 0
    app_init_ret = mmm_mp_cmd(mp_handle, MMM_MP_SET_LOOP_PLAY, TRUE);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_MP_SET_LOOP_PLAY);
        bret = FALSE;
        goto _play_err_ret_1;
    }
#endif

    //设置播放文件
    bret = _set_file();
    if (bret == FALSE)
    {
        goto _play_err_ret_2;
    }

    //设置播放参数
    music_sett.aout_setting.default_samplerate = 44;
    music_sett.aout_setting.default_channels = 2;
    music_sett.aud_output_chan = com_get_config_default(SETTING_HARDWARE_AUDIO_OUTPUT_CHANNEL);
    music_sett.fade_in_time = 100;
    music_sett.fade_out_time = 100;
    music_sett.ffb_step = MUSIC_FFB_STEP;
    music_sett.ffb_delay = MUSIC_FFB_DELAY_TIME;
    music_sett.ffb_play_time = MUSIC_FFB_PLAY_TIME;
    music_sett.music_lowpw_th_max = 0;/* 11=-70dB max *//*为0表示不采用最大值判断*/
    music_sett.music_lowpw_th_rms = 3; /* 3=-77dB rms*//*为0表示不采用有效值判断*/

    if ((g_open_disk == DISK_H) || (g_open_disk == DISK_C))
    {
        music_sett.is_uhost = FALSE;
    }
    else
    {
        music_sett.is_uhost = TRUE;
    }

#if (SUPPORT_OK_MIC == 1)
    /*! 数据输入接口 */
    music_sett.adc_chan_set.data_input_if = ADC_SOURCE_MIC;
    /*! AIN输入模式，0表示左右声道混合，1表示左声道输入，2表示右声道输入 */
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
    /*! ADC采样率 */
    music_sett.adc_chan_set.default_samplerate = 44;

    /*! LINEIN低能量检测阈值，持续一段时间可以认为AUX播放源停止，可以省电关机等；这两个参数需要同时满足才算低能量 */
    music_sett.adc_chan_set.adc_lowpw_th_max = 0;/* 11=-70dB max */ /*为0表示不采用最大值判断*/
    music_sett.adc_chan_set.adc_lowpw_th_rms = 5; /* 5=-72dB rms*/  /*为0表示不采用有效值判断*/
#endif

    app_init_ret = mmm_mp_cmd(mp_handle, MMM_MP_SET_MUSIC_SETTING, (unsigned int ) &music_sett);
    if (app_init_ret < 0)
    {
        _print_mmm_cmd_error(MMM_MP_SET_MUSIC_SETTING);
        bret = FALSE;
        goto _play_err_ret_3;
    }

    //播放模式设置,断点续播
    if (play_mode == PLAY_FFON)
    {
        //快进播放
        result = mmm_mp_cmd(mp_handle, MMM_MP_SET_FFB, (unsigned int ) 4);
    }
    else if (play_mode == PLAY_FBON)
    {
        //快退播放
        g_eg_cfg_p->bk_infor.bp_time_offset = -5000;
        g_eg_cfg_p->bk_infor.bp_file_offset = 0;
        g_eg_cfg_p->bk_infor.bp_info_ext = 0;
        mmm_mp_cmd(mp_handle, MMM_MP_SET_BREAK_POINT, (unsigned int ) &g_eg_cfg_p->bk_infor);

        result = mmm_mp_cmd(mp_handle, MMM_MP_SET_FFB, (unsigned int ) -4);
    }
    else if (play_mode == PLAY_RESUME)
    {
        //断点续播,设置断点
        if (g_eg_cfg_p->bk_infor.bp_time_offset != 0)
        {
            mmm_mp_cmd(mp_handle, MMM_MP_SET_BREAK_POINT, (unsigned int ) &g_eg_cfg_p->bk_infor);
        }
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
    mmm_mp_cmd(mp_handle, MMM_MP_SET_FREQ_POINT, (unsigned int ) &tmp_freq_point_cfg);

    //发送播放命令
    result = mmm_mp_cmd(mp_handle, MMM_MP_PLAY, (unsigned int) NULL);
    if (result != 0)
    {
        _get_errno_no();
        bret = FALSE;
        _print_mmm_cmd_error(MMM_MP_PLAY);
        goto _play_err_ret_3;
    }

    //设置播放状态
    if (play_mode == PLAY_FFON)
    {
        //快进播放
        g_eg_status_p->play_status = PlayFast;
    }
    else if (play_mode == PLAY_FBON)
    {
        //快退播放
        g_eg_status_p->play_status = PlayFast;
    }
    else if (play_mode == PLAY_RESUME)
    {
        //断点续播
        g_eg_status_p->play_status = PlaySta;
    }
    else
    {
        //正常播放
        g_eg_status_p->play_status = PlaySta;
    }

    g_eg_playinfo_p->cur_file_switch |= 0x02; //恢复播放，可以获取总时间等信息
    change_mmm_state(MMM_STATE_PLAYING);
    bret = TRUE;
    return bret;

    _play_err_ret_3:
    //关闭文件
    result = mmm_mp_cmd(mp_handle, MMM_MP_CLEAR_FILE, (unsigned int) NULL);
    if (result < 0)
    {
        _print_mmm_cmd_error(MMM_MP_CLEAR_FILE);
    }

    _play_err_ret_2:
    //关闭中间件
    mmm_mp_cmd(mp_handle, MMM_MP_CLOSE, (unsigned int) NULL);
    mp_handle = NULL;

    _play_err_ret_1:
    //卸载中间件驱动
    sys_drv_uninstall(MMM_GROUP);

    _play_err_ret_0:

    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _stop(stop_mode_e stop_mode)
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
bool _stop(stop_mode_e stop_mode)
{
    bool bret = TRUE;
    int result = 0;

    //已经播放起来，直接返回
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

    //取消快进退
    if (g_eg_status_p->play_status == PlayFast)
    {
        mmm_mp_cmd(mp_handle, MMM_MP_SET_FFB, (unsigned int ) 0);
        g_eg_status_p->play_status = PlaySta;
    }

    //断言：到这里必须是PlaySta
    if (g_eg_status_p->play_status != PlaySta)
    {
        libc_printf_warning("MUST BT PlaySta!\n");
        while (1)
            ;
    }

    //保存断点
    _get_breakpoint();

    //停止播放
    result = mmm_mp_cmd(mp_handle, MMM_MP_STOP, (unsigned int) NULL);
    if (result < 0)
    {
        _get_errno_no();
        _print_mmm_cmd_error(MMM_MP_STOP);
    }

    //停止关闭文件
    result = mmm_mp_cmd(mp_handle, MMM_MP_CLEAR_FILE, (unsigned int) NULL);
    if (result < 0)
    {
        _get_errno_no();
        _print_mmm_cmd_error(MMM_MP_CLEAR_FILE);
    }

    //关闭中间件设备
    mmm_mp_cmd(mp_handle, MMM_MP_CLOSE, (unsigned int) NULL);

    //卸载中间件驱动
    sys_drv_uninstall(MMM_GROUP);

    mp_handle = NULL;

    //设置播放状态
    if (stop_mode == STOP_PAUSE)
    {
        //暂停播放
        g_eg_status_p->play_status = PauseSta;
    }
    else
    {
        //正常停止
        g_eg_status_p->play_status = StopSta;
    }

    change_mmm_state(MMM_STATE_NO_PLAY);
    bret = TRUE;

    //设置循环模式 TODO 为什么？
    fsel_set_mode((uint8) (g_eg_cfg_p->repeat_mode | g_eg_cfg_p->shuffle_flag));

    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void engine_get_status(void)
 * \引擎状态处理,获取当前音乐播放状态
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

    //获取中间件状态
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

    //获取中间件状态
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
 * \引擎状态处理,根据需求发送快进退命令给中间件
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
 * \引擎状态处理,获取文件播放信息
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
 * \设置eq
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
 * \保存引擎错误号
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

    //读取错误号
    mmm_mp_cmd(mp_handle, MMM_MP_GET_ERRORNO, (uint32 )&mp_err_no);

    //打印中间件解码错误类型
    libc_printf_info("MP_ERRORNO:0x%x\n", mp_err_no);

    //转换错误号
    switch (mp_err_no)
    {
        case MMM_MP_ERR_OPEN_FILE:
        err_status = EG_ERR_OPEN_FILE; //打开文件失败
        break;

        case MMM_MP_ERR_FILE_NOT_SUPPORT: //文件格式不支持
        case MMM_MP_ERR_FILE_DAMAGED: //文件损毁
        err_status = EG_ERR_NOT_SUPPORT;
        break;

        case MMM_MP_ERR_DECODER_ERROR: //解码出错
        err_status = EG_ERR_DECODER_ERROR;
        break;

        case MMM_MP_ERR_NO_LICENSE: //无license
        err_status = EG_ERR_NO_LICENSE;
        break;

        case MMM_MP_ERR_SECURE_CLOCK: //DRM时钟错误
        err_status = EG_ERR_SECURE_CLOCK;
        break;

        case MMM_MP_ERR_LICENSE_INFO: //播放次数已满或是播放时间已经到期
        err_status = EG_ERR_DRM_INFO;
        break;

        case MMM_MP_ERR_DISK_ERROR:
        err_status = EG_ERR_DISK_OUT;
        break;

        default:
        err_status = EG_ERR_DECODER_ERROR; //解码出错
        break;
    }

    //保存错误状态
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

