/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：音频输入引擎应用应用消息处理。
 * 作者：cailizhen
 ********************************************************************************/
#include "app_usound_eg.h"

usoundeg_status_t last_play_status;
uint8 s_manual_set_play = 0;
uint8 s_manual_set_pause = 0;

void usound_eg_set_filter(uint8 filter)
{
    usound_set_cmd(SET_FILTER_FLAG, filter);
    libc_printf_info("filter:%d\n", filter);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 usoundeg_message_done(void)
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
app_result_e usoundeg_message_done_bank(private_msg_t *cur_msg)
{
    app_result_e usoundeg_result = RESULT_NULL;

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
            usoundeg_reply_msg(cur_msg, ret_val);
        }
        break;
#endif

        /*设置参数相关*/
        case MSG_SET_EQ_SYNC: //设置eq +eq curve data
        usoundeg_eq_set(cur_msg);
        break;

        case MSG_USOUNDEG_PAUSE_SYNC: //暂停播放音乐
        usoundeg_pause(cur_msg);
        break;

        case MSG_USOUNDEG_RESUME_SYNC: //恢复播放音乐(从暂停到播放)
        usoundeg_resume(cur_msg);
        //设置声音有变化标志
        //usound_eg_set_volume_chg_flag(1);
        break;

        /*即将切换到后台应用（通话和闹钟起闹），让应用备份状态，以便返回时恢复状态*/
        case MSG_READY_TO_BG_APP_SYNC:
        g_p_global_var->play_status_before = g_eg_status.play_status;
        usoundeg_reply_msg(cur_msg, TRUE);
        break;

        case MSG_APP_QUIT: //退出app
        usoundeg_result = RESULT_APP_QUIT;
        break;

        case MSG_TTS_START_SYNC: //准备进入TTS播报
        usoundeg_result = usoundeg_tts_start(cur_msg);
        break;

        case MSG_USOUNDEG_VOL_ADD_SYNC:
        usoundeg_result = usoundeg_vol_add(cur_msg);
        break;

        case MSG_USOUNDEG_VOL_SUB_SYNC:
        usoundeg_result = usoundeg_vol_sub(cur_msg);
        break;

        case MSG_USOUNDEG_NEXT_SONG_SYNC:
        usoundeg_result = usoundeg_next_song(cur_msg);
        break;

        case MSG_USOUNDEG_PREV_SONG_SYNC:
        usoundeg_result = usoundeg_prev_song(cur_msg);
        break;

        case MSG_TTS_STOP_SYNC: //TTS播报完成
        usoundeg_result = usoundeg_tts_stop(cur_msg);
        break;

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1) && (SUPPORT_USOUND_RECORD_FUNC == 1))
        case MSG_REC_START_SYNC:
        usoundeg_result = usoundeg_record_start(cur_msg);
        break;

        case MSG_REC_STOP_SYNC:
        usoundeg_result = usoundeg_record_stop(cur_msg);
        break;
#endif

        case MSG_SYSTEM_ENTER_S2_SYNC:
        usoundeg_result = usoundeg_enter_s2(cur_msg);
        break;

        default:
        libc_printf_warning("usound engine invalid msg!0x%x\n", cur_msg->msg.type);
        usoundeg_reply_msg(cur_msg, FALSE);
        break;
    }

    return usoundeg_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e usoundeg_pause(private_msg_t* msg_ptr)
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
 * \ingroup      usoundeg_message_done.c
 * \note
 */
