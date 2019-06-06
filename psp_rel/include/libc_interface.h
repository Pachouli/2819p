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
 * \file     libc_interface.h
 * \brief    libc总接口
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/

#ifndef __LIBC_DEFINE_H__
#define __LIBC_DEFINE_H__

#include <attr.h>
#include <typeext.h>
#include "kernel_interface.h"

typedef enum
{
    PRINTF_ASYNC = 0,
    PRINTHEX = 1,
    PRINTHEX_ASYNC = 2,
    PRINT_AGENT = 3,
    SEM_POST = 4,
    LIBC_RAM_RESERVE_API1 = 5,
    LIBC_RAM_RESERVE_API2 = 6,
    LIBC_RAM_RESERVE_API3 = 7,
} libc_cmd_e_ram0;

typedef enum
{
    STRLEN = 0,
    STRNCAT = 1,
    STRNCMP = 2,
    STRNCPY = 3,
    STRLENUNI = 4,
    STRNCPYUNI = 5,
    STRNCATUNI = 6,

    ITOA = 7,
    PRINT = 8,

    PTHREAD_EXIT = 9,
    PTHREAD_CREATE = 10,

    EXIT = 11,
    WAITPID = 12,

    GET_PROCESS_STRUCT = 13,
    FREE_PROCESS_STRUCT = 14,

    SEM_INIT = 15,
    SEM_DESTROY = 16,
    SEM_WAIT = 17,
    SEM_TRYWAIT = 18,

    PTHREAD_MUTEX_INIT = 19,
    PTHREAD_MUTEX_DESTROY = 20,
    PTHREAD_MUTEX_LOCK = 21,
    PTHREAD_MUTEX_UNLOCK = 22,
    PTHREAD_MUTEX_TRYLOCK = 23,

    PTHREAD_COND_INIT = 24,
    PTHREAD_COND_DESTROY = 25,
    PTHREAD_COND_WAIT = 26,
    PTHREAD_COND_SIGNAL = 27,

    MATH_EXP_FIXED = 28,
    MATH_UDIV64_32 = 29,
    PTHREAD_CREAT_INNER = 30,

    LIBC_RESERVE_API_1 = 31,
    LIBC_RESERVE_API_X = 39,
} libc_cmd_e;

/*
 libc提供的函数以指针形式存放在数组里面
 */
#define LIBC_RAM_ENTRY_ADDR(x)              (0xBFC20090 + (4 * x))
#define LIBC_INTERFACE_ENTRY_ADDR(x)        (LIBC_ENTRY_VA_START + (4 * x))

#define LIBC_MEMCPY_ENTRY                   (*((uint32*)(SYS_ROM_ENTRY_ADDR(MEMCPY))))
#define LIBC_MEMSET_ENTRY                   (*((uint32*)(SYS_ROM_ENTRY_ADDR(MEMSET))))
#define LIBC_MEMCMP_ENTRY                   (*((uint32*)(SYS_ROM_ENTRY_ADDR(MEMCMP))))

#define LIBC_STRLEN_ENTRY                   (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(STRLEN))))
#define LIBC_STRNCAT_ENTRY                  (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(STRNCAT))))
#define LIBC_STRNCMP_ENTRY                  (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(STRNCMP))))
#define LIBC_STRNCPY_ENTRY                  (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(STRNCPY))))
#define LIBC_STRLENUNI_ENTRY                (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(STRLENUNI))))
#define LIBC_STRNCPYUNI_ENTRY               (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(STRNCPYUNI))))
#define LIBC_STRNCATUNI_ENTRY               (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(STRNCATUNI))))

#define LIBC_ITOA_ENTRY                     (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(ITOA))))
#define LIBC_PRINT_ENTRY                    (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(PRINT))))

#define LIBC_PTHREAD_EXIT_ENTRY             (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(PTHREAD_EXIT))))
#define LIBC_PTHREAD_CREATE_ENTRY           (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(PTHREAD_CREATE))))

#define LIBC_EXIT_ENTRY                     (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(EXIT))))
#define LIBC_WAITPID_ENTRY                  (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(WAITPID))))

