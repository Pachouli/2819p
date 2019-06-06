/********************************************************************************
 * 	   Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 * 						   All Rights Reserved.
 *
 * 描述：本地播歌引擎应用 播放模式处理
 ********************************************************************************/

#include "app_mengine.h"

#ifdef DIR_NAME_SUPPORT
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_get_dirname(void* msg_ptr)
 * \获取目录名信息
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

 * \param[out]   msg_ptr->msg.content.addr //传送循环方式的指针
 * \return       int the result
 * \retval       RESULT_NULL 忽略
 * \retval       ......
 * \ingroup      mengine_event_playmode.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_get_dirname(private_msg_t *msg_ptr)
{
    uint8 *dest_ptr = (uint8 *)msg_ptr->msg.content.addr;
    //dir name info
    libc_memcpy(dest_ptr, g_dir_name_buf, sizeof(g_dir_name_buf));

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_get_filename(void* msg_ptr)
 * \获取文件名信息
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

 * \param[out]   msg_ptr->msg.content.addr //传送循环方式的指针
 * \return       int the result
 * \retval       RESULT_NULL 忽略
 * \retval       ......
 * \ingroup      mengine_event_playmode.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_get_filename(private_msg_t *msg_ptr)
{
    music_file_name_t *dest_ptr = (music_file_name_t *) msg_ptr->reply->content;

    libc_memcpy(dest_ptr->fname_buffer, g_file_name, dest_ptr->fname_length);

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_fast_forward(void* msg_ptr)
 * \快进
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
 * \ingroup      mengine_event_playmode.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_fast_forward(private_msg_t *msg_ptr)
{
    //是正常播放 or 快进
    if (mp_handle != NULL)
    {
        if ((g_eg_status_p->play_status == PlaySta) || (g_eg_status_p->play_status == PlayFast))
        {
            _set_ffb(FAST_FORWARD);
            g_eg_status_p->play_status = PlayFast; //修改状态
            g_eg_status_p->fast_status = FFPlaySta;
        }
    }

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_fast_backward(void* msg_ptr)
 * \快退
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
 * \ingroup      mengine_event_playmode.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_fast_backward(private_msg_t *msg_ptr)
{
    //当前是正常播放 or 快退
    if (mp_handle != NULL)
    {
        if ((g_eg_status_p->play_status == PlaySta) || (g_eg_status_p->play_status == PlayFast))
        {
            _set_ffb(FAST_BACKWARD);
            g_eg_status_p->play_status = PlayFast; //修改状态
            g_eg_status_p->fast_status = FBPlaySta;
        }
    }

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_cancel_ffb(void* msg_ptr)
 * \取消快进退
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
 * \ingroup      mengine_event_playmode.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_cancel_ffb(private_msg_t *msg_ptr)
{
    //当前是快进退状态
    if (mp_handle != NULL)
    {
        if (((g_eg_status_p->play_status == PlayFast) && (g_eg_status_p->fast_status != FBPlay_Null))
                || (g_eg_status_p->fast_status == FFPlayTailSta)
                || (g_eg_status_p->fast_status == FBPlayHeadSta))
        {
            if ((g_eg_status_p->fast_status == FFPlaySta) || (g_eg_status_p->fast_status == FBPlaySta))
            {
                _set_ffb(CANCLE_FAST_BACKWARD_FORWARD);
            }
            g_eg_status_p->play_status = PlaySta; //修改状态
            g_eg_status_p->fast_status = FBPlay_Null;
        }
    }

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

