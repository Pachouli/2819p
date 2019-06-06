/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：系统管理结构体定义。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _COMMON_SYS_INNER_H_
#define _COMMON_SYS_INNER_H_

#include "common_func.h"

/*!
 * \brief
 *  backlight_state_e 背光状态枚举类型
 */
typedef enum
{
    /*! 背光正常状态，亮着 */
    BACKLIGHT_STATE_NORMAL = 0,
    /*! 背光变暗状态，暗着 */
    BACKLIGHT_STATE_DARK = 1,
    /*! 背光关闭状态，关着 */
    BACKLIGHT_STATE_OFF = 2,
} backlight_state_e;

/*!
 * \brief
 *  app_info_state_t 系统当前状态结构体
 */
typedef struct
{
    /*! 卡状态，没卡，有卡，见 card_state_e 定义 */
    uint8 card_state;
    /*! linein状态，没linein，有linein，见 linein_state_e 定义 */
    uint8 linein_state;
    /*! uhost状态，没有u盘插入，有插 u盘插入，见 cable_state_e 定义 */
    uint8 uhost_state;
    /*! 线缆状态，没插 USB 线，有插 USB 线，见 cable_state_e 定义 */
    uint8 cable_state;
    /*! 充电线状态，没插 USB充电线，有插 USB 充电线，见 adaptor_state_e 定义 */
    uint8 adaptor_state;
    /*! 耳机（天线）状态 */
    bool antenna_state;
    /*! OK MIC 状态 */
    bool ok_mic_state;
    /*! UHOST设备类型，U盘或者仅充电手机，见 usb_host_in_e 定义 */
    uint8 uhost_type;
    /*! LED数码管是否点亮 */
    backlight_state_e backlight_state;
    /*! 电池电量，0 ~ 10 */
    uint8 bat_value;
    /*! 当前应用功能映射表序号*/
    uint8 cur_func_index;
    /*! 当前应用功能实际序号*/
    uint8 cur_func_id;
    /*! stub工具类型 */
    uint8 stub_tools_type;
    /*! 进入BT DEBUG (1) 或EJTAG 3口模式 (2) */
    uint8 btdbg_ejtag_flag;
    /*! TWS 主箱从箱角色 */
    dev_role_e tws_role;
    /*! TWS 左箱右箱位置 */
    dev_spk_pos_e tws_spk;
    /*! 从箱电池电量 */
    uint8 slave_bat_value;
    /*! UI显示（LED数码管和LED灯）禁止更新 */
    uint8 ui_display_forbidden;
} app_info_state_t;

/*!
 * \brief
 *  app_last_state_t 系统最近状态结构体，用于自动检测状态变化并处理
 */
typedef struct
{
    /*! 上一次应用功能序号*/
    uint8 last_func_index;
    /*! 最近系统是否处于空闲状态 */
    uint8 last_idle_flag;
} app_last_state_t;

/*!
 *  \brief
 *  g_app_info_state 全局系统当前状态结构体，存放在系统全局空间
 */
extern app_info_state_t g_app_info_state;

/*!
 *  \brief
 *  g_app_last_state 全局系统最近状态结构体
 */
extern app_last_state_t g_app_last_state;

///修改系统LINEIN状态
static inline void change_linein_state(linein_state_e linein_state)
{
    g_app_info_state.linein_state = linein_state;
}

///修改系统 uhost 状态
static inline void change_uhost_state(uhost_state_e uhost_state)
{
    g_app_info_state.uhost_state = uhost_state;
}

///修改系统 USB 线缆状态
static inline void change_cable_state(cable_state_e cable_state)
{
    g_app_info_state.cable_state = cable_state;
}

///修改系统 USB 充电线状态
static inline void change_adaptor_state(adaptor_state_e adaptor_state)
{
    g_app_info_state.adaptor_state = adaptor_state;
}

///获取耳机（天线）状态
static inline bool get_antenna_state(void)
{
    return g_app_info_state.antenna_state;
}

///修改耳机（天线）状态
static inline void change_antenna_state(bool in_out)
{
    g_app_info_state.antenna_state = in_out;
}

///获取MIC状态
static inline bool get_ok_mic_state(void)
{
    return g_app_info_state.ok_mic_state;
}

///修改MIC状态
static inline void change_ok_mic_state(bool in_out)
{
    g_app_info_state.ok_mic_state = in_out;
}

///获取背光状态，TRUE表示背光亮，FALSE表示背光灭
static inline backlight_state_e get_backlight_state(void)
{
    return g_app_info_state.backlight_state;
}

///修改背光状态
static inline void change_backlight_state(backlight_state_e state)
{
    g_app_info_state.backlight_state = state;
}

//获取上一个前台AP fuction index
static inline uint8 get_last_func_index(void)
{
    return g_app_last_state.last_func_index;
}

///设置上一个前台AP fuction index
static inline void set_last_func_index(uint8 index_num)
{
    g_app_last_state.last_func_index = index_num;
}

///获取当前前台AP fuction index
static inline uint8 get_cur_func_index(void)
{
    return g_app_info_state.cur_func_index;
}

///设置当前前台AP fuction index
static inline void set_cur_func_index(uint8 index_num)
{
    g_app_info_state.cur_func_index = index_num;
}

///获取当前前台AP funciton id
static inline uint8 get_cur_func_id(void)
{
    return g_app_info_state.cur_func_id;
}

#endif
