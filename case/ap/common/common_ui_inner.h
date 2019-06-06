/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：用户交互模块内部头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _COMMON_UI_INNER_H_
#define _COMMON_UI_INNER_H_

#include "common_ui.h"
#include "applib_inner.h"
#include "common_sys_inner.h"
#include "common_bt_inner.h"
#include "common_aset_inner.h"
#include "common_var.h"
#include "common_func_inner.h"
#include "common_tws_inner.h"
#include "common_lcd_ui.h"

extern void sys_counter_var_init(void);
extern void sys_counter_reset(void);

/*! 系统环境变量 */
extern comval_t g_com_comval;

/*! 经常变化的环境变量 */
extern env_var_t g_com_env_var;

/*! 数字音效配置参数 */
extern dae_config_t *g_p_com_dae_cfg;

extern const int16 g_aud_vol_p1_db_table[VOLUME_VALUE_MAX + 1];
extern const int16 g_aud_vol_p1_db_call_table[VOLUME_VALUE_CALL_MAX + 1];
extern const int16 g_aud_vol_p1_db_pc_vol_table[101];

extern void sys_timer_init(void);
extern void sys_timer_exit(void);


#define KT_FIFO_DEPTH  4

typedef enum
{
    KT_TYPE_HARD = 0, //硬按键音，有限单频率时长方波，不支持可定制
    KT_TYPE_SOFT = 1, //软按键音，支持可定制
} kt_type_e;

typedef struct
{
    kt_type_e kt_type;
    bool enable;
    bool playing;
    uint8 count;
    uint8 kt_fifo[KT_FIFO_DEPTH];
    uint8 volmode;
    uint8 min_volume;
    uint8 max_volume;
} keytone_infor_t;

extern void keytone_play_deal(void);
extern void keytone_play_deal_wait(void);
extern void keytone_set_on_off_inner(bool on_off);
extern bool keytone_play_inner(uint8 kt_mode);

typedef enum
{
    LED_STATUS_NULL,
    LED_STATUS_ON,
    LED_STATUS_DUTY, //占空比快闪，PWM
    LED_STATUS_DUTY_GPIO, //占空比快闪，GPIO + 硬件定时器
    LED_STATUS_BREATH, //呼吸灯
    LED_STATUS_DUMMY, //视图占据，但灯不亮，by jc0245
} led_status_e;

typedef struct
{
    led_status_e led_status;
    uint8        led_para;
} led_scene_status_t;

typedef struct
{
    led_scene_status_t scene_status[LED_SCENE_MAX];
    bool               led_gpio_on;
    bool               led_sim_breath_on;
    int8               led_timer_id;
} led_scene_info_t;

extern void led_var_init(void);
extern void com_led_twinkle_init_inner(led_id_e led_id, led_scene_e led_scene, pwm_period_e period, uint8 pwm_gpio_sel);
extern void com_led_light_on_inner(led_id_e led_id, led_scene_e led_scene);
extern void com_led_light_off_inner(led_id_e led_id, led_scene_e led_scene);
extern void com_led_light_dummy_inner(led_id_e led_id, led_scene_e led_scene);
extern void com_led_breath_inner(led_id_e led_id, led_scene_e led_scene);

//LED数码管
extern void led_module_install(void);
extern void led_module_uninstall(void);
extern void com_led_display(uint8 num_idx, uint8 c, bool display);
extern void com_led_display_icon(uint8 icon_idx, bool display);
extern void com_led_clear_screen(uint8 clr_mode);
extern void com_led_flash_screen(uint8 onoff);
extern void led_wake_up_display(void);
extern void led_sleep_display(void);
extern void led_light_onoff(bool onoff);
extern void com_led_enter_msg_show(uint16 disp_time);
extern bool com_led_force_exit_msg_show(void);
extern void com_led_twinkle_icon(uint8 icon_idx, bool start, uint16 period_ms);

//缓冲TTS播报过程中的系统消息
#define TTS_LANGUAGE_TOTAL     2   //支持TTS语言类型总数
#define TTS_SECTIONS_MAX       16  //一次语音播报最大片段数，比如数字1234，报为“一千二百三十四”，共7段

