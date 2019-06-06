/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙拍照前台应用入口，初始化和退出流程。
 * 作者：cailizhen
 ********************************************************************************/

#include  "ap_bthid.h"

/*全局变量定义*/

uint8 last_disp_status = STATUS_NONE;
uint8 g_disp_status = STATUS_NONE;
bool g_need_draw;
int8 display_timer_id = -1;
bool display_flag;

btengine_info_t g_bthid_bt_info;

/*main中要填充这个值，由系统人员分配*/
INT8U prio = AP_FRONT_LOW_PRIO;

/******************************************************************************/
/*!
 * \par  Description:
 *    MainMenu 读取应用VM全局变量,初始化全局变量
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
 *    applib初始化
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
static void _app_init(void)
{
    /*读入VM变量*/
    _read_var();

    /*初始化applib库，前台AP*/
    applib_init(APP_ID_BTHID, THIS_APP_TYPE);

    /*初始化软定时器*/
    init_app_timers(THIS_APP_TIMER_GROUP, 3);

}

/******************************************************************************/
/*!
 * \par  Description:
 *    applib退出
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

    /*执行applib库的注销操作*/
    applib_quit(THIS_APP_TYPE);
}

void _kill_conflict_engine(void)
{
    engine_type_e engine_type;
    msg_apps_t msg;

    //查看当前引擎是否为music
    engine_type = get_engine_type();
    if (engine_type != ENGINE_NULL)
    {
        //注销radio后台
        msg.type = MSG_KILL_APP_SYNC;
        msg.para[0] = APP_ID_THEENGINE;
        //发送同步消息
        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    MainMenu 应用的入口函数和场景管理器模块
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

    /*ap初始化*/
    _app_init();

    last_disp_status = STATUS_NONE;

    //确保杀死其他引擎
    _kill_conflict_engine();

    //进入独立HID拍照模式
    g_p_global_var->only_hid_mode = 1;

    com_set_mute (FALSE); //解除静音

    //语音播报进入蓝牙
    com_tts_state_play(TTS_ENTER_PHOTO, 0);

    //杀死其他模式 bt engine
    if (get_bt_state() == BT_STATE_WORK)
    {
        com_close_bt_engine();
    }

    //创建 bt engine
    com_creat_bt_engine(0);

    result = get_message_loop();

    //杀死 bt engine
    com_close_bt_engine();

    //退出独立HID拍照模式
    g_p_global_var->only_hid_mode = 0;

    //create new app
    com_ap_switch_deal(result);

    _app_exit();

    return result;
}
