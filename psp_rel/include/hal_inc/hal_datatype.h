
#ifndef __HAL_DATATYPE_H_
#define __HAL_DATATYPE_H_

#include "sysdef.h"
#include "typeext.h"
#include "storage_device.h"

typedef int (*cbk_request_irq)(uint32, void *);
typedef void (*cbk_free_irq)(uint32);

typedef uint32 (*cbk_int_close)(void);
typedef void (*cbk_int_open)(uint32);

typedef void (*cbk_memcpy)(void * dest,const void *src,uint32 count);
typedef void (*cbk_memset)(void * s,int c,uint32 count);

typedef void (*cbk_udelay)(uint32);
typedef void (*cbk_sleep)(uint16);

typedef uint32 (*cbk_get_ab_timer_us)(void);

typedef void (*cbk_printf)(const uint8 *fmt, ...);

//memcpy,memset
//短暂延时 udelay
//超长延时或者超时 get_ab_timer_us
//关中断 int_close & int_open
//清狗 提供HAL模块内宏函数  注：对外提供看门狗的多个操作函数，而非宏函数

typedef struct
{
    cbk_request_irq     hal_request_irq;
    cbk_free_irq        hal_free_irq;
    cbk_int_close       hal_int_close;
    cbk_int_open        hal_int_open;
    cbk_memcpy          hal_memcpy;
    cbk_memset          hal_memset;
    cbk_udelay          hal_udelay;
    cbk_sleep           hal_sleep;
    cbk_get_ab_timer_us hal_get_ab_timer_us;
    cbk_printf          hal_printf;
} sys_hal_callback_t;

extern sys_hal_callback_t * g_p_sys_hal_cbks;

typedef struct
{
    uint8 read_ptr;
    uint8 write_ptr;
    uint8 length;
    uint8 capacity;
    uint8 *raw_data;
    uint8 full_error : 1;  //缓冲区已满，发生数据丢弃的错误；缓冲区消费者读走数据后应清除；flush缓冲区时也要清除
    uint8 filter_flag : 1; //缓冲区过滤标志，用来设置过滤数据
} buffer_rw_small_fifo_t;

#endif