#define LIBC_GET_PROCESS_STRUCT_ENTRY       (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(GET_PROCESS_STRUCT))))
#define LIBC_FREE_PROCESS_STRUCT_ENTRY      (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(FREE_PROCESS_STRUCT))))

#define LIBC_SEM_INIT_ENTRY                 (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(SEM_INIT))))
#define LIBC_SEM_DESTROY_ENTRY              (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(SEM_DESTROY))))
#define LIBC_SEM_WAIT_ENTRY                 (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(SEM_WAIT))))
#define LIBC_SEM_TRYWAIT_ENTRY              (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(SEM_TRYWAIT))))
#define LIBC_SEM_POST_ENTRY                 (*((uint32*)(LIBC_RAM_ENTRY_ADDR(SEM_POST))))

#define LIBC_PTHREAD_MUTEX_INIT_ENTRY       (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(PTHREAD_MUTEX_INIT))))
#define LIBC_PTHREAD_MUTEX_DESTROY_ENTRY    (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(PTHREAD_MUTEX_DESTROY))))
#define LIBC_PTHREAD_MUTEX_LOCK_ENTRY       (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(PTHREAD_MUTEX_LOCK))))
#define LIBC_PTHREAD_MUTEX_UNLOCK_ENTRY     (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(PTHREAD_MUTEX_UNLOCK))))
#define LIBC_PTHREAD_MUTEX_TRYLOCK_ENTRY    (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(PTHREAD_MUTEX_TRYLOCK))))

#define LIBC_PTHREAD_COND_INIT_ENTRY        (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(PTHREAD_COND_INIT))))
#define LIBC_PTHREAD_COND_DESTROY_ENTRY     (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(PTHREAD_COND_DESTROY))))
#define LIBC_PTHREAD_COND_WAIT_ENTRY        (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(PTHREAD_COND_WAIT))))
#define LIBC_PTHREAD_COND_SIGNAL_ENTRY      (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(PTHREAD_COND_SIGNAL))))

#define LIBC_PRINT_BY_CPU_UART_INIT_ENTRY   (*((uint32*)(SYS_ROM_ENTRY_ADDR(PRINT_BY_CPU_UART_INIT))))
#define LIBC_PRINTF_ENTRY                   (*((uint32*)(SYS_ROM_ENTRY_ADDR(PRINTF))))
#define LIBC_PRINTF_ASYNC_ENTRY             (*((uint32*)(LIBC_RAM_ENTRY_ADDR(PRINTF_ASYNC))))
#define LIBC_PRINTHEX_ENTRY                 (*((uint32*)(LIBC_RAM_ENTRY_ADDR(PRINTHEX))))
#define LIBC_PRINTHEX_ASYNC_ENTRY           (*((uint32*)(LIBC_RAM_ENTRY_ADDR(PRINTHEX_ASYNC))))
#define LIBC_PRINT_AGENT_ENTRY              (*((uint32*)(LIBC_RAM_ENTRY_ADDR(PRINT_AGENT))))

#define LIBC_MATH_EXP_FIXED_ENTRY           (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(MATH_EXP_FIXED))))

#define LIBC_MATH_UDIV64_32_ENTRY           (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(MATH_UDIV64_32))))
#define LIBC_PTHREAD_CREAT_INNNER_ENTRY     (*((uint32*)(LIBC_INTERFACE_ENTRY_ADDR(PTHREAD_CREAT_INNER))))

/*
 libc函数调用接口,通过函数指针方式调用
 */

/* ROM开头全大写API接口，是无需系统加载即可调用的，可供启动阶段调用 */
#define ROM_LIBC_MEMCPY(a,b,c)              ((void*(*)(void* , const void* , uint32 ))LIBC_MEMCPY_ENTRY)((a), (b), (c))
#define ROM_LIBC_MEMSET(a,b,c)              ((void*(*)(void* , int , uint32 ))LIBC_MEMSET_ENTRY)((a), (b), (c))
#define ROM_LIBC_MEMCMP(a,b,c)              ((int(*)(const void* , const void* , uint32 ))LIBC_MEMCMP_ENTRY)((a), (b), (c))

