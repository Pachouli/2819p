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
#ifndef __AUDIO_SP_POST_LIB_DEV_H__
#define __AUDIO_SP_POST_LIB_DEV_H__

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
    SP_POST_CMD_OPEN = 0,
    SP_POST_CMD_CLOSE,
	SP_POST_CMD_FRAME_DEAL, 
} sp_post_ops_cmd_t;

typedef struct
{
    int16*  post_in;
    uint16* post_out;
    uint16  in_len;
} post_inout_t;

#define SP_POST_CMD_ENTRY     (*((uint32*)(AL_SP_POST_ENTRY_VA_START)))
/*!
*  \brief
*     接口函数声明
*     接口函数定义在kernel_api.S，加载驱动以后使用，驱动API号在api.h，后续会统一整理
 */
#define sp_post_cmd(a,b,c)    ((int(*)(void *handel, sp_post_ops_cmd_t cmd, uint32 args))SP_POST_CMD_ENTRY)((a), (b),(c))

#endif // __AUDIO_ENCODER_LIB_DEV_H__
