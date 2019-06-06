
#ifndef __HAL_PWM_H__
#define __HAL_PWM_H__

#include <attr.h>
#include <typeext.h>
#include "hal_datatype.h"
#include "hal_pad_extern.h"

//private defines
extern void hal_set_pwm_init(uint8 idx, pwm_para_t *pwm_para);
extern void hal_reset_pwm(uint8 idx);

#endif
