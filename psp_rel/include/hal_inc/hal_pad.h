
#ifndef __HAL_PAD_H__
#define __HAL_PAD_H__

#include <attr.h>
#include <typeext.h>
#include "hal_datatype.h"
#include "hal_pad_extern.h"

//private defines

extern int8 hal_pad_gpio_op_template(uint8 pad_gpio, pad_op_e pad_op, pad_function_e pad_func, pad_phy_para_t *phy_para, void *para);

extern void  hal_set_gpio_out_level(uint8 pad_gpio, uint8 level) __FAR__; //pad_gpio 0 ~ 51
extern uint8 hal_get_gpio_in_level(uint8 pad_gpio) __FAR__; //pad_gpio 0 ~ 51
extern uint8 hal_get_sio_in_level(uint8 pad_sio) __FAR__; //pad_sio 0 ~ 1

#endif
