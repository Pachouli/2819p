/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：用户交互功能接口相关的宏定义，枚举类型，结构体定义，外部接口声明等。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _COMMON_UI_H_
#define _COMMON_UI_H_

#include "applib.h"
#include "common_func.h"
#include "key_interface.h"
#include "led_interface.h"
#include "lcd_interface.h"
#include "common_keytone.h"
#include "common_kt_mode.h"
#include "common_tts.h"
#include "common_led.h"
#include "common_sound.h"
#include "common_kt_id.h"

/*!
 * \brief
 *  key_map_t 按键（gui）消息映射结构体
 */
typedef struct
{
    /*! 源按键事件 */
    key_event_t key_event;
    /*! 映射 gui 事件 */
    msg_apps_type_e event;
} key_map_t;

//common key处理接口声明
#if 0
extern bool get_gui_msg_inner(input_gui_msg_t *input_msg);
#endif
#define get_gui_msg(a)              ((bool(*)(input_gui_msg_t *))COM_GET_GUI_MSG)((a))

#if 0
extern void message_redeal_gui_msg_inner(input_gui_msg_t *p_gui_msg);
#endif
#define message_redeal_gui_msg(a)   ((bool(*)(input_gui_msg_t *))COM_MESSAGE_REDEAL_GUI_MSG)((a))

#if 0
extern void com_double_click_enable_inner(bool enable, key_value_e dc_key_1, key_value_e dc_key_2);
#endif
#define com_double_click_enable(a,b,c)  ((void(*)(bool, key_value_e, key_value_e))COM_COM_DOUBLE_CLICK_ENABLE)((a),(b),(c))

#if 0
extern bool com_key_mapping_inner(input_gui_msg_t *input_msg, msg_apps_type_e *gui_event, const key_map_t *key_map_list);
#endif
#define com_key_mapping(a,b,c)      ((bool(*)(input_gui_msg_t *, msg_apps_type_e *, const key_map_t *))COM_COM_KEY_MAPPING)((a),(b),(c))

#if 0
extern void com_filter_key_hold_inner(void);
#endif
#define com_filter_key_hold()       ((void(*)(void))COM_COM_FILTER_KEY_HOLD)()

#if 0
extern void com_filter_key_up_inner(void);
#endif
#define com_filter_key_up()         ((void(*)(void))COM_COM_FILTER_KEY_UP)()

#if 0
extern void com_filter_key_period_of_time_inner(uint16 time_length);
#endif
#define com_filter_key_period_of_time(a)  ((void(*)(uint16))COM_COM_FILTER_KEY_PERIOD_OF_TIME)((a))

#if 0
extern uint32 com_get_long_key_time_inner(void);
#endif
#define com_get_long_key_time()     ((uint32(*)(void))COM_COM_GET_LONG_KEY_TIME)()

#if 0
extern void com_start_key_tone_inner(void);
#endif
#define com_start_key_tone()        ((void(*)(void))COM_COM_START_KEY_TONE)()

#if 0
extern input_msg_type_e com_get_input_msg_type_inner(void);
#endif
#define com_get_input_msg_type()    ((input_msg_type_e(*)(void))COM_COM_GET_INPUT_MSG_TYPE)()

//杂项功能接口声明
#if 0
extern app_result_e com_message_box_inner(msg_apps_type_e msg_type);
#endif
#define com_message_box(a)      ((app_result_e(*)(msg_apps_type_e))COM_COM_MESSAGE_BOX)((a))

#if 0
extern void sys_hot_detect_handle_inner(void);
#endif
#define sys_hot_detect_handle() ((void(*)(void))COM_SYS_HOT_DETECT_HANDLE)()

#if 0
extern void com_save_sys_comval_inner(void);
#endif
#define com_save_sys_comval()   ((void(*)(void))COM_COM_SAVE_SYS_COMVAL)()

#if 0
extern void com_restore_sys_comval_by_reset_inner(void);
#endif
#define com_restore_sys_comval_by_reset() ((void(*)(void))COM_COM_RESTORE_SYS_COMVAL_BY_RESET)()

#if 0
extern void com_aux_pa_init_inner(void);
#endif
#define com_aux_pa_init()       ((void(*)(void))COM_COM_AUX_PA_INIT)()

