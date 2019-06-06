/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������FM������ǰ̨Ӧ��ͷ�ļ���
 * ���ߣ�cailizhen
 ********************************************************************************/

#ifndef __APP_RADIO_H__
#define __APP_RADIO_H__

#include "common_front_app_def.h"
#include "fm_common.h"

//#define FM_THRESHOLD_TEST

#ifdef FM_THRESHOLD_TEST

#define MAX_TH_VALUE 9
#define MIN_TH_VALUE 0
extern int16 fm_threshold_values[512]; //����ֵ��¼,-1��ʾ��������

#endif

/* ����Ԥ���̨�б�����֧�ֵı����̨�����迪����Ӧ��Сbuffer*/
#define    MAX_STATION_COUNT    40

/* �����������СƵ��*/
#define    MIN_US      87500
#define    MIN_JP      76000
#define    MIN_EU      87500
/* ������������Ƶ��*/
#define    MAX_US      108000
#define    MAX_JP      90000
#define    MAX_EU      108000

typedef enum
{
    DIR_DOWN = 0, //������̨
    DIR_UP //������̨
} FM_SeekDir_e;

typedef enum
{
    Band_China_US = 0, //china/USA band   87500---108000,  step 100kHz
    Band_Japan, //Japan band   76000---90000, step 100kHz
    Band_Europe, //Europe band  87500---108000, step 50kHz
    Band_MAX //�����û���̨ʱ������ģʽ����ΪBand_MAX
} radio_band_e;

//radio AP��������
typedef enum
{
    /* FM ���ų���*/
    FM_SCENE_PLAYING,
    /* FM�Զ���̨���� */
    FM_SCENE_AUTO_SEARCH,
    /* FM�ֶ���̨����-������� */
    FM_SCENE_MANUAL_UP_SEARCH,
    /* FM�ֶ���̨����-��ǰ���� */
    FM_SCENE_MANUAL_DOWN_SEARCH,
    /* �����˳�*/
    FM_SCENE_EXIT,
} radio_scene_e;

typedef struct
{
    uint32 freq; //��ǰ��̨��Ƶ�ʡ���λΪ1k (���λ��Ϊ1)��ȡ���2 �ֽ�
    uint8 station; //��ǰ��̨��Ӧ�ĵ�̨����ţ�from 1 to station_count����ǰ��̨δ����ʱΪ0
    uint8 station_count; //��̨���еı����̨����
} fm_play_status_t;

//radio ui���ò���
typedef struct
{
    /* ħ����*/
    uint16 magic;
    /* ����Ԥ����Ϣ��ע���⼸������λ��Ҫ������ com_fast_preview_ui.c �е���Ե�ַһ�£��ϵ��ᱣ�浽 fast_preview_info_t �� */
    uint8 preview_station_num; //0��0xff��ʾ������Ч��̨��1-40��ʾ��Ч��̨��
    uint8 reserve;
    uint32 preview_freq;

    /* Ԥ���б�*/
    uint32 Auto_tab[MAX_STATION_COUNT];
    /* ��ǰ���ŵĵ�̨��Ϣ*/
    fm_play_status_t FMStatus;
    /* ��ǰ���Ų���*/
    radio_band_e band_mode;
} radio_config_t;


//ȫ�ֱ�������
extern radio_scene_e g_radio_scene;

extern radio_config_t g_radio_config;

extern uint32 *pfreq;

extern FM_Drv_Status_t g_engine_status;

extern fmeg_status_t g_fmeg_status;
extern play_status_e g_last_fmeg_status;

extern const uint32 max_freq[];

extern const uint32 min_freq[];

extern uint32 startfreq;

extern uint8 g_enter_param;

extern bool need_tts;

extern uint32 g_radio_search_th;

extern uint8 g_freq_tts_sec_ids[10];

//for search cancel
extern const key_map_t cancel_key_map_list[];

extern bool  g_radio_seek_audition_enable;
extern uint8 g_radio_seek_audition_time;

extern uint32 g_number_input_freq;
extern uint32 g_number_input_timer;
extern uint8 g_number_input_high;
extern uint8 g_number_input_keys;
extern void get_border_freq(uint32 *start_freq, uint32 *end_freq, FM_SeekDir_e dir);

//��������

extern app_result_e radio_app_msg_callback(private_msg_t *pri_msg);

extern app_result_e scene_play_sysmsg(void);

extern bool radio_set_mute(FM_MUTE_e mode);

extern bool radio_set_freq(uint32 freq);

extern bool radio_set_softseek(uint32 freq, uint8 dir);

extern bool radio_set_throd(uint8 val);

extern bool radio_get_freq(void);

extern uint8 whether_in_list(uint32 freq, uint32 *p_freq);

extern app_result_e radio_scene_playing(bool first_flag);

extern app_result_e _playing_loop_deal(void);

extern app_result_e scene_play_guimsg(input_gui_msg_t *cur_msg);

extern app_result_e radio_scene_auto_search(FM_SeekDir_e dir);

extern app_result_e radio_scene_manual_search(FM_SeekDir_e dir);

#define SELECT_PREV_DIRECTION  0

#define SELECT_NEXT_DIRECTION  1

extern app_result_e select_station(uint8 direction);

extern app_result_e change_freq(FM_SeekDir_e dir);

extern app_result_e play_next_station(uint8 direction);

extern app_result_e radio_scene_play_search_deal(FM_SeekDir_e search_dir);

extern app_result_e radio_play_next_freq(void);

extern app_result_e radio_play_prev_freq(void);

extern app_result_e radio_play_pause_station(void);

extern void radio_freq_tts_play(uint32 freq);

extern app_result_e deal_soft_auto_seek(FM_SeekDir_e dir);

extern bool radio_send_msg(void *input_param, void *output_param, uint32 msg_type);

extern bool radio_resume(void);

extern bool radio_pause(void);

extern bool radio_get_play_status(fmeg_status_t *p_fmeg_status);

extern bool radio_get_fm_status(FM_Drv_Status_t *p_fm_drv_status);

extern void radio_show_frequency(uint32 freq, bool high_number_show);
extern void radio_show_station_num(uint8 station_num, bool msg_show);
extern void _create_engine(void);

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_RADIO_RECORD_FUNC == 1))
extern bool radio_record_start(void);
extern bool radio_record_stop(void);
#endif

#endif //__APP_RADIO_H__
