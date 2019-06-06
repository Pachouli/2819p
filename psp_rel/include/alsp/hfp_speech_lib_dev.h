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
#ifndef __HFP_SPEECH_LIB_DEV_H__
#define __HFP_SPEECH_LIB_DEV_H__

#include "actal_libc_dev.h"
#include "actionsaec_struct.h"
#include "agc_struct.h"

/*!
 * \brief
 *      定义音频解码返回的类型
 */

typedef enum
{
    /*! 一般未知错误 */
    HS_RET_UNEXPECTED = -3,
    /*! 内存空间不够 */
    HS_RET_OUTOFMEMORY,
    /*! 格式不支持，不能继续解码 */
    HS_RET_UNSUPPORTED,
    /*! 正常 */
    HS_RET_OK,
    /*! 输入数据不够 */
    HS_RET_DATAUNDERFLOW,
} hs_ret_t;

typedef struct
{
    short* Txinbuf_mic;
    short* RxinBuf_spk;
    int cnt;     //点数
} aec_inout;

typedef struct
{
    short* inoutput;
    int cnt;     //点数
} agc_inout;


/*!
 * \brief
 *      定义音频解码命令的类型
 */
typedef enum
{
    HS_CMD_AGC_PRE_OPEN = 0,
    HS_CMD_AGC_POST_OPEN,
    HS_CMD_AEC_OPEN,
    HS_CMD_CLOSE,
    HS_CMD_AGC_PROCESS,
    HS_CMD_AEC_PROCESS,
    HS_CMD_AEC_GETSTASTUS,
    HS_CMD_CNG_PROCESS
} hfp_ex_ops_cmd_t;


#define AUDIO_AEC_CMD_ENTRY     (*((uint32*)(AL_RESERVD_HFP_VA_START)))
/*!
 *  \brief
 *     接口函数声明
 *     接口函数定义在kernel_api.S，加载驱动以后使用，驱动API号在api.h，后续会统一整理
 */
//extern int32 hfp_speech_cmd(void *handel, hfp_ex_ops_cmd_t cmd, uint32 args) __FAR__;
#define hfp_speech_cmd(a,b,c)    ((int32(*)(void *handel, hfp_ex_ops_cmd_t cmd, uint32 args))AUDIO_AEC_CMD_ENTRY)((a), (b),(c))


#endif  // __AUDIO_DECODER_LIB_DEV_H__
