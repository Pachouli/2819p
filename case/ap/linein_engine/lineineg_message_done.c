/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：音频输入引擎应用应用消息处理。
 * 作者：cailizhen
 ********************************************************************************/

#include "app_lineineg.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 lineineg_message_done(void)
 * \引擎消息处理,处理引擎接收到的私有消息和系统消息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略
 * \retval       Exit_App 退出app
 * \ingroup      mengine_mesasge_done.c
 * \note
 */
/*******************************************************************************/
app_result_e lineineg_message_done_bank(private_msg_t *cur_msg)
{
    app_result_e lineineg_result = RESULT_NULL;

    switch (cur_msg->msg.type)
    {
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
            lineineg_reply_msg(cur_msg, ret_val);
        }
        break;
#endif

        /*设置参数相关*/
        case MSG_SET_EQ_SYNC: //设置eq +eq curve data
        lineineg_eq_set(cur_msg);
        break;

        case MSG_LINEINEG_PAUSE_SYNC: //暂停播放音乐
        lineineg_pause(cur_msg);
        break;

        case MSG_LINEINEG_RESUME_SYNC: //恢复播放音乐(从暂停到播放)
        lineineg_resume(cur_msg);
        break;

        /*即将切换到后台应用（通话和闹钟起闹），让应用备份状态，以便返回时恢复状态*/
        case MSG_READY_TO_BG_APP_SYNC:
        g_p_global_var->play_status_before = g_eg_status.play_status;
        lineineg_reply_msg(cur_msg, TRUE);
        break;

        case MSG_APP_QUIT: //退出app
        lineineg_result = RESULT_APP_QUIT;
        break;

        case MSG_TTS_START_SYNC: //准备进入TTS播报
        lineineg_result = lineineg_tts_start(cur_msg);
        break;

        case MSG_TTS_STOP_SYNC: //TTS播报完成
        lineineg_result = lineineg_tts_stop(cur_msg);
        break;

        case MSG_SYSTEM_ENTER_S2_SYNC:
        lineineg_result = lineineg_enter_s2(cur_msg);
        break;

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1))
        case MSG_REC_START_SYNC:
        lineineg_result = lineineg_record_start(cur_msg);
        break;

        case MSG_REC_STOP_SYNC:
        lineineg_result = lineineg_record_stop(cur_msg);
        break;

        case MSG_REC_SAVE_RESTART_NEXT_SYNC:
        lineineg_result = lineineg_record_save_restart(cur_msg);
        break;
