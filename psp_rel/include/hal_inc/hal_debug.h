
#ifndef __HAL_DEBUG_H__
#define __HAL_DEBUG_H__

#include <attr.h>
#include <typeext.h>
#include "hal_datatype.h"

//private defines

//ejtag_sel:
//0��ʾĬ��EJTAG Group1��EJ_TCK/EJ_TDO/EJ_TMS/EJ_TDI/EJ_TRST
//1��ʾEJTAG Group2������GPIO0/GPIO1/GPIO2/GPIO11/GPIO12
//2��ʾEJTAG Group3������GPIO3/GPIO4/GPIO5/GPIO6/GPIO7
extern void hal_debug_set_ejtag(uint8 ejtag_sel);
extern void hal_debug_set_btdebug(uint32 btdb_port);

#endif
