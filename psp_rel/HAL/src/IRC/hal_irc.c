
#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

void irc_interrupt_process(void);

__irc_detect_key_cbk irc_detect_key_cbk;
__irc_repeat_key_cbk irc_repeat_key_cbk;

void hal_irc_phy_init(uint8 irc_mode, uint32 usercode, __irc_detect_key_cbk detect_key_cbk, __irc_repeat_key_cbk repeat_key_cbk)
{
    *((REG32)(MRCR)) &= ~(1<<MRCR_IRC_RESET);
    *((REG32)(MRCR)) |= (1<<MRCR_IRC_RESET);
    *((REG32)(CMU_DEVCLKEN)) |= (1<<CMU_DEVCLKEN_IRCLKEN);

    irc_detect_key_cbk = detect_key_cbk;
    irc_repeat_key_cbk = repeat_key_cbk;
    g_p_sys_hal_cbks->hal_request_irq(IRQ_IRC, irc_interrupt_process);

    //clear error pendings and irq pending and User code don¡®t match pending bit and
    //Key data code don¡®t match pending bit
    act_writel((act_readl(IRC_STA) | 0x75), IRC_STA);

    //init user code
    act_writel(usercode, IRC_CC);

    act_writel((act_readl(IRC_CTL) | (1<<IRC_CTL_DBB_EN)), IRC_CTL);
    act_writel((act_readl(IRC_CTL) & (~IRC_CTL_ICMS_MASK)), IRC_CTL);
    act_writel((act_readl(IRC_CTL) | (irc_mode << IRC_CTL_ICMS_SHIFT)), IRC_CTL);

    //IRC enable;
    act_writel((act_readl(IRC_CTL) | (1 << IRC_CTL_IRE)), IRC_CTL);
    //IRC IRQ enable;
    act_writel((act_readl(IRC_CTL) | (1 << IRC_CTL_IIE)), IRC_CTL);
}

void hal_irc_phy_exit(void)
{
    g_p_sys_hal_cbks->hal_free_irq (IRQ_IRC);
}

int32 hal_irc_correct_usercode(uint32 *usercode)
{
    if ((act_readl(IRC_STA) & (1 << IRC_STA_UCMP)) != 0)
    {
        uint32 tmp;

        tmp = act_readl(IRC_CC)>>16;
        act_writel(tmp, IRC_CC);
        act_writel((act_readl(IRC_STA) | (1 << IRC_STA_UCMP)), IRC_STA);

        *usercode = tmp;
        return -1;
    }
    else
    {
        return 0;
    }
}

void irc_interrupt_process(void)
{
    uint8 key_data;

    if ((act_readl(IRC_STA)&(1<<IRC_STA_IIP)) == 0) //NOT IRC IRQ
    {
        return;
    }

    //IRC IRQ, clear irq pending
    act_writel((1<<IRC_STA_IIP), IRC_STA);

    //repeat code detcet
    if ((act_readl(IRC_STA) & (1 << IRC_STA_RCD)) != 0)
    {
        //write 1 to this bit to clear repeat flag
        act_writel((1 << IRC_STA_RCD), IRC_STA);

        irc_repeat_key_cbk();

        return;
    }

    key_data = act_readb(IRC_KDC); //read key data value
    irc_detect_key_cbk(key_data);
}

