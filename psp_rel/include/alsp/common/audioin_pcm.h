/*******************************************************************************
 *                              5003
 *                            Module: audio in
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       kkli     2008-09-05 15:00     1.0             build this file 
*******************************************************************************/
/*!
 * \file     audioin_pcm.h
 * \brief    解码输出的数据结构
 * \author   kkli
 * \version 1.0
 * \date  2008/09/05
*******************************************************************************/
#ifndef __AUDIOIN_PCM_H__
#define __AUDIOIN_PCM_H__

/*!
 * \brief  
 *      定义最大的输入声道数
 */
#define MAX_CHANNEL     2
/*!
 * \brief  
 *      音频编码插件帧解码的输入或音频输入模块的输出
 */
typedef struct
{
    /*! pcm数据指针数组，包含各声道输出数据的起始地址 */
    void *pcm[MAX_CHANNEL];
    /*! 当前输出包含的声道数 */
    int channels;
    /*! 当前输出包含的样本数，只计单个声道 */
    int samples;
    /*! 当前输出样本点占用bit数,16/24/32 */
    int sample_bits;
    /*! 当前输出小数点的位数，整数取值0 */
    int frac_bits;
} audioin_pcm_t;

#endif // __AUDIOIN_PCM_H__
