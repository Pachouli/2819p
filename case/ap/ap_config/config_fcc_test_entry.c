
#include  "ap_config.h"

#ifdef FCC_TEST_MODE
void config_fcc_test_entry(void)
{
    libc_printf("FCC TEST ENTER...\n");
    sys_os_time_dly(5);

    //����PIN�� GPIO5 -> Rx, GPIO6 -> Tx
    set_pad_function(PAD_GPIO5, GPIO5_MFP_UART0_RX, NULL, 0);
    uart_rx_set(0, 115200, UART_CLK_AHB);
    set_pad_function(PAD_GPIO6, GPIO6_MFP_UART0_TX, NULL, 0);
    uart_tx_set(0, 115200, UART_CLK_AHB);

    //װ��FCC����������Init�ӿڼ�����FCC����
    sys_adjust_clk(FREQ_54M, SET_SYS_CLK);
    sys_drv_install(DRV_GROUP_BT_CON, 0, "fcc.drv"); //0��ʾUart0���������ΪUart1����ô��Ҫ����1��fcc����
}
#endif