/*******************************************************************************/
app_result_e usoundeg_pause(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    s_manual_set_pause = 1;//set pause
    s_manual_set_play = 0;
    g_start_play = FALSE;

    usound_set_cmd(SET_HID_OPERS, PAUSE_CMD);
    if (pp_handle != NULL)
    {
        ret_vals = _stop();
    }
    us_oper.s_manual_set_pause = 1;

    usoundeg_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e usoundeg_resume(private_msg_t* msg_ptr)
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
app_result_e usoundeg_resume(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;
    s_manual_set_play = 1;
    s_manual_set_pause = 0;

    g_start_play = FALSE;

    usound_set_cmd(SET_HID_OPERS, PLAY_CMD);
    if (pp_handle == NULL)
    {
        ret_vals = _play(adc_sample);
    }

    us_oper.s_manual_set_play = 1;

    usoundeg_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

//设置声音有变化标志
/*void usound_eg_set_volume_chg_flag(uint32 flag)
{
    usound_set_cmd(SET_VOLUME_FLAG, flag);
}*/

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e usoundeg_tts_start(private_msg_t* msg_ptr)
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
 * \note  //调用该接口后，要保证不再使用 0x37000 以上的空间，直到调用 usoundeg_tts_stop 为止
 */
/*******************************************************************************/
app_result_e usoundeg_tts_start(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    //先暂停播放
    if (pp_handle != NULL)
    {
        ret_vals = _stop();
    }
    if (ret_vals == TRUE)
    {
        //g_eg_status.play_status = PauseSta;
        g_stop_by_tts_flag = TRUE;
    }

    //usound驱动buffer与TTS中间件空间冲突，设置强过滤
    usound_eg_set_filter(2);

    usoundeg_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e usoundeg_tts_stop(private_msg_t* msg_ptr)
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
app_result_e usoundeg_tts_stop(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    //TTS中间件已卸载，可以取消强过滤
    usound_eg_set_filter(1);

    if (g_stop_by_tts_flag == TRUE)
    {
        if (s_manual_set_pause != 1)
        {
            if (pp_handle == NULL)
            {
                ret_vals = _play(adc_sample);
            }
        }

        if (ret_vals == TRUE)
        {
            //g_eg_status.play_status = PlaySta;
        }
        g_stop_by_tts_flag = FALSE;
    }

    usoundeg_reply_msg(msg_ptr, ret_vals);
    return RESULT_NULL;
}

app_result_e usoundeg_enter_s2(private_msg_t *cur_msg)
{
    os_event_t *standby_sem = (os_event_t *) (cur_msg->msg.content.addr);

    //备份现场，准备进入睡眠模式
    libc_printf("engine standby enter\n");

    //回复前台，然后接着跑就会睡眠
    usoundeg_reply_msg(cur_msg, TRUE);

    libc_sem_wait(standby_sem, 0);

    //从睡眠模式回来，恢复现场
    libc_printf("engine standby exit\n");

    standby_restart_all_app_timer(THIS_APP_TIMER_GROUP);

    return RESULT_NULL;
}

//更新EQ参数
app_result_e usoundeg_eq_set(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    if (pp_handle != NULL)
    {
        ret_vals = _eq_set();
    }

    usoundeg_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

app_result_e usoundeg_next_song(private_msg_t *msg_ptr)
{
    s_manual_set_pause = 0;
    if (g_stop_by_tts_flag == FALSE)
    {
        usound_set_cmd(SET_HID_OPERS, NEXT_CMD);
        usoundeg_reply_msg(msg_ptr, TRUE);
    }

    return RESULT_NULL;
}

app_result_e usoundeg_prev_song(private_msg_t *msg_ptr)
{
    s_manual_set_pause = 0;
    if (g_stop_by_tts_flag == FALSE)
    {
        usound_set_cmd(SET_HID_OPERS, PREV_CMD);
        usoundeg_reply_msg(msg_ptr, TRUE);
    }

    return RESULT_NULL;
}

app_result_e usoundeg_vol_add(private_msg_t *msg_ptr)
{
    usound_set_cmd(SET_HID_OPERS, VOL_ADD_CMD);

    usoundeg_reply_msg(msg_ptr, TRUE);
    return RESULT_NULL;
}

app_result_e usoundeg_vol_sub(private_msg_t *msg_ptr)
{
    usound_set_cmd(SET_HID_OPERS, VOL_DEC_CMD);

    usoundeg_reply_msg(msg_ptr, TRUE);
    return RESULT_NULL;
}

