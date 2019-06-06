/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：APPLIB 模块内部头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _APPLIB_INNER_H_
#define _APPLIB_INNER_H_

#include "psp_includes.h"
#include "applib.h"
#include "mmm_ktp.h"

//#define COMMON_TEST_EN

/*! 方案最多支持 4 个应用同时运行 */
#define MAX_APP_COUNT   4

/*!
 * \brief
 *  app_info_state_all_t 系统当前状态结构体
 */
typedef struct
{
    /*! 当前运行的前台应用ID */
    uint8 app_id;
    /*! 当前运行的引擎类型，见 engine_type_e 定义 */
    uint8 engine_type;
    /*! 引擎当前状态，正在播放还是暂停，见 engine_state_e 定义 */
    uint8 engine_state;
    /*! 前台应用 UI 状态，分正在播放UI和非播放UI，见 app_state_e 定义 */
    uint8 app_state;
    /*! BT 工作状态，分未加载，正常工作，见 bt_state_e 定义 */
    uint8 bt_state;
    /*! TWS模式，见 tws_mode_e 定义 */
    uint8 tws_mode;
    /*! 前台用户交互状态 */
    ui_status_e ui_status;
    /*! 当前系统频率模式是否正在工作模式，是则固定为所设置频率，否则根据UI状态自动调节 */
    bool sys_clk_work;
    /*! 前一个APP ID */
    uint8 last_app_id;
    /*! 中间件状态 */
    uint8 mmm_state;
    /*! 按键音通道状态 */
    mmm_ktp_engine_t ktp_status;
    /*! 保留4字节以备扩展 */
    uint8 reserves[2];
} app_info_state_all_t;

/*!
 * \brief
 *  conf_type_e 配置脚本文件句柄
 */
typedef enum
{
    /*! 字符串参数 */
    CONF_TYPE_STRING = 0,
    /*! 线性数值参数 */
    CONF_TYPE_NUM_LINEAR = 1,
    /*! 非线性数值参数 */
    CONF_TYPE_NUM_NONLINEAR = 2,
    /*! 32位整型数值参数 */
    CONF_TYPE_32BIT_INTEGER = 3,
} conf_type_e;

/*!
 * \brief
 *  config_index_t 配置索引项数据结构
 */
typedef struct
{
    /*! 配置项数据相对文件头的偏移 */
    uint32 offset;
    /*! 配置项数据长度 */
    uint32 length;
} config_index_t;


extern bool com_open_config_file(void);
extern bool com_close_config_file(void);

typedef struct
{
    /* vector 应用级定时器数组指针，指向应用空间的定时器数组 */
    app_timer_t *vector;
    /* count 应用级软定时器数目 */
    uint8 count;
    /* tag 软定时器场景标志，AP场景为0 */
    uint8 tag;
    /* 正在软定时器处理函数当中 */
    bool in_handle;
} app_timer_group_t;


extern void app_timer_var_init(app_timer_group_id_e group_id);
extern void app_timer_common_group_init(void);
extern void handle_timers(app_timer_group_id_e group_id);
//修改前台及COMMON软定时器场景标志
extern void change_app_timer_tag_inner(uint8 tag);
extern bool get_app_timer_in_handle(app_timer_group_id_e group_id);

//inner function

//应用（进程）管理模块接口声明（详细说明见接口定义）
extern bool applib_init_inner(uint8 app_id, app_type_e type);
extern bool applib_quit_inner(app_type_e type);
extern bool applib_get_app_info_inner(app_type_e type, app_info_t *app_info);

extern engine_type_e get_engine_type_by_appid_inner(uint8 app_id);
extern uint8 get_engine_appid_by_type_inner(engine_type_e engine_type);

//应用级定时器管理模块接口声明（详细说明见接口定义）
extern int8 set_app_timer_inner(app_timer_group_id_e group_id, uint8 tag, timer_attrb_e attrb, uint16 timeout, timer_proc func_proc);
extern int8 set_single_shot_app_timer_inner(app_timer_group_id_e group_id, uint8 tag, timer_attrb_e attrb, uint16 timeout, timer_proc func_proc);
extern bool modify_app_timer_inner(app_timer_group_id_e group_id, int8 timer_id, uint16 timeout);
extern bool stop_app_timer_inner(app_timer_group_id_e group_id, int8 timer_id);
extern bool restart_app_timer_inner(app_timer_group_id_e group_id, int8 timer_id);
extern bool standby_restart_all_app_timer_inner(app_timer_group_id_e group_id);
extern bool kill_app_timer_inner(app_timer_group_id_e group_id, int8 timer_id);
extern bool init_app_timers_inner(app_timer_group_id_e group_id, uint8 count);

//消息通信管理模块接口声明（详细说明见接口定义）
//接收消息接口
bool get_app_msg_inner(app_type_e type, private_msg_t *private_msg);
bool get_app_msg_hotplug(app_type_e type, private_msg_t *private_msg);
bool get_hotplug_msg(private_msg_t *private_msg);
void message_redeal_app_msg_inner(private_msg_t *p_app_msg);

//发送消息接口
bool send_sync_msg_inner(uint8 app_id, msg_apps_t *msg, msg_reply_t *reply, uint32 timeout);
bool send_async_msg_inner(uint8 app_id, msg_apps_t *msg);
bool broadcast_msg_inner(msg_apps_t *msg, uint8 bc_mode);

//同步消息回执接口
bool reply_sync_msg_inner(private_msg_t *private_msg);

//配置项接口声明

extern bool com_get_config_struct_inner(uint16 config_id, uint8 *buf, uint16 buf_len);
extern uint16 com_get_config_default_inner(uint16 config_id); //仅用于数值型参数

//try vram merge
extern void com_vram_merge_inner(void);

//获取后台引擎类型
extern engine_type_e get_engine_type_inner(void);
//修改后台引擎类型
extern void change_engine_type(engine_type_e type);
//获取蓝牙协议栈工作状态
extern bt_state_e get_bt_state_inner(void);

extern tws_mode_e get_tws_mode_inner(void);
extern void change_tws_mode_inner(tws_mode_e mode);

//获取后台引擎状态
extern engine_state_e get_engine_state(void);
//修改后台引擎状态
extern void change_engine_state_inner(engine_state_e state);
//获取前台应用 UI 状态，分正在播放UI和非播放UI
extern app_state_e get_app_state(void);
//修改前台应用 UI 状态，分正在播放UI和非播放UI
extern void change_app_state_inner(app_state_e state);
//获取最近前台AP
extern uint8 get_last_app_id_inner(void);
//获取当前前台AP
extern uint8 get_cur_app_id_inner(void);
//获取 ui_status
extern ui_status_e get_ui_status(void);
//改变 ui_status
extern void change_ui_status(ui_status_e ui_sta);
//获取 ui_status
extern bool get_sys_clk_work(void);
//改变 ui_status
extern void change_sys_clk_work(bool sys);
//获取 mmm_state
extern mmm_state_e get_mmm_state_inner(void);
//改变 mmm_state
extern void change_mmm_state_inner(mmm_state_e mmm_sta);
//获取 ktp_status
extern mmm_ktp_engine_t get_ktp_status_inner(void);
//改变 ktp_status
extern void change_ktp_status_inner(mmm_ktp_engine_t ktp_sta);

extern app_info_t *applib_get_app_info_by_type(app_type_e type);
extern app_info_t *applib_get_app_info_by_app_id(uint8 app_id);

extern app_msg_dispatch_handle g_app_msg_dispatch_handle;
extern void set_app_msg_dispatch_handle_inner(app_msg_dispatch_handle dispatch_handle);

#endif
