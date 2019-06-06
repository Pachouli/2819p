/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：FM收音机引擎应用消息循环，处理前台应用消息。
 * 作者：cailizhen
 ********************************************************************************/

#include "app_fmengine.h"

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1) && (SUPPORT_RADIO_RECORD_FUNC == 1))

record_status_e g_last_record_sta = RECORD_STA_STOP;

/*更新当前录音文件大小到vram*/
void fmeg_record_update_record_file(bool force)
{
    record_file_update_t temp_file_update;
    temp_file_update.start_cluster = g_rm_status.start_cluster;
    temp_file_update.write_pages = g_rm_status.write_pages;
    temp_file_update.record_time = g_rm_status.time;
    com_record_update_record_file(force, &temp_file_update);
}

void fmeg_recording_check_status(void)
{
    bool ret ;
    //更新录音信息到vram
    if (g_eg_status.record_sta == RECORD_STA_RECORDING)
    {
        fmeg_record_update_record_file(FALSE);
    }

    if ((g_last_record_sta == RECORD_STA_RECORDING) && (g_eg_status.record_sta  != RECORD_STA_RECORDING))
    {
        //从play到pause(录音暂停时，需要强制更新时间)
        libc_printf("record pause , update record file now\n");
        fmeg_record_update_record_file(TRUE);
    }

    g_last_record_sta = g_eg_status.record_sta ;
}
#endif

void fmengine_reply_msg(private_msg_t *msg_ptr, bool ret_vals)
{
    if (ret_vals == TRUE)
    {
        msg_ptr->reply->type = MSG_REPLY_SUCCESS;
    }
    else
    {
        msg_ptr->reply->type = MSG_REPLY_FAILED;
    }

    //回复同步消息(发送信号量)
    libc_sem_post(msg_ptr->sem);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_message_deal(private_msg_t* cur_msg)
 * \引擎消息处理,处理引擎接收到的私有消息和系统消息
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略
 * \retval       RESULT_APP_QUIT 退出app
 * \note
 */
/*******************************************************************************/

app_result_e fmengine_message_deal(private_msg_t *cur_msg)
{
    app_result_e ret_value = RESULT_NULL;

    switch (cur_msg->msg.type)
    {
        /* 参数设置相关*/

        /* 同步设置频点播放*/
        case MSG_FMENGINE_SETFREQ_SYNC:
        ret_value = fmengine_set_freq_sync(cur_msg);
        break;

        /* 静音或解除静音*/
        case MSG_FMENGINE_SETMUTE:
        ret_value = fmengine_mute_sync(cur_msg);
        break;

        /* 设置搜台门限*/
        case MSG_FMENGINE_SETTHRED:
        ret_value = fmengine_set_throd(cur_msg);
        break;

        /* 启动软件搜台*/
        case MSG_FMENGINE_SET_SOFTSEEK:
        ret_value = fmengine_set_softseek(cur_msg);
        break;

        /* 停止解码 */
        case MSG_FMENGINE_PAUSE_SYNC:
        ret_value = fmengine_pause(cur_msg);
        break;

        /* 启动解码 */
        case MSG_FMENGINE_RESUME_SYNC:
        ret_value = fmengine_resume(cur_msg);
        break;

#if (SUPPORT_SOFT_KEYTONE == 1)
        /*请求播放软按键音*/
        case MSG_SOFT_KEYTONE_PLAY_SYNC:
        {
            bool ret_val = FALSE;
            if (pp_handle != NULL)
            {
                if (mmm_pp_cmd(pp_handle, MMM_PP_KEYTONE_PLAY, (uint8 *)(cur_msg->msg.content.addr)) == 0)
                {
                    change_ktp_status(MMM_KTP_ENGINE_PLAYING);
                    ret_val = TRUE;
                }
            }
            fmengine_reply_msg(cur_msg, ret_val);
        }
        break;
#endif

        /*!设置数字音效模式DAE MODE*/
        case MSG_SET_EQ_SYNC:
        ret_value = fmengine_set_eq(cur_msg);
        break;

        /* 获取信息相关*/

        /* 获取当前状态信息*/
        case MSG_FMENGINE_GET_FM_STATUS:
        ret_value = fmengine_get_fm_status(cur_msg);
        break;

        case MSG_FMENGINE_GET_PLAY_STATUS:
        ret_value = fmengine_get_play_status(cur_msg);
        break;

        /* 获取当前频率信息*/
        case MSG_FMENGINE_GET_FREQ:
        ret_value = fmengine_get_freq(cur_msg);
        break;

        /* 模组初始化及卸载相关*/

        /* 同步初始化FM  模组*/
        case MSG_FMENGINE_INIT:
        ret_value = fm_module_init(cur_msg);
        break;

        /* FM 模组进入Standby */
        case MSG_FMENGINE_STANDBY:
        ret_value = fm_module_standby();
        break;

        /*即将切换到后台应用（通话和闹钟起闹），让应用备份状态，以便返回时恢复状态*/
        case MSG_READY_TO_BG_APP_SYNC:
        g_p_global_var->play_status_before = g_eg_status.play_status;
        fmengine_reply_msg(cur_msg, TRUE);
        break;

        case MSG_APP_QUIT: //退出app
        ret_value = RESULT_APP_QUIT;
        break;

        case MSG_TTS_START_SYNC: //准备进入TTS播报
        ret_value = fmengine_tts_start(cur_msg);
        break;

        case MSG_TTS_STOP_SYNC: //TTS播报完成
        ret_value = fmengine_tts_stop(cur_msg);
        break;

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1) && (SUPPORT_RADIO_RECORD_FUNC == 1))
        case MSG_REC_START_SYNC:
        ret_value = fmengine_record_start(cur_msg);
        break;

        case MSG_REC_STOP_SYNC:
        ret_value = fmengine_record_stop(cur_msg);
        break;
#endif

        case MSG_SYSTEM_ENTER_S2_SYNC:
        ret_value = fmengine_enter_s2(cur_msg);
        break;

        default:
        libc_printf_warning("fm engine invalid msg!0x%x\n", cur_msg->msg.type);
        fmengine_reply_msg(cur_msg, FALSE);
        break;
    }

    return ret_value;
}


