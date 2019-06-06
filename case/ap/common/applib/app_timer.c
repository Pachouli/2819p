/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：软定时器模块接口实现，包括创建，修改，删除，重置，handle执行，初始化等接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "applib_inner.h"

static app_timer_group_t g_app_timer_group_vector[APP_TIMER_GROUP_MAX];

//COMMON 模块内部软件定时器数组定义
#define COMMON_APP_TIMER_COUNT  8
static app_timer_t *g_p_com_app_timer_vector;

/*! \cond COMMON_API */

//软定时器初始化
void app_timer_var_init(app_timer_group_id_e group_id)
{
    if (group_id >= APP_TIMER_GROUP_MAX)
    {
        libc_memset(g_app_timer_group_vector, 0x00, sizeof(g_app_timer_group_vector));
        if (g_p_com_app_timer_vector == NULL)
        {
            g_p_com_app_timer_vector = (app_timer_t *) sys_malloc(sizeof(app_timer_t)*COMMON_APP_TIMER_COUNT);
        }
    }
    else
    {
        if (group_id != APP_TIMER_GROUP_COMMON)
        {
            if (g_app_timer_group_vector[group_id].vector != NULL)
            {
                sys_free(g_app_timer_group_vector[group_id].vector);
            }
        }
        libc_memset(&g_app_timer_group_vector[group_id], 0x00, sizeof(app_timer_group_t));
    }
}

//COMMON软定时器初始化
void app_timer_common_group_init(void)
{
    init_app_timers_inner(APP_TIMER_GROUP_COMMON, COMMON_APP_TIMER_COUNT);
}

//修改前台及COMMON软定时器场景标志
void change_app_timer_tag_inner(uint8 tag)
{
    g_app_timer_group_vector[APP_TIMER_GROUP_GUI].tag = tag;
    g_app_timer_group_vector[APP_TIMER_GROUP_COMMON].tag = tag;
}

//获取是否正在软定时器执行当中
bool get_app_timer_in_handle(app_timer_group_id_e group_id)
{
    return g_app_timer_group_vector[group_id].in_handle;
}

static inline int8 __set_timer(app_timer_group_id_e group_id, uint8 tag, timer_attrb_e attrb, timer_type_e type, uint16 timeout, timer_proc func_proc);

/******************************************************************************/
/*!
 * \par  Description:
 *    创建普通应用级定时器。
 * \param[in]    attrb 定时器属性，属于另一种分类，分 control 和 ui 类
 * \param[in]    timeout 定时器周期，以 1ms 为单位，不能为 0
 * \param[in]    func_proc 定时器服务例程，不能为 NULL
 * \param[out]   none
 * \return       int8
 * \retval           id 申请成功，返回定时器ID，范围为 0 ~ count - 1。
 * \retval           -1 申请失败，或定时器资源用完，或 timeout 为0，或 proc 为NULL。
 * \ingroup      app_timer
 * \par          exmaple code
 * \code
 例子1：创建 80ms 字符串滚屏定时器
 //滚屏定时器 ISR
 static void __scroll_string_handle(void)
 {
 ui_show_listbox(NULL, NULL, LIST_DRAW_ACTIVE_SCROLL);
 }

 int8 scroll_timer = -1;
 //创建 UI类 应用级定时器，背光亮时起作用
 scroll_timer = set_app_timer_inner(APP_TIMER_ATTRB_UI, 80, __scroll_string_handle);
 if(scroll_timer == -1)
 {
 创建失败，原因为定时器已被申请完
 }

 //删除有效应用级定时器
 if(scroll_timer != -1)
 {
 kill_app_timer_inner(scroll_timer);
 scroll_timer = -1;
 }
 * \endcode
 * \note
 * \li  普通类型的软定时器，每次定时到后，设置好下次定时时间，然后执行 ISR，自动周期执行，直到用户调用
 *      stop_app_timer_inner 或者 kill_app_timer。
 * \li  调用 set_app_timer_inner 创建定时器后，必须在不再使用时调用 kill_app_timer_inner 删除之，以免造成内存泄漏。
 * \li  timeout不能为0，func_proc不能为空，否则返回失败。
 *******************************************************************************/
