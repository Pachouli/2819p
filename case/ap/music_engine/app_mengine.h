/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * ���������ز�������Ӧ�� ͷ�ļ�
 ********************************************************************************/

#ifndef __APP_MENGINE_H__
#define __APP_MENGINE_H__

#include "common_engine_app_def.h"
#include "music_common.h"
#include "enhanced.h"
#include "mmm_mp.h"

#define ENABLE_OK_MIC_IDLE_MODE  1 //��Ӧ��engine�ɶ��������Ƿ�֧�ֿ���ģʽOK MIC

#define PRINT_FILE_NAME

//����˲���
#define MUSIC_FFB_STEP         2000//5000
#define MUSIC_FFB_DELAY_TIME   250
#define MUSIC_FFB_PLAY_TIME    500

#define MAX_FILE_NAME  0X20

//���ֲ���ģʽ
typedef enum
{
    //��������
    PLAY_NORMAL,
    //����
    PLAY_RESUME,
    //�������
    PLAY_FFON,
    //���˲���
    PLAY_FBON,
    //����Ҫ����
    PLAY_NO_PLAY
} play_mode_e;

//����ֹͣģʽ
typedef enum
{
    //����ֹͣ
    STOP_NORMAL,
    //��ͣ
    STOP_PAUSE
} stop_mode_e;

typedef enum
{
    NORMAL_SWITCH_NEXT = 0x0,
    NORMAL_SWITCH_PREV = 0x1,
    FORCE_SWITCH_NEXT = 0x2,
    FORCE_SWITCH_PREV = 0x3,
    END_SWITCH = 0x4
} switch_mode_e;

typedef enum
{
    SWITCH_NO_ERR,
    SWITCH_ERR_STOP,
    SWITCH_ERR_DISK_OUT,
    SWITCH_ERR_SWITCH,
    SWITCH_ERR_PLAY
} switch_result_e;

typedef enum
{
    CANCLE_FAST_BACKWARD_FORWARD = 0,
    FAST_FORWARD,
    FAST_BACKWARD
}set_ffb_mode;

typedef struct
{
    /*! ħ�� */
    uint16 magic;
    /*�����˳�Ӧ�ñ��;0-�����˳���1-�쳣�˳�*/
    uint8 normal_exit_flag;
    /*! 0��ʾ�ϵ��ǰ�ȫ�ģ����ᵼ��������1��ʾδ��֤�ϵ��Ƿ�ȫ*/
    uint8 bk_infor_invalid_flag;
    /*! shuffle ���� ȡ��4bit*/
    uint8 shuffle_flag;
    /*! repeat mode*/
    fsel_mode_e repeat_mode;
    /*! �ļ�ѡ������ �б�/�ղؼ�/audible/Ŀ¼ */
    fsel_type_e fsel_type;
    uint8 reserve; //word����
    /*! ��¼��ǰ�ļ���·����Ϣ/�б��ļ��Ĳ㼶��ϵ*/
    musfile_location_u location;
    /*! �ϵ���Ϣ*/
    mmm_mp_bp_info_t bk_infor;
    //for file name
    uint8 name_buf[32];
} mengine_config_t;

/*!
 * \brief
 *  ������Ϣ�ṹ���������
 */
typedef struct
{
    /******************************************************************
     *          ����״̬��Ϣ
     *******************************************************************/
    mengine_status_t eg_status;

    /******************************************************************
     *          �����ļ���ʱ���bit����Ϣ
     *******************************************************************/
    mengine_playinfo_t eg_playinfo;

    /******************************************************************
     *          �м�������ļ���Ϣ
     *******************************************************************/
    mmm_mp_file_info_t eg_file_info;

    /******************************************************************

     *          ������Ϣ(��������)
     *******************************************************************/
    mengine_config_t eg_config;

} mengine_info_t;

//for vram write
typedef struct
{
    /*! ��¼��ǰ�ļ���·����Ϣ/�б��ļ��Ĳ㼶��ϵ*/
    musfile_location_u location;
    /*! �ϵ���Ϣ*/
    mmm_mp_bp_info_t bk_infor;
} mengine_bkinfo_t;

//Ӳ���ϵ���Ϣ
typedef struct
{
    mmm_mp_bp_info_t bk_infor; //�ϵ���Ϣ
    uint32 bk_checksum;
} hardware_bkinfor_t;

//globle variable
//�����м�����
extern void *mp_handle;
extern mmm_mp_status_t g_mp_status;

//����ļ�ϵͳid
extern uint32 g_file_sys_id;

extern uint8 g_open_disk;

