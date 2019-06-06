/*! \cond MMM_KTP_API*/
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
 * \file     mmm_ktp.h
 * \brief    ���尴����ͨ���м���ṩ�Ľӿڼ�����
 * \author   cailizhen
 * \version  1.0
 * \date     2017-3-28
 *******************************************************************************/
/*! \endcond*/
#ifndef __MMM_KTP_H__
#define __MMM_KTP_H__

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

/*! \cond MMM_KTP_API */

/*!
 *  \brief
 *      ���������������״̬
 */
typedef enum
{
    /*! ֹͣ״̬ */
    MMM_KTP_ENGINE_STOPPED = 0,
    /*! ����״̬ */
    MMM_KTP_ENGINE_PLAYING,
    /*! ����״̬ */
    MMM_KTP_ENGINE_ERROR,
    /*! δOPEN״̬��δ֪״̬ */
    MMM_KTP_ENGINE_NULL,
} mmm_ktp_engine_t;

/*! \endcond */
#ifdef __cplusplus
}
#endif

#endif // __MMM_KTP_H__
