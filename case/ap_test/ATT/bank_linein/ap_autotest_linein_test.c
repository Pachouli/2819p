/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：自动化测试：LINEIN 通道测试和播放测试。
 * 作者：zhouxl
 ********************************************************************************/

#include "ap_test_head.h"
#include "linein_common.h"
#include "ap_autotest_main_entry.h"
#include "bank_common/ap_autotest_common_entry.h"


/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_param(void *param, uint32 msg_type)
 * \ 获取引擎相关参数函数
 * \param[in]    input_param    输入参数指针
 * \param[in]    msg_type       消息类型
 * \param[out]   output_param   输出参数指针
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool linein_send_msg(void *input_param, void *output_param, uint32 msg_type)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //返回消息内容存放变量
    msg_reply_t reply;

    //存放输入参数的地址
    msg.content.addr = input_param;

    //存放获取参数的地址
    reply.content = output_param;

    //消息类型(即消息名称)
    msg.type = msg_type;

    //发送同步消息
    bret = send_sync_msg(APP_ID_LINEIN_EG, &msg, &reply, 0);

    if (reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;
    }
    return bret;
}




/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_status(mengine_status_t* play_status)
 * \获取当前播放状态
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool linein_get_status(lineineg_status_t *play_status)
{
    bool bret;
    bret = linein_send_msg(NULL, play_status, MSG_LINEINEG_GET_STATUS_SYNC);
    return bret;
}


/******************************************************************************/
/*!
 * \par  Description:
 * \  处理按键消息
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \li
 */
/*******************************************************************************/
app_result_e deal_gui_auxplay(input_gui_msg_t *gui_msg)
{
    app_result_e app_ret;

    app_ret = RESULT_NULL;

    switch (gui_msg->data.kmsg.val)
    {
        case KEY_PLAY:
        case KEY_PREV:
        case KEY_NEXT:
        default:
        app_ret = RESULT_APP_QUIT;
        break;
    }

    com_filter_key_hold();

    return app_ret;
}




/******************************************************************************/
/*!
 * \par  Description:
 * \uint6 music_scene_playing(void)
 * \??????
 * \param[in]    void
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ??
 * \retval       ......
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/
bool linein_scene_playing(void)
{
    bool result = TRUE;
    uint32 bt_test_stamp;
    input_gui_msg_t gui_msg;
    
    //当前播放状态信息
    lineineg_status_t linein_status;

    bt_test_stamp = sys_get_ab_timer();
    while (1)
    {
        //获取状态信息
        linein_get_status(&linein_status);

        if (TRUE == get_gui_msg(&gui_msg))
        {
            if(RESULT_APP_QUIT == deal_gui_auxplay(&gui_msg))
            {
                break;
            }
        }

        //挂起1ms,多任务交互
        sys_os_time_dly(1);

        //限制测试时间,超过此事件强制退出
        if ((sys_get_ab_timer() - bt_test_stamp) > 5 * 1000)
        {
            break;
        }
    }

    return result;
}


int32 act_test_linein_play_test_ext(void *arg_buffer, uint32 test_mode, uint8* log_buffer, uint32* cur_log_len)
{
    bool result;

    //play tts
    com_tts_state_play(TTS_PLAY_LINEIN, 0);

    //linein引擎的aux模式参数
    bool play = TRUE;
    uint8 para = play | (ENTER_AUX_CHAN<<4);

    //创建MUSIC引擎
    result = CREATE_APP_ENGINE(APP_ID_LINEIN_EG, para);

    if (FALSE != result)
    {
        result = linein_scene_playing();
    }

    KILL_CURRENT_APP_ENGINE();
    LOCAL_REPORT_RESULT_EXT("linein test:", TESTID_LINEIN_TEST, result, test_mode, log_buffer, cur_log_len);

    return result;
}





