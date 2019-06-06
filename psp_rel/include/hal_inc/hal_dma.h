
#ifndef __HAL_DMA_H__
#define __HAL_DMA_H__

#include <attr.h>
#include <typeext.h>
#include "hal_datatype.h"
#include "hal_dma_extern.h"

//private defines

extern dma_irq_cbk dma0_irq_cbk;
extern dma_irq_cbk dma1_irq_cbk;
extern dma_irq_cbk dma2_irq_cbk;
extern dma_irq_cbk dma3_irq_cbk;
extern dma_irq_cbk dma4_irq_cbk;

extern void hal_dma_init(void);
extern void hal_reset_dma_chan(dma_chan_e dma_chan);
extern void hal_set_dma_chan(dma_chan_e chan, dma_chan_setting_t *chan_sett);
extern void hal_start_dma_chan(dma_chan_e chan, uint32 frame_len);
extern int32 hal_wait_dma_chan_tc(dma_chan_e chan, uint8 mode);

#endif