typedef struct
{
    bool enable;
    bool control_enable;
    uint8 tts_ignore : 1;
    uint8 tts_force_stop : 1;
    uint8 tts_playing : 1;
    uint8 tts_slave_volume_delay_flag : 1;
    uint8 tts_slave_limit_delay_flag : 1;
    uint8 language;
    uint8 volmode;
    uint8 min_volume;
    uint8 max_volume;
    uint8 slave_volume_delay;
    int16 slave_limit_delay;
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    uint8 slave_tts_count; //最多可以缓冲3个TTS ID
    uint8 slave_tts_ids[3];
#endif
} tts_play_infor_t;

extern tts_play_infor_t g_tts_play_infor;

extern void tts_play_var_init(void);
extern tts_play_ret_e com_tts_state_play_inner(uint8 tts_id, uint16 mode);
extern tts_play_ret_e com_tts_state_play_ext_inner(uint8 tts_id, uint16 mode, tts_play_background_hook bg_hook);
extern tts_play_ret_e com_tts_state_play_list_inner(uint8 *tts_list, uint8 section_cnt, uint16 mode);
extern void com_tts_state_play_control_inner(tts_control_opr_e opr);

extern void * com_sd_mp3_player_play_tts_inner(uint8 * sec_ids, uint8 sec_cnt);
extern void * com_sd_mp3_player_play_mp3_inner(uint8 * mp3_name);
extern bool com_sd_mp3_player_stop_inner(void *player_handle);
extern bool com_sd_mp3_player_is_stop_inner(void *player_handle);
extern void * com_sd_mp3_player_play_tts_do_inner(uint16 * sec_ids, uint8 sec_cnt);


extern void com_get_default_setting(comval_t *setting_comval);
extern void com_get_default_env_var(env_var_t *p_env_var);

typedef enum
{
    BAT_DET_NULL, //初始状态/电量保持状态
    BAT_DET_FALL, //电量下跌状态
    BAT_DET_RISE, //电量上升状态
} bat_det_fsm_e;

#define BAT_DET_VALUES_MAX  10 //5秒钟去抖窗口
#define BAT_DET_WIN_TIME    60 //电池电量检测窗口 30S
#define BAT_DET_WIN_COUNT   10 //10个窗口观察，5分钟稳定后升高电量

#define POWEROFF_MODE_S3    0  //软关机
#define POWEROFF_MODE_S2    1  //S2低功耗模式

/*!
 * \brief
 *  sys_counter_t 系统功能计时器数据结构，各计时器均以0.5 s为单位
 */
typedef struct
{
    /*! 省电关机计时器 */
    uint16 poweroff_counter;
    /*! UI空闲状态计时器 */
    uint16 ui_idle_counter;
    /*! LED数码管屏保计时器 */
    uint16 light_counter;

    /*! LED数码管屏保时间，1秒为单位，0表示不关闭 */
    uint16 light_timer;

    //省电关机开关/模式/时间
    uint8 poweroff_enable;
    uint8 poweroff_mode;
    uint8 poweroff_timer; //1分钟为单位
    uint8 poweroff_timer_bt_wait; //1分钟为单位

    /*! 电池电量检测 */
    uint16 bat_value_counter;
    bat_det_fsm_e bat_det_sta;
    uint8 bat_value_min; //用于统计连续一段时间检测到的最小电量
    uint8 bat_value_debounce;
    uint8 bat_value_index;
    uint8 bat_values[BAT_DET_VALUES_MAX];

    /*! 充满电标志 */
    uint8  bat_full_flag;
    uint16 bat_full_cnt;

    /*! 电池电量低提示 */
    uint8  bat_low_start; //FALSE表示尚未开始提示，TRUE表示已开始提示
    uint16 bat_low_cnt; //用于周期提示
    uint8  bat_value_win_min[BAT_DET_WIN_COUNT]; //每30秒一个窗口统计最小值，每次更新时去掉第一个并整体往前挪，然后添加到最后
    uint8  bat_value_cur_win_min; //当前正在统计窗口最小值
    uint8  bat_value_win_tick; //检测窗口计时
    uint8  bat_value_win_count; //已经统计窗口数
} sys_counter_t;

