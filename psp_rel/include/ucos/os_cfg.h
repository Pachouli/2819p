/*
 *********************************************************************************************************
 *                                                uC/OS-II
 *                                          The Real-Time Kernel
 *
 *                           (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
 *                                           All Rights Reserved
 *
 *                                  uC/OS-II Configuration File for V2.60
 *
 * File : OS_CFG.H
 * By   : Jean J. Labrosse
 *********************************************************************************************************
 */
#ifndef __OS_CFG_H__
#define __OS_CFG_H__
/* ---------------------- MISCELLANEOUS ----------------------- */
#define OS_ARG_CHK_EN             1    /* Enable (1) or Disable (0) argument checking                  */
/*��������һЩ����û�й�ϵ�����ǹ̻�*/
#define OS_CPU_HOOKS_EN           1    /* uC/OS-II hooks are found in the processor port files         */
/*os_cpu_hook.c����*/

#define OS_EVENT_NAME_SIZE       0    /* Determine the size of the name of a Sem, Mutex, Mbox or Q    */
/*����Ҫ���ֱ��*/
#define OS_LOWEST_PRIO           15    /* Defines the lowest priority that can be assigned ...         */
/*0��15��16�����ȼ����Ѿ��㹻�� OS_TCB *OSTCBPrioTbl[OS_LOWEST_PRIO + 1]*//* ... MUST NEVER be higher than 63!                            */
/* ... MUST be higher than OS_MAX_TASKS!                         */
/*5110*/

#define OS_MAX_EVENTS            15   /* Max. number of event control blocks in your application      */
/*��Ϣ����2��+ 3����ͬ�������� 10��*/
#define OS_MAX_FLAGS             0    /* Max. number of Event Flag Groups    in your application      */
#define OS_MAX_MEM_PART           0    /* Max. number of memory partitions                             */
#define OS_MEMBYTE_EN            0
#define OS_MAX_MEMBYTE_PART       0
#define OS_MAX_QS                 5    /* Max. number of queue control blocks in your application      */
#define OS_MAX_TASKS             9    /* Max. number of tasks in your application, MUST be >= 2       */
/*����IDLE��8������TCB��8�� OS_TCB OSTCBTbl[OS_MAX_TASKS + OS_N_SYS_TASKS]  *//*5110*/

#define OS_SCHED_LOCK_EN          1    /*     Include code for OSSchedLock() and OSSchedUnlock()       */
/*�򿪽�ֹ���ȹ���*/
#define OS_TASK_IDLE_STK_SIZE   64   /* Idle task stack size (# of OS_STK wide entries)              */
/*���ܵ����Ӻ�������hook*/

#define OS_TASK_STAT_EN           0    /* Enable (1) or Disable(0) the statistics task                 */
#define OS_TASK_STAT_STK_SIZE    64    /* Statistics task stack size (# of OS_STK wide entries)        */
/*OS_TASK_STAT_ENΪ0ʱ��Ч*/
#define OS_TASK_STAT_STK_CHK_EN   0    /* Check task stacks from statistic task                        */
/*OS_TASK_STAT_ENΪ0ʱ��Ч*/
#define OS_TICKS_PER_SEC        100    /* Set the number of ticks in one second                        */

/*��ʹ���¼���־��*//* ----------------------- EVENT FLAGS ------------------------ */
#define OS_FLAG_EN                0    /* Enable (1) or Disable (0) code generation for EVENT FLAGS    */
#define OS_FLAG_WAIT_CLR_EN       0    /* Include code for Wait on Clear EVENT FLAGS                   */
#define OS_FLAG_ACCEPT_EN         0    /*     Include code for OSFlagAccept()                          */
#define OS_FLAG_DEL_EN            0    /*     Include code for OSFlagDel()                             */
#define OS_FLAG_NAME_SIZE         0    /*     Determine the size of the name of an event flag group    */
#define OS_FLAG_QUERY_EN          0    /*     Include code for OSFlagQuery()                           */
/*5110*/

