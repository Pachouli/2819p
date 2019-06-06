/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：应用管理模块接口实现，包括应用注册和初始化、应用注销、获取应用管理结构体等。
 * 作者：cailizhen
 ********************************************************************************/

#include "applib_inner.h"

extern void message_key_flush_inner(void);
extern bool com_led_force_exit_msg_show(void);

/*!
 *  \brief
 *  g_app_info_vector 全局应用信息结构体数组
 */
static app_info_t g_app_info_vector[MAX_APP_COUNT];

/*!
 *  \brief
 *  g_app_info_state_all 全局系统当前状态结构体
 */
static app_info_state_all_t g_app_info_state_all;

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    应用在 applib 模块上的注册和初始化。
 * \param[in]    app_id 应用ID，如 APP_ID_MUSIC，APP_ID_MANAGER，APP_ID_MENGINE 等
 * \param[in]    type 应用类型，参见 app_type_e 定义
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 表示注册和初始化成功。
 * \retval           FALSE 表示注册和初始化失败。
 * \ingroup      app_manager
 * \par          exmaple code
 * \code
 例子1：前台应用 music AP 进行 applib 模块注册和初始化
 applib_init_inner(APP_ID_MUSIC, APP_TYPE_GUI);

 例子2：后台引擎应用 music engine AP 进行 applib 模块注册和初始化
 applib_init_inner(APP_ID_MENGINE, APP_TYPE_CONSOLE);

 例子3：进程管理应用 manager AP 进行 applib 模块注册和初始化
 applib_init_inner(APP_ID_MANAGER, APP_TYPE_PMNG);
 * \endcode
 * \note
 * \li  在应用 main 函数入口处调用，在此之前，不许调用其他 applib 接口。
 *******************************************************************************/