#define ROM_LIBC_PRINT_BY_CPU_UART_INIT(a,b,c) ((void(*)(uint8, uint8 *, print_info_mem_t *))LIBC_PRINT_BY_CPU_UART_INIT_ENTRY)((a),(b),(c))
#define ROM_LIBC_PRINTF(...)                   ((void(*)(const uint8 *fmt, ...))LIBC_PRINTF_ENTRY)(__VA_ARGS__)


#define libc_memcpy(a,b,c)                  ((void*(*)(void* , const void* , uint32 ))LIBC_MEMCPY_ENTRY)((a), (b), (c))
#define libc_memset(a,b,c)                  ((void*(*)(void* , int , uint32 ))LIBC_MEMSET_ENTRY)((a), (b), (c))
#define libc_memcmp(a,b,c)                  ((int(*)(const void* , const void* , uint32 ))LIBC_MEMCMP_ENTRY)((a), (b), (c))

#define libc_strlen(a)                      ((uint32(*)(const char* ))LIBC_STRLEN_ENTRY)(a)
#define libc_strncat(a,b,c)                 ((char*(*)(char* , const char* , uint32 ))LIBC_STRNCAT_ENTRY)((a), (b), (c))
#define libc_strncmp(a,b,c)                 ((int(*)(const char* , const char* , uint32 ))LIBC_STRNCMP_ENTRY)((a), (b), (c))
#define libc_strncpy(a,b,c)                 ((char*(*)(char* , const char* , uint32 ))LIBC_STRNCPY_ENTRY)((a), (b), (c))
#define libc_strlenuni(a)                   ((int(*)(int8* ))LIBC_STRLENUNI_ENTRY)(a)
#define libc_strncpyuni(a,b,c)              ((int8*(*)(int8* , int8* , int ))LIBC_STRNCPYUNI_ENTRY)((a), (b), (c))
#define libc_strncatuni(a,b,c)              ((char*(*)(char* , const char* , uint32 ))LIBC_STRNCATUNI_ENTRY)((a), (b), (c))

#define libc_itoa(a,b,c)                    ((uint8(*)(uint32 , uint8* , uint8 ))LIBC_ITOA_ENTRY)((a), (b), (c))

/*
 *    参数说明
 * \param[in]    a    待打印字符串基地址
 * \param[in]    b    待打印数字值
 * \param[in]    c    打印模式： 0，为仅打印字符串；1，仅打印数值；2，字符串和数值都打印
 * \note 后台蓝牙模式下如果也想使用该函数，请将蓝牙引擎打印字符串的宏打开
 */
#define PRINT_STRING_ONLY       0
#define PRINT_DATA_ONLY         1
#define PRINT_STRING_AND_DATA   2
#define PRINT_HEX_BYTES         3
#define PRINT_STRING_NONULL     4 //打印不带结束符的字符串，会指定长度
#define PRINT_RETURN_DISABLE    0x80
//#define libc_print(a,b,c)                   ((void(*)(uint8* , uint32 , uint8 ))LIBC_PRINT_ENTRY)((a), (b), (c))

#define libc_pthread_exit()                 ((void(*)(void))LIBC_PTHREAD_EXIT_ENTRY)()
#define libc_pthread_create(a,b,c)          ((int(*)(pthread_param_t* , INT8U , uint8 ))LIBC_PTHREAD_CREATE_ENTRY)((a), (b), (c))

#define libc_exit(a)                        ((void(*)(int8 ))LIBC_EXIT_ENTRY)(a)
#define libc_waitpid(a,b)                   ((int(*)(int8* , int ))LIBC_WAITPID_ENTRY)((a), (b))

#define libc_get_process_struct()           ((int8(*)(void))LIBC_GET_PROCESS_STRUCT_ENTRY)()
#define libc_free_process_struct(a)         ((void(*)(int8 ))LIBC_FREE_PROCESS_STRUCT_ENTRY)(a)

