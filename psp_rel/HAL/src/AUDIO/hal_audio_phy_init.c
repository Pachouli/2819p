
#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

uint8 audio_init_count;

int32 hal_audio_phy_init(void)
{
    uint32 cpu_sr;

    audio_init_count++;
    if (audio_init_count > 1)
    {
        return HAL_OK;
    }

    //Audio硬件初始化
    cpu_sr = g_p_sys_hal_cbks->hal_int_close();

    //cmu set, audio clk set CK_24M
    *((REG32)(SPLL_CTL)) |= (1 << SPLL_CTL_CK_24M_EN) | (1 << SPLL_CTL_SPLL_EN);
    *((REG32)(SPLL_DEBUG)) = (*((REG32)(SPLL_DEBUG)) & (~SPLL_DEBUG_IB_SPLL_MASK) & (~SPLL_DEBUG_SPLL_ICPP_MASK))
        | (0x03<<SPLL_DEBUG_SPLL_ICPP_SHIFT) | (0x02<<SPLL_DEBUG_IB_SPLL_SHIFT);

    g_p_sys_hal_cbks->hal_int_open(cpu_sr);

    g_p_sys_hal_cbks->hal_udelay(100);

    cpu_sr = g_p_sys_hal_cbks->hal_int_close();

    *((REG32)(AUDIO_PLL_CTL)) = (*((REG32)(AUDIO_PLL_CTL)) & (~(1 << AUDIO_PLL_CTL_AUDPLL_SCLK_SEL))) | (0x00<<AUDIO_PLL_CTL_AUDPLL_SCLK_SEL); // audio pll from ck24
    *((REG32)(AUDIO_PLL_CTL)) = (*((REG32)(AUDIO_PLL_CTL)) & (~AUDIO_PLL_CTL_APS_MASK)) | (0x0C<<AUDIO_PLL_CTL_APS_SHIFT);
    *((REG32)(AUDIO_PLL_CTL)) = *((REG32)(AUDIO_PLL_CTL)) | (1 << AUDIO_PLL_CTL_AUDPLLEN); // audio pll en

    *((REG32)(AUDIO_PLL_DEBUG)) = (*((REG32)(AUDIO_PLL_DEBUG)) & (~AUDIO_PLL_DEBUG_REG_SPLL_ICPI_MASK))
        | (0x01<<AUDIO_PLL_DEBUG_REG_SPLL_ICPI_SHIFT);

    //audio rmu reset and then normal
    *((REG32)(MRCR)) &= (~(1<<MRCR_AUDIO_RESET));
    g_p_sys_hal_cbks->hal_udelay(1);
    *((REG32)(MRCR)) |= (1 << MRCR_AUDIO_RESET);

    g_p_sys_hal_cbks->hal_int_open(cpu_sr);

    return HAL_OK;
}

int32 hal_audio_phy_exit(void)
{
    if (audio_init_count == 0)
    {
        return HAL_OK;
    }
    else
    {
        audio_init_count--;
    }

    if (audio_init_count > 0)
    {
        return HAL_OK;
    }

    //Audio硬件关闭
    //audio rmu reset
    *((REG32)(MRCR)) &= ~(1 << MRCR_AUDIO_RESET);

    *((REG32)(AUDIO_PLL_CTL)) = *((REG32)(AUDIO_PLL_CTL)) & ~(1 << AUDIO_PLL_CTL_AUDPLLEN); // audio pll disable

    return HAL_OK;
}

int32 hal_audio_set_audio_aps(uint8 aps_idx)
{
    uint32 temp;

    temp = (*((REG32)(AUDIO_PLL_CTL)) & AUDIO_PLL_CTL_APS_MASK) >> AUDIO_PLL_CTL_APS_SHIFT;
    if (temp < 8) //fs = 44.1K
    {
        //set audio pll clk
        *((REG32)(AUDIO_PLL_CTL)) &= (~AUDIO_PLL_CTL_APS_MASK);
        *((REG32)(AUDIO_PLL_CTL)) |= (aps_idx << AUDIO_PLL_CTL_APS_SHIFT);
    }
    else //fs = 48K
    {
        //set audio pll clk
        *((REG32)(AUDIO_PLL_CTL)) &= (~AUDIO_PLL_CTL_APS_MASK);
        *((REG32)(AUDIO_PLL_CTL)) |= ((8 + aps_idx) << AUDIO_PLL_CTL_APS_SHIFT);
    }

    return HAL_OK;
}

const int8 aps_freq_values_44100[APS_LEVEL_MAX] = {-91, -45, -22, -14, 0, 31, 53, 96};
const int8 aps_freq_values_48000[APS_LEVEL_MAX] = {-100, -49, -22, -12, 0, 27, 56, 104};

int32 hal_audio_get_audio_aps_info(audio_aps_info_t *p_audio_aps_info)
{
    g_p_sys_hal_cbks->hal_memset(p_audio_aps_info, 0x00, sizeof(audio_aps_info_t));

    g_p_sys_hal_cbks->hal_memcpy(p_audio_aps_info->aps_freq_values[0], aps_freq_values_44100, APS_LEVEL_MAX);
    g_p_sys_hal_cbks->hal_memcpy(p_audio_aps_info->aps_freq_values[1], aps_freq_values_48000, APS_LEVEL_MAX);

    p_audio_aps_info->normal_aps          = 4;
    p_audio_aps_info->slow_aps            = 3;
    p_audio_aps_info->fast_aps            = 5;
    p_audio_aps_info->slow_tws_slower_aps = 2;
    p_audio_aps_info->slow_tws_faster_aps = 4;
    p_audio_aps_info->fast_tws_slower_aps = 4;
    p_audio_aps_info->fast_tws_faster_aps = 6;

    return HAL_OK;
}
