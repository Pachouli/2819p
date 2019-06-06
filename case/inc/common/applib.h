/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：应用管理相关宏定义，枚举类型，结构体定义，外部接口声明等。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _APPLIB_H_
#define _APPLIB_H_

#include "psp_includes.h"
#include "case_type.h"
#include "common_api_def.h"
#include "app_msg.h"
#include "app_timer.h"
#include "fw_config.h"
#include "typestruct_tws_case.h"

/*!
 * \brief
 *  app_info_t 应用信息结构体
 */
typedef struct
{
    /*! 结构体使用标志，1表示已被使用，0表示未被使用 */
    uint8 used;
    /*! 进程ID号，见 case_type.h 中定义的列表 */
    uint8 app_id;
    /*! 应用类型，见app_type_e 定义 */
    uint8 app_type;
    /*! 进程私有消息队列ID，见mq_id_e 定义 */
    uint8 mq_id;
} app_info_t;

/*!
 * \brief
 *  bt_state_e 蓝牙引擎类型枚举类型
 */
typedef enum
{
    BT_STATE_NONE = 0,
    BT_STATE_WORK = 1,
} bt_state_e;

/*TWS模式*/
typedef enum
{
    TWS_MODE_SINGLE  = 0, //单箱应用模式
    TWS_MODE_BT_TWS  = 1, //蓝牙TWS推歌
    TWS_MODE_AUX_TWS = 2, //AUX TWS推歌
} tws_mode_e;

/*!
 * \brief
 *  engine_type_e 引擎类型枚举类型，用于应答用户请求后台引擎类型
 */
typedef enum
{
    /*! 没有引擎 */
    ENGINE_NULL = 0,
    /*! music引擎 */
    ENGINE_MUSIC = 1,
    /*! fm引擎 */
    ENGINE_RADIO = 2,
    /*! LINEIN引擎 */
    ENGINE_LINEIN = 3,
    /*! 蓝牙推歌引擎 */
    ENGINE_BTPLAY   = 4,
    /*! 蓝牙打电话引擎 */
    ENGINE_BTCALL   = 5,
    /*! usb音箱引擎 */
    ENGINE_USOUND   = 6,
} engine_type_e;

/*!
 * \brief
 *  engine_state_e 引擎状态枚举类型
 */
typedef enum
{
    /*! 没有引擎 */
    ENGINE_STATE_NULL = 0,
    /*! 引擎正在播放 */
    ENGINE_STATE_PLAYING = 1,
    /*! 引擎处于暂停状态 */
    ENGINE_STATE_PAUSE = 2,
} engine_state_e;

/*!
 * \brief
 *  app_state_e 前台应用 UI 状态枚举类型
 */
typedef enum
{
    /*! 前台AP处于非播放 UI 状态 */
    APP_STATE_NO_PLAY = 0,
    /*! 前台AP处于播放 UI 状态 */
    APP_STATE_PLAYING = 1,
    /*! 前台AP处于播放 UI 状态，且希望背光常亮 */
    APP_STATE_PLAYING_ALWAYS = 2,
} app_state_e;

#define UI_IDLE_TIMER    14 //空闲状态持续7秒钟就调低频率
/*! 前台用户交互状态 */
typedef enum
{
    UI_STATUS_IDLE = 0, //表示没有UI需要处理
    UI_STATUS_BUSY = 1, //表示正在进行UI处理
} ui_status_e;

//mmm state
typedef enum
{
    MMM_STATE_NO_PLAY = 0, //没有中间件
    MMM_STATE_PLAYING = 1, //播放中间件
    MMM_STATE_TTS_PLAYING = 2, //TTS播放中间件
    MMM_STATE_KT_PLAYING = 3, //按键音通道中间件
} mmm_state_e;


//应用管理模块接口声明
#if 0
extern bool applib_init_inner(uint8 app_id, app_type_e type);
#endif
#define applib_init(a,b)             ((bool(*)(uint8, app_type_e))COM_APPLIB_INIT)((a),(b))

#if 0
extern bool applib_quit_inner(app_type_e type);
#endif
#define applib_quit(a)               ((bool(*)(app_type_e))COM_APPLIB_QUIT)((a))

#if 0
extern bool applib_get_app_info_inner(app_type_e type, app_info_t *app_info);
#endif
#define applib_get_app_info(a,b)     ((bool(*)(app_type_e, app_info_t *))COM_APPLIB_GET_APP_INFO)((a),(b))

#if 0
extern engine_type_e get_engine_type_by_appid_inner(uint8 app_id);
#endif
#define get_engine_type_by_appid(a)  ((engine_type_e(*)(uint8))COM_GET_ENGINE_TYPE_BY_APPID)((a))

#if 0
extern uint8 get_engine_appid_by_type_inner(engine_type_e engine_type);
#endif
#define get_engine_appid_by_type(a)  ((uint8(*)(engine_type_e))COM_GET_ENGINE_APPID_BY_TYPE)((a))

#if 0
extern engine_type_e get_engine_type_inner(void);
#endif
#define get_engine_type()           ((engine_type_e(*)(void))COM_GET_ENGINE_TYPE)() //获取后台引擎类型

#if 0
extern bt_state_e get_bt_state_inner(void);
#endif
#define get_bt_state()              ((bt_state_e(*)(void))COM_GET_BT_STATE)() //获取蓝牙工作状态

#if 0
extern tws_mode_e get_tws_mode_inner(void);
#endif
#define get_tws_mode()              ((tws_mode_e(*)(void))COM_GET_TWS_MODE)()

#if 0
extern void change_tws_mode_inner(tws_mode_e mode);
#endif
#define change_tws_mode(a)          ((void(*)(tws_mode_e))COM_CHANGE_TWS_MODE)((a))

#if 0
extern void change_engine_state_inner(engine_state_e state);
#endif
#define change_engine_state(a)      ((void(*)(engine_state_e))COM_CHANGE_ENGINE_STATE)((a)) //修改后台引擎状态

#if 0
extern void change_app_state_inner(app_state_e state);
#endif
#define change_app_state(a)         ((void(*)(app_state_e))COM_CHANGE_APP_STATE)((a)) //修改前台应用 UI 状态，分正在播放UI和非播放UI

#if 0
extern uint8 get_last_app_id_inner(void);
#endif
#define get_last_app_id()           ((uint8(*)(void))COM_GET_LAST_APP_ID)() //获取最后一个前台应用

#if 0
extern uint8 get_cur_app_id_inner(void);
#endif
#define get_cur_app_id()            ((uint8(*)(void))COM_GET_CUR_APP_ID)() //获取当前前台应用

#if 0
extern mmm_state_e get_mmm_state_inner(void);
#endif
#define get_mmm_state()             ((mmm_state_e(*)(void))COM_GET_MMM_STATE)()

#if 0
extern void change_mmm_state_inner(mmm_state_e mmm_sta);
#endif
#define change_mmm_state(a)         ((void(*)(mmm_state_e))COM_CHANGE_MMM_STATE)((a))

#if 0
extern mmm_ktp_engine_t get_ktp_status_inner(void);
#endif
#define get_ktp_status()            ((mmm_ktp_engine_t(*)(void))COM_GET_KTP_STATUS)()

#if 0
extern void change_ktp_status_inner(mmm_ktp_engine_t ktp_sta);
#endif
#define change_ktp_status(a)        ((void(*)(mmm_ktp_engine_t))COM_CHANGE_KTP_STATUS)((a))

//try vram merge
#if 0
extern void com_vram_merge_inner(void);
#endif
#define com_vram_merge()            ((void(*)(void))COM_COM_VRAM_MERGE)()

#endif

