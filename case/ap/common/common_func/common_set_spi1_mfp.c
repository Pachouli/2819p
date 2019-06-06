/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：设置SPI1功能
 * 作者：cailizhen
 ********************************************************************************/

#include "common_func_inner.h"

void common_spi1_set_mfp(void)
{
    pad_phy_para_t phy_para;

    phy_para.pad_pu_en = 0;
    phy_para.pad_pd_en = 0;
    phy_para.smit_en = 1;
    phy_para.pad_driver = 3;
    set_pad_function(SPI1_MOSI_PIN, SPI1_MOSI_FUNC, &phy_para, 0);

    phy_para.pad_pu_en = 0;
    phy_para.pad_pd_en = 0;
    phy_para.smit_en = 1;
    phy_para.pad_driver = 3;
    set_pad_function(SPI1_MISO_PIN, SPI1_MISO_FUNC, &phy_para, 0);

    phy_para.pad_pu_en = 0;
    phy_para.pad_pd_en = 0;
    phy_para.smit_en = 1;
    phy_para.pad_driver = 3;
    set_pad_function(SPI1_1ST_SS_PIN, SPI1_1ST_SS_FUNC, &phy_para, 0);

    phy_para.pad_pu_en = 0;
    phy_para.pad_pd_en = 0;
    phy_para.smit_en = 1;
    phy_para.pad_driver = 3;
    set_pad_function(SPI1_CLK_PIN, SPI1_CLK_FUNC, &phy_para, 0);

#if (SUPPORT_2ND_SS_PIN == 1)
    phy_para.pad_pu_en = 0;
    phy_para.pad_pd_en = 0;
    phy_para.smit_en = 1;
    phy_para.pad_driver = 3;
    set_pad_function(SPI1_2ND_SS_PIN, SPI1_2ND_SS_FUNC, &phy_para, 0);

    set_pad_function(SPI1_1ST_SS_PIN, PAD_FUNC_GPIOOUT, NULL, 1);
#endif 

    spi1_phy_init(NULL, 10 * 1000); //10MHZ
}
