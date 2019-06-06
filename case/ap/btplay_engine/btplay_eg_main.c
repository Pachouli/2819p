/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �����������Ƹ�����Ӧ����ڣ���ʼ�����˳����̡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "app_btplay_engine.h"

//globle variable
//�����м�����
void *bp_handle = NULL;
mmm_bp_status_t bp_status;
btengine_info_t btplay_bt_info;

btplay_info_t g_btplay_info;

/*main��Ҫ������ֵ����ϵͳ��Ա����*/
INT8U prio = AP_BACK_LOW_PRIO;

bool g_filter_by_tts_flag;
bool g_filter_by_tws_flag;
bool g_filter_by_tws_mode_tdb;

btplay_data_source_e g_btplay_data_source;
btplay_data_sub_source_e g_btplay_data_sub_source;

uint8 g_bt_tws_enable;

bool g_lowenergy_poweroff_enable;

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_BT_RECORD_FUNC == 1) && (SUPPORT_BTPLAY_RECORD_FUNC == 1))
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
    applib_init(APP_ID_BTPLAY_EG, THIS_APP_TYPE);

    //��ʼ����ʱ��
    init_app_timers(THIS_APP_TIMER_GROUP, 3);

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    g_bt_tws_enable = FALSE;
    if (com_get_config_default(BTMANAGER_ENABLE_TWS) == TRUE)
    {
        g_bt_tws_enable = TRUE;
    }

    if (g_bt_tws_enable == TRUE)
    {
        btplay_eg_tws_flush_pipe();
    }
#endif

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
    uint8 btplay_eg_para = (uint8) (uint32) argv[0];
    uint8 tws_mode_tbd   = btplay_eg_para&0x1;          //bit0
    g_btplay_data_sub_source = (btplay_eg_para>>1)&0x7; //bit1~3
    g_btplay_data_source = btplay_eg_para>>4;           //bit4~7

    //��ʼ��
    _app_init();

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if ((get_tws_role() == TWS_MASTER) && (tws_mode_tbd == TRUE))
    {
        g_filter_by_tws_mode_tdb = TRUE;
    }
#endif

    //ѭ��
    btplay_engine_control_block(); //app���ܴ���

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_BT_RECORD_FUNC == 1) && (SUPPORT_BTPLAY_RECORD_FUNC == 1))
    libc_printf("record exit, update record file now\n");

    extern void btplay_engine_record_update_record_file(bool force);
    btplay_engine_record_update_record_file(TRUE);

    _record_close_file();
#endif

    //�˳�
    _app_deinit();

    return 0;
}
