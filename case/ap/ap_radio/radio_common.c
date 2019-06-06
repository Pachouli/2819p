/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：FM收音机前台应用公共函数实现。
 * 作者：cailizhen
 ********************************************************************************/

#include "app_radio.h"

//cancel search
const key_map_t cancel_key_map_list[] =
{
    { { KEY_NEXT_VOLADD, 0, KEY_TYPE_DOWN }, EVENT_RADIO_SEARCH_EXIT },
    { { KEY_PREV_VOLSUB, 0, KEY_TYPE_DOWN }, EVENT_RADIO_SEARCH_EXIT },
    { { KEY_NEXT, 0, KEY_TYPE_DOWN }, EVENT_RADIO_SEARCH_EXIT },
    { { KEY_PREV, 0, KEY_TYPE_DOWN }, EVENT_RADIO_SEARCH_EXIT },
    { { KEY_PLAY, 0, KEY_TYPE_DOWN }, EVENT_RADIO_SEARCH_IGNORE }, //忽略PLAY DOWN，暂不退出
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_SEARCH_EXIT }, //长按PLAY会关机
    { { KEY_MODE, 0, KEY_TYPE_DOWN }, EVENT_RADIO_SEARCH_IGNORE }, //忽略MODE DOWN，暂不退出
    { { KEY_MODE, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RADIO_SEARCH_EXIT }, //短按Mode会切换应用
    { { KEY_F1, 0, KEY_TYPE_DOWN }, EVENT_RADIO_SEARCH_EXIT },
    { { KEY_VADD, 0, KEY_TYPE_DOWN }, EVENT_RADIO_SEARCH_EXIT },
    { { KEY_VSUB, 0, KEY_TYPE_DOWN }, EVENT_RADIO_SEARCH_EXIT },
    { { KEY_NUMBER, 0, KEY_TYPE_DOWN }, EVENT_RADIO_SEARCH_EXIT },

    /*! 结束标志 */
    { { KEY_NULL_NO_SHORTCUT, 0, KEY_TYPE_NULL }, MSG_NULL }
};

/******************************************************************************/
/*!
 * \par  Description:
 * \RadioUI  获取私有消息的处理
 * \param[in]    msg  私有消息结构
 * \param[out]   none
 * \return  app_result_e 结果消息
 * \note RadioUI 的处理的各种私有消息的回调函数
 *******************************************************************************/
app_result_e radio_app_msg_callback(private_msg_t *pri_msg)
{
    app_result_e ret_result = RESULT_NULL;

    ret_result = com_message_box(pri_msg->msg.type);

    return ret_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e  scene_play_sysmsg(void)
 * \播放场景系统消息及私有消息处理
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e scene_play_sysmsg(void)
{
    private_msg_t play_msg;
    app_result_e msg_result = RESULT_NULL;
    if (get_app_msg(THIS_APP_TYPE, &play_msg) == TRUE)
    {
        msg_result = radio_app_msg_callback(&play_msg);
    }
    return msg_result;
}


/******************************************************************************/
/*!
 * \par  Description:
 * \uint8 whether_in_list( uint16  freq, uint16 *p_freq)
 * \判断指定频率的电台是否在当前的保存列表中
 * \param[in]    freq
 * \param[in]    p_freq
 * \param[out]   none
 * \return       当前频率在指定的列表中返回电台号，否则返回0xff
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
uint8 whether_in_list(uint32 freq, uint32 *p_freq)
{
    uint8 ret;
    uint32 *ptr;
    uint8 num;
    uint32 tmp;

    /* 当前波段电台列表*/
    ptr = p_freq;

    for (num = 0; num < MAX_STATION_COUNT; num++)
    {
        tmp = *(ptr + num);
        if (tmp == freq)
        {
            //在列表中已找到
            break;
        }
    }

    //在列表中未找到
    if (num == MAX_STATION_COUNT)
    {
        ret = 0xff;
    }
    else
    {
        ret = num + 1;
    }

    return ret;
}

bool radio_send_msg(void *input_param, void *output_param, uint32 msg_type)
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
    bret = send_sync_msg(APP_ID_THEENGINE, &msg, &reply, 0);

    if (reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_set_freq(uint32 freq)
 * \同步设置频率播放
 * \param[in]    freq  设置的频率值( AP 层)
 * \param[out]   none
 * \return
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_set_freq(uint32 freq)
{
    bool bret = TRUE;

    bret = radio_send_msg(&freq, NULL, MSG_FMENGINE_SETFREQ_SYNC);

    //for SPP, 同步电台频点
    radio_get_freq();

    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_set_mute(FM_MUTE_e mode)
 * \设置静音或解除静音
 * \param[in]    mode
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_set_mute(FM_MUTE_e mode)
{
    bool bret = TRUE;

    g_engine_status.FM_Mute_Status = mode;

    bret = radio_send_msg(&mode, NULL, MSG_FMENGINE_SETMUTE);

    return bret;
}


/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_set_throd(uint8 val)
 * \设置搜台门限
 * \param[in]    val 电台门限值
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_set_throd(uint8 val)
{
    bool bret = TRUE;

    bret = radio_send_msg(&val, NULL, MSG_FMENGINE_SETTHRED);

    return bret;
}


/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_set_softseek(uint32 freq)
 * \设置软件搜台频率
 * \param[in]    freq, dir
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note   软件搜台:   如果当前频率为真台，返回TRUE
 */
/*******************************************************************************/
bool radio_set_softseek(uint32 freq, uint8 dir)
{
    bool bret = TRUE;

    bret = radio_send_msg(&freq, NULL, MSG_FMENGINE_SET_SOFTSEEK);

    //for SPP, 同步电台频点
    radio_get_freq();

    return bret;
}


/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_get_freq(void)
 * \获取当前频率信息
 * \param[in]     none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_get_freq(void)
{
    bool bret = TRUE;

    bret = radio_send_msg(NULL, &g_engine_status.FM_CurrentFreq, MSG_FMENGINE_GET_FREQ);

    return bret;
}

//播放电台
bool radio_resume(void)
{
    bool bret = TRUE;

    com_speaker_on(SPK_PLAY);

    bret = radio_send_msg(NULL, NULL, MSG_FMENGINE_RESUME_SYNC);

    return bret;
}

//暂停电台
bool radio_pause(void)
{
    bool bret = TRUE;

    bret = radio_send_msg(NULL, NULL, MSG_FMENGINE_PAUSE_SYNC);

    return bret;
}

//获取引擎状态
bool radio_get_play_status(fmeg_status_t *p_fmeg_status)
{
    bool bret = TRUE;

    bret = radio_send_msg(NULL, p_fmeg_status, MSG_FMENGINE_GET_PLAY_STATUS);

    return bret;
}

//获取FM收台状态
bool radio_get_fm_status(FM_Drv_Status_t *p_fm_drv_status)
{
    bool bret = TRUE;

    bret = radio_send_msg(NULL, p_fm_drv_status, MSG_FMENGINE_GET_FM_STATUS);

    return bret;
}

/*! \endcond */

