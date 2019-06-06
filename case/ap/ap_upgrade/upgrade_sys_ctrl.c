/*!
 * \file      sys_ctrl.c
 * \brief     ϵͳ���ƽӿ�
 * \details   
 * \author    Tommy Wang
 * \date      
 * \copyright Actions
 */

#include "upgrade_app.h"

#include <../HAL/include/hal_datatype_inner.h>


/* �Ƿ�����
 */
void sys_enable_reboot(bool enable)
{
    __reg32(WD_CTL) |= (1 << WD_CTL_CLR);

    __reg32(WD_CTL) = 
        (0 << WD_CTL_SIGS) |
        (enable << WD_CTL_WDEN) |
        (3 << WD_CTL_CLKSEL_SHIFT);  // 1.4 ��
}


