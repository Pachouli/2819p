/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：KEY驱动内部头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _IR_INNER_H
#define _IR_INNER_H

#include "psp_includes.h"
#include "key_interface.h"
#include "key_common_inner.h"

#if (SUPPORT_IR_KEY == 1)

extern void key_ir_init(irmap_para_t *p_irmap_para);
extern void key_ir_deinit(void);
extern void key_ir_check_status(void);

#endif

#endif/*_KEY_H*/
