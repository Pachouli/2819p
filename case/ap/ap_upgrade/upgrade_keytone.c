/*!
 * \file      keytone.c
 * \brief     按键音播放接口
 * \details   
 * \author    Tommy Wang
 * \date      
 * \copyright Actions
 */

#include "upgrade_app.h"

#include <../HAL/include/hal_datatype_inner.h>


#if (SUPPORT_HW_KEYTONE == 1)

/* 按键音播放
 */
void keytone_play(uint_t freq, uint_t period)
{
    u32_t   kt_ctl = __reg32(KT_CTL);
    
    uint_t  volume = 8;  // 0 ~ 63

    kt_ctl &= ~(
        (KT_CTL_KTVOLL_MASK) |
        (KT_CTL_KTFR_MASK)   |
        (KT_CTL_KTCNT_MASK));

    kt_ctl |= (
        (1      << KT_CTL_SKT)          |
        (volume << KT_CTL_KTVOLL_SHIFT) |
        (freq   << KT_CTL_KTFR_SHIFT)   |
        (period << KT_CTL_KTCNT_SHIFT));

    __reg32(KT_CTL) = kt_ctl;

    while (__reg32(KT_CTL) & (1 << KT_CTL_SKT))
        ;
}

#endif  // SUPPORT_HW_KEYTONE


