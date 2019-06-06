/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������Welcome ��ڣ�������ADFU�������͵�ػ�������LED�Ʊ�ʾ������
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "psp_includes.h"
#include "case_independent.h"
#include "boot.h"
#include "boot_config_id.h"
#include "hal_includes.h"
#include "hal_pad.h"
#include "hal_pmu.h"
#include "hal_usb_device.h"

sys_hal_callback_t g_sys_hal_cbks;
sys_hal_callback_t * g_p_sys_hal_cbks;

//ǿ��ʹ�ܶ�����ģʽ
#define p_force_enable_udisk  ((uint8 *)(ESD_BK_INFOR_ADDR + 2))

//delay us @26MHz
void welcome_udelay(uint32 delay)
{
    int i, j;
    for (i = 0; i < delay; i++)
    {
        for (j = 0; j < (26/4+1); j++) //*.lst�鵽��ѭ��ֻ��3��ָ���תָ����Ҫ2�����ڣ���4������
        {
            ; //delay
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *  �ػ�����
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
void welcome_power_off(wakeup_en_para_t *p_wakeup_en_para)
{
    uint8 release_cnt;

    //���û��ѷ�ʽ�Ͳ���
    hal_pmu_en_wake_up(p_wakeup_en_para);

    release_cnt = 0;
    while (1)
    {
        if (hal_pmu_read_onoff_key_inner() == FALSE)
        {
            release_cnt++;
            if (release_cnt >= 3)
            {
                break;
            }
        }
        else
        {
            release_cnt = 0;
        }

        welcome_udelay(10*1000);
    }

    //�ϵ�
    hal_pmu_power_off();
}

/******************************************************************************/
/*!
 * \par  Description:
 *    description the function  in here
 * \param[in]    res_buff       welcome resource data buffer
 * \param[in]    null
 * \param[out]   none
 * \return       the result
 * \retval           0 sucess
 * \retval           1 failed
 * \ingroup      spi_nor_brec
 * \note
 * \li
 *******************************************************************************/
void __section__(".ENTRY") welcome(int argc, void *argv[])
{
    uint32 *p_boot_config = (uint32 *) argv[1];
    uint16 lradc_key, bat_adc;
    bool is_low_power = FALSE;
    int8 adckey_idx, batdet_idx;

#if 0
    print_info_mem_t temp_print_info_mem;
    *(volatile uint32 *)(0xC009002C) &= ~0x0f; //0xC009002C��GPIO8��ַ����������ΪUart1Tx����
    ROM_LIBC_PRINT_BY_CPU_UART_INIT(1, 0x9fc23e80, &temp_print_info_mem); //1��ʾUart1Tx��0x9fc23e80��printf�ַ���������
    ROM_LIBC_PRINTF("welcome print test : %s, 0x%x", "hello", 0x1234);
#endif

    //��ʼ��HALӲ���ӿ�ʹ�û���
    g_p_sys_hal_cbks = &g_sys_hal_cbks;
    g_p_sys_hal_cbks->hal_udelay = welcome_udelay;

    if (p_boot_config[SETTING_SYS_ADCKEY_ADFU_ENABLE] > 0)
    {
        adckey_idx = hal_pad_lradc1_op(PAD_OP_SET, PAD_FUNC_ANOLOG, NULL, (void *) 0);
        if (adckey_idx >= 0)
        {
            hal_pmu_enable_lradc(adckey_idx, TRUE);
            if (adckey_idx == 1)
            {
                hal_pmu_enable_lradc1_resistance(p_boot_config[SETTING_SYS_ADCKEY_RES_ENABLE]);
            }
        }
        lradc_key = hal_pmu_read_lradc_inner(adckey_idx);
    }

    //STEP 1: ʹ��LRADC KEY������Ƿ��а������£�������½���adfu
    if ((p_boot_config[SETTING_SYS_ADCKEY_ADFU_ENABLE] == 1) || (p_boot_config[SETTING_SYS_ADCKEY_ADFU_ENABLE] == 3))
    {
        if (lradc_key <= LRADC_0_50_V)
        {
            hal_usb_boot_detect_init();
            welcome_udelay(1000);//�˴���������ʱ
            if(TRUE == hal_usb_is_cable_plug())
            {
                //����ADFU����ģʽ���ýӿڲ��ٷ���
                boot_adfu_launcher();
            }
        }
    }

    
    //STEP 2: �ж��Ƿ�ǿ��ʹ�ܶ�����ģʽ PREV��
    *p_force_enable_udisk = 0;
    if ((p_boot_config[SETTING_SYS_ADCKEY_ADFU_ENABLE] == 2) || (p_boot_config[SETTING_SYS_ADCKEY_ADFU_ENABLE] == 3))
    {
        if ((lradc_key >= LRADC_0_70_V) && (lradc_key <= LRADC_0_88_V))
        {
            *p_force_enable_udisk = 1;
        }
    }

    //STEP 3: ��ص�����⣬����3.3V���ܿ���
#if (BATTERY_VOL_DET_TYPE == BATTERY_VOL_DET_LRADC)
    batdet_idx = hal_pad_lradcx_op(PAD_OP_SET, PAD_FUNC_ANOLOG, NULL, (void*) 0);
    if (batdet_idx >= 0)
    {
        hal_pmu_enable_lradc(batdet_idx, TRUE);
    }
    bat_adc = hal_pmu_read_lradc_inner(batdet_idx);

    if ((bat_adc < LRADC_LOW_POWER) && (bat_adc >= LRADC_1_00_V))//��ѹС��1V��ʾIC��������ع��磬���п��ܵ�ػ��˻�û�Ӻã����Է���������ֵ
    {
        is_low_power = TRUE;
    }
#else
    hal_pmu_enable_batadc(TRUE);
    bat_adc = hal_pmu_read_batadc_inner();
    if (bat_adc < BAT_ADC_LOW_POWER)
    {
        is_low_power = TRUE;
    }
#endif

    //�͵粻�ܿ���
    if (is_low_power == TRUE)
    {
        wakeup_en_para_t temp_wakeup_en_para;

        //�ϵ�ػ����ýӿڲ��ٷ���
        temp_wakeup_en_para.onoff_short_en = 1;
        temp_wakeup_en_para.onoff_long_en = 1;
        temp_wakeup_en_para.onoff_long_time = ONOFF_KEY_LPT_2S;
#if (SUPPORT_WIO_WAKEUP == 1)
        temp_wakeup_en_para.wio0_en = 1;
        temp_wakeup_en_para.wio0_dir = WIO_WAKE_DIR_HIGH;
#endif
        welcome_power_off(&temp_wakeup_en_para);
    }

    //���ó���ONOFF��λʱ�䣬ע��ʱ�䳤�ȿ���������20%��ƫ��
    //time_set:0-8S,1-12S,2-16S,3-24S
    extern void hal_pmu_onoff_reset_time(uint8 time_set);
    hal_pmu_onoff_reset_time(0);
}
