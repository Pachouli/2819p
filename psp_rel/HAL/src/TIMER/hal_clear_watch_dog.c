
#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

//clear watch dog
void hal_clear_watch_dog(void)
{
    act_writel(((1<<WD_CTL_CLR) | act_readl(WD_CTL)), WD_CTL);
}

//enable watch dog
void hal_enable_watch_dog(uint8 wd_period)
{
    act_writel((~(1<<WD_CTL_WDEN) & act_readl(WD_CTL)), WD_CTL);
    act_writel((~WD_CTL_CLKSEL_MASK & act_readl(WD_CTL)), WD_CTL);
    act_writel((((wd_period<<WD_CTL_CLKSEL_SHIFT)&WD_CTL_CLKSEL_MASK) | act_readl(WD_CTL)), WD_CTL);
    act_writel(((1<<WD_CTL_WDEN) | act_readl(WD_CTL)), WD_CTL);
}

//disable watch dog
void hal_disable_watch_dog(void)
{
    act_writel((~(1<<WD_CTL_WDEN) & act_readl(WD_CTL)), WD_CTL);
}
