/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：实现RTC时钟显示功能。
 * 作者：cailizhen
 ********************************************************************************/

#include  "ap_alarm.h"

/*全局变量定义*/

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
    applib_init(APP_ID_ALARM, THIS_APP_TYPE);

    /*初始化软定时器*/
    init_app_timers(THIS_APP_TIMER_GROUP, 3);

    com_speaker_on(SPK_PLAY);
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

    result = get_message_loop();

    if (g_p_global_var->alarm_poweron_flag == TRUE)
    {
        result = RESULT_ALARM_POWER_OFF;
    }
    else
    {
        if ((result == RESULT_NULL) || (result == RESULT_REDRAW) || (result == RESULT_IGNORE))
        {
            result = RESULT_LASTPLAY;
        }
    }

    //create new app
    com_ap_switch_deal(result);

    _app_exit();

    return result;
}
