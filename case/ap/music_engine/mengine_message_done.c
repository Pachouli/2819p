/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：本地播歌引擎应用 系统消息及私有消息处理
 ********************************************************************************/

#include "app_mengine.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 mengine_message_done(void)
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
app_result_e mengine_message_done_bank(private_msg_t *cur_msg)
{
    g_mengine_result = RESULT_NULL;

    switch (cur_msg->msg.type)
    {
#if (SUPPORT_SOFT_KEYTONE == 1)
        /*请求播放软按键音*/
        case MSG_SOFT_KEYTONE_PLAY_SYNC:
        {
            bool ret_val = FALSE;
            if (mp_handle != NULL)
            {
                if (mmm_mp_cmd(mp_handle, MMM_MP_KEYTONE_PLAY, (uint8 *)(cur_msg->msg.content.addr)) == 0)
                {
                    change_ktp_status(MMM_KTP_ENGINE_PLAYING);
                    ret_val = TRUE;
                }
            }
            mengine_reply_msg(cur_msg, ret_val);
        }
        break;
#endif

        /*设置参数相关*/
        case MSG_SET_EQ_SYNC: //设置eq +eq curve data
        mengine_eq_set(cur_msg);
        break;

        case MSG_MENGINE_SET_PLAYMODE_SYNC: //设置循环模式+shuffle
        mengine_set_playmode(cur_msg);
        break;

        /*播放音乐相关*/
        case MSG_MENGINE_PLAY_SYNC: //开始播放音乐
        mengine_play(cur_msg);
        break;

        case MSG_MENGINE_STOP_SYNC: //停止播放音乐
        mengine_stop(cur_msg);
        break;

        case MSG_MENGINE_PAUSE_SYNC: //暂停播放音乐
        mengine_pause(cur_msg);
        break;

        case MSG_MENGINE_RESUME_SYNC: //恢复播放音乐(从暂停到播放)
        mengine_resume(cur_msg);
        break;

        /*快进退相关*/
        case MSG_MENGINE_FFWD_SYNC:
        mengine_fast_forward(cur_msg); //快进
        break;

        case MSG_MENGINE_FBWD_SYNC:
        mengine_fast_backward(cur_msg); //快退
        break;

        case MSG_MENGINE_CANCEL_FFB_SYNC:
        mengine_cancel_ffb(cur_msg); //取消快进退
        break;

        /*选择文件播放*/ //设置播放文件的路径location
        case MSG_MENGINE_SET_FILEPATH_SYNC:
        mengine_set_filepath(cur_msg);
        break;

        case MSG_MENGINE_PLAY_NEXT_SYNC: //播放下一曲
        mengine_play_next(cur_msg);
        break;

        case MSG_MENGINE_PLAY_PREV_SYNC: //播放上一曲
        mengine_play_prev(cur_msg);
        break;

        case MSG_MENGINE_FOLFER_NEXT_SYNC: //切换下一个目录
        mengine_play_folder_next(cur_msg);
        break;

        case MSG_MENGINE_FOLFER_PREV_SYNC: //切换上一个目录
        mengine_play_folder_prev(cur_msg);
        break;
        case MSG_MENGINE_GET_FILEINFO_SYNC: //获取文件格式、b/s、总时间等信息
        mengine_get_fileinfo(cur_msg);
        break;

        /*其他处理*/
        case MSG_MENGINE_CLEAR_ERR_SYNC: //清除错误号
        mengine_clear_error(cur_msg);
        break;

        case MSG_MENGINE_SET_SORTNUMBER_SYNC: //更新文件序号
        mengine_set_sortnumber(cur_msg);
        break;

        case MSG_MENGINE_GET_DISKNUMBER_SYNC: //获取当前文件序号
        mengine_get_disknumber(cur_msg);
        break;

        case MSG_MENGINE_GET_DIRNAME_SYNC:
        #ifdef DIR_NAME_SUPPORT
        mengine_get_dirname(cur_msg);
#endif
        break;
        /*获取文件名*/
        case MSG_MENGINE_GET_FILENAME_SYNC:
        mengine_get_filename(cur_msg);
        break;

        /*即将切换到后台应用（通话和闹钟起闹），让应用备份状态，以便返回时恢复状态*/
        case MSG_READY_TO_BG_APP_SYNC:
        g_p_global_var->play_status_before = g_eg_status_p->play_status;
        mengine_reply_msg(cur_msg, TRUE);
        break;

        case MSG_APP_QUIT: //退出app
        //exit_msg_loop();
        g_mengine_result = RESULT_APP_QUIT;
        break;

        case MSG_TTS_START_SYNC: //准备进入TTS播报
        g_mengine_result = mengine_tts_start(cur_msg);
        break;

        case MSG_TTS_STOP_SYNC: //TTS播报完成
        g_mengine_result = mengine_tts_stop(cur_msg);
        break;

        case MSG_SYSTEM_ENTER_S2_SYNC:
        g_mengine_result = mengine_enter_s2(cur_msg);
        break;

        default:
        libc_printf_warning("music engine invalid msg!0x%x\n", cur_msg->msg.type);
        mengine_reply_msg(cur_msg, FALSE);
        break;
    }

    return g_mengine_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_play(void* msg_ptr)
 * \开始播放
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
 * \ingroup      mengine_message_done.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_play(private_msg_t *msg_ptr)
{
    bool ret_vals;

#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
    if (get_mmm_state() == MMM_STATE_KT_PLAYING)
    {
        com_ok_mic_idle_stop();
    }
#endif

    //设置播放文件
    //ret_vals = _set_file();
    //if (ret_vals == TRUE)
    //{

    //设置文件播放
    ret_vals = _play(PLAY_NORMAL); //播放

    //}

    g_eg_playinfo_p->cur_file_switch |= 0x01;

    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_stop(void* msg_ptr)
 * \停止播放
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
app_result_e mengine_stop(private_msg_t *msg_ptr)
{
    bool ret_vals;

    ret_vals = _stop(STOP_NORMAL); //停止播放

#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
    if (get_mmm_state() != MMM_STATE_KT_PLAYING)
    {
        com_ok_mic_idle_play();
    }
#endif

    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_pause(void* msg_ptr)
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
 * \ingroup      mengine_message_done.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_pause(private_msg_t *msg_ptr)
{
    bool ret_vals;

    //ret_vals = _stop(STOP_PAUSE);//暂停播放

    ret_vals = _stop(STOP_NORMAL); //停止播放

#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
    if (get_mmm_state() != MMM_STATE_KT_PLAYING)
    {
        com_ok_mic_idle_play();
    }
#endif

    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_resume(void* msg_ptr)
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
app_result_e mengine_resume(private_msg_t *msg_ptr)
{
    bool ret_vals;

#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
    if (get_mmm_state() == MMM_STATE_KT_PLAYING)
    {
        com_ok_mic_idle_stop();
    }
#endif

    //重新开始播放, 设置文件
    //ret_vals = _set_file();
    //if (ret_vals == TRUE)
    //{

    //设置文件播放
    ret_vals = _play(PLAY_RESUME); //播放

    //}

    mengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

//更新EQ参数
app_result_e mengine_eq_set(private_msg_t *msg_ptr)
{
    bool ret = TRUE;

    if (mp_handle != NULL)
    {
        ret = _set_eq();
    }

    mengine_reply_msg(msg_ptr, ret);

    return RESULT_NULL;
}

