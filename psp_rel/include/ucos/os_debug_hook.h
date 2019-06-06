/********************************************************************************
 *                              USDK(1100)
 *                            Module: OS_DEBUG_HOOK
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      wuyueqian     2011-9-10 15:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     os_debug_hook.h
 * \brief    debug�ں�֧����غ궨��ͺ�������
 * \author   wuyueqian
 * \version 1.0
 * \date   2011-9-10
 *******************************************************************************/
#ifndef __OS_DEBUG_HOOK_H__
#define __OS_DEBUG_HOOK_H__

#ifndef _ASSEMBLER_

typedef struct {
	unsigned char prio_cur;
	unsigned char prio_high_rdy;
} prio_change_info_t;

typedef struct {
	unsigned char prio_cur;
	unsigned char prio_high_rdy;
	unsigned int cause;
	void *event;
} sw_info_t;

typedef struct {
	unsigned int mode;
	void *arg;
} uview_mode_t;

/*!
 *  \brief
 *  UVIEWģʽ
 *  \li UVIEW_MODE_RT : ʵʱģʽ
 *  \li UVIEW_MODE_CACHE : ����ģʽ
 */
enum uview_mode {
	UVIEW_MODE_RT, UVIEW_MODE_CACHE
};

#define OSTASK_DEBUG_WHOLE_ENABLE       (1)
#define OSTASK_DEBUG_WHOLE_DISABLE      (0)

#define OSTASK_DEBUG_PCNT_ENABLE        (1)
#define OSTASK_DEBUG_PCNT_DISABLE       (0)

struct ostask_debug_cfg_t {
	char debug_whole_enable;
	char debug_pcnt_enable;
};

struct ostask_debug_data_t {
	unsigned long long debug_pcnt_event0_data;
	unsigned long long debug_pcnt_event1_data;
};
#endif/*_ASSEMBLER_*/
#endif/*__OS_DEBUG_HOOK_H__*/
