/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：USB硬件参数配置。
 * 作者：cailizhen
 ********************************************************************************/

#include  "common_func_inner.h"

void common_usb_config(void)
{
    pad_phy_para_t phy_para;

    phy_para.pad_pu_en = 0;
    phy_para.pad_pd_en = 0;
    phy_para.smit_en = 0;
    phy_para.pad_driver = 4;
    set_pad_function(USB_DM_PIN, USB_DM_FUNC, &phy_para, 0);

    phy_para.pad_pu_en = 0;
    phy_para.pad_pd_en = 0;
    phy_para.smit_en = 0;
    phy_para.pad_driver = 4;
    set_pad_function(USB_DP_PIN, USB_DP_FUNC, &phy_para, 0);
}