#endif

        default:
        libc_printf_warning("linein engine invalid msg!0x%x\n", cur_msg->msg.type);
        lineineg_reply_msg(cur_msg, FALSE);
        break;
    }

    return lineineg_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e lineineg_pause(private_msg_t* msg_ptr)
 * \暂停播放
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;      //同步标志，1B
 *                 sem_id_t sem_id;           //信号量ID，1B
 *                 msg_apps_t msg;          //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;   //消息类型，2B
 *                 union
 *                 {
 *                   unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;                    //消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_NULL 忽略
 * \retval       ......
 * \ingroup      lineineg_message_done.c
 * \note
 */
/*******************************************************************************/
app_result_e lineineg_pause(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    if (g_eg_status.play_status == PlaySta)
    {
        ret_vals = _stop(); //停止播放
        if (ret_vals == TRUE)
        {
            g_eg_status.play_status = PauseSta;
        }
    }

#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
    if (get_mmm_state() != MMM_STATE_KT_PLAYING)
    {
        com_ok_mic_idle_play();
    }
#endif

    lineineg_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e lineineg_resume(private_msg_t* msg_ptr)
 * \恢复播放(由暂停恢复)
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;      //同步标志，1B
 *                 sem_id_t sem_id;           //信号量ID，1B
 *                 msg_apps_t msg;          //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;   //消息类型，2B
 *                 union
 *                 {
 *                   unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;                    //消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_NULL 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e lineineg_resume(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1))
    if ((g_linein_play_chan == ENTER_MIC_CHAN_U) || (g_linein_play_chan == ENTER_MIC_CHAN_C))
    {
        if (g_eg_status.record_sta == RECORD_STA_STOP)
        {
            if (_record_create_file() == FALSE)
            {
                g_eg_status.record_sta = RECORD_STA_ERROR;
            }
        }
    }
#endif

    //恢复播放
    if ((g_eg_status.play_status == StopSta) || (g_eg_status.play_status == PauseSta))
    {
#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
        if (get_mmm_state() == MMM_STATE_KT_PLAYING)
        {
            com_ok_mic_idle_stop();
        }
#endif

        ret_vals = _play(); //播放
        if (ret_vals == TRUE)
        {
            g_eg_status.play_status = PlaySta;
        }
    }

    lineineg_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e lineineg_tts_start(private_msg_t* msg_ptr)
 * \开始语音播报,做好互斥准备
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;      //同步标志，1B
 *                 sem_id_t sem_id;           //信号量ID，1B
 *                 msg_apps_t msg;          //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;   //消息类型，2B
 *                 union
 *                 {
 *                   unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;                    //消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_NULL 忽略
 * \retval       ......
 * \ingroup      mengine_event_param.c
 * \note  //调用该接口后，要保证不再使用 0x37000 以上的空间，直到调用 lineineg_tts_stop 为止
 */
/*******************************************************************************/
app_result_e lineineg_tts_start(private_msg_t *msg_ptr)
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

    lineineg_reply_msg(msg_ptr, ret_vals);
    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e lineineg_tts_stop(private_msg_t* msg_ptr)
 * \语音播报完成,可恢复备份
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;      //同步标志，1B
 *                 sem_id_t sem_id;           //信号量ID，1B
 *                 msg_apps_t msg;          //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;   //消息类型，2B
 *                 union
 *                 {
 *                   unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;                    //消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_NULL 忽略
 * \retval       ......
 * \ingroup      mengine_event_param.c
 * \note  //调用该接口，表示 0x37000 以上空间可以完全由music应用支配
 */
/*******************************************************************************/
app_result_e lineineg_tts_stop(private_msg_t *msg_ptr)
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

    lineineg_reply_msg(msg_ptr, ret_vals);
    return RESULT_NULL;
}

app_result_e lineineg_enter_s2(private_msg_t *cur_msg)
{
    os_event_t *standby_sem = (os_event_t *) (cur_msg->msg.content.addr);

    //如果是LINEIN支持低能量检测一段时间关机的功能，那么接收该消息时，LINEIN中间件可能仍在运行，需要先退出中间件
    if (g_eg_status.play_status == PlaySta)
    {
        if (_stop() == TRUE)
        {
            g_eg_status.play_status = PauseSta;
        }
    }

    //备份现场，准备进入睡眠模式
    libc_printf("engine standby enter\n");

    //回复前台，然后接着跑就会睡眠
    lineineg_reply_msg(cur_msg, TRUE);

    libc_sem_wait(standby_sem, 0);

    //从睡眠模式回来，恢复现场
    libc_printf("engine standby exit\n");

    standby_restart_all_app_timer(THIS_APP_TIMER_GROUP);

    return RESULT_NULL;
}

//更新EQ参数
app_result_e lineineg_eq_set(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    if (pp_handle != NULL)
    {
        ret_vals = _eq_set();
    }

    lineineg_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1))

bool _record_create_file(void)
{
    uint8 disk = DISK_DEFAULT;
    record_fs_err_type_e common_err_ret;

    if (((g_eg_status.record_sta == RECORD_STA_RECORDING) || (g_eg_status.record_sta == RECORD_STA_PAUSE))
    	&& (g_record_param.file_handle != NULL))
    {
        return TRUE; //已经创建
    }

    if ((g_linein_play_chan == ENTER_MIC_CHAN_U) || (g_linein_play_chan == ENTER_MIC_CHAN_C))
    {
        if (g_linein_play_chan == ENTER_MIC_CHAN_U)
        {
            disk = DISK_U;
        }
        else
        {
            disk = DISK_H;
        }
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
        g_eg_status.record_sta = RECORD_STA_ERROR;
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

app_result_e lineineg_record_start(private_msg_t *msg_ptr)
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
    lineineg_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

app_result_e lineineg_record_stop(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    if ((g_linein_play_chan == ENTER_MIC_CHAN_U) || (g_linein_play_chan == ENTER_MIC_CHAN_C))
    {
        if (g_eg_status.record_sta == RECORD_STA_PAUSE)
        {
            _record_close_file();
            g_eg_status.record_sta = RECORD_STA_STOP;
            ret_vals = TRUE;
        }
        else
        {
            ret_vals = FALSE;
        }
    }
    else
    {
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
    }

    lineineg_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

bool lineineg_record_save_restart_do(void)
{
    bool ret_vals = FALSE;

    if ((g_linein_play_chan == ENTER_MIC_CHAN_U) || (g_linein_play_chan == ENTER_MIC_CHAN_C))
    {
        if (g_eg_status.play_status == PauseSta) //暂停状态也是可以开始下一次录音
        {
            libc_printf("already pause\n");
            ret_vals = TRUE;
        }
        else if (g_eg_status.play_status == PlaySta) //先pause再close record/file
        {
            ret_vals = _stop(); //停止播放
            if (ret_vals == TRUE)
            {
                libc_printf("<1> pause\n");
                g_eg_status.play_status = PauseSta;
            }
        }
        else
        {
            ;
        }

        //close record/file
        if (ret_vals == TRUE)
        {
            if (g_eg_status.record_sta == RECORD_STA_PAUSE)
            {
                _record_close_file();
                g_eg_status.record_sta = RECORD_STA_STOP;
                libc_printf("<2> save\n");
            }

            //resume
            if (g_eg_status.record_sta == RECORD_STA_STOP)
            {
                ret_vals = _record_create_file();
                libc_printf("<3> create next\n");
            }

            if (ret_vals == TRUE)
            {
                ret_vals = _play(); //播放
                if (ret_vals == TRUE)
                {
                    g_eg_status.play_status = PlaySta;
                    libc_printf("<4> resume\n");
                }
            }
        }

        if (ret_vals == FALSE)
        {
            g_eg_status.record_sta = RECORD_STA_ERROR;
        }
    }
    else
    {
        //close record/file
        if (g_eg_status.play_status == PlaySta)
        {
            ret_vals = _record_stop();
            _record_close_file();
            g_eg_status.record_sta = RECORD_STA_STOP;
            libc_printf("<1> save\n");
        }

        //restart record
        if (ret_vals == TRUE)
        {
            ret_vals = _record_create_file();
            if (ret_vals == TRUE)
            {
                ret_vals = _record_start(1, RECORD_SOURCE_LR_MIX);
                if (ret_vals == TRUE)
                {
                    g_eg_status.record_sta = RECORD_STA_RECORDING;
                    libc_printf("<2> create next\n");
                }
            }
        }

        if (ret_vals == FALSE)
        {
            g_eg_status.record_sta = RECORD_STA_ERROR;
        }
    }

    return ret_vals;
}

app_result_e lineineg_record_save_restart(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    ret_vals = lineineg_record_save_restart_do();

    lineineg_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

#endif
