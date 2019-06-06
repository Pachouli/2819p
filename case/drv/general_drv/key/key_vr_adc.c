/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：可变电阻器-ADC参数接口实现。
 * 作者：cailizhen
 ********************************************************************************/

#include "key_inner.h"

#if (SUPPORT_VR_ADCKEY == 1)

#define VR_ADC_MAX 2

uint16 key_vr_adc_value[VR_ADC_MAX];
uint8 key_vr_adc_mux_switch_flag; //0表示片选GPIO13,1表示片选GPIO19

#define VR_ADC_DEBOUNCE_COUNT  8
uint16 key_vr_adc_value_debounce[VR_ADC_MAX][VR_ADC_DEBOUNCE_COUNT];
uint8 key_vr_adc_value_debounce_index[VR_ADC_MAX];

int8 vr_adckey_idx;

void key_inner_vr_adc_init(void)
{
    uint8 index;
    uint16 vr_adcdat;

    vr_adckey_idx = set_pad_function(PAD_GPIO13, PAD_FUNC_ANOLOG, NULL, 0);
    if (vr_adckey_idx >= 0)
    {
        pmu_enable_lradc(vr_adckey_idx, TRUE);
    }

    key_vr_adc_mux_switch_flag = 1;

    key_inner_vr_adc_mux_switch(); //switch to 0
    sys_mdelay(1);
    vr_adcdat = hal_pmu_read_lradc(vr_adckey_idx);
    key_vr_adc_value[0] = vr_adcdat;

    key_inner_vr_adc_mux_switch(); //switch to 1
    sys_mdelay(1);
    vr_adcdat = hal_pmu_read_lradc(vr_adckey_idx);
    key_vr_adc_value[1] = vr_adcdat;

    for (index = 0; index < VR_ADC_MAX; index++)
    {
        libc_memset(key_vr_adc_value_debounce[index], key_vr_adc_value[index], VR_ADC_DEBOUNCE_COUNT);
    }
    libc_memset(key_vr_adc_value_debounce_index, 0x00, VR_ADC_MAX);

    key_inner_vr_adc_mux_switch(); //switch to 0
}

void key_inner_vr_adc_sample(void)
{
    uint16 adc_total;
    bool need_update_value;
    uint8 index,i;
    uint16 vr_adcdat;

    vr_adcdat = hal_pmu_read_lradc(vr_adckey_idx);
    if (key_vr_adc_mux_switch_flag == 0)
    {
        key_vr_adc_value_debounce[0][key_vr_adc_value_debounce_index[0]] = vr_adcdat;
        key_vr_adc_value_debounce_index[0]++;
        key_vr_adc_value_debounce_index[0] %= VR_ADC_DEBOUNCE_COUNT;
    }
    else
    {
        key_vr_adc_value_debounce[1][key_vr_adc_value_debounce_index[1]] = vr_adcdat;
        key_vr_adc_value_debounce_index[1]++;
        key_vr_adc_value_debounce_index[1] %= VR_ADC_DEBOUNCE_COUNT;
    }

    for (index = 0; index < VR_ADC_MAX; index++)
    {
        adc_total = 0;
        need_update_value = FALSE;
        for (i = 0; i < VR_ADC_DEBOUNCE_COUNT; i++)
        {
            adc_total += key_vr_adc_value_debounce[index][i];
        }
        adc_total /= VR_ADC_DEBOUNCE_COUNT;
        if ((adc_total == 0) || (adc_total == 0x3ff))
        {
            need_update_value = TRUE;
        }
        else if ((adc_total > key_vr_adc_value[index]) && (adc_total - key_vr_adc_value[index] > 8))
        {
            need_update_value = TRUE;
        }
        else if ((adc_total < key_vr_adc_value[index]) && (key_vr_adc_value[index] - adc_total > 8))
        {
            need_update_value = TRUE;
        }

        if (need_update_value == TRUE)
        {
            key_vr_adc_value[index] = adc_total;
            #if 0
            if (index == 0)
            {
                libc_printf("IO13:%x\n", adc_total);
            }
            else
            {
                libc_printf("IO19:%x\n", adc_total);
            }
            #endif
        }
    }
}

void key_inner_vr_adc_mux_switch(void)
{
    if (key_vr_adc_mux_switch_flag == 0)
    {
        key_vr_adc_mux_switch_flag = 1;

        set_pad_function(PAD_GPIO13, PAD_FUNC_GPIOIN, NULL, (void *)0);
        set_pad_function(PAD_GPIO19, PAD_FUNC_ANOLOG, NULL, (void *)0);
    }
    else if (key_vr_adc_mux_switch_flag == 1)
    {
        key_vr_adc_mux_switch_flag = 0;

        set_pad_function(PAD_GPIO19, PAD_FUNC_GPIOIN, NULL, (void *)0);
        set_pad_function(PAD_GPIO13, PAD_FUNC_ANOLOG, NULL, (void *)0);
    }
}

void key_inner_get_vr_adc_value(uint16 *p_adc_value)
{
    libc_memcpy(p_adc_value, key_vr_adc_value, sizeof(key_vr_adc_value));
}

#endif