/******************************************************************************/
/*!
 * \Description: FM 引擎应用为被动模块，也是其他应用进程
 * \ 和FM 驱动通讯的接口模块
 * \app_result_e fmengine_control_block(void)
 * \引擎功能处理函数
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_control_block(void)
{
    app_result_e result = RESULT_NULL;
    private_msg_t fmengine_msg;
    bool ret_vals;

    while (1)
    {
        if (pp_handle != NULL)
        {
            ret_vals = _get_mmm_status();
            if (ret_vals == TRUE)
            {
                sound_energy_t tmp_sound_energy;

                change_ktp_status(g_pp_status.ktp_status);

                if ((g_pp_status.status == MMM_PP_ENGINE_PLAYING)
                    || (g_pp_status.status == MMM_PP_ENGINE_FADEIN)
                    || (g_pp_status.status == MMM_PP_ENGINE_FADEOUT)
                    || (g_pp_status.status == MMM_PP_ENGINE_FADEOUT_PAUSE))
                {
                    tmp_sound_energy.play_cur_energy = g_pp_status.adc_energy.adc_cur_energy;
                    tmp_sound_energy.play_cur_energy_rms = g_pp_status.adc_energy.adc_cur_energy_rms;
                    tmp_sound_energy.mic_cur_energy = g_pp_status.adc_energy.mic_cur_energy;
                    tmp_sound_energy.mic_cur_energy_rms = g_pp_status.adc_energy.mic_cur_energy_rms;
                    libc_memcpy(tmp_sound_energy.freq_point_energy, g_pp_status.freq_point_energy, sizeof(tmp_sound_energy.freq_point_energy));
                    com_update_sound_energy(&tmp_sound_energy);
                }
                else
                {
                    //清除声音能量值
                    com_update_sound_energy(NULL);
                }

                g_eg_status.lowpw_time = g_pp_status.adc_energy.adc_lowpw_time;

                if (g_pp_status.status == MMM_PP_ENGINE_ERROR)
                {
                    //检测到错误，暂时没有这样的状态返回  TODO
                }
            }

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1) && (SUPPORT_RADIO_RECORD_FUNC == 1))
            ret_vals = _get_mmm_record_status();

            if (ret_vals == TRUE)
            {
                if (g_rm_status.err_no != MMM_MR_ERR_OK)
                {
                    //record_module出错
                    g_eg_status.record_sta = RECORD_STA_ERROR;
                }
            }
#endif
        }

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1) && (SUPPORT_RADIO_RECORD_FUNC == 1))
        fmeg_recording_check_status();
#endif

        //获取并处理消息
        if (get_app_msg(THIS_APP_TYPE, &fmengine_msg) == TRUE)
        {
            result = fmengine_message_deal(&fmengine_msg);
            if (result == RESULT_APP_QUIT)
            {
                break;
            }
        }

        if ((g_eg_status.play_status == StopSta) || (g_eg_status.play_status == PauseSta))
        {
            change_engine_state (ENGINE_STATE_PAUSE);
        }
        else
        {
            change_engine_state (ENGINE_STATE_PLAYING);
        }

        //挂起10ms 多任务交互
        sys_os_time_dly(1);
    }

    //清除声音能量值
    com_update_sound_energy(NULL);

    //退出应用关闭处理
    if (g_eg_status.play_status == PlaySta)
    {
        if (_stop() == TRUE)
        {
            g_eg_status.play_status = PauseSta;
        }
    }

    /*其他进程也可向FM 引擎发MSG_FMENGINE_CLOSE  消息要求FM 引擎退出*/
    if (g_module_status != MODULE_STANDBY)
    {
        fm_module_standby();
    }

