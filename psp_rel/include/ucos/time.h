/********************************************************************************
 *                               USDK(1100)
 *                            Module: TIME
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *        wuyueqian     2011-9-10 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     time.h
 * \brief    ʱ��ͷ�ļ�
 * \author   wuyueqian
 * \version 1.0
 * \date   2011-9-10
 *******************************************************************************/
#ifndef __OS_TIME_H__
#define __OS_TIME_H__

#define HZ 100

#ifndef _ASSEMBLER_

#include <ucos/types.h>
#include <ucos/init.h>

/*! time, second */
typedef long _time_t;

/*! time, micro second */
typedef long _suseconds_t;

/*!
 * \brief
 *      timeval, ϵͳʱ�䣬����m��us
 */
struct timeval
{
    /*! seconds */
    _time_t tv_sec;
    /*! microseconds */
    _suseconds_t tv_usec;
};

/* Parameters used to convert the timespec values */
#define MSEC_PER_SEC (1000L)
#define USEC_PER_SEC (1000000L)
//#define NSEC_PER_SEC (1000000000L)
//#define NSEC_PER_USEC (1000L)

#endif /*_ASSEMBLER_*/
#endif /*__OS_TIME_H__*/
