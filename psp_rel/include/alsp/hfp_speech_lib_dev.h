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
#ifndef __HFP_SPEECH_LIB_DEV_H__
#define __HFP_SPEECH_LIB_DEV_H__

#include "actal_libc_dev.h"
#include "actionsaec_struct.h"
#include "agc_struct.h"

/*!
 * \brief
 *      ������Ƶ���뷵�ص�����
 */

typedef enum
{
    /*! һ��δ֪���� */
    HS_RET_UNEXPECTED = -3,
    /*! �ڴ�ռ䲻�� */
    HS_RET_OUTOFMEMORY,
    /*! ��ʽ��֧�֣����ܼ������� */
    HS_RET_UNSUPPORTED,
    /*! ���� */
    HS_RET_OK,
    /*! �������ݲ��� */
    HS_RET_DATAUNDERFLOW,
} hs_ret_t;

typedef struct
{
    short* Txinbuf_mic;
    short* RxinBuf_spk;
    int cnt;     //����
} aec_inout;

typedef struct
{
    short* inoutput;
    int cnt;     //����
} agc_inout;


/*!
 * \brief
 *      ������Ƶ�������������
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
 *     �ӿں�������
 *     �ӿں���������kernel_api.S�����������Ժ�ʹ�ã�����API����api.h��������ͳһ����
 */
//extern int32 hfp_speech_cmd(void *handel, hfp_ex_ops_cmd_t cmd, uint32 args) __FAR__;
#define hfp_speech_cmd(a,b,c)    ((int32(*)(void *handel, hfp_ex_ops_cmd_t cmd, uint32 args))AUDIO_AEC_CMD_ENTRY)((a), (b),(c))


#endif  // __AUDIO_DECODER_LIB_DEV_H__
