/********************************************************************************
 * 	   Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 * 						   All Rights Reserved.
 *
 * 描述：本地播歌引擎应用 引擎获取播放状态
 ********************************************************************************/

#include "app_mengine.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_get_filepath(void* msg_ptr)
 * \获取当前播放文件的location信息
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   msg_ptr->msg.content.addr //存放文件路径信息
 * \return       int the result
 * \retval       RESULT_NULL 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_get_filepath(private_msg_t *msg_ptr)
{
    //uint8 location_size;
    //返回路径的指针
    file_path_info_t *locat_info = (file_path_info_t *) (msg_ptr->reply->content);

    //是目录模式
    //location_size = sizeof(file_location_t);

    libc_memcpy(&(locat_info->file_path), &(g_eg_cfg_p->location), sizeof(file_location_t));

    locat_info->file_source = g_eg_cfg_p->fsel_type; //当前文件模式

    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_get_status(void* msg_ptr)
 * \获取当前播放状态
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   msg_ptr->msg.content.addr //存放状态信息
 * \return       int the result
 * \retval       RESULT_NULL 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_get_status(private_msg_t *msg_ptr)
{
    //copy当前播放状态到消息指针指向的数据区
    libc_memcpy(msg_ptr->reply->content, g_eg_status_p, sizeof(mengine_status_t));

    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_get_playmode(void* msg_ptr)
 * \获取循环模式
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   msg_ptr->msg.content.addr //传送循环方式的指针
 * \return       int the result
 * \retval       RESULT_NULL 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_get_playmode(private_msg_t *msg_ptr)
{
    //保存shuffle模式+循环模式
    *(uint8 *) (msg_ptr->reply->content) = (uint8)(g_eg_cfg_p->shuffle_flag | g_eg_cfg_p->repeat_mode);

    //返回成功
    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_get_playinfo(void* msg_ptr)
 * \获取当前播放时间和比特率
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体

 * \param[out]   msg_ptr->msg.content.addr //存放播放时间和比特率
 * \return       int the result
 * \retval       RESULT_NULL 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_get_playinfo(private_msg_t *msg_ptr)
{
    //copy当前播放时间和比特率到消息指针指向的数据区
    libc_memcpy(msg_ptr->reply->content, g_eg_playinfo_p, sizeof(mengine_playinfo_t));

    //清除文件切换标志
    g_eg_playinfo_p->cur_file_switch &= (uint8)(0xfc);

    mengine_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}
