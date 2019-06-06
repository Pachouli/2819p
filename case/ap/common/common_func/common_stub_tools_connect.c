/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：STUB连接接口实现，支持ASET、ASQT、PRINT、ATT等工具。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_aset_inner.h"

static uint8 pc_connect_probe(void);

#define SERIAL_NUMBER_MAX 5
const uint16 c_adc_val[SERIAL_NUMBER_MAX + 1] =
{
    56,
    160,
    256,
    352,
    464,
    576,
};

app_result_e com_stub_tools_connect_inner(void)
{
    app_result_e result = RESULT_NULL;
    uint8 stub_tools_type;
    uint8 stub_serial_number = 0;

    //在cfgdbg.txt中配置是否只支持一台小机自动化测试
    if (com_get_config_default_inner(SETTING_AUTOTEST_ONLY_ONE_CHIP) == 1)
    {
        libc_printf("ONLY_ONE_CHIP!\n");
    }
    else
    {
        uint16 adcdat,i;
        int8 adc_idx;

        adc_idx = set_pad_function(PAD_GPIO4, PAD_FUNC_ANOLOG, NULL, 0);
        pmu_enable_lradc(adc_idx, TRUE);

        adcdat = hal_pmu_read_lradc(adc_idx);

        for (i = 0; i < (SERIAL_NUMBER_MAX+1); i++)
        {
            if (adcdat < c_adc_val[i])
            {
                stub_serial_number = i;
                break;
            }
        }
        libc_printf("MORE THAN 2 CHIP [adc=%d,sn=%d]\n", adcdat, stub_serial_number);
    }

    sys_drv_install(DRV_GROUP_STUB, stub_serial_number, "stub.drv");

    while (0 != stub_open(0))
    {
        //wait for stub device setup complete
    }

    //等待PC STUB工具连接；更新本地和全局状态
    stub_tools_type = pc_connect_probe();
    com_set_stub_tools_type_inner(stub_tools_type);

    if (stub_tools_type == STUB_PC_TOOL_ASET_EQ_MODE)
    {
#if (SUPPORT_ASET_TEST == 1)
        com_stub_ack_ext_cmd_inner(STUB_CMD_ASET_ACK);
        aset_test_global_init();
#endif
    }
    else if (stub_tools_type == STUB_PC_TOOL_ASQT_MODE)
    {
#if (SUPPORT_ASQT_TEST == 1)
        //ASQT工具不用回复ACK
        result = RESULT_ENTER_ASQT_TEST;
#endif
    }
#if (1 == SUPPORT_ATT_TEST)
    else if (stub_tools_type == STUB_PC_TOOL_ATT_MODE)
    {
        com_stub_ack_ext_cmd_inner((STUB_PC_TOOL_ATT_MODE<<8)|STUB_CMD_ACK);
        result = RESULT_TESTAP_ATD_ENTER;
    }
#endif
    else if (stub_tools_type == STUB_PC_TOOL_PRINT_MODE)
    {
        com_stub_ack_ext_cmd_inner((STUB_PC_TOOL_PRINT_MODE<<8)|STUB_CMD_ACK);
        //切换到stub打印模式
        stub_switch_print_mode(1);
    }
    else
    {
        ; //nothing
    }

    return result;
}

void com_stub_tools_disconnect(void)
{
    uint8 stub_tools_type = com_get_stub_tools_type_inner();
    bool aset_tools_type = FALSE;

    if (stub_tools_type != 0)
    {
        if (stub_tools_type == STUB_PC_TOOL_PRINT_MODE)
        {
            //切换到uart打印模式
            stub_switch_print_mode(0);
        }
        else if (stub_tools_type == STUB_PC_TOOL_ASET_EQ_MODE)
        {
            aset_tools_type = TRUE;
        }

        //如果USB线断开连接，则卸载stub资源
        libc_printf_info("stub disconnect!\n");
        stub_close();
        sys_drv_uninstall (DRV_GROUP_STUB);

        //更新全局状态
        com_set_stub_tools_type_inner(0);

        if (aset_tools_type == TRUE)
        {
            aset_test_global_exit();
        }
    }
}

static uint8 pc_connect_probe(void)
{
    uint32 pc_type = 0;
    int8 ret;

    libc_printf_info("wait stub tools connect!\n");

    while (1)
    {
        ret = (int8) stub_get_data(STUB_CMD_OPEN, &pc_type, 4);

        if (ret == 0)
        {
            break;
        }

        sys_os_time_dly(5);

        libc_printf("*");
    }

    libc_printf_info("stub tool connect:%d\n", (uint8) pc_type);

    return (uint8) pc_type;
}