/*��ʹ�����䣬���䷢һ����Ϣ�����о������൱�ڶ��еĳ���Ϊ1*//* -------------------- MESSAGE MAILBOXES --------------------- */
#define OS_MBOX_EN                0    /* Enable (1) or Disable (0) code generation for MAILBOXES      */
#define OS_MBOX_ACCEPT_EN         0    /*     Include code for OSMboxAccept()                          */
#define OS_MBOX_DEL_EN            0    /*     Include code for OSMboxDel()                             */
#define OS_MBOX_POST_EN           0    /*     Include code for OSMboxPost()                            */
#define OS_MBOX_POST_OPT_EN       0    /*     Include code for OSMboxPostOpt()                         */
#define OS_MBOX_QUERY_EN          0    /*     Include code for OSMboxQuery()                           */

/*�Լ�ʵ�ֶ�̬�ѹ�����ʹ��ucos���ڴ����ucos���ڴ����������3X�������ڴ�����ڴ��Ĵ�С��һ���ģ������ݽṹ�Ŀ���С*//* --------------------- MEMORY MANAGEMENT -------------------- */
#define OS_MEM_EN                 0    /* Enable (1) or Disable (0) code generation for MEMORY MANAGER */
#define OS_MEM_QUERY_EN           0    /*     Include code for OSMemQuery()                            */
#define OS_MEM_NAME_SIZE         32    /*     Determine the size of a memory partition name            */

/*��ʹ�û����������������Խ�����ȼ���ת�����⣬����Ҫָ��һ�����ȼ��������á��������ź�����ʵ�ֻ���������Ҫ������Ա��֤������*//* ---------------- MUTUAL EXCLUSION SEMAPHORES --------------- */
#define OS_MUTEX_EN               0    /* Enable (1) or Disable (0) code generation for MUTEX          */
#define OS_MUTEX_ACCEPT_EN        0    /*     Include code for OSMutexAccept()                         */
#define OS_MUTEX_DEL_EN           0    /*     Include code for OSMutexDel()                            */
#define OS_MUTEX_QUERY_EN         0    /*     Include code for OSMutexQuery()                          */

/*ʹ����Ϣ����*//* ---------------------- MESSAGE QUEUES ---------------------- */
#define OS_Q_EN                   0    /* Enable (1) or Disable (0) code generation for QUEUES         */
#define OS_Q_ACCEPT_EN            0    /*     Include code for OSQAccept()                             */
/*ʹ��accept*/
#define OS_Q_DEL_EN               0    /*     Include code for OSQDel()                                */
/*����ɾ����Ϣ���У����Կ��Բ�Ҫ�������ڹ̻����룬�ݱ���*/
#define OS_Q_FLUSH_EN             0    /*     Include code for OSQFlush()                              */
#define OS_Q_POST_EN              0    /*     Include code for OSQPost()                               */
/*<<<<<add by wurui for posix queue*/
#define OS_Q_POST_BY_PRIO_EN      0    /*     Include code for OSQPostByPrio()                         */
/*>>>>>*/
/*��Ϊ��ʱ�����Ƶ�post���ɣ�����������ȼ�����������ͬ����Ϣ����*/
#define OS_Q_POSTISR_EN           0    /*     Include code for OSQPost()                               */
/*�������Ҫ������QSQPost����*/
#define OS_Q_POST_FRONT_EN        0    /*     Include code for OSQPostFront()                          */
#define OS_Q_POST_OPT_EN          0    /*     Include code for OSQPostOpt()                            */
#define OS_Q_QUERY_EN             0    /*     Include code for OSQQuery()                              */
/*��Ϣ����FIFO������ʵ��LIFO*/

/*ʹ���ź�������������ڻ��������ź���*//* ------------------------ SEMAPHORES ------------------------ */
#define OS_SEM_EN                 1    /* Enable (1) or Disable (0) code generation for SEMAPHORES     */
#define OS_SEM_ACCEPT_EN          1    /*    Include code for OSSemAccept()                            */
#define OS_SEM_DEL_EN             1    /*    Include code for OSSemDel()                               */
#define OS_SEM_QUERY_EN           0    /*    Include code for OSSemQuery()                             */