extern void com_load_dae_config(dae_config_t *p_dae_cfg);
extern void com_init_dae_config(dae_config_t *p_dae_cfg);
extern void com_update_dae_config(dae_config_t *p_dae_cfg);
extern bool com_set_dae_onoff_inner(bool onoff);
extern bool com_set_drc_onoff_inner(bool onoff);
extern uint8 com_switch_drc_mode_inner(uint8 mode);
extern bool com_set_drc_precut_inner(int16 precut);
extern bool com_set_speech_peq_para_inner(bool in);
extern bool com_update_speech_peq_para_inner(bool peq_enable, peq_band_t * peq_array);

extern bool com_set_dae_reverb_depth_add(void);
extern bool com_set_dae_reverb_depth_sub(void);
extern bool com_set_dae_reverb_depth_set(uint8 ratio_index);
extern bool com_set_dae_ducking_switch(void);
extern bool com_set_dae_voice_removal_switch(void);
extern bool com_set_dae_ok_mic_mute(bool mute);

extern void vr_adc_mic_low_enhance_cbk(int16 p1_gain, int16 p2_gain);
extern void vr_adc_mic_high_enhance_cbk(int16 p1_gain, int16 p2_gain);
extern void vr_adc_play_low_enhance_cbk(int16 p1_gain, int16 p2_gain);
extern void vr_adc_play_high_enhance_cbk(int16 p1_gain, int16 p2_gain);

extern void sys_counter_handle(void);
extern void hardware_timer_500ms_isr(void);
extern void com_load_sys_comval(void);

#ifdef ENABLE_TRUE_WIRELESS_STEREO
typedef enum
{
    KEY_PERIOD_NULL,
    KEY_PERIOD_MASTER,
    KEY_PERIOD_SLAVE,
} key_period_e;
#endif

typedef enum
{
    DC_KEY_STATUS_NULL = 0,        //没有按键；判断成功后，将 key_type 改为 KEY_TYPE_DOUBLE_CLICK
    DC_KEY_STATUS_FIRST_DOWN = 1,  //按键第一次按下
    DC_KEY_STATUS_FIRST_UP = 2,    //按键第一次短按抬起
    DC_KEY_STATUS_SECOND_DOWN = 3, //按键第二次按下
} double_click_key_status_e;

typedef enum
{
    DC_FAIL_NO_FAIL = 0,
    DC_FAIL_FIRSE_OVERTIME = 1,   //因第一个按键按太久变成长按而失败
    DC_FAIL_FIRST_LONG = 2,       //遇到长按按键而失败
    DC_FAIL_SECOND_NOT_DOWN = 3,  //因第二个按键没有在短时间内按下而失败
    DC_FAIL_SECOND_OVERTIME = 4,  //遇到长按按键而失败
    DC_FAIL_SECOND_LONG = 5,      //因第二个按键按太久变成长按而失败
    DC_FAIL_SECOND_OTHER_KEY = 6, //因第二个按键按了别的按键而失败
} double_click_fail_reason_e;

typedef enum
{
    DC_FAIL_STATUS_NULL = 0,             //失败后刚开始执行补发按键
    DC_FAIL_STATUS_SEND_FIRST_DOWN = 1,  //失败后已补发第一个按键DOWN消息
    DC_FAIL_STATUS_SEND_FIRST_UP = 2,    //失败后已补发第一个按键UP消息
    DC_FAIL_STATUS_SEND_SECOND_DOWN = 3, //失败后已补发第二个按键DOWN消息
} double_click_fail_status_e;

#define DOUBLE_CLICK_TIME_SPACE_MAX  250 //双击按键中按下到抬起，抬起到第二个按键按下的时间检测最长

