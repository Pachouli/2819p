
#ifndef __HAL_CPU_H__
#define __HAL_CPU_H__

#include <attr.h>
#include <typeext.h>
#include "hal_datatype.h"

//private defines

typedef struct
{
	uint32 epc;
	uint32 epc_cause;
	uint32 err_data_addr;
	uint32 err_code_addr;
} exception_dump_info_t;

extern void hal_Reserved_Instruction_exception_handle(exception_dump_info_t *p_e_dump_info);
extern void hal_all_exception_handle(exception_dump_info_t *p_e_dump_info);

#endif
