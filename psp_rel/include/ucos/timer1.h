/*******************************************************************************
 *                              USDK1100
 *                            Module: IRQ_TIMER
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       wuyueqian     2011-9-10 9:42     1.0             build this file
*******************************************************************************/
/*!
 * \file     irq_timer.h
 * \brief    ���жϷ�����������еĶ�ʱ��
 * \author   wuyueqian
 * \version 1.0
 * \date    2011-9-10 
*******************************************************************************/

#ifndef __TIMER1_H__
#define __TIMER1_H__

#include <ucos/init.h>
#ifndef _ASSEMBLER_

#define MAX_IRQ_TIMERS 10

/*!
 * \brief
 *      �ж϶�ʱ�����ݽṹ
 */
typedef struct
{
    /* ������ */
    void (*handler)(void);
    /* ʱ���� */
    uint16 period;
    /* ��ǰ����ֵ */
    uint16 cur_value;
}irq_timer_t;

#endif /*_ASSEMBLER_*/
#endif /*__IRQ_TIMER_H__*/