//����ģʽ
extern mengine_enter_mode_e g_mengine_enter_mode;

//������Ϣ
extern mengine_info_t g_mengine_info;

//�ļ���buffer
extern uint8 g_file_name[MAX_FILE_NAME];

//ϵͳȫ�ֱ���

//�ؼ�����ֵ
extern app_result_e g_mengine_result;

//�������ñ����ṹ��ָ��
extern mengine_config_t *g_eg_cfg_p;

//����״̬�����ṹ��ָ��
extern mengine_status_t *g_eg_status_p;

//���沥����Ϣ�ṹ��ָ��
extern mengine_playinfo_t *g_eg_playinfo_p;

//��TTS��������ͣ�������֣�TTS������������Ҫ�ָ���������
extern bool g_stop_by_tts_flag;

#ifdef DIR_NAME_SUPPORT

extern uint8 g_dir_name_buf[32];

#endif
extern uint16 play_file_num;

//for dir name buffer
//extern const uint8 g_music_folder_name_long[][12];
//extern const uint8 g_music_folder_name_short[][12];

//for folder vram
extern const uint32 g_music_folder_vm_addr[];
//functions

//extern bool mengine_file_init(void);
uint16 get_bp_checksum(void);

extern bool _set_file(void);
extern bool _play(play_mode_e play_mode);
extern bool _stop(stop_mode_e stop_mode);
extern bool _get_mmm_status(void);
extern bool _get_breakpoint(void);
extern bool _set_ffb(set_ffb_mode mode);
extern bool _get_file_info(void);
extern bool _set_eq(void);
extern bool _get_errno_no(void);

extern bool _mengine_check_disk_in(uint8 disk_type);

extern void mengine_reply_msg(private_msg_t *msg_ptr, bool ret_vals);

extern app_result_e mengine_control_block(void);

extern app_result_e mengine_message_done(private_msg_t *cur_msg);

extern app_result_e mengine_message_done_bank(private_msg_t *cur_msg);

extern switch_result_e _mengine_file_switch(stop_mode_e stop_mode, switch_mode_e force_mode, play_mode_e play_mode);

extern app_result_e mengine_get_dirname(private_msg_t *msg_ptr);

//for event

extern app_result_e mengine_get_playmode(private_msg_t *msg_ptr);

extern app_result_e mengine_get_filepath(private_msg_t *msg_ptr);

extern app_result_e mengine_get_status(private_msg_t *msg_ptr);

extern app_result_e mengine_get_playinfo(private_msg_t *msg_ptr);

extern app_result_e mengine_eq_set(private_msg_t *msg_ptr);

extern app_result_e mengine_set_playmode(private_msg_t *msg_ptr);

extern app_result_e mengine_play_folder_next(private_msg_t *msg_ptr);

extern app_result_e mengine_play_folder_prev(private_msg_t *msg_ptr);

extern app_result_e mengine_play(private_msg_t *msg_ptr);

extern app_result_e mengine_stop(private_msg_t *msg_ptr);

extern app_result_e mengine_pause(private_msg_t *msg_ptr);

extern app_result_e mengine_resume(private_msg_t *msg_ptr);

extern app_result_e mengine_fast_forward(private_msg_t *msg_ptr);

extern app_result_e mengine_fast_backward(private_msg_t *msg_ptr);

extern app_result_e mengine_cancel_ffb(private_msg_t *msg_ptr);

extern app_result_e mengine_play_next(private_msg_t *msg_ptr);

extern app_result_e mengine_play_prev(private_msg_t *msg_ptr);

extern app_result_e mengine_set_filepath(private_msg_t *msg_ptr);

extern app_result_e mengine_clear_error(private_msg_t *msg_ptr);

extern app_result_e mengine_get_filename(private_msg_t *msg_ptr);

//extern bool _mengine_special_switch(uint8 direct, uint16 dest_num);

extern app_result_e mengine_get_fileinfo(private_msg_t *msg_ptr);

extern app_result_e mengine_set_sortnumber(private_msg_t *msg_ptr);

extern app_result_e mengine_get_disknumber(private_msg_t *msg_ptr);

//for vram write&read
extern void mengine_vram_write(uint8 save_disk);
extern int32 mengine_vram_read(uint8 restore_disk, mengine_config_t * p_cfg);

//for tts sychonous
extern app_result_e mengine_tts_start(private_msg_t *msg_ptr);

extern app_result_e mengine_tts_stop(private_msg_t *msg_ptr);

extern app_result_e mengine_enter_s2(private_msg_t *cur_msg);

#endif //__APP_MENGINE_H__
