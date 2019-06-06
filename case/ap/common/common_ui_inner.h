/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������û�����ģ���ڲ�ͷ�ļ���
 * ���ߣ�cailizhen
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

/*! ϵͳ�������� */
extern comval_t g_com_comval;

/*! �����仯�Ļ������� */
extern env_var_t g_com_env_var;

/*! ������Ч���ò��� */
extern dae_config_t *g_p_com_dae_cfg;

extern const int16 g_aud_vol_p1_db_table[VOLUME_VALUE_MAX + 1];
extern const int16 g_aud_vol_p1_db_call_table[VOLUME_VALUE_CALL_MAX + 1];
extern const int16 g_aud_vol_p1_db_pc_vol_table[101];

extern void sys_timer_init(void);
extern void sys_timer_exit(void);


#define KT_FIFO_DEPTH  4

typedef enum
{
    KT_TYPE_HARD = 0, //Ӳ�����������޵�Ƶ��ʱ����������֧�ֿɶ���
    KT_TYPE_SOFT = 1, //��������֧�ֿɶ���
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
    LED_STATUS_DUTY, //ռ�ձȿ�����PWM
    LED_STATUS_DUTY_GPIO, //ռ�ձȿ�����GPIO + Ӳ����ʱ��
    LED_STATUS_BREATH, //������
    LED_STATUS_DUMMY, //��ͼռ�ݣ����Ʋ�����by jc0245
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

//LED�����
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

//����TTS���������е�ϵͳ��Ϣ
#define TTS_LANGUAGE_TOTAL     2   //֧��TTS������������
#define TTS_SECTIONS_MAX       16  //һ�������������Ƭ��������������1234����Ϊ��һǧ������ʮ�ġ�����7��

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
    uint8 slave_tts_count; //�����Ի���3��TTS ID
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
    BAT_DET_NULL, //��ʼ״̬/��������״̬
    BAT_DET_FALL, //�����µ�״̬
    BAT_DET_RISE, //��������״̬
} bat_det_fsm_e;

#define BAT_DET_VALUES_MAX  10 //5����ȥ������
#define BAT_DET_WIN_TIME    60 //��ص�����ⴰ�� 30S
#define BAT_DET_WIN_COUNT   10 //10�����ڹ۲죬5�����ȶ������ߵ���

#define POWEROFF_MODE_S3    0  //��ػ�
#define POWEROFF_MODE_S2    1  //S2�͹���ģʽ

/*!
 * \brief
 *  sys_counter_t ϵͳ���ܼ�ʱ�����ݽṹ������ʱ������0.5 sΪ��λ
 */
typedef struct
{
    /*! ʡ��ػ���ʱ�� */
    uint16 poweroff_counter;
    /*! UI����״̬��ʱ�� */
    uint16 ui_idle_counter;
    /*! LED�����������ʱ�� */
    uint16 light_counter;

    /*! LED���������ʱ�䣬1��Ϊ��λ��0��ʾ���ر� */
    uint16 light_timer;

    //ʡ��ػ�����/ģʽ/ʱ��
    uint8 poweroff_enable;
    uint8 poweroff_mode;
    uint8 poweroff_timer; //1����Ϊ��λ
    uint8 poweroff_timer_bt_wait; //1����Ϊ��λ

    /*! ��ص������ */
    uint16 bat_value_counter;
    bat_det_fsm_e bat_det_sta;
    uint8 bat_value_min; //����ͳ������һ��ʱ���⵽����С����
    uint8 bat_value_debounce;
    uint8 bat_value_index;
    uint8 bat_values[BAT_DET_VALUES_MAX];

    /*! �������־ */
    uint8  bat_full_flag;
    uint16 bat_full_cnt;

    /*! ��ص�������ʾ */
    uint8  bat_low_start; //FALSE��ʾ��δ��ʼ��ʾ��TRUE��ʾ�ѿ�ʼ��ʾ
    uint16 bat_low_cnt; //����������ʾ
    uint8  bat_value_win_min[BAT_DET_WIN_COUNT]; //ÿ30��һ������ͳ����Сֵ��ÿ�θ���ʱȥ����һ����������ǰŲ��Ȼ����ӵ����
    uint8  bat_value_cur_win_min; //��ǰ����ͳ�ƴ�����Сֵ
    uint8  bat_value_win_tick; //��ⴰ�ڼ�ʱ
    uint8  bat_value_win_count; //�Ѿ�ͳ�ƴ�����
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
    DC_KEY_STATUS_NULL = 0,        //û�а������жϳɹ��󣬽� key_type ��Ϊ KEY_TYPE_DOUBLE_CLICK
    DC_KEY_STATUS_FIRST_DOWN = 1,  //������һ�ΰ���
    DC_KEY_STATUS_FIRST_UP = 2,    //������һ�ζ̰�̧��
    DC_KEY_STATUS_SECOND_DOWN = 3, //�����ڶ��ΰ���
} double_click_key_status_e;