#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
    if (get_mmm_state() == MMM_STATE_KT_PLAYING)
    {
        com_ok_mic_idle_stop();
    }
#endif

    return result;
}


//进入TTS播报前，暂停收音
app_result_e fmengine_tts_start(private_msg_t *cur_msg)
{
    bool ret_vals = TRUE;

    //先暂停播放
    if (g_eg_status.play_status == PlaySta)
    {
        ret_vals = _stop(); //暂停播放
        if (ret_vals == TRUE)
        {
            g_eg_status.play_status = PauseSta;
            g_stop_by_tts_flag = TRUE;
        }
    }
    else
    {
#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
        if (get_mmm_state() == MMM_STATE_KT_PLAYING)
        {
            com_ok_mic_idle_stop();
        }
#endif
    }

    fmengine_reply_msg(cur_msg, TRUE);

    return RESULT_NULL;
}

//TTS播报结束，恢复收音
app_result_e fmengine_tts_stop(private_msg_t *cur_msg)
{
    bool ret_vals = TRUE;

    if (g_stop_by_tts_flag == TRUE)
    {
        if ((g_eg_status.play_status == StopSta) || (g_eg_status.play_status == PauseSta))
        {
            ret_vals = _play(); //续播
            if (ret_vals == TRUE)
            {
                g_eg_status.play_status = PlaySta;
            }
        }

        g_stop_by_tts_flag = FALSE;
    }
    else
    {
#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
        if (get_mmm_state() != MMM_STATE_KT_PLAYING)
        {
            com_ok_mic_idle_play();
        }
#endif
    }

    fmengine_reply_msg(cur_msg, TRUE);

    return RESULT_NULL;
}

app_result_e fmengine_enter_s2(private_msg_t *cur_msg)
{
    os_event_t *standby_sem = (os_event_t *) (cur_msg->msg.content.addr);

    //备份现场，准备进入睡眠模式
    libc_printf("engine standby enter\n");

    fm_close(FM_STANDBY_MODE);

    //回复前台，然后接着跑就会睡眠
    fmengine_reply_msg(cur_msg, TRUE);

    libc_sem_wait(standby_sem, 0);

    //从睡眠模式回来，恢复现场
    libc_printf("engine standby exit\n");

    if (fm_module_open_do(&g_init_para_backup) == FALSE)
    {
        libc_printf_warning("fm module reopen fail!!\n");
    }
    else
    {
        fm_set_freq(g_fmengine_status.FM_CurrentFreq);
        libc_printf("exit S2 set FM freq:%d\n", g_fmengine_status.FM_CurrentFreq);

        fm_mute(releaseMUTE);
    }

    standby_restart_all_app_timer(THIS_APP_TIMER_GROUP);

    return RESULT_NULL;
}

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1) && (SUPPORT_RADIO_RECORD_FUNC == 1))

bool _record_create_file(void)
{
    uint8 disk = DISK_DEFAULT;
    record_fs_err_type_e common_err_ret;

    if (((g_eg_status.record_sta == RECORD_STA_RECORDING) || (g_eg_status.record_sta == RECORD_STA_PAUSE))
    	&& (g_record_param.file_handle != NULL))
    {
        return TRUE; //已经创建
    }

    //创建文件
    common_err_ret = com_record_disk_init(disk);
    if (common_err_ret == REC_FS_ERR_NONE)
    {
        common_err_ret = com_record_create_file(FILE_FIRST, &g_record_param);
    }
    if (common_err_ret != REC_FS_ERR_NONE)
    {
        libc_printf_error("common_err_ret = %d\n", common_err_ret);
        com_record_disk_exit();
        return FALSE;
    }

    return TRUE;
}

bool _record_close_file(void)
{
    if (g_eg_status.record_sta != RECORD_STA_STOP)
    {
        com_record_close_file(FILE_FIRST);
        com_record_disk_exit();
    }

    return TRUE;
}

app_result_e fmengine_record_start(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    if ((g_eg_status.play_status == StopSta) || (g_eg_status.play_status == PauseSta))
    {
        ret_vals = FALSE;
    }
    else
    {
        ret_vals = _record_create_file();
        if (ret_vals == TRUE)
        {
            ret_vals = _record_start(1, RECORD_SOURCE_LR_MIX);
            if (ret_vals == TRUE)
            {
                g_eg_status.record_sta = RECORD_STA_RECORDING;
            }
        }
        else
        {
            ret_vals = FALSE;
        }

        if (ret_vals == FALSE)
        {
            g_eg_status.record_sta = RECORD_STA_ERROR;
        }
    }
    fmengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

app_result_e fmengine_record_stop(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    if ((g_eg_status.play_status == StopSta) || (g_eg_status.play_status == PauseSta))
    {
        ret_vals = FALSE;
    }
    else
    {
        ret_vals = _record_stop();
        _record_close_file();
        g_eg_status.record_sta = RECORD_STA_STOP;
    }

    fmengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

#endif

