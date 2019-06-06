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
#ifndef __HFP_PLC_LIB_DEV_H__
#define __HFP_PLC_LIB_DEV_H__

#include "actal_libc_dev.h"

/*!
 * \brief
 *      ������Ƶ���뷵�ص�����
 */

//typedef enum
//{
//    /*! һ��δ֪���� */
//    HS_RET_UNEXPECTED = -3,
//    /*! �ڴ�ռ䲻�� */
//    HS_RET_OUTOFMEMORY,
//    /*! ��ʽ��֧�֣����ܼ������� */
//    HS_RET_UNSUPPORTED,
//    /*! ���� */
//    HS_RET_OK,
//    /*! �������ݲ��� */
//    HS_RET_DATAUNDERFLOW,
//} plc_ret_t;

typedef struct
{
    short* inoutput;
    int cnt;     //����
} plc_inout;


/*!
 * \brief
 *      ������Ƶ�������������
 */
typedef enum
{
    HS_CMD_PLC_OPEN = 0,
    HS_CMD_PLC_CLOSE,
    HS_CMD_PLC_PROCESS_NORMAL,
    HS_CMD_PLC_PROCESS_LOSS,
} plc_ex_ops_cmd_t;


#define AUDIO_PLC_CMD_ENTRY     (*((uint32*)(AL_RESERVD_PLC_VA_START)))
/*!
 *  \brief
 *     �ӿں�������
 *     �ӿں���������kernel_api.S�����������Ժ�ʹ�ã�����API����api.h��������ͳһ����
 */
//extern int32 hfp_plc_cmd(void *handel, plc_ex_ops_cmd_t cmd, uint32 args) __FAR__;
#define hfp_plc_cmd(a,b,c)      ((int32(*)(void *handel, plc_ex_ops_cmd_t cmd, uint32 args))AUDIO_PLC_CMD_ENTRY)((a), (b),(c))


#endif  // __AUDIO_DECODER_LIB_DEV_H__
