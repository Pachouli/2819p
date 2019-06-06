/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������Ƶ��������Ӧ����ڣ���ʼ�����˳����̡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "app_lineineg.h"

//globle variable

//�����м�����
void *pp_handle = NULL;
mmm_pp_status_t g_pp_status;

//����״̬�����ṹ��ָ��
lineineg_status_t g_eg_status;

/*main��Ҫ������ֵ����ϵͳ��Ա����*/
INT8U prio = AP_BACK_LOW_PRIO;


//��TTS��������ͣ�������֣�TTS������������Ҫ�ָ���������
bool g_stop_by_tts_flag;

bool g_lowenergy_poweroff_enable;

bool g_need_auto_play_flag;

uint8 g_linein_play_chan;

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1))
record_param_t g_record_param;
record_module_status_t g_rm_status;
#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \int _app_init(void)
 * \��ʼ�����桢��ȡ������Ϣ����ʼ���ļ�ѡ�������м��
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval          -1 failed
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
int _app_init(void)
{
    //��ʼ��applib�⣬��̨AP
    applib_init(APP_ID_LINEIN_EG, THIS_APP_TYPE);

    //��ʼ����ʱ��
    init_app_timers(THIS_APP_TIMER_GROUP, 3);

    g_lowenergy_poweroff_enable = (uint8) com_get_config_default(LINEIN_LOWENERGY_POWEROFF_ENABLE);

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_deinit(void)
 * \�˳�app�Ĺ��ܺ���,����������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
bool _app_deinit(void)
{
    //ִ��applib���ע������
    applib_quit(THIS_APP_TYPE);

    return TRUE;
}

bool _app_play(void)
{
    bool init_fsel_ret = TRUE;

    g_eg_status.play_status = StopSta;

    //ESD RESET �Ҷϵ�״̬Ϊ StopSta/PauseSta ����������
    if ((g_p_esd_bk_info->reset_flag == 1)
        && ((g_p_esd_bk_info->play_status == StopSta) || (g_p_esd_bk_info->play_status == PauseSta)))
    {
        //ESD RESETǰ����ͣ״̬����Ҫ _play
    }
    else if ((g_p_global_var->bg_app_sta == BG_STATUS_BACK)
        && ((g_p_global_var->play_status_before == StopSta) || (g_p_global_var->play_status_before == PauseSta)))
    {
        //��绰ǰ����ͣ״̬����Ҫ _play
    }
    else if (g_need_auto_play_flag == TRUE)
    {
        init_fsel_ret = _play();
        if (init_fsel_ret == TRUE)
        {
            g_eg_status.play_status = PlaySta;
        }
    }
    g_p_global_var->bg_app_sta = BG_STATUS_NULL;

    return init_fsel_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int main(int argc, const void *argv[])
 * \app��ں���
 * \param[in]    argc  para1
 * \param[in]    argc  para2
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
int main(int argc, const void *argv[])
{
    app_result_e lineineg_result = RESULT_NULL;
    g_need_auto_play_flag = ((bool) (uint32) argv[0]) & 0x0f;
    g_linein_play_chan = ((uint8) (uint32) argv[0]) >> 4;

    //��ʼ��
    _app_init();

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1))
    if ((g_linein_play_chan == ENTER_MIC_CHAN_U) || (g_linein_play_chan == ENTER_MIC_CHAN_C))
    {
        if (_record_create_file() == FALSE)
        {
            g_need_auto_play_flag = FALSE;
            g_eg_status.record_sta = RECORD_STA_ERROR;
        }
    }
#endif

    _app_play();

    //��Ϣѭ��
    lineineg_result = lineineg_control_block();

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1))
    libc_printf("record exit, update record file now\n");
    extern void lineineg_record_update_record_file(bool force);
    lineineg_record_update_record_file(TRUE);

    _record_close_file();
#endif

    //�˳�
    _app_deinit();

    return 0;
}
