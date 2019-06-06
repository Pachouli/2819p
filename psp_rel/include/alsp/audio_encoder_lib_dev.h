/*******************************************************************************
 *                              5003
 *                            Module: musicenc
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       kkli     2008-09-18 11:00     1.0             build this file
*******************************************************************************/
/*!
 * \file     audio_encoder_lib_dev.h
 * \brief    定义了audioenc插件通用的接口
 * \author   kkli
 * \version 1.0
 * \date  2008/9/18
*******************************************************************************/
#ifndef __AUDIO_ENCODER_LIB_DEV_H__
#define __AUDIO_ENCODER_LIB_DEV_H__

#include "actal_libc_dev.h"
#include "./common/music_info.h"
#include "./common/storageio.h"
#include "./common/audioin_pcm.h"


/*!
* \brief  
*      音频编码引擎提供的命令字
*/
typedef enum
{
    AE_CMD_OPEN = 0,
    AE_CMD_CLOSE,
	AE_CMD_FRAME_ENCODE, 
	AE_CMD_GET_FHDR, 
	AE_CMD_SET_DENOISE,
	AE_CMD_RUN_DENOISE,
} audioenc_ex_ops_cmd_t;

/*!
* \brief  
*      控制线程内部的流转以及提供给外部的状态查询
*/
typedef enum
{
	/*! 启动录音之前或停止录音之后 */
	AE_STATUS_IDLE = 0,
	/*! 正常录音状态 */
	AE_STATUS_RECORDING,
	/*! 内部出错主动停止 */
	AE_STATUS_STOPPED,
	/*! 线程终止 */
	AE_STATUS_TERMINATED,
	/*! 启动监听 */
	AE_STATUS_MONITOR,
} ae_status_t;

typedef enum
{
    /*! 一般未知错误 */
    AE_RET_UNEXPECTED = -7,
    /*! 编码库加载出错 */
    AE_RET_LIBLOAD_ERROR,
    /*! 加载了非法的编码库 */
    AE_RET_LIB_ERROR,
     /*! 编码出错 */
    AE_RET_ENCODER_ERROR,
     /*! 采样率错误 */
    AE_RET_FS_ERROR,
    /*! 内存空间不够 */
    AE_RET_OUTOFMEMORY,
    /*! 格式不支持，不能继续解码 */
    AE_RET_UNSUPPORTED,
    /*! 正常 */
    AE_RET_OK,
    /*! 输出数据超过缓冲区大小 */
    AE_RET_DATAOVERFLOW,
} audioenc_ret_t;

typedef struct
{
    /*! 编码一帧所需的样本数 */
    int samples_per_frame;
    /*! payload/packet大小，单位字节 */
    int chunk_size;
    /*! 初始写入文件起始的数据 */
    /*! 数据指针 */
    void *buf;
    /*! 数据长度 */
    int buf_len;
} audioenc_attribute_t;


#define AUDIO_ENCODER_CMD_ENTRY     (*((uint32*)(AL_ENCODER_WAV_ENTRY_VA_START)))
/*!
*  \brief
*     接口函数声明
*     接口函数定义在kernel_api.S，加载驱动以后使用，驱动API号在api.h，后续会统一整理
 */
//extern int audio_encoder_cmd(void *, audioenc_ex_ops_cmd_t, unsigned int)  __FAR__;
#define audio_encoder_cmd(a,b,c)    ((int(*)(void *handel, audioenc_ex_ops_cmd_t cmd, uint32 args))AUDIO_ENCODER_CMD_ENTRY)((a), (b),(c))


#define SBC_ENCODER_CMD_ENTRY       (*((uint32*)(AL_ENCODER_SBC_ENTRY_VA_START)))
/*!
*  \brief
*     接口函数声明
*     接口函数定义在kernel_api.S，加载驱动以后使用，驱动API号在api.h，后续会统一整理
 */
//extern int sbc_encoder_cmd(void *, audioenc_ex_ops_cmd_t, unsigned int)  __FAR__;
#define sbc_encoder_cmd(a,b,c)      ((int(*)(void *handel, audioenc_ex_ops_cmd_t cmd, uint32 args))SBC_ENCODER_CMD_ENTRY)((a), (b),(c))

#endif // __AUDIO_ENCODER_LIB_DEV_H__
