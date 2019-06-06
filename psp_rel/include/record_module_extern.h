/*! \cond MMM_MP_API*/
/********************************************************************************
 *                              5110
 *                            Module: music_player
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       jpliao    2010-08-27 15:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     record_module_extern.h
 * \brief    定义录音模块，可以为任意模块提供录音
 * \author   cailizhen
 * \version  1.0
 * \date     2018-8-18
 *******************************************************************************/
/*! \endcond*/
#ifndef __RECORD_MODULE_EXTERN_H__
#define __RECORD_MODULE_EXTERN_H__


#define SUPPORT_RECORD_FUNC  0 //是否支持录音功能


#ifdef __cplusplus
extern "C"
{
#endif

/*!
 * \brief
 *      加载系统头文件
 */
#ifdef WIN32
/* VC */
#define __FAR__
#else
/* MIPS */
#include <psp_includes.h>
#endif

/*! \cond MMM_MP_API */

#define ENCODE_WAV_HEAD_SIZE    0x200 //录音文件WAV文件头长度

#define ENCODE_OUT_FRAME_SIZE   64 //帧输出长度，修改后需要确认 ENCODE_OUT_BUFFER_SIZE 和 ENCODE_IN_FRAME_SIZE 是否需要修改；如果是立体声，则需要在中间件×2
#define ENCODE_OUT_BUFFER_SIZE  0x200 //满足2条件：1.不小于512字节文件头长度。2.不小于ENCODE_OUT_FRAME_SIZE

#define ENCODE_IN_FRAME_SIZE    256 //不小于ENCODE_OUT_FRAME_SIZE×4；如果是立体声，则需要在中间件×2

#define BYPASS_FRAME_SIZE       128 //BYPASS模式帧长度

//单声道录音时数据源选择
#define RECORD_SOURCE_LR_MIX    0 //左右混合，对于通话来说就是喇叭和MIC混音
#define RECORD_SOURCE_L_CH      1 //左声道，对于通话来说就是单录喇叭
#define RECORD_SOURCE_R_CH      2 //右声道，对于通话来说就是单录MIC

#define RECORD_FREQ_MIPS        10 //启动录音需要10M运算量

/*!
 * \brief
 *      录音模块提供给应用层的命令字
 */
typedef enum
{
    /*! 打开录音模块 */
    RECORD_MODULE_OPEN = 0,
    /*! 关闭录音模块 */
    RECORD_MODULE_CLOSE,

    /*! 设置录音文件句柄 */
    RECORD_MODULE_SET_FS,
    /*! 设置录音编码器参数 */
    RECORD_MODULE_SET_ENCODE,
    /*! 设置缓冲区 */
    RECORD_MODULE_SET_BUFFER,

    /*! 启动录音模块 */
    RECORD_MODULE_START,
    /*! 停止录音模块 */
    RECORD_MODULE_STOP,

    /*! 帧处理，如果有编码就编码，没有就直接推送到output cache */
    RECORD_MODULE_FRAME_DEAL,

    /*! 获取编码细节信息，比如帧输入大小 */
    RECORD_MODULE_GET_ENCODE_DETAIL,

    /*! 读取状态和一些信息 */
    RECORD_MODULE_GET_STATUS,
} record_module_cmd_e;

/*!
 *  \brief
 *      录音模块状态
 */
typedef enum
{
    RECORD_MODULE_STA_CLOSE = 0,
    RECORD_MODULE_STA_OPEN,
    RECORD_MODULE_STA_START,
    RECORD_MODULE_STA_STOP,
    RECORD_MODULE_STA_ERROR,
} record_module_status_e;

/*!
 * \brief
 *      错误号定义,即mmm_mr_status_t中err_no取值
 */
typedef enum
{
    /*! 一般未知错误 */
    MMM_MR_ERR_UNEXPECTED = -4,
    /*! 文件超出大小限制 */
    MMM_MR_ERR_FILE_SIZE_OVER_LIMIT = -3,
    /*! 磁盘满错误 */
    MMM_MR_ERR_DISK_FULL = -2,
    /*! 文件写入错误，但是文件系统未报异常 */
    MMM_MR_ERR_WRITE_FILE_FAIL = -1,
    /*! 正常 */
    MMM_MR_ERR_OK = 0,
} record_module_err_e;

typedef struct
{
    record_module_status_e status;
    record_module_err_e    err_no;
    uint32 start_cluster;
    uint32 write_pages;
    uint32 time;
} record_module_status_t;

typedef enum
{
    MMM_RECORD_STOP = 0,
    MMM_RECORD_RECORDING = 1,
    MMM_RECORD_ERR_PAUSE = 2,
} mmm_record_status_e;

typedef struct
{
    uint16 encode_frame_len;
} record_module_encode_detail_t;

/*!
 *  \brief
 *     打开文件所需的参数信息
 */
typedef struct
{
    /*! 0: vfs, 1: sd file, 2: file handle*/
    int fs_type;
    /*! need mount id when fs_type == 0*/
    void *file_mount_id;
    /*! need file handle when fs_type == 2, need file name when fs_type == 1*/
    void *file_handle;
    /*! 文件最大长度，单位为扇区 */
    uint32 file_max_length;
} record_module_fs_para_t;

/*!
 * \brief
 *      编码格式
 */
typedef enum
{
    /*! PCM */
    ENCODE_PCM = 0,
    /*! ADPCM */
    ENCODE_ADPCM,
    /*! MP3 */
    ENCODE_MP3,
    /*! ACT */
    ENCODE_ACT,
    /*! BYPASS，不做编码 */
    ENCODE_BYPASS,
} record_module_encode_mode_e;

/*!
 *  \brief
 *     音频编码的参数设置
 */
typedef struct
{
    /*! 音频编码的模式 */
    record_module_encode_mode_e encode_mode;
    /*! 音频采样的通道数如:1 单声道；2 双声道*/
    uint8 channel;
    /*! 录音音源，如 RECORD_SOURCE_LR_MIX */
    uint8 record_source;
    /*! 采样精度，默认16*/
    uint8 bps;
    /*! 编码的比特率，如64000,128000等*/
    int bitrate;
    /*! 采样频率如8000，16000，44100等*/
    int sample_rate_hz;
} record_module_encode_para_t;

/*!
 *  \brief
 *     栈空间和缓冲区设置
 */
typedef struct
{
    uint8 *ptos;
    uint32 stk_size;
    uint8 *output_cache;
    uint8 *output_buffer;
    uint16 output_cache_len;
} record_module_buffer_set_t;

#define RECORD_MODULE_CMD_ENTRY     (*((uint32*)(RECORD_MODULE_ENTRY_VA_START)))
/*!
 *  \brief
 *     接口函数声明
 *     驱动API号在api.h
 */
#define record_module_cmd(a,b,c)    ((int32(*)(void *handel, record_module_cmd_e cmd, uint32 args))RECORD_MODULE_CMD_ENTRY)((a), (b),(c))

/*! \endcond */
#ifdef __cplusplus
}
#endif

#endif // __MMM_MP_H__