typedef enum
{
    DC_FAIL_NO_FAIL = 0,
    DC_FAIL_FIRSE_OVERTIME = 1,   //���һ��������̫�ñ�ɳ�����ʧ��
    DC_FAIL_FIRST_LONG = 2,       //��������������ʧ��
    DC_FAIL_SECOND_NOT_DOWN = 3,  //��ڶ�������û���ڶ�ʱ���ڰ��¶�ʧ��
    DC_FAIL_SECOND_OVERTIME = 4,  //��������������ʧ��
    DC_FAIL_SECOND_LONG = 5,      //��ڶ���������̫�ñ�ɳ�����ʧ��
    DC_FAIL_SECOND_OTHER_KEY = 6, //��ڶ����������˱�İ�����ʧ��
} double_click_fail_reason_e;

typedef enum
{
    DC_FAIL_STATUS_NULL = 0,             //ʧ�ܺ�տ�ʼִ�в�������
    DC_FAIL_STATUS_SEND_FIRST_DOWN = 1,  //ʧ�ܺ��Ѳ�����һ������DOWN��Ϣ
    DC_FAIL_STATUS_SEND_FIRST_UP = 2,    //ʧ�ܺ��Ѳ�����һ������UP��Ϣ
    DC_FAIL_STATUS_SEND_SECOND_DOWN = 3, //ʧ�ܺ��Ѳ����ڶ�������DOWN��Ϣ
} double_click_fail_status_e;

#define DOUBLE_CLICK_TIME_SPACE_MAX  250 //˫�������а��µ�̧��̧�𵽵ڶ����������µ�ʱ�����

typedef struct
{
    /*! ˫���������� */
    bool double_click_enable;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    //��ǰ�����������䰴�����ڻ��Ǵ�����䰴������
    key_period_e key_period;
    bool key_period_filter_master_flag; //���䰴�����ڹ������䰴��
    bool key_period_filter_slave_flag;  //���䰴�����ڹ��˴��䰴��
#endif

    /*! ˫������ֵ1 */
    key_value_e dc_key_1;
    /*! ˫������ֵ2 */
    key_value_e dc_key_2;
    /*! ��ǰ˫������ֵ */
    key_value_e dc_key_cur;
    /*! ˫������״̬�� */
    double_click_key_status_e dc_sta;
    /*! ˫������ȷ��ʧ�� */
    bool double_click_fail_flag;
    /*! ˫������ʧ��ԭ�� */
    double_click_fail_reason_e dc_fail_reason;
    /*! ˫�������ڶ��������������������ð���ֵ */
    key_value_e other_key;
    /*! ˫������ʧ��ִ��״̬�� */
    double_click_fail_status_e dc_fail_sta;
    /*! ˫�������жϼ�ʱ�� */
    uint32 dc_last_time;
} key_deal_infor_t;

extern key_deal_infor_t g_key_deal_infor;

//����ӳ��ӿ�����
extern bool com_key_mapping_inner(input_gui_msg_t *input_msg, msg_apps_type_e *gui_event,
        const key_map_t *key_map_list);

//common key����ӿ�����
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

//��������

#define TWS_SYNC_VOL_NULL       0 //0��ʾ�Ѿ�����250msû��ͬ������������ֱ��ͬ��
#define TWS_SYNC_VOL_RIGHT_NOW  1 //1��ʾ��ͬ����������Ҫ��ʱ250ms�Ż�ͬ�����������������ݰ�
#define TWS_SYNC_VOL_DELAY      2 //2��ʾ�ȴ�250msͬ������
#define TWS_SYNC_VOL_DELAY_TIME (250) //ͬ��������ʱʱ�� 250ms

typedef struct
{
    /*! ��ǰ����ģʽ */
    uint8 volume_mode;
    /*! ��ǰ���� */
    uint8 volume_current;
    /*! ��ǰ�������ֵ */
    uint8 volume_current_max;
    /*! ���������� */
    uint8 main_volume;
    /*! ��绰�������� */
    uint8 call_volume;
    /*! �Ƿ�������USB��������ģʽ�� */
    bool usbbox_volume_flag;
    /*! USB������������ */
    uint8 usbbox_volume;
    /*! �Ƿ�����IC�ڲ�PA������Ϊmute */
    bool mute;
    /*! HDMI�Ƿ��� */
    bool cec_mute;
    /*! �ⲿ����MAINʹ�ܣ�1��ʾ�򿪣�0��ʾ�ر� */
    bool pa_main_on;
    /*! �ⲿ����SUBʹ�ܴ�������0��ʾ�򿪣�0��ʾ�ر� */
    uint8 pa_sub_on_count;
    /*! DCDC��ѹ�Ƿ��Ѿ����� */
    bool dcdc_enable;
    /*! AB���D�๦��ģʽ,PA_SWITCH_D=0,PA_SWITCH_AB=1 */
    uint8 pa_ab_d_flag;
    /*! ��������ģʽ */
    uint8 gain_mode;
    /*! ������˥�� */
    int16 main_gain;
    /*! ��绰����˥�� */
    int16 call_gain;
    /*! line-in����˥�� */
    int16 linein_gain;
    /*! fm ����������˥�� */
    int16 radio_gain;
    /*! ��ǰ�������� */
    int16 gain_current;
    /*! ��ǰ��������ֵ��δ���� gain_current ֵ */
    int16 phy_current;
    /*! ���������������У���λ0.1db���ֱ�Ϊ����0�� ~ ����9����Ӧ����������� */
    int16 phy_limit_tbl[BATTERY_GRADE_MAX + 1];
    /*! �����������ƣ���λ0.1db������ */
    int16 phy_limit_current;
    /*! TWS ������Ҫͬ�����������䣬����ʱ250ms�Ż�ͬ�����������������ݰ� */
    uint32 tws_sync_volume_delay_timer;
    uint8  tws_sync_volume_value;
    uint8  tws_sync_volume_flag;
    uint8  tws_sync_volume_delay;
    /*! MICͨ����ǰ���� */
    uint8 mic_volume_current;
    /*! MICͨ������������ */
    int16 mic_soft_volume_gain;
} volume_infor_t;

