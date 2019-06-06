/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ��������������ǰ̨Ӧ����ڣ���ʼ�����˳����̡�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include  "ap_bthid.h"

/*ȫ�ֱ�������*/

uint8 last_disp_status = STATUS_NONE;
uint8 g_disp_status = STATUS_NONE;
bool g_need_draw;
int8 display_timer_id = -1;
bool display_flag;

btengine_info_t g_bthid_bt_info;

/*main��Ҫ������ֵ����ϵͳ��Ա����*/
INT8U prio = AP_FRONT_LOW_PRIO;

/******************************************************************************/
/*!
 * \par  Description:
 *    MainMenu ��ȡӦ��VMȫ�ֱ���,��ʼ��ȫ�ֱ���
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
static void _read_var(void)
{
}

/******************************************************************************/
/*!
 * \par  Description:
 *    applib��ʼ��
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
static void _app_init(void)
{
    /*����VM����*/
    _read_var();

    /*��ʼ��applib�⣬ǰ̨AP*/
    applib_init(APP_ID_BTHID, THIS_APP_TYPE);

    /*��ʼ����ʱ��*/
    init_app_timers(THIS_APP_TIMER_GROUP, 3);

}

/******************************************************************************/
/*!
 * \par  Description:
 *    applib�˳�
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
void _app_exit(void)
{
    com_led_light_off(LED_ID_STATUS, LED_SCENE_MAIN);
    com_speaker_off(SPK_PLAY);

    com_save_sys_comval();

    /*ִ��applib���ע������*/
    applib_quit(THIS_APP_TYPE);
}

void _kill_conflict_engine(void)
{
    engine_type_e engine_type;
    msg_apps_t msg;

    //�鿴��ǰ�����Ƿ�Ϊmusic
    engine_type = get_engine_type();
    if (engine_type != ENGINE_NULL)
    {
        //ע��radio��̨
        msg.type = MSG_KILL_APP_SYNC;
        msg.para[0] = APP_ID_THEENGINE;
        //����ͬ����Ϣ
        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    MainMenu Ӧ�õ���ں����ͳ���������ģ��
 * \param[in]    PARAM_FROM_XXX
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
int main(int argc, const void *argv[])
{
    app_result_e result = RESULT_NULL;

    /*ap��ʼ��*/
    _app_init();

    last_disp_status = STATUS_NONE;

    //ȷ��ɱ����������
    _kill_conflict_engine();

    //�������HID����ģʽ
    g_p_global_var->only_hid_mode = 1;

    com_set_mute (FALSE); //�������

    //����������������
    com_tts_state_play(TTS_ENTER_PHOTO, 0);

    //ɱ������ģʽ bt engine
    if (get_bt_state() == BT_STATE_WORK)
    {
        com_close_bt_engine();
    }

    //���� bt engine
    com_creat_bt_engine(0);

    result = get_message_loop();

    //ɱ�� bt engine
    com_close_bt_engine();

    //�˳�����HID����ģʽ
    g_p_global_var->only_hid_mode = 0;

    //create new app
    com_ap_switch_deal(result);

    _app_exit();

    return result;
}
