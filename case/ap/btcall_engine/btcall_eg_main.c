/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������������ͨ������Ӧ����ڣ���ʼ�����˳����̡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "app_btcall_engine.h"

void *sp_handle;
mmm_sp_status_t sp_status;
btengine_info_t btcall_bt_info;

btcall_info_t g_btcall_info;

btcall_speech_play_para_t g_play_para_backup;

INT8U prio = AP_BACK_LOW_PRIO;

//��TTS��������ͣ�������֣�TTS������������Ҫ�ָ���������
bool g_stop_by_tts_flag;

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1) && (SUPPORT_BTCALL_RECORD_FUNC == 1))
record_param_t g_record_param;
record_module_status_t g_rm_status;
#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \void _load_cfg(void)
 * \��ȡ������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval
 * \retval
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
void _load_cfg(void)
{

}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _save_cfg(void)
 * \����������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval
 * \retval
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
void _save_cfg(void)
{

}

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
    //��ȡ������Ϣ (����vm��Ϣ)
    _load_cfg();

    //��ʼ��applib�⣬��̨AP
    applib_init(APP_ID_BTCALL_EG, THIS_APP_TYPE);

    //��ʼ����ʱ��
    init_app_timers(THIS_APP_TIMER_GROUP, 3);

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
    //save config дvram
    _save_cfg();

    //ִ��applib���ע������
    applib_quit(THIS_APP_TYPE);

    return TRUE;
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
    //��ʼ��
    _app_init();

    //��������Э��ջ���������ѿ�ʼ
    com_btengine_set_hfp_call(TRUE);

    //ѭ��
    btcall_engine_control_block(); //app���ܴ���

    //��������Э��ջ����������ֹͣ
    com_btengine_set_hfp_call(FALSE);

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1) && (SUPPORT_BTCALL_RECORD_FUNC == 1))
    libc_printf("record exit, update record file now\n");

    extern void btcall_engine_record_update_record_file(bool force);
    btcall_engine_record_update_record_file(TRUE);

    _record_close_file();
#endif

    //�˳�
    _app_deinit();

    return 0;
}
