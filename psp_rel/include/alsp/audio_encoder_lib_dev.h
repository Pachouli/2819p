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
 * \brief    ������audioenc���ͨ�õĽӿ�
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
*      ��Ƶ���������ṩ��������
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
*      �����߳��ڲ�����ת�Լ��ṩ���ⲿ��״̬��ѯ
*/
typedef enum
{
	/*! ����¼��֮ǰ��ֹͣ¼��֮�� */
	AE_STATUS_IDLE = 0,
	/*! ����¼��״̬ */
	AE_STATUS_RECORDING,
	/*! �ڲ���������ֹͣ */
	AE_STATUS_STOPPED,
	/*! �߳���ֹ */
	AE_STATUS_TERMINATED,
	/*! �������� */
	AE_STATUS_MONITOR,
} ae_status_t;

typedef enum
{
    /*! һ��δ֪���� */
    AE_RET_UNEXPECTED = -7,
    /*! �������س��� */
    AE_RET_LIBLOAD_ERROR,
    /*! �����˷Ƿ��ı���� */
    AE_RET_LIB_ERROR,
     /*! ������� */
    AE_RET_ENCODER_ERROR,
     /*! �����ʴ��� */
    AE_RET_FS_ERROR,
    /*! �ڴ�ռ䲻�� */
    AE_RET_OUTOFMEMORY,
    /*! ��ʽ��֧�֣����ܼ������� */
    AE_RET_UNSUPPORTED,
    /*! ���� */
    AE_RET_OK,
    /*! ������ݳ�����������С */
    AE_RET_DATAOVERFLOW,
} audioenc_ret_t;

typedef struct
{
    /*! ����һ֡����������� */
    int samples_per_frame;
    /*! payload/packet��С����λ�ֽ� */
    int chunk_size;
    /*! ��ʼд���ļ���ʼ������ */
    /*! ����ָ�� */
    void *buf;
    /*! ���ݳ��� */
    int buf_len;
} audioenc_attribute_t;


#define AUDIO_ENCODER_CMD_ENTRY     (*((uint32*)(AL_ENCODER_WAV_ENTRY_VA_START)))
/*!
*  \brief
*     �ӿں�������
*     �ӿں���������kernel_api.S�����������Ժ�ʹ�ã�����API����api.h��������ͳһ����
 */
//extern int audio_encoder_cmd(void *, audioenc_ex_ops_cmd_t, unsigned int)  __FAR__;
#define audio_encoder_cmd(a,b,c)    ((int(*)(void *handel, audioenc_ex_ops_cmd_t cmd, uint32 args))AUDIO_ENCODER_CMD_ENTRY)((a), (b),(c))


#define SBC_ENCODER_CMD_ENTRY       (*((uint32*)(AL_ENCODER_SBC_ENTRY_VA_START)))
/*!
*  \brief
*     �ӿں�������
*     �ӿں���������kernel_api.S�����������Ժ�ʹ�ã�����API����api.h��������ͳһ����
 */
//extern int sbc_encoder_cmd(void *, audioenc_ex_ops_cmd_t, unsigned int)  __FAR__;
#define sbc_encoder_cmd(a,b,c)      ((int(*)(void *handel, audioenc_ex_ops_cmd_t cmd, uint32 args))SBC_ENCODER_CMD_ENTRY)((a), (b),(c))

#endif // __AUDIO_ENCODER_LIB_DEV_H__