typedef struct
{
    /*! 双击按键开关 */
    bool double_click_enable;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    //当前按键处于主箱按键周期还是处理从箱按键周期
    key_period_e key_period;
    bool key_period_filter_master_flag; //从箱按键周期过滤主箱按键
    bool key_period_filter_slave_flag;  //主箱按键周期过滤从箱按键
#endif

    /*! 双击按键值1 */
    key_value_e dc_key_1;
    /*! 双击按键值2 */
    key_value_e dc_key_2;
    /*! 当前双击按键值 */
    key_value_e dc_key_cur;
    /*! 双击按键状态机 */
    double_click_key_status_e dc_sta;
    /*! 双击按键确认失败 */
    bool double_click_fail_flag;
    /*! 双击按键失败原因 */
    double_click_fail_reason_e dc_fail_reason;
    /*! 双击按键第二个按键是其他按键，该按键值 */
    key_value_e other_key;
    /*! 双击按键失败执行状态机 */
    double_click_fail_status_e dc_fail_sta;
    /*! 双击按键判断计时器 */
    uint32 dc_last_time;
} key_deal_infor_t;

extern key_deal_infor_t g_key_deal_infor;

//按键映射接口声明
extern bool com_key_mapping_inner(input_gui_msg_t *input_msg, msg_apps_type_e *gui_event,
        const key_map_t *key_map_list);

//common key处理接口声明
extern void com_filter_key_hold_inner(void);
extern void com_filter_key_up_inner(void);
extern void com_filter_key_period_of_time_inner(uint16 time_length);
extern uint32 com_get_long_key_time_inner(void);
extern void com_start_key_tone_inner(void);

extern input_msg_type_e com_get_input_msg_type_inner(void);

extern bool get_gui_msg_inner(input_gui_msg_t *input_msg);
extern void message_redeal_gui_msg_inner(input_gui_msg_t *p_gui_msg);
extern void message_key_flush_inner(void);
extern void com_double_click_enable_inner(bool enable, key_value_e dc_key_1, key_value_e dc_key_2);

//声音处理

#define TWS_SYNC_VOL_NULL       0 //0表示已经超过250ms没有同步音量，可以直接同步
#define TWS_SYNC_VOL_RIGHT_NOW  1 //1表示刚同步音量，需要延时250ms才会同步，减少主副箱数据包
#define TWS_SYNC_VOL_DELAY      2 //2表示等待250ms同步音量
#define TWS_SYNC_VOL_DELAY_TIME (250) //同步音量延时时间 250ms

typedef struct
{
    /*! 当前音量模式 */
    uint8 volume_mode;
    /*! 当前音量 */
    uint8 volume_current;
    /*! 当前音量最大值 */
    uint8 volume_current_max;
    /*! 主音量备份 */
    uint8 main_volume;
    /*! 打电话音量备份 */
    uint8 call_volume;
    /*! 是否正处于USB音箱音量模式中 */
    bool usbbox_volume_flag;
    /*! USB音箱音量备份 */
    uint8 usbbox_volume;
    /*! 是否静音，IC内部PA音量调为mute */
    bool mute;
    /*! HDMI是否静音 */
    bool cec_mute;
    /*! 外部功放MAIN使能，1表示打开，0表示关闭 */
    bool pa_main_on;
    /*! 外部功放SUB使能次数，非0表示打开，0表示关闭 */
    uint8 pa_sub_on_count;
    /*! DCDC升压是否已经工作 */
    bool dcdc_enable;
    /*! AB类和D类功放模式,PA_SWITCH_D=0,PA_SWITCH_AB=1 */
    uint8 pa_ab_d_flag;
    /*! 音量增益模式 */
    uint8 gain_mode;
    /*! 主音量衰减 */
    int16 main_gain;
    /*! 打电话音量衰减 */
    int16 call_gain;
    /*! line-in音量衰减 */
    int16 linein_gain;
    /*! fm 收音机音量衰减 */
    int16 radio_gain;
    /*! 当前音量增益 */
    int16 gain_current;
    /*! 当前物理音量值，未加上 gain_current 值 */
    int16 phy_current;
    /*! 物理音量限制序列，单位0.1db，分别为电量0级 ~ 电量9级对应最大音量限制 */
    int16 phy_limit_tbl[BATTERY_GRADE_MAX + 1];
    /*! 物理音量限制，单位0.1db，负数 */
    int16 phy_limit_current;
    /*! TWS 主箱需要同步音量给副箱，会延时250ms才会同步，减少主副箱数据包 */
    uint32 tws_sync_volume_delay_timer;
    uint8  tws_sync_volume_value;
    uint8  tws_sync_volume_flag;
    uint8  tws_sync_volume_delay;
    /*! MIC通道当前音量 */
    uint8 mic_volume_current;
    /*! MIC通道软音量增益 */
    int16 mic_soft_volume_gain;
} volume_infor_t;

