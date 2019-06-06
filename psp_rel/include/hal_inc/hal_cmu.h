
#ifndef __HAL_CMU_H__
#define __HAL_CMU_H__

#include <attr.h>
#include <typeext.h>
#include "hal_datatype.h"

//private defines

typedef struct
{
    uint16 mucpll : 7;
    uint16 vdd : 4;
    uint16 vcc : 3;
    uint16 cpuclk_ctl;
} freq_para_t;

typedef struct
{
    freq_para_t freq_para;
} frequency_param_t;

typedef struct
{
    /*! 频点cp0_count溢出时间*/
    uint32 overflow_value_us;
}overflow_value_t;

typedef struct
{
    uint8 front_freq_level;
    uint8 back_freq_level;
    uint8 btctl_freq_level;
    uint8 btengine_freq_level;
    //M4K run frequency, Mhz
    uint8 sys_m4k_freq;
    uint8 cur_freq;
    uint8 lock_freq_level;
}freqval_t;

typedef enum
{
    DISABLE_SSC = 0, //关闭展频
    ENABLE_SSC
} ssc_set_e;

extern uint32 hal_set_hosc_cap(uint32 hosc_cap);
extern void hal_set_fm_clock_inner(uint8 clk_select);
extern void hal_disable_fm_clock_inner(void);
extern void hal_cmu_adjust_freq(freq_para_t *p_freq_para, bool freq_up_flag, bool drv_need_mcupll);
extern bool hal_cmu_is_spi_enable(void) __FAR__;
extern void hal_cmu_ssc_enable(bool enable);
extern void hal_cmu_init(uint32 hosc_cap);
extern  void hal_adfus_cmu_set(void);
extern uint32 hal_spi_set_clock_check(uint32 spi_clock) __FAR__;

#endif
