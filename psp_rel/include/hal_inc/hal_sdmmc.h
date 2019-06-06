
#ifndef __HAL_SDMMC_H__
#define __HAL_SDMMC_H__

#include <attr.h>
#include <typeext.h>
#include "hal_datatype.h"
#include "hal_sdmmc_extern.h"

//private defines

typedef struct
{
    uint32 ctl;
    uint32 state;
    uint32 arg;
    uint32 buf0;
    uint32 buf1;
    uint32 buf2;
    uint32 buf3;
    uint32 buf4;
}card_reg_t;

extern int32 hal_sdmmc_trans_err_check(void);
extern int32 hal_sdmmc_send_cmd(uint32 cmd, uint32 param, uint32 ctl);
extern void  hal_sdmmc_set_clk(uint8 sdclk_div);
extern void  hal_sdmmc_send_continuous_clock(uint32 time_ms);
extern void  hal_sdmmc_reset(void);
extern int32 hal_sdmmc_start_data_trans(uint32 tm_mode, uint32 cmd, uint32 arg, uint32 blk_num, uint32 blk_size);
extern bool  hal_sdmmc_is_data_trans_cmp(void);

extern void hal_sdmmc_phy_init(void);
extern void hal_sdmmc_phy_exit(void);

extern int8 hal_sdmmc_is_sd_exist(void);
extern int8 hal_sdmmc_is_mmc_exist(void);

extern void hal_sdmmc_get_reginfo(card_reg_t * info);

extern void hal_sdmmc_set_phy_card_type(uint8 card_type);
extern void hal_sdmmc_set_phy_delay_chain(uint8 delay_chain);

#endif
