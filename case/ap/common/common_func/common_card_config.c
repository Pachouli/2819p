/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：卡硬件参数配置。
 * 作者：cailizhen
 ********************************************************************************/

#include  "common_func_inner.h"
#include  "card.h"

#define CARD_PADDRV_LEVEL_MAX   7

static void get_card_pm_param(card_pm_cfg_t *p_card_pm_param)
{
    pad_phy_para_t phy_para;
    
    p_card_pm_param->cmd_drv_level = (uint8) com_get_config_default_inner(SETTING_CARD_CMD_PAD_LEVEL);

    if (p_card_pm_param->cmd_drv_level > CARD_PADDRV_LEVEL_MAX)
    {
        p_card_pm_param->cmd_drv_level = CARD_PADDRV_LEVEL_MAX;
    }

    p_card_pm_param->clk_drv_level = (uint8) com_get_config_default_inner(SETTING_CARD_CLK_PAD_LEVEL);

    if (p_card_pm_param->clk_drv_level > CARD_PADDRV_LEVEL_MAX)
    {
        p_card_pm_param->clk_drv_level = CARD_PADDRV_LEVEL_MAX;
    }

    p_card_pm_param->data_drv_level = (uint8) com_get_config_default_inner(SETTING_CARD_DAT_PAD_LEVEL);

    if (p_card_pm_param->data_drv_level > CARD_PADDRV_LEVEL_MAX)
    {
        p_card_pm_param->data_drv_level = CARD_PADDRV_LEVEL_MAX;
    }

    p_card_pm_param->card_para = 0x07;
    p_card_pm_param->drv_mode = 0;

    phy_para.pad_pu_en = 1;
    phy_para.pad_pd_en = 0;
    phy_para.smit_en = 0;
    phy_para.pad_driver = p_card_pm_param->cmd_drv_level;
    set_pad_function(SDCARD_CMD_PIN, SDCARD_CMD_FUNC, &phy_para, 0);

    phy_para.pad_pu_en = 0;
    phy_para.pad_pd_en = 0;
    phy_para.smit_en = 0;
    phy_para.pad_driver = p_card_pm_param->clk_drv_level;
    set_pad_function(SDCARD_CLK_PIN, SDCARD_CLK_FUNC, &phy_para, 0);

    phy_para.pad_pu_en = 1;
    phy_para.pad_pd_en = 0;
    phy_para.smit_en = 0;
    phy_para.pad_driver = p_card_pm_param->data_drv_level;
    set_pad_function(SDCARD_DAT0_PIN, SDCARD_DAT0_FUNC, &phy_para, 0);
}

void common_card_config(void)
{
    card_pm_cfg_t card_pm_param;
    uint8 card_power_cfg = 0;

    //用于SD卡电源控制端口配置
    card_power_cfg = CARD_POWER_CFG;
    hardware_set(HAL_SET_CARD_PW_CTRL_CFG, (void*) (uint32) card_power_cfg);

    get_card_pm_param(&card_pm_param);
    hal_set_card_pm_param(&card_pm_param);
    sys_set_system_info(SET_CARD_MCUPLL_USE, 0);
}
