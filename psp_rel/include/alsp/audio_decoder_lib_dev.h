/*******************************************************************************
 *                              5003
 *                            Module: musicdec
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       kkli     2008-09-18 15:00     1.0             build this file
*******************************************************************************/
/*!
 * \file     audio_decoder_lib_dev.h
 * \brief    音频解码库所需要的头文件
 * \author   kkli
 * \version 1.0
 * \date  2008/09/18
*******************************************************************************/
#ifndef __AUDIO_DECODER_LIB_DEV_H__
#define __AUDIO_DECODER_LIB_DEV_H__

#include "actal_libc_dev.h"
#include "./common/music_info.h"
#include "./common/storageio.h"
#include "./common/audiout_pcm.h"

/*!
 * \brief
 *      定义音频解码返回的类型
 */
typedef enum
{
    /*! 一般未知错误 */
    AD_RET_UNEXPECTED = -3,
    /*! 内存空间不够 */
    AD_RET_OUTOFMEMORY,
    /*! 格式不支持，不能继续解码 */
    AD_RET_UNSUPPORTED,
    /*! 正常 */
    AD_RET_OK,
    /*! 输入数据不够 */
    AD_RET_DATAUNDERFLOW,
} audiodec_ret_t;

//SBC解码库返回值
typedef enum
{
    SBC_AD_RET_OK = 0,              //正常
    BITSTREAM_TOO_SHORT = -1,       //Data stream too short
    SYNC_BYTE_INCORRECT = -2,       //Sync byte incorrect
    CRC8_INCORRECT = -3,            //CRC8 incorrect
    BITPOOL_VALUE_INCORRECT = -4,   //Bitpool value out of bounds
    SUBBAND_VALUE_INCORRECT = -5,   //subbands value error
    POINTER_VALUE_INCORRECT = -6,   //decode-strcut pointer is NULL
    SYNC_SEARCH_ERROR = -7,         //can not syn
    SBCDEC_LEN_ERROR = -8,          //frame len is zero
} sbc_audiodec_ret_t;

/*!
 * \brief
 *      定义音频解码命令的类型
 */
typedef enum
{
    AD_CMD_OPEN = 0,
    AD_CMD_CLOSE,
    AD_CMD_CHUNK_RESET,
    AD_CMD_FRAME_DECODE,
    AD_CMD_CHUNK_SEEK,
    AD_CMD_GET_ERROR_INFO,
} audiodec_ex_ops_cmd_t;


#define AUDIO_DECODER_CMD_ENTRY     (*((uint32*)(AL_DECODER_ENTRY_VA_START)))
/*!
 *  \brief
 *     接口函数声明
 *     接口函数定义在kernel_api.S，加载驱动以后使用，驱动API号在api.h，后续会统一整理
 */
//extern int32 audio_decoder_cmd(void *handel, audiodec_ex_ops_cmd_t cmd, uint32 args) __FAR__;
#define audio_decoder_cmd(a,b,c)    ((int32(*)(void *handel, audiodec_ex_ops_cmd_t cmd, uint32 args))AUDIO_DECODER_CMD_ENTRY)((a), (b),(c))


#define SBC_DECODER_CMD_ENTRY     (*((uint32*)(AL_DECODER_SBC_ENTRY_VA_START)))
/*!
 *  \brief
 *     接口函数声明
 *     接口函数定义在kernel_api.S，加载驱动以后使用，驱动API号在api.h，后续会统一整理
 */
//extern int32 sbc_decoder_cmd(void *handel, audiodec_ex_ops_cmd_t cmd, uint32 args) __FAR__;
#define sbc_decoder_cmd(a,b,c)    ((int32(*)(void *handel, audiodec_ex_ops_cmd_t cmd, uint32 args))SBC_DECODER_CMD_ENTRY)((a), (b),(c))



#endif  // __AUDIO_DECODER_LIB_DEV_H__