extern volume_infor_t g_volume_infor;

///获取静音与否，TRUE表示静音，FALSE表示非静音
static inline bool get_mute_enable(void)
{
    return g_volume_infor.mute;
}

///设置静音，TRUE表示静音，FALSE表示非静音
static inline void set_mute_enable(bool mute)
{
    g_volume_infor.mute = mute;
}

///获取CEC静音与否，TRUE表示静音，FALSE表示非静音
static inline bool get_cec_mute_enable(void)
{
    return g_volume_infor.cec_mute;
}

///获取CEC静音，TRUE表示静音，FALSE表示非静音
static inline void set_cec_mute_enable(bool mute)
{
    g_volume_infor.cec_mute = mute;
}

extern uint8 com_get_sound_volume_inner(void);
extern uint8 com_get_sound_volume_pc_inner(void);
extern bool com_reset_sound_volume_inner(uint8 mode);
extern bool com_set_sound_volume_inner(uint8 mode, uint8 volume);
extern void com_set_sound_volume_mode_inner(uint8 volume_mode, uint8 gain_mode);
extern void com_set_sound_volume_gain_inner(int16 gain);

extern uint8 com_get_sound_volume_mode(void);
extern uint8 com_get_sound_volume_gain_mode(void);

extern void com_volume_add_inner(uint8 mode);
extern void com_volume_sub_inner(uint8 mode);
extern void com_volume_set_inner(uint8 mode, uint8 set_vol);
extern bool com_set_mute_inner(bool mute);
extern bool com_set_mute_toggle_inner(void);
extern bool com_hdmi_cec_mute(bool flag, bool mute);
extern void com_set_usbbox_volume_ctrl_inner(uint8 ctrl_mode);

extern void com_set_volume_limit_inner(int16 vol_limit);

extern void com_set_mic_volume(uint8 volume);

extern bool com_switch_geq_inner(uint8 mode, com_eqtype_e eq_type, uint8 *eq_para);

//杂项功能接口声明（详细说明见接口定义）
extern app_result_e com_message_box_inner(msg_apps_type_e msg_type);
extern void sys_hot_detect_handle_inner(void);
extern void com_save_sys_comval_inner(void);
extern void com_restore_sys_comval_by_reset_inner(void);

//喇叭功能接口
extern bool is_speaker_on_inner(void);
extern void com_speaker_on_do(void);
extern void com_speaker_off_do(void);
extern void com_speaker_on_inner(uint8 mode);
extern void com_speaker_off_inner(uint8 mode);
extern void com_speaker_ab_d_switch_inner(uint8 ab_d);

extern bool common_init_inner(void);

extern void set_cur_func_id_inner(uint8 func_id);
extern card_state_e get_card_state_inner(void);
extern void change_card_state_inner(card_state_e card_state);
extern uhost_state_e get_uhost_state_inner(void);
extern linein_state_e get_linein_state_inner(void);
extern cable_state_e get_cable_state_inner(void);
extern adaptor_state_e get_adaptor_state_inner(void);

extern usb_host_in_e get_uhost_type_inner(void);
extern void change_uhost_type_inner(usb_host_in_e uhost_type);

extern void com_aux_pa_init_inner(void);

extern app_result_e com_standby(void);

