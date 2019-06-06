/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：FM收音机引擎应用设置类接口实现。
 * 作者：cailizhen
 ********************************************************************************/

#include "app_fmengine.h"

bool fm_module_open_do(fmeg_init_para_t *p_fmeg_init_para)
{
    bool ret_vals = TRUE;
    int result;
    FM_Drv_Init_Para_t tmp_init_para;

    tmp_init_para.freq_init = p_fmeg_init_para->freq_init;
    tmp_init_para.threshold = p_fmeg_init_para->threshold;
    tmp_init_para.de_emphasis_tc = p_fmeg_init_para->de_emphasis_tc;

    //步长
    tmp_init_para.span = p_fmeg_init_para->span;
    //最低频率
    tmp_init_para.freq_low = p_fmeg_init_para->freq_low;
    //最高频率
    tmp_init_para.freq_high = p_fmeg_init_para->freq_high;

    //模组初始化
    result = fm_open(&tmp_init_para);

    //初始化 0成功 -1失败
    if (result != -1)
    {
        //模组初始化成功，处于work 状态
        g_module_status = MODULE_WORK;
    }
    else
    {
        ret_vals = FALSE;
    }

    //初始化后FM处于静音状态
    g_fmengine_status.FM_Mute_Status = SetMUTE;

    return ret_vals;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fm_module_init(private_msg_t* cur_msg)
 * \FM 模组初始化消息处理，负责调用驱动执行初始化操作
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval       RESULT_APP_QUIT 失败退出
 * \note   FM 模组初始化配置，并配置PA , FM 增益等
 */
/*******************************************************************************/
app_result_e fm_module_init(private_msg_t *cur_msg)
{
    bool ret_vals = TRUE;
    fmeg_init_para_t *p_fmeg_init_para;

    p_fmeg_init_para = (fmeg_init_para_t *)(cur_msg->msg.content.addr);
    libc_memcpy(&g_init_para_backup, p_fmeg_init_para, sizeof(fmeg_init_para_t));

    ret_vals = fm_module_open_do(p_fmeg_init_para);

    fmengine_reply_msg(cur_msg, ret_vals);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fm_module_standby(void)
 * \FM 模组进Standby  消息处理，负责调用驱动执行
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval       RESULT_APP_QUIT 失败退出
 * \note
 */
/*******************************************************************************/
app_result_e fm_module_standby(void)
{
    int result;

    /* 有些模组可能存在漏电，在未初始化时就需要让其进入standby */
    if (g_module_status != MODULE_STANDBY)
    {
        libc_printf("fm standby!\n");
        if (g_enter_mode == FM_EG_ENTER_IDLE)
        {
            result = fm_close(FM_IDLE_MODE);
        }
        else
        {
            result = fm_close(FM_STANDBY_MODE);
        }
        //进入Standby 0成功 -1失败
        if (result != -1)
        {
            //模组进入Standby 状态
            g_module_status = MODULE_STANDBY;
        }
    }

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_mute_sync(private_msg_t* cur_msg)
 * \设置静音或者解除静音
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval       RESULT_APP_QUIT 失败退出
 * \note:
 **/
/*******************************************************************************/
app_result_e fmengine_mute_sync(private_msg_t *cur_msg)
{
    FM_MUTE_e mode;
    int result = 0;
    bool ret_vals = TRUE;

    mode = *(FM_MUTE_e *)(cur_msg->msg.content.addr);
    g_fmengine_status.FM_Mute_Status = mode;

    result = fm_mute(mode);

    //0成功 -1失败
    if (result == -1)
    {
        ret_vals = FALSE;
    }

    fmengine_reply_msg(cur_msg, ret_vals);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_set_freq_sync(private_msg_t* cur_msg)
 * \同步设置单个频点，开始FM 播放
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval       RESULT_APP_QUIT 失败退出
 * \note:   频率值的设置，对于FM ui 及引擎应用而言是统一的
 * \ 具体针对各种模组的转换在驱动中完成
 **/
/*******************************************************************************/
app_result_e fmengine_set_freq_sync(private_msg_t *cur_msg)
{
    uint32 freq;
    int result;
    bool ret_vals = TRUE;

    freq = *(uint32 *) (cur_msg->msg.content.addr);

    result = fm_set_freq(freq);

    /* 设置频点成功*/ //0成功 -1失败
    if (result != -1)
    {
        g_fmengine_status.FM_CurrentFreq = freq;
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
 * \app_result_e fmengine_set_throd(private_msg_t* cur_msg)
 * \设置搜台门限
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval
 * \note:
 **/
/*******************************************************************************/
app_result_e fmengine_set_throd(private_msg_t *cur_msg)
{
    uint8 level;
    int result;
    bool ret_vals = TRUE;

    level = *(uint8 *)(cur_msg->msg.content.addr);

    result = fm_set_throd(level);

    if (result == -1) //0成功 -1失败
    {
        ret_vals = FALSE;
    }

    fmengine_reply_msg(cur_msg, ret_vals);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_set_softseek(private_msg_t* cur_msg)
 * \进行软件搜台
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval       RESULT_APP_QUIT 失败退出
 * \note:  单个频点设置，并判断是否真台，真台回复成功
 **/
/*******************************************************************************/
app_result_e fmengine_set_softseek(private_msg_t *cur_msg)
{
    int result;
    bool ret_vals = TRUE;
    uint32 freq;
    bool fake_station_flag = FALSE;

    freq = *(uint32 *)(cur_msg->msg.content.addr);

    switch (fm_clock_freq)
    {
    case 26000000:
        if ((freq == 104000) || (freq == 78000))
        {
            fake_station_flag = TRUE;
        }
        break;

    case 24000000:
        if (freq == 96000)
        {
            fake_station_flag = TRUE;
        }
        break;

    default:
        break;
    }

    if (fake_station_flag == TRUE)
    {
        result = -1;
    }
    else
    {
        result = fm_soft_seek(freq);
    }

    g_fmengine_status.FM_CurrentFreq = freq;

    if (result == -1) //0成功 -1失败
    {
        ret_vals = FALSE;
    }

    fmengine_reply_msg(cur_msg, ret_vals);

    return RESULT_NULL;
}


/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_pause(private_msg_t* msg_ptr)
 * \暂停播放
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_NULL 忽略
 * \retval       ......
 * \ingroup      lineineg_message_done.c
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_pause(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    if (g_eg_status.play_status == PlaySta)
    {
        ret_vals = _stop(); //停止播放
        if (ret_vals == TRUE)
        {
            g_eg_status.play_status = PauseSta;
        }
    }

#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
    if (get_mmm_state() != MMM_STATE_KT_PLAYING)
    {
        com_ok_mic_idle_play();
    }
#endif

    fmengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_resume(private_msg_t* msg_ptr)
 * \恢复播放(由暂停恢复)
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_NULL 忽略
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_resume(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    //恢复播放
    if ((g_eg_status.play_status == StopSta) || (g_eg_status.play_status == PauseSta))
    {
#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
        if (get_mmm_state() == MMM_STATE_KT_PLAYING)
        {
            com_ok_mic_idle_stop();
        }
#endif

        ret_vals = _play(); //播放
        if (ret_vals == TRUE)
        {
            g_eg_status.play_status = PlaySta;
        }
    }

    fmengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_set_eq(void* msg_ptr)
 * \设置eq
 * \param[in]    msg_ptr消息指针  para1
 * \param[in]    private_msg_t
 * \param[out]   msg_ptr->msg.content.addr //传送eq信息的指针
 * \return       int the result
 * \retval       RESULT_IGNORE 忽略
 * \retval       ......
 * \ingroup      mengine_event_volume.c
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_set_eq(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    //设置当前eq到解码器
    if (pp_handle != NULL)
    {
        ret_vals = _eq_set();
    }

    fmengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}
