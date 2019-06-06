/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������FM����������Ӧ��������ӿ�ʵ�֡�
 * ���ߣ�cailizhen
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

    //����
    tmp_init_para.span = p_fmeg_init_para->span;
    //���Ƶ��
    tmp_init_para.freq_low = p_fmeg_init_para->freq_low;
    //���Ƶ��
    tmp_init_para.freq_high = p_fmeg_init_para->freq_high;

    //ģ���ʼ��
    result = fm_open(&tmp_init_para);

    //��ʼ�� 0�ɹ� -1ʧ��
    if (result != -1)
    {
        //ģ���ʼ���ɹ�������work ״̬
        g_module_status = MODULE_WORK;
    }
    else
    {
        ret_vals = FALSE;
    }

    //��ʼ����FM���ھ���״̬
    g_fmengine_status.FM_Mute_Status = SetMUTE;

    return ret_vals;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fm_module_init(private_msg_t* cur_msg)
 * \FM ģ���ʼ����Ϣ���������������ִ�г�ʼ������
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval       RESULT_APP_QUIT ʧ���˳�
 * \note   FM ģ���ʼ�����ã�������PA , FM �����
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
 * \FM ģ���Standby  ��Ϣ���������������ִ��
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval       RESULT_APP_QUIT ʧ���˳�
 * \note
 */
/*******************************************************************************/
app_result_e fm_module_standby(void)
{
    int result;

    /* ��Щģ����ܴ���©�磬��δ��ʼ��ʱ����Ҫ�������standby */
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
        //����Standby 0�ɹ� -1ʧ��
        if (result != -1)
        {
            //ģ�����Standby ״̬
            g_module_status = MODULE_STANDBY;
        }
    }

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_mute_sync(private_msg_t* cur_msg)
 * \���þ������߽������
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval       RESULT_APP_QUIT ʧ���˳�
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

    //0�ɹ� -1ʧ��
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
 * \ͬ�����õ���Ƶ�㣬��ʼFM ����
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval       RESULT_APP_QUIT ʧ���˳�
 * \note:   Ƶ��ֵ�����ã�����FM ui ������Ӧ�ö�����ͳһ��
 * \ ������Ը���ģ���ת�������������
 **/
/*******************************************************************************/
app_result_e fmengine_set_freq_sync(private_msg_t *cur_msg)
{
    uint32 freq;
    int result;
    bool ret_vals = TRUE;

    freq = *(uint32 *) (cur_msg->msg.content.addr);

    result = fm_set_freq(freq);

    /* ����Ƶ��ɹ�*/ //0�ɹ� -1ʧ��
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
 * \������̨����
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
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

    if (result == -1) //0�ɹ� -1ʧ��
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
 * \���������̨
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR ����;
 * \retval       RESULT_APP_QUIT ʧ���˳�
 * \note:  ����Ƶ�����ã����ж��Ƿ���̨����̨�ظ��ɹ�
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

    if (result == -1) //0�ɹ� -1ʧ��
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
 * \��ͣ����
 * \param[in]    msg_ptr��Ϣָ��  para1
 * \param[in]    private_msg_t
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_NULL ����
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
        ret_vals = _stop(); //ֹͣ����
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
 * \�ָ�����(����ͣ�ָ�)
 * \param[in]    msg_ptr��Ϣָ��  para1
 * \param[in]    private_msg_t
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_NULL ����
 * \retval       ......
 * \ingroup      mengine_event.c
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_resume(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    //�ָ�����
    if ((g_eg_status.play_status == StopSta) || (g_eg_status.play_status == PauseSta))
    {
#if ((SUPPORT_OK_MIC == 1) && (ENABLE_OK_MIC_IDLE_MODE == 1))
        if (get_mmm_state() == MMM_STATE_KT_PLAYING)
        {
            com_ok_mic_idle_stop();
        }
#endif

        ret_vals = _play(); //����
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
 * \����eq
 * \param[in]    msg_ptr��Ϣָ��  para1
 * \param[in]    private_msg_t
 * \param[out]   msg_ptr->msg.content.addr //����eq��Ϣ��ָ��
 * \return       int the result
 * \retval       RESULT_IGNORE ����
 * \retval       ......
 * \ingroup      mengine_event_volume.c
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_set_eq(private_msg_t *msg_ptr)
{
    bool ret_vals = TRUE;

    //���õ�ǰeq��������
    if (pp_handle != NULL)
    {
        ret_vals = _eq_set();
    }

    fmengine_reply_msg(msg_ptr, ret_vals);

    return RESULT_NULL;
}