extern bool com_keytone_play_inner(void *kt_para);
extern bool com_keytone_stop_inner(void);
extern void com_keytone_check_status(void);

//RTC时钟闹钟模块

typedef struct
{
    /*! 闹钟占用标志，1表示已占用 */
    uint8 alarm_used;
    /*! 闹钟开关 */
    uint8 alarm_onoff;
    /*! 闹钟时间, 3Bytes */
    time_t alarm_time;
    /*! 闹钟星期，bit0 ~ bit6 分别表示周日 ~ 周六，0表示只闹一次 */
    uint8 alarm_day;
    /*! 闹铃类型，详见 ring_type_e 定义 */
    uint8 ring_type;
    uint8 reserve;
    /*! 闹铃参数 */
    ring_sound_u sound;
} alarm_info_t;

typedef struct
{
    /*! 闹钟记录 */
    alarm_info_t alarm;
    /*! 当前闹钟时间 */
    time_t last_time;
    /*! 闹钟snooze次数 */
    uint8 snooze;
} alarm_record_t;

#define ALARM_RECORD_MAX 1
typedef struct
{
    /*! VM 变量魔数*/
    uint16 magic;
    /*! 是否支持贪睡模式 */
    bool snooze_enable;
    /*! snooze时间，1-30分钟 */
    uint8 snooze_time;
    /*! snooze次数，1-10次 */
    uint8 snooze_max_count;
    /*! 超时时间，1-30分钟*/
    uint8 overtime;
    /*! 闹钟超时处理选项,0表示关闭闹钟，1表示自动选择贪睡 */
    uint8 overtime_deal;
    /*! 闹钟闹铃播放音量 */
    uint8 alarm_volume;

    uint8 reserve[3];
    /*! 当前闹钟记录INDEX，0xff表示无效 */
    uint8 alarm_idx;
    /*! 闹钟记录列表 */
    alarm_record_t alarm_records[ALARM_RECORD_MAX];
} rtc_alarm_manager_info_t;

#define ALARM_UPDATE_MODIFY  0
#define ALARM_UPDATE_DELETE  1

typedef enum
{
    RTC_TIME_DISPLAY = 0,
    RTC_ALARM_SET,
    RTC_TIME_SET,
    RTC_CALENDAR_SET,
    RTC_SCENE_MAX,
} rtc_scene_e;

#define RTC_TIME_OVERTIME  40 //20S超时返回

extern app_result_e com_rtc_time_entry_inner(void);
extern app_result_e com_rtc_clock_display_inner(void);
extern app_result_e com_rtc_alarm_set_inner(void);
extern app_result_e com_rtc_time_set_inner(void);
extern app_result_e com_rtc_calendar_set_inner(void);

extern void com_rtc_get_alarm_info(uint8 idx, alarm_info_t *alarm);
extern void com_rtc_update_alarm_info(uint8 idx, alarm_info_t *alarm, uint8 mode);

extern void com_rtc_get_alarm_detail_inner(now_alarm_detail_t *p_alarm_detail);
extern void com_rtc_update_alarm_inner(uint8 mode);

#if (SUPPORT_SPI_LCD_DISPLAY == 1)
//LCD UI Interface
#define UI_OVERTIME 16 //8S超时
extern app_result_e ui_slider_inner(slider_t *slider, slider_callback s_cbk);
extern app_result_e ui_set_contrast(void);
#endif

typedef struct
{
    /*! 默认数值 */
    uint16 default_value;
    /*! 有效值个数 */
    uint16 total;
    /*! 离散参数列表，最多20个参数 */
    uint16 value[20];
} echo_reverb_para_t;

#if (SUPPORT_FAST_PREVIEW == 1)
extern void com_fast_preview_cbk_inner(input_gui_msg_t *input_msg);
extern void com_fast_preview_update_ap_welcome_ui_inner(uint8 func_id, void *para);
extern fast_preview_status_e com_fast_preview_get_status_inner(void);
extern void com_fast_preview_next_app_inner(app_result_e switch_result);
#endif

#endif
