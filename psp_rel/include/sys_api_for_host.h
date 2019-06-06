/********************************************************************************
 *                            US281B
 *                            Module: Bluetooth
 *                 Copyright(c) 2017-2020 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>      <time>                  <version >      <desc>
 *      huanganbang   2017/10/24 13:32:52     1.0             build this file
 ********************************************************************************/
/*!
 * \file     sys_api_for_host.h
 * \brief    define the sys api type and sys api function list struct
 * \author   huanganbang
 * \version  1.0
 * \date  2017/10/23
 *******************************************************************************/
#ifndef  __SYS_API_FOR_HOST_H__
#define  __SYS_API_FOR_HOST_H__

#include <ucos/types.h>
#include <ucos/os.h>
#include <ucos/ctype_m4k.h>
#include <ucos/os_cfg.h>
#include <ucos/ucos_types.h>

//memory op refer api
typedef	void *(*func_memcpy_p)(void *cs, const void *ct, unsigned int count);
typedef	void *(*func_memset_p)(void * s, int c, unsigned int count);
typedef	int (*func_memcmp_p)(const void * cs, const void * ct, unsigned int count);

//string op refer api
typedef	int (*func_strlen_p)(const char *);
typedef	int (*func_strncmp_p)(const char *, const char *, int);
typedef	char *(*func_strncpy_p)(char *, const char *, int);

//semaphore op refer api
typedef	int (*func_sem_init_p)(os_event_t  ** sem, uint32 value);
typedef	int (*func_sem_destroy_p)(os_event_t  ** sem);
typedef	int (*func_sem_wait_p)(os_event_t *sem, uint16 timeout);
typedef	int (*func_sem_post_p)(os_event_t *sem);

//debug op refer api
typedef	void (*func_printf_p)(const uint8 *fmt, ...);

//system op refer api
typedef	void (*func_OSSchedLock_p)(void);
typedef	void (*func_OSSchedUnlock_p)(void);

typedef	uint32 (*func_local_irq_save_p)(void);
typedef	void (*func_local_irq_restore_p)(uint32 flags);

typedef	uint32 (*func_local_intc_save_p)(uint32 irq_src);
typedef	void (*func_local_intc_restore_p)(uint32 flags);

//sys api function list struct
typedef	struct
{
				func_memcpy_p host_memcpy;
				func_memset_p host_memset;
				func_memcmp_p host_memcmp;
				func_strlen_p host_strlen;
				func_strncmp_p host_strncmp;
				func_strncpy_p host_strncpy;
				func_sem_init_p host_sem_init;
				func_sem_destroy_p host_sem_destroy;
				func_sem_wait_p host_sem_wait;
				func_sem_post_p host_sem_post;
				func_printf_p host_printf;
				func_OSSchedLock_p host_OSSchedLock;
				func_OSSchedUnlock_p host_OSSchedUnlock;
				func_local_irq_save_p host_local_irq_save;
				func_local_irq_restore_p host_local_irq_restore;
				func_local_intc_save_p host_local_intc_save;
				func_local_intc_restore_p host_local_intc_restore;
}sys_api_list_for_host_t;

extern sys_api_list_for_host_t host_sys_api_func_list;

extern void host_sys_api_func_list_init(void);

#endif