#if 0
extern bool com_creat_bt_engine_inner(uint32 bt_att_mode);
#endif
#define com_creat_bt_engine(a)      ((bool(*)(uint32 bt_mode))COM_COM_CREAT_BT_ENGINE)(a)

#if 0
extern bool com_close_bt_engine_inner(void);
#endif
#define com_close_bt_engine()       ((bool(*)(void))COM_COM_CLOSE_BT_ENGINE)()

//RTC时钟闹钟接口

#define RING_FOLDER_FILENUM_OFFSET   (4) //该offset对应time_alarm_var_t中uint8 ring_folder_filenum[]
#define RCP_RING_FOLDER_MAX          (4) //自定义闹钟闹铃目录数目
#define RING_FOLDER_FILENAME_LEN     (32)//闹铃目录文件名长度
#define RING_FOLDER_FILENUM_PER_SEC  (512/RING_FOLDER_FILENAME_LEN)//一个扇区可以容纳的闹铃目录文件名个数
#define RING_FOLDER_FILENUM_MAX      (64)//闹铃目录文件总数，受VRAM大小限制

/*! 闹钟铃声类型 */

typedef enum
{
    RING_TYPE_BUILTIN   = 0, //内置铃声，目录信息由 ring_seq 指定
    RING_TYPE_UHOST     = 1, //U盘指定目录，目录信息由 u_location 指定
    RING_TYPE_SDCARD    = 2, //SDCARD指定目录，目录信息由 sd_location 指定
    RING_TYPE_FMSTATION = 3, //FM电台，电台由 frequency 指定，单位KHZ
    RING_TYPE_BTPLAY    = 4, //蓝牙推歌
} ring_type_e;

typedef uint32 ring_location_t;

typedef union
{
    uint32 ring_seq;
    ring_location_t u_location;
    ring_location_t sd_location;
    uint32 frequency;
    uint32 btplay_para;
} ring_sound_u;

typedef struct
{
    time_t       start_time;
    ring_type_e  ring_type;
    ring_sound_u sound;
    uint8        overtime;
    uint8        volume;
} now_alarm_detail_t;

#define ALARM_UPDATE_CLOSE    2
#define ALARM_UPDATE_SNOOZE   3
#define ALARM_UPDATE_OVERTIME 4

#if 0
extern app_result_e com_rtc_time_entry_inner(void);
#endif
#define com_rtc_time_entry()        ((app_result_e(*)(void))COM_COM_RTC_TIME_ENTRY)()

#if 0
extern void com_rtc_get_alarm_detail_inner(now_alarm_detail_t *p_alarm_detail);
#endif
#define com_rtc_get_alarm_detail(a) ((void(*)(now_alarm_detail_t *))COM_COM_RTC_GET_ALARM_DETAIL)((a))

#if 0
extern void com_rtc_update_alarm_inner(uint8 mode);
#endif
#define com_rtc_update_alarm(a)     ((void(*)(uint8))COM_COM_RTC_UPDATE_ALARM)((a))

typedef enum
{
    FAST_PREVIEW_STA_NULL = 0,    //空闲状态
    FAST_PREVIEW_STA_READY = 1,   //第一次切换应用按键
    FAST_PREVIEW_STA_RUNNING = 2, //处于快速预览模式中
} fast_preview_status_e;

#if 0
extern void com_fast_preview_cbk_inner(input_gui_msg_t *input_msg);
#endif
#define com_fast_preview_cbk(a)     ((void(*)(input_gui_msg_t *))COM_COM_FAST_PREVIEW_CBK)((a))

#if 0
extern void com_fast_preview_update_ap_welcome_ui_inner(uint8 func_id, void *para);
#endif
#define com_fast_preview_update_ap_welcome_ui(a,b) ((void(*)(uint8, void *))COM_COM_FAST_PREVIEW_UPDATE_AP_WELCOME_UI)((a),(b))

#if 0
extern fast_preview_status_e com_fast_preview_get_status_inner(void);
#endif
#define com_fast_preview_get_status() ((fast_preview_status_e(*)(void))COM_COM_FAST_PREVIEW_COM_GET_STATUS)()

#if 0
extern void com_fast_preview_next_app_inner(app_result_e switch_result);
#endif
#define com_fast_preview_next_app(a) ((void(*)(app_result_e))COM_COM_FAST_PREVIEW_NEXT_APP)((a))

#endif