/* --------------------- TASK MANAGEMENT ---------------------- */
#define OS_TASK_CHANGE_PRIO_EN    0    /*     Include code for OSTaskChangePrio()                      */
/*APʹ�ù����в����޸����ȼ�*/
#define OS_TASK_CREATE_EN         1    /*     Include code for OSTaskCreate()                          */
/*ʹ�÷���չ�Ĵ���������ָ����̹�������ݽṹָ�����ͨ��pdata��������Լ���ü���*/
#define OS_TASK_CREATE_EXT_EN     0    /*     Include code for OSTaskCreateExt()                       */
/*�˷����ݿռ䣬���Բ�ʹ��*/
#define OS_TASK_DEL_EN            1    /*     Include code for OSTaskDel()                             */
/*AP�˳�ʱ��Ҫ���*/
#define OS_TASK_NAME_SIZE        0    /*     Determine the size of a task name                        */
/*5110*/
#define OS_TASK_PROFILE_EN        0    /*     Include variables in OS_TCB for profiling                */
#define OS_TASK_QUERY_EN          1    /*     Include code for OSTaskQuery()                           */
#define OS_TASK_SUSPEND_EN        1    /*     Include code for OSTaskSuspend() and OSTaskResume()      */
#define OS_TASK_SW_HOOK_EN        1    /*     Include code for OSTaskSwHook()                          */
/*��Ҫ���hook�����л�������һЩ����*/

/* --------------------- TIME MANAGEMENT ---------------------- */
#define OS_TIME_DLY_HMSM_EN       0    /*     Include code for OSTimeDlyHMSM()                         */
/*�Ѿ���10msһ�����ĵ���ʱ����������Ҫ���ʱ�������ʱ����*/
#define OS_TIME_DLY_RESUME_EN     1    /*     Include code for OSTimeDlyResume()                       */
/*��Ҫ*/
#define OS_TIME_GET_SET_EN        0    /*     Include code for OSTimeGet() and OSTimeSet()             */
/*����Ҫ��ȡ���ʱ�䣬ϵͳ�Ѿ���RTC ������CPUʱ��*//*5110*/

/*!
 *   ���ж��������ȼ����ã�5110��ʹ�����ж�
 */
#define SOFTIRQ_TASK_PRIO       0
#define TIMER_TASK_PRIO_LO  (OS_LOWEST_PRIO-2)
#define TIMER_TASK_ID_LO    (TIMER_TASK_PRIO_LO | (TIMER_TASK_PRIO_LO<<8))

/*!
 *   �߳�ջ��С������ֵ��64K��
 */
#define MAX_TASK_STACK_SIZE  0x10000

/*!
 *   �߳��ں�ջ��С��2K��
 */
#define KERNEL_STACK_SIZE  0x800

/*!
 *   ���������ڴ�Ĵ�С����(*4KB)
 */
#define LARGE_DMA_MEM_MIN_SIZE  200

/*!
 *   ����MALLOC�ĵ�ʵ�ַ�ʽ��0---noya��ʽ��1---kmalloc��ʽ��
 */
#define MALLOC_REALIZE_TYPE 0

/*!
 *   �����Ƿ�ʹ��ȱҳ�쳣�п��ж�
 */
#define ENABLE_PAGE_INVALID_EI  1

/*!
 *   �ַ��豸��������ĸ������ã������Ĳ��ֽ�ͨ����̬����/�ͷţ�
 */
#define MAX_CHRDEV      9
/*!
 *   ���豸��������ĸ������ã������Ĳ��ֽ�ͨ����̬����/�ͷţ�
 */
#define MAX_BLKDEV      3

/*!
 *   �����Ƿ�ʹ�ܼ���ʱ����У��
 */
#define ENALBE_LOAD_CODECHECK 1
/*!
 *   �����Ƿ�ʹ��chipid
 */
#define ENABLE_CHIPID 0

/*!
 *   ����ϵͳ��Ϣ�ز���
 */
#define MSGPOOL_DEPTH 10

#define SEIRAL_PRINT_ENABLE     (1)
#define SERIAL_PEINT_DISABLE    (0)

#ifndef _ASSEMBLER_

#endif

#endif/*__OS_CFG_H__*/
