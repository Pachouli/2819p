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
 * \file     mmm_pp.h
 * \brief    ����LINEIN��RADIO��USB Speaker�����м��ṩ�Ľӿڼ�����
 * \author   jpliao
 * \version 1.0
 * \date  2010/08/27
 *******************************************************************************/
/*! \endcond*/
#ifndef __MMM_SP_COMMON_H__
#define __MMM_SP_COMMON_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*!
 * \brief
 *      ����ϵͳͷ�ļ�
 */
#ifdef WIN32
/* VC */
#define __FAR__
#else
/* MIPS */
#include <psp_includes.h>

#endif

#define SP_ASQT_DUMP_SCO_IN    (1<<0)
#define SP_ASQT_DUMP_PRE_PLC   (1<<1)
#define SP_ASQT_DUMP_SPK_IN    (1<<2) //PRE AGC
#define SP_ASQT_DUMP_MIC_IN    (1<<3)
#define SP_ASQT_DUMP_POST_AEC  (1<<4)
#define SP_ASQT_DUMP_POST_RES  (1<<5) //POST AGC
#define SP_ASQT_DUMP_AEC_INFO  (1<<6)

//4�ֽ�ͷ�� TAG+BITMAP+length
#define SP_ASQT_DUMP_TAG       (0x5a)

typedef struct
{
    uint8 tag;
    uint8 bitmap;
    uint16 length;
} asqt_dump_tag_t;

typedef struct
{
    uint8 *dump_buf;
    uint16 dump_capacity;
    uint16 dump_length;
    uint16 dump_write_ptr;
    uint16 dump_read_ptr;
    uint8 dump_bitmap;
    uint8 sco_in_flag; //�Ƿ��SCO_INPIPE_ASQT��ȡ����
} asqt_dump_buf_t;

//�������ò���
typedef struct
{
    int aud_param_rev[4]; //MIC��DAC CHAN����
    int volume_call_gain; //��绰�������棬��λ-1db��0~9 ��ʾ 0db ~ -9db
    int agc_param_rev[4]; //AGC ����
    int sv_prms_rev[14]; //�� actionsaec_prms_t ����һ��
    int analysis_mode; //����ģʽ��0��ʾģ���绰ģʽ��1��ʾ��绰ģʽ
    int half_mode; //��˫��ģʽ��0��ʾȫ˫����1��ʾ��˫��

} asqt_configs_t; //NOTE��ͬ asqt_sv_configs_t һ�£���

/*! \endcond */
#ifdef __cplusplus
}
#endif

#endif // __MMM_MP_H__
