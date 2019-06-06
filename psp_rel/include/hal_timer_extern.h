#ifndef __HAL_TIMER_EXTERN_H__
#define __HAL_TIMER_EXTERN_H__

#include <attr.h>
#include <typeext.h>

typedef struct
{
    uint8 calendar_en : 1;
    uint8 alarm_en : 1;
} rtc_status_t;

#endif
