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
 * \brief    ��Ƶ���������Ҫ��ͷ�ļ�
 * \author   kkli
 * \version 1.0
 * \date  2008/09/18
*******************************************************************************/
#ifndef __AUDIO_SP_PRE_LIB_DEV_H__
#define __AUDIO_SP_PRE_LIB_DEV_H__

#include "actal_libc_dev.h"
#include "./common/music_info.h"
#include "./common/storageio.h"
#include "./common/audiout_pcm.h"

/*!
 * \brief
 *      ������Ƶ�������������
 */
typedef enum
{
    SP_PRE_CMD_OPEN = 0,
    SP_PRE_CMD_CLOSE,
    SP_PRE_CMD_RESET,
    SP_PRE_CMD_FRAME_DEAL,
    SP_PRE_CMD_GET_ERROR_INFO,
} sp_pre_ops_cmd_t;

typedef struct
{
    uint16* pre_in;
    int16*  pre_out;
    uint16  in_len;
} pre_inout_t;

#define SP_PRE_CMD_ENTRY     (*((uint32*)(AL_SP_PRE_ENTRY_VA_START)))
/*!
 *  \brief
 *     �ӿں�������
 *     �ӿں���������kernel_api.S�����������Ժ�ʹ�ã�����API����api.h��������ͳһ����
 */
#define sp_pre_cmd(a,b,c)    ((int32(*)(void *handel, sp_pre_ops_cmd_t cmd, uint32 args))SP_PRE_CMD_ENTRY)((a), (b),(c))


#endif  // __AUDIO_SP_PRE_LIB_DEV_H__
