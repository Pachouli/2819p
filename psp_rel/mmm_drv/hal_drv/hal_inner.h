/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：HAL驱动内部头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef __HAL_INNER_H__
#define __HAL_INNER_H__

#include <attr.h>
#include <psp_includes.h>
#include "hal_interface.h"
#include "hal_usb_interface.h"
#include "hal_card_interface.h"
#include "hal_uhost_interface.h"
#include "hal_includes_inner.h"
#include "hal_lib_fs.h"

extern sys_global_info_t* p_sys_global_info;

extern int32 audio_set_audio_volume(int32 vol_p1_db);

extern int32 hal_hardware_set(hw_set_op_e set_op, void *para);
extern int32 hal_hardware_get(hw_get_op_e get_op, void *para);

extern void hal_TWI_set_hw_info_sim(twi_hw_info_t *p_twi_hw_info);
extern uint8 hal_TWI_write_bytes(uint8 mode, uint8 idx, uint8 *buf, uint8 address, uint8 length, uint8 read_mode);
extern uint8 hal_TWI_read_bytes(uint8 mode, uint8 idx, uint8 *buf, uint8 address, uint8 length);
extern void hal_TWI_set_hw_info(void);

extern void hal_iic_init(SMBUS_ID bus, SMBUS_SPEED speed, hdmi_hw_info_t *p_hdmi_hw_info);
extern SMBUS_STATUS hal_iic_sync(SMBUS_ID bus, uint8 addr, uint8 *buf, uint16 length, SMBUS_MODE mode);

extern void hal_TWI_slave_set_hw_info(twi_slave_hw_info_t *p_hw_info);

extern void uart_recv_cache_init(void);
extern uint32 uart_recv_bytes_inner(uint8 idx, uint8 *buf, uint32 bytes);
extern void uart_recv_bytes_from_fifo(void);

extern void hal_timer_init(void);
extern void hal_timer_deinit(void);

extern int32 hal_usb_device_uhost_detect_switch_inner(uint8 det_mode);

extern int32 hal_sdmmc_app_start_phy_rw(uint32 opr, uint32 cmd, uint32 arg, uint32 blk_num, uint32 blk_size, uint8 *ram);
extern int32 hal_sdmmc_app_wait_phy_rw_cmp(uint32 timeout_ms);

extern int32 hal_sdmmc_app_select(void);
extern int32 hal_sdmmc_app_deselect(void);

extern int hal_sdmmc_app_detect_with_mutex(void);

extern void delay_us_hosc(void)__FAR__;
extern void delay_ms_32k(int ms)__FAR__;
#endif

