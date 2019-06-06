
#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

int8 hal_pad_gpio7_op(pad_op_e pad_op, pad_function_e pad_func, pad_phy_para_t *phy_para, void *para)
{
    *((REG32)(GPIO7_CTL)) |= (1 << GPIO7_CTL_AD_Select); //0 for digital, 1 for anolog lradc1

    return 1;
}

int8 hal_pad_gpio3_op(pad_op_e pad_op, pad_function_e pad_func, pad_phy_para_t *phy_para, void *para)
{
    *((REG32)(GPIO3_CTL)) |= (1 << GPIO3_CTL_AD_Select); //0 for digital, 1 for anolog lradc9

    return 9;
}

