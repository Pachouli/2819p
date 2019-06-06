/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������RTC������ʼ�����˳�����ʼ������IC����������IC��ʼ����
 * ���ߣ�wangli
 ********************************************************************************/

#include "ext_rtc_inner.h"

#if (SUPPORT_EXT_RTC == 1)

uint8 rtc_twi_master_idx;//0��ʾtwi0, 1��ʾtwi1
uint8 rtc_inited_flag = 0;
/******************************************************************************/
/*!
 * \par  Description:
 *    RTC �����ĳ�ʼ��
 * \param[in]   none
 * \param[out]  none
 * \return      0: success;    -1:  fail
 * \note  ������ʼ������
 *******************************************************************************/
int32 rtcdrv_init(void)
{
    twi_hw_info_t twi_hw_info;

    rtc_twi_master_idx = 0;

    //����TWI�ӿ�
    twi_hw_info.scl_pad = FM_TWI_SCL_PIN;
    twi_hw_info.scl_func = FM_TWI_SCL_FUNC;
    twi_hw_info.scl_phy.pad_pu_en = TRUE;
    twi_hw_info.scl_phy.pad_pd_en = FALSE;
    twi_hw_info.scl_phy.pad_driver = 3;
    twi_hw_info.sda_pad = FM_TWI_SDA_PIN;
    twi_hw_info.sda_func = FM_TWI_SDA_FUNC;
    twi_hw_info.sda_in_func = FM_TWI_SDA_IN_FUNC;
    twi_hw_info.sda_phy.pad_pu_en = TRUE;
    twi_hw_info.sda_phy.pad_pd_en = FALSE;
    twi_hw_info.sda_phy.pad_driver = 3;
    twi_hw_info.bitrate_bps = 100*1000;//100kbps
    twi_hw_info.delay_chain_us = 4;//4us

    set_pad_function(FM_TWI_SCL_PIN, FM_TWI_SCL_FUNC, &(twi_hw_info.scl_phy), (void *) 1);
    set_pad_function(FM_TWI_SDA_PIN, FM_TWI_SDA_FUNC, &(twi_hw_info.sda_phy), (void *) 1);

    twi_set_hw_info_sim(rtc_twi_master_idx, &twi_hw_info);

    //����wiox�����͵�ƽ��Ч
    pad_phy_para_t phy_para;
    phy_para.pad_pu_en = 1;
    phy_para.pad_pd_en = 0;
    phy_para.pad_driver = 1;
    set_pad_function(EXT_RTC_INT_PIN, PAD_FUNC_SIOIN, &phy_para, 0);

    rtc_inited_flag = 1;
    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    RTC �������˳�
 * \param[in]   none
 * \param[out]  none
 * \return      0: success;    -1:  fail
 * \note
 *******************************************************************************/
int32 rtcdrv_exit(void)
{
    return 0;
}

#endif
