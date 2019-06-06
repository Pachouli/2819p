
#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

//ejtag_sel: EJ_TCK/EJ_TDO/EJ_TMS/EJ_TDI/EJ_TRST
//0表示EJTAG Group1，复用GPIO32/GPIO31/GPIO33/GPIO34/GPIO35
//1表示EJTAG Group2，复用GPIO0/GPIO1/GPIO2/GPIO10/GPIO11
//2表示EJTAG Group3，复用GPIO3/GPIO4/GPIO5/GPIO6/GPIO7
//3表示EJTAG Group4，复用GPIO21/GPIO22/GPIO23/GPIO24/GPIO25  --  方案DVB板选择这一组
//0xff表示关闭EJTAG
void hal_debug_set_ejtag(uint8 ejtag_sel)
{
    if (ejtag_sel > 3)
    {
        act_writel((act_readl(JTAG_CTL) & ~(1<<JTAG_CTL_EJTAGEN)), JTAG_CTL);
    }
    else
    {
        act_writel((act_readl(JTAG_CTL) & (~JTAG_CTL_EJTAGMAP_MASK)) | (ejtag_sel<<JTAG_CTL_EJTAGMAP_SHIFT) | (1<<JTAG_CTL_EJTAGEN), JTAG_CTL);
        if (ejtag_sel == 3)
        {
            //与LED冲突，关掉LED控制器
            act_writel(act_readl(CMU_DEVCLKEN)&(~(0x1<<CMU_DEVCLKEN_SEGLCDCLKEN)), CMU_DEVCLKEN);
            act_writel(act_readl(MRCR)&(~(0x1<<MRCR_SEG_LCD_LED_RESET)), MRCR);
        }
    }
}

//btdb_port: 对应DB PIN，比如0xf8表示DB3/DB4/DB5/DB6/DB7，DB即对应GPIO
void hal_debug_set_btdebug(uint32 btdb_port)
{
    act_writel(act_readl(DEBUGOE0) | (btdb_port<<0), DEBUGOE0);
}
