#include "hal_includes_inner.h"
#include "hal_datatype_inner.h"

#define UART_52M_DIV        (0x01C301C3)
#define UART_26M_DIV        (0x00E200E2)
#define UART_24M_DIV        (0x00D000D0)

static void _Short_Delay(void)
{
    UINT32 i;

    for (i = 0; i < 10; i++)
    {
        //just for delay
    }
}

void boot_print(UINT8 *string, UINT32 data)
{
    UINT8 *pDat;
    UINT32 index;
    UINT8 dat, t1, t2;

    UINT32 uart_ctl_s;

    act_writel(act_readl(MRCR) & ~(1 << MRCR_UART1_RESET), MRCR);
    _Short_Delay();
    _Short_Delay();
    act_writel(act_readl(MRCR) | (1 << MRCR_UART1_RESET), MRCR);

    //UART clock enable
    act_writel(act_readl(CMU_DEVCLKEN) | (1 << CMU_DEVCLKEN_UART1CLKEN), CMU_DEVCLKEN);

    _Short_Delay();
    _Short_Delay();

    act_writel(0x01, CMU_UARTCLK);
    _Short_Delay();
    _Short_Delay();
    act_writel((act_readl(GPIO8_CTL) & ~(GPIO8_CTL_MFP_MASK)) | (3 << GPIO8_CTL_PADDRV_SHIFT), GPIO8_CTL);

    act_writel(UART_26M_DIV, UART1_BR);
    _Short_Delay();
    _Short_Delay();
    uart_ctl_s = act_readl(UART1_CTL);

    //enable UART TX & RX, AHB bus access, 8bit data width, none parity
    act_writel(0x40808003, UART1_CTL);//bit30 和 bit31 = 1表示使能，与spec刚好相反。

    pDat = string;
    while (*pDat != 0)
    {
        while (0 != (act_readl(UART1_STA) & 0x00000040))
        {
            //wait until UART tx FIFO NOT full
        }
        act_writel(*pDat, UART1_TXDAT);
        pDat++;
    }

    while (0 != (act_readl(UART1_STA) & 0x00200000))
    {
        //wait until UART tx FIFO empty
    }

    pDat = (UINT8*) (&data);
    pDat += 3;

    for (index = 0; index < 4; index++)
    {
        dat = *pDat;
        pDat--;

        t1 = dat & 0x0f;
        t2 = (dat & (UINT8) 0xf0) >> 4;
        if (t2 < 10)
        {
            act_writel(t2 + '0', UART1_TXDAT);
        }
        else
        {
            act_writel(t2 - 10 + 'a', UART1_TXDAT);
        }
        if (t1 < 10)
        {
            act_writel(t1 + '0', UART1_TXDAT);
        }
        else
        {
            act_writel(t1 - 10 + 'a', UART1_TXDAT);
        }
    }

    act_writel('\n', UART1_TXDAT);
    while (0 != (act_readl(UART1_STA) & 0x00200000))
    {
        //wait until UART tx FIFO empty
    }
}

