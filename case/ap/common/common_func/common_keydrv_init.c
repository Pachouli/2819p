/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：KEY驱动安装，初始化按键映射表等。
 * 作者：cailizhen
 ********************************************************************************/

#include  "common_func_inner.h"

typedef struct
{
    uint16 onoff_key; //IC ONOFF PIN 按键值，如果没有使用ONOFF PIN，可以写0
    uint16 total; //按键总数
    uint16 keyID[KEY_IR_NUM_MAX]; //按键定义数组
} cfg_key_var_t;

void common_keydrv_init(void)
{
    keymap_para_t keymap_para;
    irmap_para_t irmap_para;
    cfg_key_var_t g_cfg_key;
    uint8 i;

    com_get_config_struct_inner(SETTING_INPUT_KEY_ARRAY, (uint8 *) &g_cfg_key, sizeof(g_cfg_key));

    if (g_cfg_key.total > KEY_NUM_MAX)
    {
        keymap_para.key_num = KEY_NUM_MAX;
    }
    else
    {
        keymap_para.key_num = (uint8)(g_cfg_key.total);
    }

    for (i = 0; i < keymap_para.key_num; i++)
    {
        keymap_para.key_val[i] = (uint8)(g_cfg_key.keyID[i]);
    }

    keymap_para.key_onoff = (uint8)(g_cfg_key.onoff_key);

    com_get_config_struct_inner(SETTING_INPUT_KEY_ADC_ARRAY, (uint8 *) &g_cfg_key, sizeof(g_cfg_key));
    for (i = 0; i < keymap_para.key_num; i++)
    {
        keymap_para.key_adc_val[i] = g_cfg_key.keyID[i];
    }

    keymap_para.uart_sim_key_support = (uint8) com_get_config_default_inner(SETTING_UART_SIM_KEY_SUPPORT);
    keymap_para.lradc_res_enable = (uint8) com_get_config_default_inner(SETTING_SYS_ADCKEY_RES_ENABLE);
    keymap_para.lradc_pin = PAD_KEY_LRADC1;

    //U盘播歌支持、USB音箱支持、大容量磁盘支持
    if ((g_com_comval.support_uhostplay == 1)
        || (g_com_comval.support_usbbox == 1)
        || (g_com_comval.support_usbmsc == 1))
    {
        keymap_para.need_usb_detect = TRUE;
    }
    else
    {
        keymap_para.need_usb_detect = FALSE;
    }
    keymap_para.vcc_voltage_select = (uint8) com_get_config_default_inner(SETTING_SYS_VCC_VOLTAGE_SELECT);

    //安装按键驱动
    if ((int32) sys_drv_install(DRV_GROUP_KEY, (uint32) &keymap_para, "general.drv") == -1)
    {
        while (1);
    }

#if (SUPPORT_IR_KEY == 1)
    if (g_com_comval.support_ir_key == 1)
    {
        irmap_para.ir_protocol = (uint8) com_get_config_default_inner(SETTING_IR_KEY_PROTOCOL);
        irmap_para.ir_usercode = com_get_config_default_inner(SETTING_IR_KEY_USERCODE);

        com_get_config_struct_inner(SETTING_IR_KEY_ARRAY, (uint8 *) &g_cfg_key, sizeof(g_cfg_key));

        if (g_cfg_key.total > KEY_IR_NUM_MAX)
        {
            irmap_para.ir_key_num = KEY_IR_NUM_MAX;
        }
        else
        {
            irmap_para.ir_key_num = (uint8)(g_cfg_key.total);
        }

        for (i = 0; i < irmap_para.ir_key_num; i++)
        {
            irmap_para.ir_key_val[i]= (uint8)(g_cfg_key.keyID[i]);
        }

        com_get_config_struct_inner(SETTING_IR_KEY_KEY_CODE_ARRAY, (uint8 *) &g_cfg_key, sizeof(g_cfg_key));

        for (i = 0; i < irmap_para.ir_key_num; i++)
        {
            irmap_para.ir_key_code[i]= (uint8)(g_cfg_key.keyID[i]);
        }

        //安装IR驱动
        ir_module_install(&irmap_para);
    }
#endif
}