extern volume_infor_t g_volume_infor;

///��ȡ�������TRUE��ʾ������FALSE��ʾ�Ǿ���
static inline bool get_mute_enable(void)
{
    return g_volume_infor.mute;
}

///���þ�����TRUE��ʾ������FALSE��ʾ�Ǿ���
static inline void set_mute_enable(bool mute)
{
    g_volume_infor.mute = mute;
}

///��ȡCEC�������TRUE��ʾ������FALSE��ʾ�Ǿ���
static inline bool get_cec_mute_enable(void)
{
    return g_volume_infor.cec_mute;
}

///��ȡCEC������TRUE��ʾ������FALSE��ʾ�Ǿ���
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

//����ܽӿ���������ϸ˵�����ӿڶ��壩
extern app_result_e com_message_box_inner(msg_apps_type_e msg_type);
extern void sys_hot_detect_handle_inner(void);
extern void com_save_sys_comval_inner(void);
extern void com_restore_sys_comval_by_reset_inner(void);

//���ȹ��ܽӿ�
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

//RTCʱ������ģ��

typedef struct
{
    /*! ����ռ�ñ�־��1��ʾ��ռ�� */
    uint8 alarm_used;
    /*! ���ӿ��� */
    uint8 alarm_onoff;
    /*! ����ʱ��, 3Bytes */
    time_t alarm_time;
    /*! �������ڣ�bit0 ~ bit6 �ֱ��ʾ���� ~ ������0��ʾֻ��һ�� */
    uint8 alarm_day;
    /*! �������ͣ���� ring_type_e ���� */
    uint8 ring_type;
    uint8 reserve;
    /*! ������� */
    ring_sound_u sound;
} alarm_info_t;

typedef struct
{
    /*! ���Ӽ�¼ */
    alarm_info_t alarm;
    /*! ��ǰ����ʱ�� */
    time_t last_time;
    /*! ����snooze���� */
    uint8 snooze;
} alarm_record_t;

#define ALARM_RECORD_MAX 1
typedef struct
{
    /*! VM ����ħ��*/
    uint16 magic;
    /*! �Ƿ�֧��̰˯ģʽ */
    bool snooze_enable;
    /*! snoozeʱ�䣬1-30���� */
    uint8 snooze_time;
    /*! snooze������1-10�� */
    uint8 snooze_max_count;
    /*! ��ʱʱ�䣬1-30����*/
    uint8 overtime;
    /*! ���ӳ�ʱ����ѡ��,0��ʾ�ر����ӣ�1��ʾ�Զ�ѡ��̰˯ */
    uint8 overtime_deal;
    /*! �������岥������ */
    uint8 alarm_volume;

    uint8 reserve[3];
    /*! ��ǰ���Ӽ�¼INDEX��0xff��ʾ��Ч */
    uint8 alarm_idx;
    /*! ���Ӽ�¼�б� */
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

#define RTC_TIME_OVERTIME  40 //20S��ʱ����

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
#define UI_OVERTIME 16 //8S��ʱ
extern app_result_e ui_slider_inner(slider_t *slider, slider_callback s_cbk);
extern app_result_e ui_set_contrast(void);
#endif

typedef struct
{
    /*! Ĭ����ֵ */
    uint16 default_value;
    /*! ��Чֵ���� */
    uint16 total;
    /*! ��ɢ�����б����20������ */
    uint16 value[20];
} echo_reverb_para_t;

#if (SUPPORT_FAST_PREVIEW == 1)
extern void com_fast_preview_cbk_inner(input_gui_msg_t *input_msg);
extern void com_fast_preview_update_ap_welcome_ui_inner(uint8 func_id, void *para);
extern fast_preview_status_e com_fast_preview_get_status_inner(void);
extern void com_fast_preview_next_app_inner(app_result_e switch_result);
#endif

#endif
