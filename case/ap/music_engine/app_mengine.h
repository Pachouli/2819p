/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：本地播歌引擎应用 头文件
 ********************************************************************************/

#ifndef __APP_MENGINE_H__
#define __APP_MENGINE_H__

#include "common_engine_app_def.h"
#include "music_common.h"
#include "enhanced.h"
#include "mmm_mp.h"

#define ENABLE_OK_MIC_IDLE_MODE  1 //各应用engine可独立定义是否支持空闲模式OK MIC

#define PRINT_FILE_NAME

//快进退参数
#define MUSIC_FFB_STEP         2000//5000
#define MUSIC_FFB_DELAY_TIME   250
#define MUSIC_FFB_PLAY_TIME    500

#define MAX_FILE_NAME  0X20

//音乐播放模式
typedef enum
{
    //正常播放
    PLAY_NORMAL,
    //续播
    PLAY_RESUME,
    //继续快进
    PLAY_FFON,
    //快退播放
    PLAY_FBON,
    //不需要播放
    PLAY_NO_PLAY
} play_mode_e;

//音乐停止模式
typedef enum
{
    //正常停止
    STOP_NORMAL,
    //暂停
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
    /*! 魔数 */
    uint16 magic;
    /*正常退出应用标记;0-正常退出；1-异常退出*/
    uint8 normal_exit_flag;
    /*! 0表示断点是安全的，不会导致死机，1表示未验证断点是否安全*/
    uint8 bk_infor_invalid_flag;
    /*! shuffle 开关 取高4bit*/
    uint8 shuffle_flag;
    /*! repeat mode*/
    fsel_mode_e repeat_mode;
    /*! 文件选择类型 列表/收藏夹/audible/目录 */
    fsel_type_e fsel_type;
    uint8 reserve; //word对齐
    /*! 记录当前文件的路径信息/列表文件的层级关系*/
    musfile_location_u location;
    /*! 断点信息*/
    mmm_mp_bp_info_t bk_infor;
    //for file name
    uint8 name_buf[32];
} mengine_config_t;

/*!
 * \brief
 *  引擎信息结构体变量定义
 */
typedef struct
{
    /******************************************************************
     *          引擎状态信息
     *******************************************************************/
    mengine_status_t eg_status;

    /******************************************************************
     *          播放文件的时间和bit率信息
     *******************************************************************/
    mengine_playinfo_t eg_playinfo;

    /******************************************************************
     *          中间件播放文件信息
     *******************************************************************/
    mmm_mp_file_info_t eg_file_info;

    /******************************************************************

     *          设置信息(保存配置)
     *******************************************************************/
    mengine_config_t eg_config;

} mengine_info_t;

//for vram write
typedef struct
{
    /*! 记录当前文件的路径信息/列表文件的层级关系*/
    musfile_location_u location;
    /*! 断点信息*/
    mmm_mp_bp_info_t bk_infor;
} mengine_bkinfo_t;

//硬件断点信息
typedef struct
{
    mmm_mp_bp_info_t bk_infor; //断点信息
    uint32 bk_checksum;
} hardware_bkinfor_t;

//globle variable
//保存中间件句柄
extern void *mp_handle;
extern mmm_mp_status_t g_mp_status;

//存放文件系统id
extern uint32 g_file_sys_id;

extern uint8 g_open_disk;

//进入模式
extern mengine_enter_mode_e g_mengine_enter_mode;

//引擎信息
extern mengine_info_t g_mengine_info;

//文件名buffer
extern uint8 g_file_name[MAX_FILE_NAME];

//系统全局变量

//控件返回值
extern app_result_e g_mengine_result;

//引擎配置变量结构体指针
extern mengine_config_t *g_eg_cfg_p;

//引擎状态变量结构体指针
extern mengine_status_t *g_eg_status_p;

//引擎播放信息结构体指针
extern mengine_playinfo_t *g_eg_playinfo_p;

//因TTS播报而暂停播放音乐，TTS播报结束后需要恢复播放音乐
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
