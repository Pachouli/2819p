
#ifndef __HAL_AUDIO_H__
#define __HAL_AUDIO_H__

#include <attr.h>
#include <typeext.h>
#include "hal_datatype.h"
#include "hal_audio_extern.h"

//private defines

extern int32 hal_audio_phy_init(void);
extern int32 hal_audio_phy_exit(void);
extern int32 hal_audio_set_audio_aps(uint8 aps_idx);
extern int32 hal_audio_get_audio_aps_info(audio_aps_info_t *p_audio_aps_info);

extern int32 hal_audio_enable_pa(pa_setting_t *pa_sett);
extern int32 hal_audio_disable_pa(void);
extern int32 hal_audio_mute_pa(bool mute);

extern int32 hal_audio_set_audio_volume(int32 db_exp);
extern void hal_audio_reset_pcmbuf(void);
extern void hal_audio_set_pcmbuf(pcmbuf_setting_t *pcmbuf_sett);
extern uint32 hal_audio_get_pcmbuf_length(void);
extern void hal_audio_reset_at_poweron(void);
extern uint8 hal_audio_get_key_tone_status(void);
extern void hal_audio_start_key_tone(kt_setting_t *kt_sett);
extern uint8 hal_audio_get_dac_rate(void);
extern void hal_audio_set_dac_rate(uint8 dac_rate);
extern void hal_audio_wait_dacfifo_empty(void);
extern void hal_audio_enable_dac(dac_setting_t *dac_sett);
extern void hal_audio_disable_dac(void);
extern void hal_audio_i2stx_phy_init(void);
extern void hal_audio_enable_ain(ain_source_type_e ain_src, ain_setting_t *ain_sett);
extern void hal_audio_disable_ain(ain_source_type_e ain_src);
extern void hal_audio_set_adc_gain(uint8 adc_gain);
extern void hal_audio_set_adc_rate(uint8 adc_rate);
extern void hal_audio_enable_adc(ain_source_type_e ain_src, adc_setting_t *adc_sett);
extern void hal_audio_disable_adc(ain_source_type_e ain_src);
extern bool hal_audio_is_sim_adc_dual(void);

extern void hal_audio_enable_i2srx(i2srx_setting_t *i2srx_sett);
extern void hal_audio_disable_i2srx(void);

#endif
