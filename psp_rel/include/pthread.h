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
 * \brief    �����̹߳���ӿ�
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
 *     �߳��ڲ��ṹ:���ڱ����̵߳���ڼ�������
 */
struct pthread_start_args
{
    /* function to run */
    void * (*start_routine)(void *);
    /* its argument */
    void * arg;
};

/*!
 *     �߳��ڲ��ṹ:�߳�������ָ��Ľṹ
 */
typedef struct _pthread_descr_struct
{
    /* ����������ṹָ�� */
    struct _pthread_descr_struct *next;
    /* �̴߳����Ĳ��� */
    struct pthread_start_args p_start_args;
    /* �����Ľ��̽ṹ���� */
    uint8 process_descr_index;
    /* ��pthread_exit()��ֹ,��Ϊtrue */
    int8 p_terminated;
    /* ���ȼ� */
    uint8 prio;
} pthread_descr_t;

/*!
 *  \brief
 *     pthread_once��������
 */
typedef int pthread_once_t;

/* �������ݽṹ */
typedef struct
{
    pthread_descr_t *descr_head; /* �߳�����������ͷ */
    pthread_once_t exit_once; /* ȷ��exitֻ������һ�� */
    int8 visible_thread_num; /*��ǰ��Ծ�߳������ǽ����߳�*/
    int8 is_used;
    int8 exit_val; /* �����˳�����ֵ */
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