bool applib_init_inner(uint8 app_id, app_type_e type)
{
    app_info_t *cur_app_info;

    if (app_id >= APP_ID_MAX)
    {
        return FALSE;
    }

    if (type == APP_TYPE_GUI)
    {
        g_app_info_state_all.app_id = app_id;
        g_app_info_state_all.sys_clk_work = FALSE;
        change_ui_status(UI_STATUS_BUSY);
        sys_adjust_clk(SYS_CLK_ON_BUSY, SET_SYS_CLK);
    }

    //上锁，禁止任务调度
    sys_os_sched_lock();

    //获取 app_info 结构体指针
    cur_app_info = &g_app_info_vector[type];

    //初始化应用信息
    libc_memset(cur_app_info, 0x0, sizeof(app_info_t));
    cur_app_info->used = 1; //标志已被分配使用
    cur_app_info->app_id = app_id;
    cur_app_info->app_type = (uint8) type;
    //cur_app_info->soft_watchdog_timeout_expires = 0;
    if (type == APP_TYPE_PMNG)
    {
        cur_app_info->mq_id = (uint8) MQ_ID_MNG;
    }
    else if (type == APP_TYPE_GUI)
    {
        cur_app_info->mq_id = (uint8) MQ_ID_DESK;
        g_app_info_state_all.app_state = APP_STATE_NO_PLAY; //前台AP注册，初始化为 NO_PLAY 状态

        //清空 gui 消息队列
        message_key_flush_inner();
        //清空 应用消息队列
        sys_mq_flush (MQ_ID_DESK);
        message_redeal_app_msg_inner(NULL);//传NULL清0
    }
    else if (type == APP_TYPE_CONSOLE)
    {
        //开启引擎设置引擎状态
        cur_app_info->mq_id = (uint8) MQ_ID_EGN;
        g_app_info_state_all.engine_type = get_engine_type_by_appid_inner(app_id);
        g_app_info_state_all.engine_state = ENGINE_STATE_NULL;

        //清空 应用消息队列
        sys_mq_flush (MQ_ID_EGN);
    }
    else //APP_TYPE_BTENGINE
    {
        cur_app_info->mq_id = (uint8) MQ_ID_BT;
        g_app_info_state_all.bt_state = BT_STATE_WORK;

        //清空 应用消息队列
        sys_mq_flush (MQ_ID_BT);
    }

    //解锁，允许任务调度
    sys_os_sched_unlock();

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    应用在 applib 模块上的注销处理。
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 表示注销成功。
 * \retval           FALSE 表示注销失败。
 * \ingroup      app_manager
 * \par          exmaple code
 * \code
 例子1：应用进行 applib 模块注销
 applib_quit_inner();
 * \endcode
 * \note
 * \li  在应用 main 函数退出处调用，在此之后，不许调用其他 applib 接口。
 *******************************************************************************/
bool applib_quit_inner(app_type_e type)
{
    app_info_t *cur_app_info;

    if (type == APP_TYPE_GUI)
    {
        sys_adjust_clk(SYS_CLK_ON_BUSY, SET_SYS_CLK);
    }

    //记录上一次退出的前台AP，以便返回
    if (type == APP_TYPE_GUI)
    {
        g_app_info_state_all.last_app_id = g_app_info_state_all.app_id;
        g_app_info_state_all.app_id = APP_ID_MAX;
        g_app_info_state_all.sys_clk_work = FALSE;
        g_app_info_state_all.app_state = APP_STATE_NO_PLAY; //前台AP注销，初始化为 NO_PLAY 状态
        app_timer_var_init(APP_TIMER_GROUP_GUI);
        com_led_force_exit_msg_show();
        g_app_msg_dispatch_handle = NULL;
    }

    else if (type == APP_TYPE_CONSOLE)
    {
        //修改后台引擎类型为 NONE
        g_app_info_state_all.engine_type = ENGINE_NULL;
        g_app_info_state_all.engine_state = ENGINE_STATE_NULL;
        app_timer_var_init(APP_TIMER_GROUP_CONSOLE);
    }
    else if (type == APP_TYPE_BTENGINE)
    {
        g_app_info_state_all.bt_state = BT_STATE_NONE;
        app_timer_var_init(APP_TIMER_GROUP_BTENGINE);
    }
    else
    {
        //manager永远不会退出
    }

    //修改 app_info 结构体为未被使用
    sys_os_sched_lock();

    cur_app_info = &g_app_info_vector[type];
    cur_app_info->used = 0;
    cur_app_info->app_id = APP_ID_MAX;

    sys_os_sched_unlock();

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    获取 app_info_t
 * \param[in]    type 应用类型，参见 app_type_e 定义
 * \param[out]   app_info_t *
 * \return       bool
 * \retval           TRUE 表示获取成功。
 * \retval           FALSE 表示获取失败。
 * \ingroup      app_manager
 *******************************************************************************/
bool applib_get_app_info_inner(app_type_e type, app_info_t *app_info)
{
    sys_os_sched_lock();
    libc_memcpy(app_info, &(g_app_info_vector[type]), sizeof(app_info_t));
    sys_os_sched_unlock();

    return TRUE;
}

//根据应用类型获取应用信息
app_info_t *applib_get_app_info_by_type(app_type_e type)
{
    return &(g_app_info_vector[type]);
}

//根据应用ID获取应用信息
app_info_t *applib_get_app_info_by_app_id(uint8 app_id)
{
    app_info_t *app;
    uint8 i;

    if (app_id >= APP_ID_MAX)
    {
        return NULL;
    }

    for (i = 0; i < MAX_APP_COUNT; i++)
    {
        app = &g_app_info_vector[i];
        if ((app->used == 1) && (app->app_id == app_id))
        {
            return app;
        }
    }

    return NULL;
}

//获取后台引擎类型
engine_type_e get_engine_type_inner(void)
{
    return g_app_info_state_all.engine_type;
}

//修改后台引擎类型
void change_engine_type(engine_type_e type)
{
    g_app_info_state_all.engine_type = type;
}

//获取蓝牙协议栈工作状态
bt_state_e get_bt_state_inner(void)
{
    return g_app_info_state_all.bt_state;
}

tws_mode_e get_tws_mode_inner(void)
{
    return g_app_info_state_all.tws_mode;
}

void change_tws_mode_inner(tws_mode_e mode)
{
    g_app_info_state_all.tws_mode = mode;
}

//获取后台引擎状态
engine_state_e get_engine_state(void)
{
    return g_app_info_state_all.engine_state;
}

//改后台引擎状态
void change_engine_state_inner(engine_state_e state)
{
    g_app_info_state_all.engine_state = state;
}

//获取前台应用 UI 状态，分正在播放UI和非播放UI
app_state_e get_app_state(void)
{
    return g_app_info_state_all.app_state;
}

//修改前台应用 UI 状态，分正在播放UI和非播放UI
void change_app_state_inner(app_state_e state)
{
    g_app_info_state_all.app_state = state;
}

//获取最近前台AP
uint8 get_last_app_id_inner(void)
{
    return g_app_info_state_all.last_app_id;
}

//获取当前前台AP
uint8 get_cur_app_id_inner(void)
{
    return g_app_info_state_all.app_id;
}

//获取 ui_status
ui_status_e get_ui_status(void)
{
    return g_app_info_state_all.ui_status;
}

//改变 ui_status
void change_ui_status(ui_status_e ui_sta)
{
    g_app_info_state_all.ui_status = ui_sta;
}

//获取 ui_status
bool get_sys_clk_work(void)
{
    return g_app_info_state_all.sys_clk_work;
}

//改变 ui_status
void change_sys_clk_work(bool sys)
{
    g_app_info_state_all.sys_clk_work = sys;
}

//获取 mmm_state
mmm_state_e get_mmm_state_inner(void)
{
    return g_app_info_state_all.mmm_state;
}

//改变 mmm_state
void change_mmm_state_inner(mmm_state_e mmm_sta)
{
    g_app_info_state_all.mmm_state = mmm_sta;
}

//获取 mmm_state
mmm_ktp_engine_t get_ktp_status_inner(void)
{
    return g_app_info_state_all.ktp_status;
}

//改变 mmm_state
void change_ktp_status_inner(mmm_ktp_engine_t ktp_sta)
{
    g_app_info_state_all.ktp_status = ktp_sta;
}


void applib_var_init(void)
{
    uint8 i;

    for (i = 0; i < MAX_APP_COUNT; i++)
    {
        libc_memset(&(g_app_info_vector[i]), 0x00, sizeof(app_info_t));
        g_app_info_vector[i].app_id = APP_ID_MAX;
    }

    libc_memset(&g_app_info_state_all, 0x00, sizeof(g_app_info_state_all));
    g_app_info_state_all.engine_type = ENGINE_NULL;
    g_app_info_state_all.engine_state = ENGINE_STATE_NULL;
    g_app_info_state_all.app_state = APP_STATE_NO_PLAY;
    g_app_info_state_all.bt_state = BT_STATE_NONE;
    g_app_info_state_all.last_app_id = APP_ID_MAX;

    g_app_info_state_all.ui_status = UI_STATUS_BUSY;
    g_app_info_state_all.sys_clk_work = FALSE;
}

/*! \endcond */

