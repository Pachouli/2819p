
#ifndef __HAL_INTC_H__
#define __HAL_INTC_H__

#include <attr.h>
#include <typeext.h>
#include "hal_datatype.h"
#include "irq.h"

//private defines

extern bool hal_intc_is_irq_pending_inner(uint8 irq_idx);
extern uint32 hal_intc_mask_except(uint32 irq_src);
extern void hal_intc_mask_restore(uint32 irq_save);
extern void hal_intc_enable_irq(uint32 irq_src);
extern void hal_intc_disable_irq(uint32 irq_src);

extern void hal_intc_init(void);

#endif
