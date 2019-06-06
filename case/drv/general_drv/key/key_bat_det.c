/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������KEY������ص�����⣬���ö�β���ȡƽ��ֵ���ص�ص�����ͳ�ƴ���Ϊ500ms��
 * ���ߣ�cailizhen
 ********************************************************************************/

#define USE_BATTERY_GRADE_VEL

#include "key_inner.h"

//��ص������
#define BAT_ADC_DATA_NUM      25 //��������25�Σ���0.5S
uint16 g_bat_adc_val[BAT_ADC_DATA_NUM];
uint8 g_bat_adc_val_index = 0;
uint16 cur_battery_val = 0;
ic_pad_e g_batdet_lradc_pin;
int8 batdet_idx;

/******************************************************************************
 * \par  Description:   ��ȡ��س��״̬
 * \param[in]     null
 * \param[out]   ad_val  ���ζ�ȡ�ĵ�ز���adcֵ
 * \param[out]   vol_limit  ��ѹ���ͺ���Ҫ����������������������ʣ�����ᱻ��������λΪ db
 * \return           null
 * \note
 *   <author>    <time>           <version >             <desc>
 *    Wekan   2015-3-26am         1.0             review this
 *******************************************************************************/
uint8 key_inner_get_battery(void)
{
    uint32 bat_val;
    int32 i;

    bat_val = 0;
    for (i = 0; i < BAT_ADC_DATA_NUM; i++)
    {
        bat_val += g_bat_adc_val[i];
    }

    cur_battery_val = (uint16)(bat_val / BAT_ADC_DATA_NUM);

#if (BATTERY_VOL_DET_TYPE == BATTERY_VOL_DET_BATADC)
    if (cur_battery_val >= BAT_ADC_FULL)
    {
        return BATTERY_GRADE_MAX + 1;
    }

    for (i = BATTERY_GRADE_MAX; i >= 0; i--)
    {
        if (cur_battery_val >= battery_grade_vel[i])
        {
            return i;
        }
    }
#else
    if (cur_battery_val >= LRADC_FULL)
    {
        return BATTERY_GRADE_MAX + 1;
    }

    for (i = BATTERY_GRADE_MAX; i >= 0; i--)
    {
        if (cur_battery_val >= battery_grade_vel_lradc[i])
        {
            return i;
        }
    }
#endif

    return 0;
}

/******************************************************************************
 * \par  Description:  ��ص�ѹ����
 * \param[in]    void
 * \param[out]   null
 * \return           null
 * \note
 *   <author>    <time>
 *    Wekan   2015-3-26am
 *******************************************************************************/
void __section__(".rcode") key_sampling_battery_voltage(void)
{
    uint16 bat_adc;

#if (BATTERY_VOL_DET_TYPE == BATTERY_VOL_DET_BATADC)
    //batadc for bat vol detect
    bat_adc = hal_pmu_read_batadc();
#else
    //LRADC1/2 for bat vol detect
    bat_adc = hal_pmu_read_lradc(batdet_idx);
    if (bat_adc < LRADC_1_00_V)//��ʾIC��������ع��磬���п��ܵ�ػ��˻�û�Ӻã����Է���������ֵ
    {
        bat_adc = LRADC_3_00_V;
    }
#endif

    if (g_bat_adc_val_index >= BAT_ADC_DATA_NUM)
    {
        g_bat_adc_val_index = 0;
    }

    g_bat_adc_val[g_bat_adc_val_index] = bat_adc;
    g_bat_adc_val_index++;
}

/******************************************************************************
 * \par  Description:  ��ص�ѹ������ʼ��
 * \param[in]    void
 * \param[out]   null
 * \return           null
 * \note
 *   <author>    <time>
 *    Wekan   2015-3-26am
 *******************************************************************************/
void key_sampling_battery_voltage_init(void)
{
    uint16 bat_adc;
    uint8 i;

#if (BATTERY_VOL_DET_TYPE == BATTERY_VOL_DET_BATADC)
    //batadc for bat vol detect
    bat_adc = hal_pmu_read_batadc();
#else
    //LRADC1/2 for bat vol detect
    bat_adc = hal_pmu_read_lradc(batdet_idx);
    if (bat_adc < LRADC_1_00_V)//��ʾIC��������ع��磬���п��ܵ�ػ��˻�û�Ӻã����Է���������ֵ
    {
        bat_adc = LRADC_3_00_V;
    }
#endif

    for (i = 0; i < BAT_ADC_DATA_NUM; i++)
    {
        g_bat_adc_val[i] = bat_adc;
    }

    g_bat_adc_val_index = 0;
    cur_battery_val = bat_adc;
}

void key_inner_bat_det_init(void)
{
    //batadc for bat vol detect
#if (BATTERY_VOL_DET_TYPE == BATTERY_VOL_DET_BATADC)
    pmu_enable_batadc(TRUE);
    key_sampling_battery_voltage_init();
#else
    g_batdet_lradc_pin = PAD_KEY_LRADCX;

    //LRADC1/2 for bat vol detect
    batdet_idx = set_pad_function(g_batdet_lradc_pin, PAD_FUNC_ANOLOG, NULL, 0);
    if (batdet_idx >= 0)
    {
        pmu_enable_lradc(batdet_idx, TRUE);
    }

    key_sampling_battery_voltage_init();
#endif
}