int8 set_app_timer_inner(app_timer_group_id_e group_id, uint8 tag, timer_attrb_e attrb, uint16 timeout, timer_proc func_proc)
{
    return __set_timer(group_id, tag, attrb, TIMER_TYPE_NORMAL, timeout, func_proc);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    创建单发应用级定时器。
 * \param[in]    attrb 定时器属性，属于另一种分类，分 control 和 ui 类
 * \param[in]    timeout 定时器周期，以 1ms 为单位
 * \param[in]    func_proc 定时器服务例程
 * \param[out]   none
 * \return       int8
 * \retval           id 申请成功，返回定时器ID，范围为 0 ~ count - 1。
 * \retval           -1 申请失败，或定时器资源用完，或 timeout 为0，或 proc 为NULL。
 * \ingroup      app_timer
 * \par          exmaple code
 * \code
 例子1：创建 5s 超时定时器，用于启动某个事件
 //5s 超时定时器 ISR，置标志 g_timeout_flagd
 static void __timeout_handle(void)
 {
 g_timeout_flagd = TRUE;
 }

 bool g_timeout_flagd = FALSE;//全局变量
 int8 timeout_timer = -1;
 //创建 control类 应用级定时器
 timeout_timer = set_single_shot_app_timer_inner(APP_TIMER_ATTRB_UI, 5000, __timeout_handle);
 if(timeout_timer == -1)
 {
 创建失败，原因为定时器已被申请完
 }

 //删除有效应用级定时器，如果能够确保单发定时器会执行到，那么就不需要执行删除动作了
 if(timeout_timer != -1)
 {
 kill_app_timer_inner(timeout_timer);//如果在此之前单发定时器已自行删除，会返回错误，但是不会崩溃
 timeout_timer = -1;
 }
 * \endcode
 * \note
 * \li  单发应用级定时器在唯一一次 ISR 执行时就被自动销毁，用户可以不用自己销毁。
 * \li  但是这种定时器也有问题，就是用户不能完全把握定时器的生命周期，不能确定定时器
 *      是否被销毁了，这样就有可能重复创建相同的定时器。
 * \li  timeout不能为0，func_proc不能为空，否则返回失败。
 *******************************************************************************/
int8 set_single_shot_app_timer_inner(app_timer_group_id_e group_id, uint8 tag, timer_attrb_e attrb, uint16 timeout, timer_proc func_proc)
{
    return __set_timer(group_id, tag, attrb, TIMER_TYPE_SINGLE_SHOT, timeout, func_proc);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    修改应用级定时器的定时周期。
 * \param[in]    timer_id 定时器ID
 * \param[in]    timeout 更新的定时器周期，以 1ms 为单位
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 修改定时周期成功。
 * \retval           FALSE 修改定时周期失败，或找不到指定ID的定时器，或 timeout 非法。
 * \ingroup      app_timer
 * \par          exmaple code
 * \code
 例子1：修改定时器超时时间为 5s，之后重新计时
 if(modify_app_timer_inner(timer_id, 5000) == FALSE)
 {
 修改失败
 }
 * \endcode
 * \note
 * \li  修改软定时器的定时周期后，会重新开始计时。
 * \li  timerout不能为0，否则返回失败。
 * \li  timer_id 不在0 ~ count - 1区域中返回FALSE。
 *******************************************************************************/
bool modify_app_timer_inner(app_timer_group_id_e group_id, int8 timer_id, uint16 timeout)
{
    app_timer_t *tm;

    if ((timer_id == -1) || ((uint8) timer_id >= g_app_timer_group_vector[group_id].count)) //访问越界
    {
        return FALSE;
    }

    tm = &(g_app_timer_group_vector[group_id].vector[timer_id]);

    if (0 == timeout)
    {
        return FALSE;
    }

    tm->timeout = timeout;
    tm->timeout_expires = sys_get_ab_timer() + timeout;
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    停止应用级定时器计时，将定时器状态改为 TIMER_STATE_STOPED。
 * \param[in]    timer_id 定时器ID
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 停止计时成功。
 * \retval           FALSE 停止计时失败，原因为找不到指定ID的定时器。
 * \ingroup      app_timer
 * \par          exmaple code
 * \code
 例子1：停止定时器计时，之后可通过 restart_app_timer_inner 恢复
 if(stop_app_timer_inner(timer_id) == FALSE)
 {
 停止失败
 }
 * \endcode
 * \note
 * \li  停止计时后不能再恢复到停止时间点继续计时，只能调用 restart_app_timer_inner 重新计时。
 * \li  timer_id 不在0 ~ count - 1区域中返回FALSE。
 *******************************************************************************/
bool stop_app_timer_inner(app_timer_group_id_e group_id, int8 timer_id)
{
    app_timer_t *tm;

    if ((timer_id == -1) || ((uint8) timer_id >= g_app_timer_group_vector[group_id].count)) //访问越界
    {
        return FALSE;
    }

    tm = &(g_app_timer_group_vector[group_id].vector[timer_id]);

    tm->state = TIMER_STATE_STOPED;
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    重启应用级定时器计时，并将状态改为 TIMER_STATE_RUNNING。
 * \param[in]    timer_id 定时器ID
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 重启计时成功。
 * \retval           FALSE 重启计时失败，原因为找不到指定ID的定时器。
 * \ingroup      app_timer
 * \par          exmaple code
 * \code
 例子1：恢复定时器计时，定时器将从头开始计时
 if(restart_app_timer_inner(timer_id) == FALSE)
 {
 恢复计时失败
 }
 * \endcode
 * \note
 * \li  重启计时就是重新开始一个计时周期。
 * \li  timer_id 不在0 ~ count - 1区域中返回FALSE。
 *******************************************************************************/
bool restart_app_timer_inner(app_timer_group_id_e group_id, int8 timer_id)
{
    app_timer_t *tm;

    if ((timer_id == -1) || ((uint8) timer_id >= g_app_timer_group_vector[group_id].count)) //访问越界
    {
        return FALSE;
    }

    tm = &(g_app_timer_group_vector[group_id].vector[timer_id]);

    tm->state = TIMER_STATE_RUNNING;
    tm->timeout_expires = sys_get_ab_timer() + tm->timeout;
    return TRUE;
}

//软定时器STANDBY退出，重启所有定时器
bool standby_restart_all_app_timer_inner(app_timer_group_id_e group_id)
{
    app_timer_t *tm;
    uint8 i;

    for (i = 0; i < g_app_timer_group_vector[group_id].count; i++)
    {
        tm = &(g_app_timer_group_vector[group_id].vector[i]);

        if (tm->state == TIMER_STATE_RUNNING)
        {
            tm->timeout_expires = sys_get_ab_timer() + tm->timeout;
        }
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    删除应用级定时器，将定时器状态改为 TIMER_STATE_NOUSED。
 * \param[in]    timer_id 定时器ID
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 删除定时器成功。
 * \retval           FALSE 删除定时器失败，原因为找不到指定ID的定时器。
 * \ingroup      app_timer
 * \par          exmaple code
 * \code
 例子1：删除定时器
 if(kill_app_timer_inner(timer_id) == FALSE)
 {
 删除定时器失败
 }
 * \endcode
 * \note
 * \li  timer_id 不在0 ~ count - 1区域中返回FALSE。
 *******************************************************************************/
bool kill_app_timer_inner(app_timer_group_id_e group_id, int8 timer_id)
{
    app_timer_t *tm;

    if ((timer_id == -1) || ((uint8) timer_id >= g_app_timer_group_vector[group_id].count)) //访问越界
    {
        return FALSE;
    }

    tm = &(g_app_timer_group_vector[group_id].vector[timer_id]);

    tm->state = TIMER_STATE_NOUSED;
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    初始化应用级定时器管理模块，将所有定时器状态改为 TIMER_STATE_NOUSED。
 * \param[in]    count 应用层定义的软定时器数组的软定时器数目
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 初始化成功。
 * \retval           FALSE 初始化失败，原因为参数非法。
 * \ingroup      app_timer
 * \par          exmaple code
 * \note
 * \li  在应用 main 函数入口处调用，在此之前，不许调用其他 app_timer 接口。
 * \li  该接口在进入AP后，调用完 applib_init_inner 接口后调用。
 *******************************************************************************/
bool init_app_timers_inner(app_timer_group_id_e group_id, uint8 count)
{
    app_timer_t *tm;
    uint8 i;

    if (count == 0) //定时器数目不能为0
    {
        return FALSE;
    }

    if (group_id != APP_TIMER_GROUP_COMMON)
    {
        g_app_timer_group_vector[group_id].vector = (app_timer_t *) sys_malloc(sizeof(app_timer_t)*count);
    }
    else
    {
        g_app_timer_group_vector[group_id].vector = g_p_com_app_timer_vector;
    }
    g_app_timer_group_vector[group_id].count = count;

    for (i = 0; i < count; i++)
    {
        tm = &(g_app_timer_group_vector[group_id].vector[i]);
        tm->state = TIMER_STATE_NOUSED;
    }

    //初始化为 AP 场景
    g_app_timer_group_vector[group_id].tag = APP_TIMER_TAG_MAIN;

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    应用级定时器 handle 处理，在应用消息循环中循环调用，循环扫描所有定时器，执行
 *    定时溢出的定时器的服务例程 proc，并根据定时器类型进行设置。
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      app_timer
 * \note
 *******************************************************************************/
void handle_timers(app_timer_group_id_e group_id)
{
    app_timer_t *tm;
    uint32 cur_time_ms = sys_get_ab_timer(); //如果没有执行软定时器服务例程，认为当前时间不变
    uint32 next_time_ms;
    uint32 i;

    for (i = 0; i < g_app_timer_group_vector[group_id].count; i++)
    {
        tm = &(g_app_timer_group_vector[group_id].vector[i]);
        if ((tm->state == TIMER_STATE_RUNNING) //定时器正在运行才需要执行handle
                && ((tm->tag == g_app_timer_group_vector[group_id].tag) || (tm->tag == APP_TIMER_TAG_SYS))) //场景校验
        {
            next_time_ms = cur_time_ms + tm->timeout;
            if (cur_time_ms >= tm->timeout_expires) //定时时间已到
            {
                //重设软定时器超时时间
                if (tm->timeout > (cur_time_ms - tm->timeout_expires))
                {
                    tm->timeout_expires += tm->timeout; //直接在expires上加定时周期，以确保多次定时时间准确
                }
                else
                {
                    tm->timeout_expires = next_time_ms;
                }

                //单发软定时器一次执行完后 kill 掉
                if (tm->type == TIMER_TYPE_SINGLE_SHOT)
                {
                    tm->state = TIMER_STATE_NOUSED;
                }

                //调用软定时器服务例程
                if (tm->func_proc != NULL)
                {
                    g_app_timer_group_vector[group_id].in_handle = TRUE;
                    tm->func_proc();
                    g_app_timer_group_vector[group_id].in_handle = FALSE;
                }

                //执行过软定时器服务例程，重新加载当前时间，这样时间较为准确
                cur_time_ms = sys_get_ab_timer();
            }

            if (next_time_ms <= tm->timeout_expires)
            {
                //如果系统时间因某种原因落在超时时间点后面，那么必须修复该时间点
                tm->timeout_expires = next_time_ms;
            }
        }
    }
}


/*! \endcond */

/*! \cond */

//创建应用级定时器
static inline int8 __set_timer(app_timer_group_id_e group_id, uint8 tag, timer_attrb_e attrb, timer_type_e type, uint16 timeout, timer_proc func_proc)
{
    app_timer_t *tm;
    int8 timer_id = -1;
    int8 i;

    //服务例程不能为NULL
    if (func_proc == NULL)
    {
        return -1;
    }

    //定时周期不能为0
    if (0 == timeout)
    {
        return -1;
    }

    //检索符合条件的可用软定时器
    for (i = 0; i < g_app_timer_group_vector[group_id].count; i++)
    {
        tm = &(g_app_timer_group_vector[group_id].vector[i]);

        if (tm->state == TIMER_STATE_NOUSED)
        {
            timer_id = i;

            //初始化软定时器
            tm->attrb = attrb;
            tm->timeout = timeout;
            tm->timeout_expires = sys_get_ab_timer() + timeout;
            tm->func_proc = func_proc;
            tm->type = type;
            tm->tag = tag;
            tm->state = TIMER_STATE_RUNNING;
            break;
        }
    }

    if (timer_id < 0)
    {
        libc_printf_error("set app timer fail!!\n");
        while (1);
    }

    return timer_id;
}

/*! \endcond */
