/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙免提通话前台应用LED灯显示处理。
 * 作者：cailizhen
 ********************************************************************************/

#include  "ap_bluetooth.h"

void bt_ui_display(void)
{
    switch (g_disp_status)
    {
        case STATUS_NONE:
        led_display(NUMBER1, 0, FALSE);
        led_display(NUMBER2, 'b', TRUE);
        led_display(NUMBER3, 'L', TRUE);
        led_display(NUMBER4, 0, FALSE);
        libc_printf_info("STATUS_NONE\n");
        break;

        case STATUS_WAIT_PAIRED:
        led_display(NUMBER1, 0, FALSE);
        led_display(NUMBER2, 'b', TRUE);
        led_display(NUMBER3, 'L', TRUE);
        led_display(NUMBER4, 0, FALSE);
        libc_printf_info("STATUS_WAIT_PAIRED\n");
        break;

        case STATUS_LINKED:
        libc_printf_info("STATUS_LINKED\n");
        break;

        case STATUS_A2DP_PLAY:
        libc_printf_info("STATUS_A2DP_PLAY\n");
        break;

        case STATUS_A2DP_PAUSE:
        libc_printf_info("STATUS_A2DP_PAUSE\n");
        break;

        case STATUS_CALL_INCOMING:
        led_display(NUMBER1, 'C', TRUE);
        led_display(NUMBER3, 'I', TRUE);
        led_display(NUMBER4, 'N', TRUE);
        libc_printf_info("STATUS_CALL_INCOMING\n");
        break;

        case STATUS_CALL_OUTGOING:
        led_display(NUMBER1, 'C', TRUE);
        led_display(NUMBER3, 'O', TRUE);
        led_display(NUMBER4, 'U', TRUE);
        libc_printf_info("STATUS_CALL_OUTGOING\n");
        break;

        case STATUS_CALL_HFP:
        led_display(NUMBER1, 'C', TRUE);
        led_display(NUMBER3, 'H', TRUE);
        led_display(NUMBER4, 'F', TRUE);
        libc_printf_info("STATUS_CALL_HFP\n");
        break;

        case STATUS_CALL_PHONE:
        led_display(NUMBER1, 'C', TRUE);
        led_display(NUMBER3, 'A', TRUE);
        led_display(NUMBER4, 'G', TRUE);
        libc_printf_info("STATUS_CALL_PHONE\n");
        break;

        case STATUS_SIRI:
        led_display(NUMBER1, 'S', TRUE);
        led_display(NUMBER2, 'I', TRUE);
        led_display(NUMBER3, 'R', TRUE);
        led_display(NUMBER4, 'I', TRUE);
        libc_printf_info("STATUS_SIRI\n");
        break;

        default:
        break;
    }
}

