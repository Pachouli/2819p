
#ifndef __HAL_TIMER_H__
#define __HAL_TIMER_H__

#include <attr.h>
#include <typeext.h>
#include "hal_datatype.h"
#include "hal_timer_extern.h"

//private defines

#define WD_PERIOD_172MS   0
#define WD_PERIOD_352MS   1
#define WD_PERIOD_703MS   2
#define WD_PERIOD_1P4S    3
#define WD_PERIOD_2P8S    4
#define WD_PERIOD_5P6S    5
#define WD_PERIOD_11P2S   6
#define WD_PERIOD_REBOOT  7

extern void hal_clear_watch_dog(void) __FAR__;
extern void hal_enable_watch_dog(uint8 wd_period);
extern void hal_disable_watch_dog(void);

extern void hal_timer0_init(uint32 t0_period_us, void *t0_isr);
extern void hal_timer1_init(uint32 t1_period_us, void *t1_isr);

typedef void (*rtc_alarm_isr)(void);

extern void hal_rtc_phy_init(void);
extern void hal_rtc_set_time(time_t *time);
extern void hal_rtc_get_time(time_t *time);
extern void hal_rtc_set_date(date_t *date);
extern void hal_rtc_get_date(date_t *date);
extern void hal_rtc_set_alarm_time(time_t *time);
extern void hal_rtc_get_alarm_time(time_t *time);
extern void hal_rtc_enable_alarm(rtc_alarm_isr alarm_isr);
extern void hal_rtc_disable_alarm(void);
extern void hal_rtc_get_status(rtc_status_t *p_rtc_status);
extern void hal_rtc_bakdata_write(uint8 bak_idx, uint32 bak_val);
extern uint32 hal_rtc_bakdata_read(uint8 bak_idx);

#endif
