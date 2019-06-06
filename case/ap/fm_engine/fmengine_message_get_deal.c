/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：FM收音机引擎应用获取类接口实现。
 * 作者：cailizhen
 ********************************************************************************/

#include "app_fmengine.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_get_fm_status(private_msg_t* cur_msg)
 * \获取状态信息
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
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

    //获取状态成功//0成功 -1失败
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
 * \获取当前播放状态
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 * \param[out]   msg_ptr->msg.content.addr //存放状态信息
 * \return       int the result
 * \retval       RESULT_NULL 忽略
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
 * \获取当前频率
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
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
    if (result != -1) //0成功 -1失败
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

