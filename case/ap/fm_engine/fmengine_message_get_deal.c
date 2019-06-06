/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������FM����������Ӧ�û�ȡ��ӿ�ʵ�֡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "app_fmengine.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_get_fm_status(private_msg_t* cur_msg)
 * \��ȡ״̬��Ϣ
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_get_fm_status(private_msg_t *cur_msg)
{
    int result;
    bool ret_vals = TRUE;
    FM_Drv_Status_t fm_drv_status_tmp;

    result = fm_get_status(&fm_drv_status_tmp);

    //��ȡ״̬�ɹ�//0�ɹ� -1ʧ��
    if (result != -1)
    {
        g_fmengine_status.FM_CurrentFreq = fm_drv_status_tmp.FM_CurrentFreq;
        g_fmengine_status.FM_SignalStg = fm_drv_status_tmp.FM_SignalStg;
        g_fmengine_status.FM_Stereo_Status = fm_drv_status_tmp.FM_Stereo_Status;

        libc_memcpy(cur_msg->reply->content, &g_fmengine_status, sizeof(FM_Drv_Status_t));
    }
    else
    {
        ret_vals = FALSE;
    }

    fmengine_reply_msg(cur_msg, ret_vals);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_get_play_status(void* msg_ptr)
 * \��ȡ��ǰ����״̬
 * \param[in]    msg_ptr��Ϣָ��  para1
 * \param[in]    private_msg_t
 * \param[out]   msg_ptr->msg.content.addr //���״̬��Ϣ
 * \return       int the result
 * \retval       RESULT_NULL ����
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_get_play_status(private_msg_t *msg_ptr)
{
    libc_memcpy(msg_ptr->reply->content, &g_eg_status, sizeof(fmeg_status_t));

    fmengine_reply_msg(msg_ptr, TRUE);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_get_freq(private_msg_t* cur_msg)
 * \��ȡ��ǰƵ��
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_get_freq(private_msg_t *cur_msg)
{
    FM_Drv_Status_t tmp_Drv_Status;
    int result;
    bool ret_vals = TRUE;

    result = fm_get_status(&tmp_Drv_Status);
    if (result != -1) //0�ɹ� -1ʧ��
    {
        g_fmengine_status.FM_CurrentFreq = tmp_Drv_Status.FM_CurrentFreq;

        *(uint32 *) cur_msg->reply->content = g_fmengine_status.FM_CurrentFreq;
    }
    else
    {
        ret_vals = FALSE;
    }

    fmengine_reply_msg(cur_msg, ret_vals);

    return RESULT_NULL;
}

