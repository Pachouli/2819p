/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：软定时器相关宏定义，枚举类型，结构体定义，外部接口声明等。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _APP_TIMER_H_
#define _APP_TIMER_H_

#include "psp_includes.h"
#include "case_type.h"
#include "common_api_def.h"

/*!
 * \brief
 *  应用级定时器所属场景 tag id，用于把定时器捆绑于某个场景，更好地管理应用下所有定时器。
 */
/*! 主场景 tag */
#define APP_TIMER_TAG_MAIN          0x00
/*! 子场景 tag 起始ID */
#define APP_TIMER_TAG_SUB           0x80
/*! 系统场景 tag，虚拟场景，任何场景下都会执行 */
#define APP_TIMER_TAG_SYS           0xff

/*!
 * \brief
 *  timer_type_e 软定时器类型枚举结构
 */
typedef enum
{
    /*! 普通模式的定时器，自动周期发送 */
    TIMER_TYPE_NORMAL,
    /*! 单发定时器，只定时一次后进入TIMER_STATE_STOPED 状态 */
    TIMER_TYPE_SINGLE_SHOT,
} timer_type_e;

/*!
 * \brief
 *  timer_state_e 软定时器状态枚举结构
 */
typedef enum
{
    /*! 标志可用状态，可用来创建定时器 */
    TIMER_STATE_NOUSED = 0,
    /*! 正常计时状态 */
    TIMER_STATE_RUNNING = 1,
    /*! 停止计时状态，只能通过restart重新开始计时 */
    TIMER_STATE_STOPED = 2,
} timer_state_e;

/*!
 * \brief
 *  timer_attrb_e 软定时器属性枚举结构，用来对软定时器资源进行初始分配
 */
typedef enum
{/*! NOTE: 该属性废弃掉 */
    /*! UI 类软定时器，在背光熄灭后不再执行 ISR */
    APP_TIMER_ATTRB_UI,
    /*! control 类软定时器，任何时候都会执行 ISR */
    APP_TIMER_ATTRB_CONTROL,
} timer_attrb_e;

/*!
 * \brief
 *  timer_proc 软定时器服务例程
 */
typedef void (*timer_proc)(void);

/*!
 * \brief
 *  app_timer_t 软定时器管理结构体
 */
typedef struct
{
    /*! 软定时器状态，见timer_state_e 定义，TIMER_STATE_NOUSED表示可用 */
    uint8 state;
    /*! 软定时器特性，见timer_attrb_e 定义 */
    uint8 attrb;
    /*! 软定时器 定时周期，单位为 1ms */
    uint16 timeout;
    /*! 软定时器类型，见timer_type_e 定义 */
    uint8 type;
    /*! 软定时器场景标志 tag */
    uint8 tag;
    uint8 reserve[2];
    /*! 软定时器 下一次超时绝对时间点，单位为 1ms */
    uint32 timeout_expires;
    /*! 软定时器 服务例程 */
    timer_proc func_proc;
} app_timer_t;

/*! 软定时器分组ID */
typedef enum
{
    /*! 进程管理器 */
    APP_TIMER_GROUP_PMNG = 0,
    /*! 前台应用（gui进程） */
    APP_TIMER_GROUP_GUI = 1,
    /*! 后台应用（console进程） */
    APP_TIMER_GROUP_CONSOLE = 2,
    /*! 后台应用（BTENGINE进程） */
    APP_TIMER_GROUP_BTENGINE = 3,
    /*! COMMON 模块*/
    APP_TIMER_GROUP_COMMON = 4,
    /*! 分组总数 */
    APP_TIMER_GROUP_MAX,
} app_timer_group_id_e;


//接口声明
#if 0
extern int8 set_app_timer_inner(app_timer_group_id_e group_id, uint8 tag, timer_attrb_e attrb, uint16 timeout, timer_proc func_proc);
#endif
#define set_app_timer(a, b, c, d, e)              ((int8(*)(app_timer_group_id_e , uint8 , timer_attrb_e , uint16 , timer_proc ))COM_SET_APP_TIMER)((a),(b),(c),(d),(e))

#if 0
extern int8 set_single_shot_app_timer_inner(app_timer_group_id_e group_id, uint8 tag, timer_attrb_e attrb, uint16 timeout, timer_proc func_proc);
#endif
#define set_single_shot_app_timer(a, b, c, d, e)  ((int8(*)(app_timer_group_id_e , uint8 , timer_attrb_e , uint16 , timer_proc ))COM_SET_SINGLE_SHOT_APP_TIMER)((a),(b),(c),(d),(e))

#if 0
extern bool modify_app_timer_inner(app_timer_group_id_e group_id, int8 timer_id, uint16 timeout);
#endif
#define modify_app_timer(a, b, c)                 ((bool(*)(app_timer_group_id_e , int8 , uint16 ))COM_MODIFY_APP_TIMER)((a),(b),(c))

#if 0
extern bool stop_app_timer_inner(app_timer_group_id_e group_id, int8 timer_id);
#endif
#define stop_app_timer(a, b)                      ((bool(*)(app_timer_group_id_e , int8 ))COM_STOP_APP_TIMER)((a),(b))

#if 0
extern bool restart_app_timer_inner(app_timer_group_id_e group_id, int8 timer_id);
#endif
#define restart_app_timer(a, b)                   ((bool(*)(app_timer_group_id_e , int8 ))COM_RESTART_APP_TIMER)((a),(b))

#if 0
extern bool standby_restart_all_app_timer_inner(app_timer_group_id_e group_id);
#endif
#define standby_restart_all_app_timer(a)          ((bool(*)(app_timer_group_id_e ))COM_STANDBY_RESTART_ALL_APP_TIMER)((a))

#if 0
extern bool kill_app_timer_inner(app_timer_group_id_e group_id, int8 timer_id);
#endif
#define kill_app_timer(a,b)                       ((bool(*)(app_timer_group_id_e , int8 ))COM_KILL_APP_TIMER)((a),(b))

#if 0
extern bool init_app_timers_inner(app_timer_group_id_e group_id, uint8 count);
#endif
#define init_app_timers(a,b)                      ((bool(*)(app_timer_group_id_e , uint8 ))COM_INIT_APP_TIMERS)((a),(b))

#if 0
extern void change_app_timer_tag_inner(uint8 tag);
#endif
#define change_app_timer_tag(a)                   ((void(*)(uint8))COM_CHANGE_APP_TIMER_TAG)((a))

#endif

