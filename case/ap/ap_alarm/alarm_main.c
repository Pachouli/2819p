/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ʵ��RTCʱ����ʾ���ܡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include  "ap_alarm.h"

/*ȫ�ֱ�������*/

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
    applib_init(APP_ID_ALARM, THIS_APP_TYPE);

    /*��ʼ����ʱ��*/
    init_app_timers(THIS_APP_TIMER_GROUP, 3);

    com_speaker_on(SPK_PLAY);
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
