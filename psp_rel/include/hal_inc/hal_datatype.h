
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
//������ʱ udelay
//������ʱ���߳�ʱ get_ab_timer_us
//���ж� int_close & int_open
//�幷 �ṩHALģ���ں꺯��  ע�������ṩ���Ź��Ķ���������������Ǻ꺯��

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
    uint8 full_error : 1;  //�������������������ݶ����Ĵ��󣻻����������߶������ݺ�Ӧ�����flush������ʱҲҪ���
    uint8 filter_flag : 1; //���������˱�־���������ù�������
} buffer_rw_small_fifo_t;

#endif
