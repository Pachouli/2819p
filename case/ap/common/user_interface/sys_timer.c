/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：COMMON 公共软定时器管理。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

/*!
 *  \brief
 *  sys_counter_timer_id 系统定时功能（包括背光，屏保，省电关机，定时关机，等）计时器 timer id
 */
static int8 sys_counter_timer_id = -1;

/*! 设备热拔插检测定时器 */
static int8 sys_hot_detect_timer_id = -1;

/*! 硬件检查定时器，-1表示为创建 */
static int8 sys_hw_timer_id = -1;

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    创建系统计时软定时器和 headbar 周期更新软定时器，由前台应用进入应用时调用。
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      misc_func
 * \note
 * \li  除了 config.app，前台应用在进入时必须调用该接口，否则背光，屏保，省电关机，
 *      定时关机，状态栏更新，等等功能将无法使用。
 *******************************************************************************/
void sys_timer_init(void)
{
    sys_hw_timer_id = sys_set_irq_timer1(hardware_timer_500ms_isr, 50);
    hardware_timer_500ms_isr(); //先主动执行一次

    sys_counter_var_init();
    sys_counter_timer_id = set_app_timer_inner(APP_TIMER_GROUP_COMMON, APP_TIMER_TAG_SYS, APP_TIMER_ATTRB_CONTROL, 500, sys_counter_handle);

    sys_hot_detect_timer_id = set_app_timer_inner(APP_TIMER_GROUP_COMMON, APP_TIMER_TAG_SYS, APP_TIMER_ATTRB_CONTROL, 200, sys_hot_detect_handle_inner);

}

/******************************************************************************/
/*!
 * \par  Description:
 *    删除系统计时软定时器和 headbar 周期更新软定时器，由前台应用退出应用时调用。
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      misc_func
 * \note
 * \li  退出应用时，需要调用 sys_counter_exit，以删除 sys_counter_init 创建的软定时器。
 *******************************************************************************/
void sys_timer_exit(void)
{
    kill_app_timer_inner (APP_TIMER_GROUP_COMMON, sys_counter_timer_id);
    sys_counter_timer_id = -1;

    kill_app_timer_inner (APP_TIMER_GROUP_COMMON, sys_hot_detect_timer_id);
    sys_hot_detect_timer_id = -1;

    sys_del_irq_timer1(sys_hw_timer_id);
}

/*! \endcond */
