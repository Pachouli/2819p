/********************************************************************************
 * 	   Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 * 						   All Rights Reserved.
 *
 * 描述：本地播歌前台应用  私有消息及系统消息处理
 ********************************************************************************/


#include "app_music.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e music_message_deal(private_msg_t *msg)
 * \处理music的私有消息和系统消息
 * \param[in]    msg消息指针  para1
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
 * \param[out]   none
 * \return       app_result_e the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control_sub.c
 * \note
 */
/*******************************************************************************/
app_result_e music_message_deal(private_msg_t *msg)
{
    app_result_e result = RESULT_NULL;

    switch (msg->msg.type)
    {
        //卡拔出
        case MSG_SD_OUT:
        result = play_disk_out_deal(DISK_H);
        break;

        case MSG_UH_OUT:
        result = play_disk_out_deal(DISK_U);
        break;

        default:
        result = com_message_box(msg->msg.type);
        break;
    }
    return result;
}

