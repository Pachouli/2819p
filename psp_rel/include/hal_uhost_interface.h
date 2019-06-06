
#ifndef __HAL_UHOST_INTERFACE_H__
#define __HAL_UHOST_INTERFACE_H__

#include "typeext.h"

extern int hal_uhost_detect_init_inner(void);
extern int hal_uhost_detect_inner(void);
extern uint8 audio_device_host_bus_reset(void);
extern void audio_device_host_ep_reset(uint8 mode);
extern uint8 audio_device_host_controller_enable(uint8 speed_mode,uint8 * init_already,uint8 *uhost_state);
extern uint8 audio_device_host_controller_disable(void);
extern void audio_device_host_clear_eperrorirq(void);
extern uint8 audio_device_host_clear_feature_ep_stall(uint8 stall_stage,void* host_info);

extern uint8 audio_device_host_setup_stage_transaction(uint8 *data_address, uint8 data_length);
extern uint8 audio_device_host_control_in_data(uint8 *data_address, uint8 data_length);
extern uint8 audio_device_host_status_stage_transaction_out(void);
extern uint8 audio_device_host_status_stage_transaction_in(void);
//extern uint8 audio_device_host_set_endpoint(device_configuration_t* device_config,usb_host_information_t* host_info);
extern uint8 audio_device_host_set_endpoint(void* device_config,void* host_info);

extern uint8 audio_device_host_bulk_data(uint8 *buf, uint32 len, void* host_info,uint8 rw_mode);

extern uint8 hal_get_uhost_in_type(void);

#endif 
