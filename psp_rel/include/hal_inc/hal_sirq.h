
#ifndef __HAL_SIRQ_H__
#define __HAL_SIRQ_H__

#include <attr.h>
#include <typeext.h>
#include "hal_datatype.h"
#include "hal_pad_extern.h"

//private defines
extern void hal_set_sirq_inner(uint8 sirq_num, sirq_set_para_t *sirq_set);
extern void hal_disable_sirq_inner(uint8 sirq_num);

#endif
