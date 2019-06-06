/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：KEY驱动内部头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _KEY_INNER_H
#define _KEY_INNER_H

#include "psp_includes.h"
#include "key_interface.h"
#include "key_inner_sim_key.h"
#include "key_common_inner.h"
#include "ir_inner.h"
#include "ext_rtc_inner.h"

//声明变量区
extern int key_timer_id;
extern int8 adckey_idx;

extern uint8 key_inner_get_battery(void);
extern void key_sampling_battery_voltage(void);
extern void key_inner_bat_det_init(void);

extern void key_inner_scan_init(keymap_para_t *p_keymap_para);
extern void key_inner_key_scan(void);

extern void key_inner_device_detect_init(bool need_usb_detect);
extern bool key_inner_aux_hot_detect(bool chk_in);
extern bool key_inner_usbhost_hot_detect(bool chk_in);
extern bool key_inner_usbdev_hot_detect(bool chk_in);
extern bool key_inner_adaptor_hot_detect(bool chk_in);
extern bool key_inner_card_hot_detect(bool chk_in);
extern void key_inner_usb_detect_switch(uint8 det_mode);

extern void key_inner_adckey_switch(void);

#if (SUPPORT_FAST_PREVIEW == 1)
extern void key_inner_set_fast_preview_cbk(fast_preview_cbk cbk);
#endif

extern void key_inner_timer_handle(void);

extern void key_inner_vr_adc_init(void);
extern void key_inner_vr_adc_sample(void);
extern void key_inner_vr_adc_mux_switch(void);
extern void key_inner_get_vr_adc_value(uint16 *p_adc_value);

#if (SUPPORT_ROTARY_ENCODER == 1)

typedef enum
{
    EnKEY_STOP = 0,
    EnKEY_FORWARD = 1,
    EnKEY_BACKWARD = 2,
} EnKEY_Status;

typedef struct
{
    EnKEY_Status EnStatus;
    bool start_check_flag;
	sirq_triggle_e A_sirq_triggle;
	sirq_triggle_e B_sirq_triggle;
	bool A_low_flag;
	bool B_low_flag;
	bool A_high_flag;
	bool B_high_flag;
	uint32 A_jitter_timer;
	uint32 B_jitter_timer;
} rotary_encoder_info_t;

extern rotary_encoder_info_t g_rotary_encode_info;
extern void key_inner_rotary_encoder_init(void);

#endif

#endif/*_KEY_H*/
