
#ifndef __HAL_SPI_H__
#define __HAL_SPI_H__

#include <attr.h>
#include <typeext.h>
#include <hal_spi1_extern.h>
#include "hal_datatype.h"

//private defines
typedef enum
{
    SPI_CLK_SEL_CPU = 0,
    SPI_CLK_SEL_HOSC,
    SPI_CLK_SEL_PLL,
    SPI_CLK_SEL_CK48M,
} spi_clk_sel_e;

extern void hal_spi_phy_init(bool cache_randomize)__FAR__;
extern void hal_spi_set_line(bool spi_dual_read_mode, bool spi_4wire_mode);
extern void hal_spi_set_mode(bool mode0);
extern void hal_spi_set_3wire(void);

extern uint32 hal_spi_set_clock(uint32 spi_clock) __FAR__;
extern void hal_spi_crc_irq_enable(void) __FAR__;

#endif
