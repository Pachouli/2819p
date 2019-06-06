/*******************************************************************************
 *                              5003
 *                            Module: audio out
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       kkli     2008-09-05 15:00     1.0             build this file 
*******************************************************************************/
/*!
 * \file     audiout_pcm.h
 * \brief    ���������������ݽṹ
 * \author   kkli
 * \version 1.0
 * \date  2008/09/05
*******************************************************************************/
#ifndef __AUDIOUT_PCM_H__
#define __AUDIOUT_PCM_H__

/*!
 * \brief  
 *      �����������������
 */
#define MAX_CHANNEL     2
/*!
 * \brief  
 *      ��Ƶ������֡������������Ƶ���ģ�������
 */
typedef struct
{
    /*! pcm����ָ�����飬����������������ݵ���ʼ��ַ */
    void *pcm[MAX_CHANNEL];
    /*! ��ǰ��������������� */
    int channels;
    /*! ��ǰ�����������������ֻ�Ƶ������� */
    int samples;
    /*! ��ǰ���������ռ��bit��,16/24/32 */
    int sample_bits;
    /*! ��ǰ���С�����λ��������ȡֵ0 */
    int frac_bits;
} audiout_pcm_t;

#endif // __AUDIOUT_PCM_H__
