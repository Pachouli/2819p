/********************************************************************************
 * 	   Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 * 						   All Rights Reserved.
 *
 * ���������ز���ǰ̨Ӧ��ͷ�ļ�
 ********************************************************************************/

#ifndef __APP_MUSIC_H__
#define __APP_MUSIC_H__

#include "common_front_app_def.h"
#include "music_common.h"


typedef struct
{
    /*! ħ�� */
    uint16 magic;
    /*! eqģʽ */
    com_eqtype_e eq_type;
    /*! ѭ��ģʽ */
    fsel_mode_e repeat_mode;
    /*! shuffle ���� ȡ��4bit*/
    uint8 shuffle_flag;
    /*! �����ٶ�*/
    uint16 playspeed;
    /*!֧�ֿ�����*/
    uint8 card_open;

} music_config_t;

typedef struct
{
    uint8 enter_mode;
    uint8 play_disk;
} mode2disk_t;


//for globle variables

//globle variable

//����music�ķ�ʽ
extern uint8 g_enter_mode;

//�������������
extern uint16 g_error_num;

//ȫ�����л��̱�־
extern bool g_all_error_switch;

//��ǰ��
extern uint16 g_switch_prev;

//�ļ�·����Դ��Ϣ���棬����ʱ����
extern file_path_info_t g_file_path_info;

//��ǰ����״̬��Ϣ
extern mengine_status_t g_music_status;
extern play_status_e last_play_status;

//��ǰ����ʱ��ͱ�������Ϣ
extern mengine_playinfo_t g_play_info;

//music��ʱ����Ϣ
extern mmm_mp_file_info_t g_music_fileinfo;

//music ui����������Ϣ
extern music_config_t g_music_config;

//��ǰ������ʱ��
extern uint32 g_total_time;

//��ǰ����ʱ��
extern uint32 g_cur_time;

//ǰһ�μ�¼����ʱ��
extern uint32 g_prev_time;

extern uint8 last_minute, last_col;

//�ļ�ϵͳ���
extern uint32 eh_vfs_mount;

#if (SUPPORT_SPI_LCD_DISPLAY == 1)
extern uint8 g_fname_buffer[32];
extern bool  g_fname_scroll_flag;
extern bool  g_scroll_flag;
#endif

extern app_result_e music_scene_playing(void);

extern app_result_e music_scene_linein(void);

//for function rcode
extern app_result_e music_message_deal(private_msg_t *msg);

extern bool music_get_filepath(file_path_info_t *filepath);

extern bool music_get_status(mengine_status_t *play_status);

extern bool music_get_playinfo(mengine_playinfo_t *play_info);

extern bool music_get_playmode(uint8 *playmode);

extern bool music_get_filename(music_file_name_t *music_file_name);

extern bool music_play(void);

extern bool music_pause(void);

extern bool music_resume(void);

extern bool music_stop(void);

extern bool music_fast_forward(void);

extern bool music_fast_backward(void);

extern bool music_cancel_ffb(void);

extern bool music_send_msg(void *input_param, void *output_param, uint32 msg_type);

extern bool music_set_volume(uint8 volume);

extern bool music_set_playmode(uint8 playmode);

extern bool music_play_next(void);

extern bool music_play_prev(void);

extern bool music_switch_disk(uint8 disk_flag);

extern bool music_clear_error(void);

extern bool music_get_fileinfo(mmm_mp_file_info_t *fileinfo);

extern bool music_get_disknumber(uint16 *disk_num);

//�ֶ��и貥����Ŀ�Ż��и�TTS��֮���ٻָ�����
extern bool music_tts_play_song_seq(uint16 song_seq);
extern bool music_switch_song_play(void);

//�����и赽�ڼ���
extern uint16 g_manul_switch_song_seq;

extern app_result_e play_eror_deal(eg_err_e error_number);

//for play functions

extern app_result_e _scene_play_guimsg(input_gui_msg_t *gui_msg);

extern app_result_e play_key_mode(void);

extern app_result_e play_key_prev(void);

extern app_result_e play_key_next(void);

extern app_result_e play_key_play(void);

extern app_result_e play_key_playfast(uint8 direct);

extern app_result_e _scene_play_fast_deal(uint8 ffb);

extern app_result_e play_key_repeat_next(void);

extern app_result_e play_key_repeat_prev(void);

extern app_result_e play_key_folderadd(void);

extern app_result_e play_key_foldersub(void);

extern app_result_e _playing_loop_deal(void);

extern app_result_e play_disk_out_deal(uint8 out_disk);

extern app_result_e play_disk_switch_deal(uint8 dest_disk);

extern void _playing_show_seq(uint32 song_seq);

#endif //__APP_MUSIC_H__
