/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������Ƶ����ǰ̨Ӧ����ڣ���ʼ�����˳����̡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "app_linein.h"

//globle variable

//��ǰ����״̬��Ϣ
lineineg_status_t g_linein_status;
play_status_e g_last_linein_status;
bool g_linein_tws_flag;

uint8 g_linein_play_mode;

#if (SUPPORT_RECORD_FUNC == 1)
uint32 rec_time_bak;//������һ����ͣ¼��ʱ�䣬ÿ�λָ�¼�����м���ϱ���¼��ʱ�䶼�Ǵ�0��ʼ����ͣ¼��ʱ����
uint32 rec_time_cur;//���λָ�¼����ʱ�������ͣ¼��ʱ��0
uint32 rec_time_display;//�����ʾ¼��ʱ��

//0-ȫ��ɨ��;1-32M;2-64M;3-128M;4-256M;5-512M;6-1024M(1G);7-1536M(1.5G);8-2048M(2G)
const uint32 rec_scan_space_size_map[9] =
    { 0xffffffff, 0x10000, 0x20000, 0x40000, 0x80000 ,0x100000,0x200000,0x300000,0x400000};
#endif

/*main��Ҫ������ֵ����ϵͳ��Ա����*/
INT8U prio = AP_FRONT_LOW_PRIO;

/******************************************************************************/
/*!
 * \par  Description:
 * \void _load_cfg(void)
 * \��ȡ������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
void _load_cfg(void)
{
#if (SUPPORT_RECORD_FUNC == 1)
    sys_global_info_t* p_sys_global_info;
    p_sys_global_info = sys_get_system_info(GET_SYS_GLOBAL_DATA_INFO);
    p_sys_global_info->fs_scan_space = rec_scan_space_size_map[(uint8) com_get_config_default(RECORD_SCAN_MAX_SPACE)];
#endif
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _save_cfg(void)
 * \����������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
void _save_cfg(void)
{
    /*����VM����*/
    com_save_sys_comval();
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_init(void)
 * \��ȡ������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_main
 * \note
 * \li  ��ʼ���ļ�ѡ����ʱ����Ĭ���̷�Ϊ���̣��ļ�����ΪCOMMONDIR
 * \li  �����̨��music���Ż��browser�Ƚ��룬ͬ��location
 * \li  music uiĬ��û���̷����䣬��playlist������˳����̷�д��browser VM����
 */
/*******************************************************************************/
bool _app_init(void)
{
    //��ȡ������Ϣ (����vm��Ϣ)
    _load_cfg();

    //��ʼ��applib�⣬ǰ̨AP
    applib_init(APP_ID_LINEIN, THIS_APP_TYPE);

    //��ʼ����ʱ��
    init_app_timers(THIS_APP_TIMER_GROUP, 3);

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    g_linein_tws_enable = FALSE;
    if ((com_get_config_default(BTMANAGER_ENABLE_TWS) == TRUE)
        && (com_get_config_default(SETTING_APP_SUPPORT_BT_INBACK) == 1))
    {
        if ((g_linein_play_mode == LINEIN_MODE_AUX) && (com_get_config_default(LINEIN_ENABLE_TWS_MODE) == TRUE))
        {
            g_linein_tws_enable = TRUE;
        }
        #if (0)
        if (g_linein_play_mode == LINEIN_MODE_SPDIF)
        {
            g_linein_tws_enable = TRUE;
        }
        #endif
    }
#endif

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_deinit(void)
 * \�˳�app�Ĺ��ܺ��� ����������Ϣ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
bool _app_deinit(void)
{
    com_led_light_off(LED_ID_STATUS, LED_SCENE_MAIN);
    com_speaker_off(SPK_PLAY);

    //ִ��applib���ע������
    applib_quit(THIS_APP_TYPE);

    //save config дvram
    _save_cfg();

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int main(int argc  const char *argv[])
 * \app��ں���
 * \param[in]    argc  para1
 * \param[in]    argc  para2
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
int main(int argc, const void *argv[])
{
    app_result_e retval;

    g_linein_play_mode = (uint8) (uint32) argv[0];

    //��ʼ��
    if (_app_init() == TRUE)
    {
        com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);
        com_speaker_on(SPK_PLAY);
        retval = scene_linein_play();
    }
    else
    {
        //��ʾ����(��ʾERRO)
        retval = RESULT_NEXT_FUNCTION;
    }

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (g_linein_tws_enable == TRUE)
    {
        if ((com_ap_switch_ask_next_func(retval) != APP_FUNC_BTCALL) && (com_ap_switch_ask_next_func(retval) != APP_FUNC_ALARM))
        {
            tws_dev_reconn_phone_policy_t param;
            param.tws_paired_src_reconn_phone_enable = 1;
            param.cancel_cur_create_connection = 0;
            com_btengine_connect_deal(CON_DEAL_SET_TWS_RECONN_PHONE_POLICY, &param);
        }
    }
#endif

    com_ap_switch_deal(retval);

    //�˳�
    _app_deinit();

    return 0;
}
