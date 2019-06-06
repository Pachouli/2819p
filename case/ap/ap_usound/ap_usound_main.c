/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：
 ********************************************************************************/
#include "ap_usound.h"
/*全局变量定义*/

app_timer_t usound_app_timer_vector[2];
usoundeg_status_t g_cur_play_status;

/*main中要填充这个值，由系统人员分配*/
INT8U prio = AP_FRONT_LOW_PRIO;
uint8 g_pc_change_volume;
bool g_need_volume_sync;
usound_vol_sync_e g_volume_sync;
play_status_e g_last_usbbox_status;

/******************************************************************************/
/*!
 * \par  Description:
 *      applib初始化
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
static bool _app_init(void)
{
    /*初始化applib库，前台AP*/
    applib_init(APP_ID_USOUND, THIS_APP_TYPE);

    //初始化软定时器
    init_app_timers(THIS_APP_TIMER_GROUP, 3);

    //USB音箱音量同步：0不可调，1小机可调，2PC可调，3小机PC均可调且小机优先，4小机PC均可调且PC优先
    g_volume_sync = (uint8) com_get_config_default(INF_VOLUME_FLAG);

    return TRUE;
}

//退出app的功能函数 保存配置信息
static bool _app_deinit(void)
{
    com_led_light_off(LED_ID_STATUS, LED_SCENE_MAIN);
    com_speaker_off(SPK_PLAY);

    applib_quit(THIS_APP_TYPE);

    com_save_sys_comval();

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *      MainMenu 应用的入口函数和场景管理器模块
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

    _app_init();

    if (sys_usb_detect() == TRUE)
    {
        result = scene_usound_play();
    }
    else
    {
        result = RESULT_NEXT_FUNCTION;
    }

    com_ap_switch_deal(result);

    //退出
    _app_deinit();

    return result;
}
