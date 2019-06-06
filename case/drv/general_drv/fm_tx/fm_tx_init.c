/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������FM����������ʼ�����˳�����ʼ������IC����������IC��ʼ����
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "fm_tx_drv.h"

#if (SUPPORT_FM_TX == 1)

uint8 fm_tx_twi_master_idx;//0��ʾtwi0, 1��ʾtwi1

/******************************************************************************/
/*!
 * \par  Description:
 *    FM �����ĳ�ʼ��
 * \param[in]   none
 * \param[out]  none
 * \return      0: success;    -1:  fail
 * \note  ������ʼ������
 *******************************************************************************/
int32 fm_tx_drv_init(void)
{
    set_pad_function(FM_FMCLKOUT_PIN, FM_FMCLKOUT_FUNC, NULL, 0);
    hal_set_fm_clock(FMCLKSEL_CK24M);

    //����TWI�ӿ�
#if (FM_TWI_MASTER_MODE == TWI_MASTER_SIM)
    {
        twi_hw_info_t twi_hw_info;

        fm_tx_twi_master_idx = 0;

        twi_hw_info.scl_pad = FM_TWI_SCL_PIN;
        twi_hw_info.scl_func = FM_TWI_SCL_FUNC;
        twi_hw_info.scl_phy.pad_pu_en = 2;
        twi_hw_info.scl_phy.pad_pd_en = 0;
        twi_hw_info.scl_phy.smit_en = 0;
        twi_hw_info.scl_phy.pad_driver = 3;
        twi_hw_info.sda_pad = FM_TWI_SDA_PIN;
        twi_hw_info.sda_func = FM_TWI_SDA_FUNC;
        twi_hw_info.sda_in_func = FM_TWI_SDA_IN_FUNC;
        twi_hw_info.sda_phy.pad_pu_en = 2;
        twi_hw_info.sda_phy.pad_pd_en = 0;
        twi_hw_info.sda_phy.smit_en = 0;
        twi_hw_info.sda_phy.pad_driver = 3;
        twi_hw_info.bitrate_bps = 100*1000;//100kbps
        twi_hw_info.delay_chain_us = 4;//4us

        set_pad_function(FM_TWI_SCL_PIN, FM_TWI_SCL_FUNC, &(twi_hw_info.scl_phy), (void *) 1);
        set_pad_function(FM_TWI_SDA_PIN, FM_TWI_SDA_FUNC, &(twi_hw_info.sda_phy), (void *) 1);

        twi_set_hw_info_sim(fm_tx_twi_master_idx, &twi_hw_info);
    }
#else
    {
        twi_master_hw_info_t twi_master_hw_info;
        pad_phy_para_t phy_para;

        phy_para.pad_pu_en = 2;
        phy_para.pad_pd_en = 0;
        phy_para.smit_en = 0;
        phy_para.pad_driver = 3;
        fm_tx_twi_master_idx = set_pad_function(FM_TWI_SCL_PIN, FM_TWI_SCL_FUNC, &phy_para, 0);
        set_pad_function(FM_TWI_SDA_PIN, FM_TWI_SDA_FUNC, &phy_para, 0);

        twi_master_hw_info.bitrate_bps = 100*1000;//100kbps
        twi_master_set_hw_info(fm_tx_twi_master_idx, &twi_master_hw_info);
    }
#endif

    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    FM �������˳�
 * \param[in]   none
 * \param[out]  none
 * \return      0: success;    -1:  fail
 * \note
 *******************************************************************************/
int32 fm_tx_drv_exit(void)
{
    //�ر�Ƭ��ʱ��Դ
    hal_disable_fm_clock();

    return 0;
}

#endif