#define libc_sem_init(a,b)                  ((int(*)(os_event_t** , uint32 ))LIBC_SEM_INIT_ENTRY)((a), (b))
#define libc_sem_destroy(a)                 ((int(*)(os_event_t** ))LIBC_SEM_DESTROY_ENTRY)(a)
#define libc_sem_wait(a,b)                  ((int(*)(os_event_t*, uint16 ))LIBC_SEM_WAIT_ENTRY)((a), (b))
#define libc_sem_trywait(a)                 ((int(*)(os_event_t* ))LIBC_SEM_TRYWAIT_ENTRY)(a)
#define libc_sem_post(a)                    ((int(*)(os_event_t* ))LIBC_SEM_POST_ENTRY)(a)

#define libc_pthread_mutex_init(a)          ((int(*)(os_event_t** ))LIBC_PTHREAD_MUTEX_INIT_ENTRY)(a)
#define libc_pthread_mutex_destroy(a)       ((int(*)(os_event_t** ))LIBC_PTHREAD_MUTEX_DESTROY_ENTRY)(a)
#define libc_pthread_mutex_lock(a)          ((int(*)(os_event_t* ))LIBC_PTHREAD_MUTEX_LOCK_ENTRY)(a)
#define libc_pthread_mutex_unlock(a)        ((int(*)(os_event_t* ))LIBC_PTHREAD_MUTEX_UNLOCK_ENTRY)(a)
#define libc_pthread_mutex_trylock(a)       ((int(*)(os_event_t* ))LIBC_PTHREAD_MUTEX_TRYLOCK_ENTRY)(a)

#define libc_pthread_cond_init(a,b)         ((int(*)(os_event_t** , uint32 ))LIBC_PTHREAD_COND_INIT_ENTRY)((a), (b))
#define libc_pthread_cond_destroy(a)        ((int(*)(os_event_t** ))LIBC_PTHREAD_COND_DESTROY_ENTRY)(a)
#define libc_pthread_cond_wait(a,b,c)       ((int(*)(os_event_t* , os_event_t* , uint16 ))LIBC_PTHREAD_COND_WAIT_ENTRY)((a), (b), (c))
#define libc_pthread_cond_signal(a)         ((int(*)(os_event_t* ))LIBC_PTHREAD_COND_SIGNAL_ENTRY)(a)

/*#define libc_print_by_cpu_uart_init(a,b,c)    ((void(*)(uint8, uint8 *, print_info_mem_t *))LIBC_PRINT_BY_CPU_UART_INIT_ENTRY)((a), (b), (c))*/
#define libc_printf(...)                    ((void(*)(const uint8 *fmt, ...))LIBC_PRINTF_ENTRY)(__VA_ARGS__)
#define libc_printf_async(...)              ((void(*)(const uint8 *fmt, ...))LIBC_PRINTF_ASYNC_ENTRY)(__VA_ARGS__)
#define libc_printhex(a,b,c)                ((void(*)(uint8*, uint16, uint32))LIBC_PRINTHEX_ENTRY)((a),(b),(c))
#define libc_printhex_async(a,b,c)          ((void(*)(uint8*, uint16, uint32))LIBC_PRINTHEX_ASYNC_ENTRY)((a),(b),(c))
#define libc_print_agent()                  ((void(*)(void))LIBC_PRINT_AGENT_ENTRY)()

/*以下是分等级打印的接口，请注意系统没有提供等级过滤的机制，需要各个应用自己控制等级过滤*/
#define ERR_PRE      "<ERROR> "
#define WARNING_PRE  "<WARNING> "
#define INFO_PRE     "<INFO> "
#define libc_printf_error(...)              libc_printf(ERR_PRE     __VA_ARGS__)
#define libc_printf_warning(...)            libc_printf(WARNING_PRE __VA_ARGS__)
#define libc_printf_info(...)               libc_printf(INFO_PRE    __VA_ARGS__)


#define libc_math_exp_fixed(a)              ((int32(*)(int32))LIBC_MATH_EXP_FIXED_ENTRY)((a))

#define libc_math_udiv64_32(a,b)            ((uint32(*)(UINT64,uint32))LIBC_MATH_UDIV64_32_ENTRY)((a),(b))
#define libc_pthread_creat_inner(a,b)       ((uint32(*)(void*,process_param_t *))LIBC_PTHREAD_CREAT_INNNER_ENTRY)((a),(b))
#endif


