/********************************************************************************
 * 	   Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 * 						   All Rights Reserved.
 *
 * ���������ز���ǰ̨Ӧ��  ˽����Ϣ��ϵͳ��Ϣ����
 ********************************************************************************/


#include "app_music.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e music_message_deal(private_msg_t *msg)
 * \����music��˽����Ϣ��ϵͳ��Ϣ
 * \param[in]    msg��Ϣָ��  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//ͬ����־��1B
 *                 sem_id_t sem_id; 		  //�ź���ID��1B
 *                 msg_apps_t msg;		    //��Ϣ���ݣ�6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//��Ϣ���ͣ�2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//��Ϣ���ݻ����ַ������Ϣ�Լ�Լ����4B
 *               } msg_apps_t;//˽����Ϣ���е���Ϣ�ṹ��
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
        //���γ�
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

