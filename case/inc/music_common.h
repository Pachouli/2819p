/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：本地播歌相关宏定义，枚举类型，结构体定义等，为本地播歌前台和引擎共享。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef __MUSIC_COMMON_H__
#define __MUSIC_COMMON_H__

#include "psp_includes.h"
#include "enhanced.h"

#define SUPPORT_RES_DIR     1  //是否支持内置磁盘资源文件播放
#define SUPPORT_EXTLIB_PLAY 0  //是否支持extlib歌曲播放

/*!
 * \brief
 *  当前播放状态
 */
typedef enum
{
    /*! 停止 */
    StopSta = 0,
    /*! 暂停 */
    PauseSta,
    /*! 播放 */
    PlaySta,
    /*! 快进退 */
    PlayFast
} play_status_e;

/*!
 * \brief
 *  AB复读状态
 */
typedef enum
{
    /*! 非AB复读状态 */
    PlayAB_Null,
    /*! 定A点状态 */
    PlayAB_A,
    /*! 定B点状态 */
    PlayAB_B,
    /*! AB复读完成状态 */
    PlayAB_Do
} ab_status_e;

/*!
 * \brief
 *  快进退状态
 */
typedef enum
{
    /*! 正常播放 */
    FBPlay_Null,
    /*! 快进状态 */
    FFPlaySta,
    /*! 快退状态 */
    FBPlaySta,
    /*! 快进到尾恢复正常播放 */
    FFPlayTailSta,
    /*! 快进到头从头开始播放 */
    FBPlayHeadSta,
} fast_status_e;

/*!
 * \brief
 *  错误信息
 */
typedef enum
{
    /*! 没有错误 */
    EG_ERR_NONE = 0,
    /*磁盘无文件*/
    EG_ERR_NO_FILE,
    /*介质拔出*/
    EG_ERR_DISK_OUT,
    /*! 打开文件失败 */
    EG_ERR_OPEN_FILE,
    /*! 文件格式不支持 */
    EG_ERR_NOT_SUPPORT,
    /*! 解码出错 */
    EG_ERR_DECODER_ERROR,
    /*! 无license，播放次数已满或是播放时间已经到期  */
    EG_ERR_NO_LICENSE,
    /*! DRM时钟错误  */
    EG_ERR_SECURE_CLOCK,
    /*! DRM信息错误  */
    EG_ERR_DRM_INFO
} eg_err_e;

/*!
 * \brief
 *  引擎状态
 */
typedef struct
{
    /*! 播放状态  */
    play_status_e play_status;
    /*! AB复读状态  */
    ab_status_e ab_status;
    /*! 快进退状态  */
    fast_status_e fast_status;
    /*! 错误状态,获取后清除  */
    eg_err_e err_status;
    /*! 低能量持续时间 */
    int32 lowpw_time;
} mengine_status_t;

/*!
 * \brief
 *  当前播放文件信息
 */
typedef struct
{
    /*! 当前时间 */
    uint32 cur_time;
    /*! 比特率 */
    uint32 cur_bitrate;
    /*! audible图片位置偏移 */
    uint32 cur_imag_pos;
    /*! audible图片存在标志(非分辨图) */
    uint8 cur_imag_flag;
    /*! 歌词存在标志 */
    uint8 cur_lyric;
    /*! 文件切换标志 bit0: 歌曲是否切换 bit1:audible章节是否切换  */
    uint8 cur_file_switch;
} mengine_playinfo_t;


//MUSIC前台进入模式
#define PARAM_MUSIC_CARD_DIR        0 //卡盘目录播放
#define PARAM_MUSIC_UHOST_DIR       1 //U盘目录播放
#define PARAM_MUSIC_CARD_REPLAY     2 //卡录音回放
#define PARAM_MUSIC_UHOST_REPLAY    3 //U盘录音回放
#define PARAM_MUSIC_RES_DIR         4 //NOR 资源文件播放
#define PARAM_MUSIC_UHOST_DIR1_PLAY 5 //U盘目录1播放
#define PARAM_MUSIC_UHOST_DIR2_PLAY 6 //U盘目录2播放
#define PARAM_MUSIC_CARD_DIR1_PLAY  7 //卡盘目录1播放
#define PARAM_MUSIC_CARD_DIR2_PLAY  8 //卡盘目录2播放

/*!
 * \brief
 *  开启music引擎进入模式；高4bit用来表示盘符 0x00 for DISK_SD, 0x10 for DISK_H, 0x20 for DISK_U
 */
typedef enum
{
    /*! 选择歌曲播放 */
    ENTER_MUSIC_START = 0x00,
    /*! NOR内置资源列表播放 */
    ENTER_MDISK_PLAY = 0x01,
    /*! 录音回放 */
    ENTER_RECORD_PLAY = 0x02,
    /*! 闹铃播放 */
    ENTER_ALARM_PLAY = 0x03,
    /*特定目录1播放*/
    ENTER_DIR1_PLAY = 0x04,
    /*特定目录2播放*/
    ENTER_DIR2_PLAY = 0x05,
} mengine_enter_mode_e;

typedef struct
{
    /*! 文件名缓冲区 */
    uint8 *fname_buffer;
    /*! 文件名最大长度 */
    uint8 fname_length;
} music_file_name_t;

#endif //__MUSIC_COMMON_H__
