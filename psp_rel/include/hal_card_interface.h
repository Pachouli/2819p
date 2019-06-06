
#ifndef __HAL_CARD_INTERFACE_H__
#define __HAL_CARD_INTERFACE_H__

#include "typeext.h"
#include "card.h"

extern int32 hal_sdmmc_app_phy_configure(void *card_param, uint8 *configure_buffer);
extern int hal_sdmmc_app_detect(void);
extern int audio_device_set_card_pm_param(card_pm_cfg_t *card_pm_cfg);

extern int32 hal_sdmmc_app_start_phy_rw(uint32 opr, uint32 cmd, uint32 arg, uint32 blk_num, uint32 blk_size, uint8 *ram);
extern int32 hal_sdmmc_app_wait_phy_rw_cmp(uint32 timeout_ms);
extern int32 hal_sdmmc_app_stop_transmission(void);

extern void hal_sdmmc_app_set_pad(void *para);
extern void hal_sdmmc_app_reset_pad(void *para);

extern int32 hal_sdmmc_get_cap(uint32 *ret_cap);

extern int32 hal_sdmmc_app_get_status(uint8 *status);

extern int32 hal_sdmmc_delay_chain_test(uint8 *test_buf);

#endif  //__HAL_CARD_INTERFACE_H__
