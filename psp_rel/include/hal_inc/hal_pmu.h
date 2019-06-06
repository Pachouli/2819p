
#ifndef __HAL_PMU_H__
#define __HAL_PMU_H__

#include <attr.h>
#include <typeext.h>
#include "hal_datatype.h"
#include "hal_pmu_extern.h"

//private defines

typedef struct
{   
    uint32 BIAS_CTL_reg_bak;
    uint32 BDG_CTL_reg_bak;
    uint32 SPD_CTL_reg_bak;
    uint32 VOUT_CTL_reg_bak;
    uint32 MRCR_reg_bak;
    uint32 CMU_DEVCLKEN_reg_bak;
    uint32 SPLL_CTL_reg_bak;
    uint32 AUDIO_PLL_CTL_reg_bak;
    uint32 CORE_PLL_CTL_reg_bak;
    uint32 CMU_SYSCLK_reg_bak;
    uint32 HOSC_CTL_reg_bak;
} low_power_reg_bak_t;

extern void hal_pmu_poweron_init(uint8 vcc_vol_sel);
extern int hal_pmu_enter_low_power_mode_inner(low_power_reg_bak_t *p_low_power_reg_bak)__FAR__;
extern int hal_pmu_exit_low_power_mode_inner(low_power_reg_bak_t *p_low_power_reg_bak)__FAR__;
extern void hal_pmu_en_wake_up(wakeup_en_para_t *p_wakeup_en_para);
extern void hal_pmu_power_off(void);
extern void hal_pmu_enable_lradc(uint8 lradc_id, bool enable);
extern void hal_pmu_enable_lradc1_resistance(bool enable);
extern uint32 hal_pmu_read_lradc_inner(uint8 lradc_id)__FAR__;
extern void hal_pmu_enable_tempadc(bool enable);
extern uint32 hal_pmu_read_tempadc(void);
extern void hal_pmu_enable_batadc(bool enable);
extern uint32 hal_pmu_read_batadc_inner(void)__FAR__;
extern uint32 hal_pmu_read_onoff_key_inner(void)__FAR__;
extern void hal_pmu_analyz_and_clear_wake_pd(sys_poweron_info_t *p_poweron_info);

extern int hal_pmu_vdd_adjust(uint8 vdd_idx);
#endif
