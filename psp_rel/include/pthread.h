/********************************************************************************
 *                              USDK(GL5110)
 *                            Module: KERNEL
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>      <time>           <version >             <desc>
 *      wuyueqian     2011-9-10 09:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     pthread.h
 * \brief    进程线程管理接口
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/

#ifndef __PTHREAD_H__
#define __PTHREAD_H__

#include <typeext.h>
#include <ucos/os.h>

#define CREATE_NOT_MAIN_THREAD 0xff
#define WAITPID_ONLY_PROCESS_MANAGER -1
#define WAITPID_NO_PROCESS_EXIT 0xf

/*!
 *     线程内部结构:用于保存线程的入口及参数构
 */
struct pthread_start_args
{
    /* function to run */
    void * (*start_routine)(void *);
    /* its argument */
    void * arg;
};

/*!
 *     线程内部结构:线程描述符指向的结构
 */
typedef struct _pthread_descr_struct
{
    /* 描述符链表结构指针 */
    struct _pthread_descr_struct *next;
    /* 线程创建的参数 */
    struct pthread_start_args p_start_args;
    /* 所属的进程结构索引 */
    uint8 process_descr_index;
    /* 被pthread_exit()终止,则为true */
    int8 p_terminated;
    /* 优先级 */
    uint8 prio;
} pthread_descr_t;

/*!
 *  \brief
 *     pthread_once参数定义
 */
typedef int pthread_once_t;

/* 进程数据结构 */
typedef struct
{
    pthread_descr_t *descr_head; /* 线程描述符链表头 */
    pthread_once_t exit_once; /* 确保exit只被调用一次 */
    int8 visible_thread_num; /*当前活跃线程数，非僵死线程*/
    int8 is_used;
    int8 exit_val; /* 进程退出返回值 */
} process_struct_t;

#define PROCESS_STRUCT_NUM 5
#define WNOHANG 0xff

typedef struct
{
    void *(*start_rtn)(void*);
    void *arg;
    OS_STK *ptos;
    uint32 stk_size;
} pthread_param_t;
#endif



