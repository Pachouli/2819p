
#ifndef __HAL_MMU_H__
#define __HAL_MMU_H__

#include <attr.h>
#include <typeext.h>
#include "hal_datatype.h"

//private defines

typedef struct
{
    uint32 phy_addr;
    uint32 mapping_addr;
} mmu_cache_entry_t;

typedef struct
{
    uint32 cache_miss;
    uint32 cache_hit;
} mmu_performance_info_t;

extern void hal_mmu_read_cache_entry(uint8 entry_id, uint32 *p_phy_addr, uint32 *p_mapping_addr) __FAR__;
extern void hal_mmu_set_cache_entry(uint8 entry_id, uint32 phy_addr, uint32 mapping_addr) __FAR__;

extern bool hal_mmu_is_cache_crc_error(void);
extern void hal_mmu_cache_crc_recover(uint32 va_start);
extern void hal_mmu_access_error_check(void);

extern void hal_performance_monitoring_restart(void);
extern void hal_performance_monitoring_stop(void);
extern void hal_performance_monitoring_report(mmu_performance_info_t *p_mmu_performance_info);

#define RAM_BLK_RAM3   0
#define RAM3_CLK_CPU   0
#define RAM3_CLK_VAD   1

#define RAM_BLK_RAM4   1
#define RAM4_CLK_CPU   0
#define RAM4_CLK_VAD   1
#define RAM4_CLK_DAC   2

#define RAM_BLK_PCMRAM 2
#define PCMRAM_CLK_CPU 0
#define PCMRAM_CLK_DAC 1

#define RAM_BLK_URAM   3
#define URAM_CLK_CPU   0
#define URAM_CLK_USBC  1

extern void hal_mmu_ram_select(uint8 ram_block, uint8 clk_sel);

#endif
