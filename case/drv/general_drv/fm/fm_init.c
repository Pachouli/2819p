/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������FM������ʼ�����˳�����ʼ������IC����������IC��ʼ����
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "fm_drv.h"

#if (SUPPORT_FM_FUNC == 1)

uint8 twi_master_idx;//0��ʾtwi0, 1��ʾtwi1
uint8 fm_clk_source_sel;//0��ʾƬ��ʱ��Դ��1��ʾƬ��ʱ��Դ
uint32 fm_clk_hz;//��λΪHz��֧��32.768KHz��ֻ֧����Ҿ��񣩡�6.5MHz��13MHz��24MHz��26MHz

/******************************************************************************/
/*!
 * \par  Description:
 *    FM �����ĳ�ʼ��
 * \param[in]   none
 * \param[out]  none
 * \return      0: success;    -1:  fail
 * \note  ������ʼ������
 *******************************************************************************/
int32 fmdrv_init(uint32 para)
{
    FM_install_Para_t *p_ins_para = (FM_install_Para_t *) para;

    fm_clk_source_sel = p_ins_para->clk_src;
    fm_clk_hz = p_ins_para->clk_hz;

    if (fm_clk_source_sel == FM_CLK_SRC_INNER)
    {//�ڲ�ʱ��Դ GPIO5 ������� GPIO5 ����Ϊ FMCLKOUT
        uint8 fmclksel_val = FMCLKSEL_HOSC26M;

        if (fm_clk_hz == 24000000)
        {
            fmclksel_val = FMCLKSEL_CK24M;
        }
        else if (fm_clk_hz == 26000000)
        {
            fmclksel_val = FMCLKSEL_HOSC26M;
        }
        else if (fm_clk_hz == 6500000)
        {
            fmclksel_val = FMCLKSEL_CORED10;
            //TODO COREPLL��Ƶ
        }
        else if (fm_clk_hz == 13000000)
        {
            fmclksel_val = FMCLKSEL_CORED10;
            //TODO COREPLL��Ƶ
        }
        else
        {
            libc_printf_error("NO support FM CLK:%d\n", fm_clk_hz);
            while (1);
        }

        set_pad_function(FM_FMCLKOUT_PIN, FM_FMCLKOUT_FUNC, NULL, 0);
        hal_set_fm_clock(fmclksel_val);
    }

    //����TWI�ӿ�
#if (FM_TWI_MASTER_MODE == TWI_MASTER_SIM)
    {
        twi_hw_info_t twi_hw_info;

        twi_master_idx = 0;

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

        twi_set_hw_info_sim(twi_master_idx, &twi_hw_info);
    }
#else
    {
        twi_master_hw_info_t twi_master_hw_info;
        pad_phy_para_t phy_para;

        phy_para.pad_pu_en = 2;
        phy_para.pad_pd_en = 0;
        phy_para.smit_en = 0;
        phy_para.pad_driver = 3;
        twi_master_idx = set_pad_function(FM_TWI_SCL_PIN, FM_TWI_SCL_FUNC, &phy_para, 0);
        set_pad_function(FM_TWI_SDA_PIN, FM_TWI_SDA_FUNC, &phy_para, 0);

        twi_master_hw_info.bitrate_bps = 100*1000;//100kbps
        twi_master_set_hw_info(twi_master_idx, &twi_master_hw_info);
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
int32 fmdrv_exit(void)
{
    //�ر�Ƭ��ʱ��Դ
    if (fm_clk_source_sel == FM_CLK_SRC_INNER)
    {
        hal_disable_fm_clock();
    }

    return 0;
}

#endif
