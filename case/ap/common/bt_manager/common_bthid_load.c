/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙HID拍照应用管理器装载和卸载，固件升级后初次升级会生成随机地址。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_bt_inner.h"

extern void gen_bt_addr(t_bdaddr *device_addr);

bool init_btengine_vram_info_hid(btengine_var_t *p_btengine_var)
{
    bool ret_val = TRUE;

    if (sys_vm_read(p_btengine_var, VM_BTENGINE_HID, sizeof(btengine_var_t)) < 0)
    {
        p_btengine_var->magic = 0xffff;
    }
    //初始化config的VM变量
    if (p_btengine_var->magic != VRAM_MAGIC(VM_BTENGINE_HID))
    {
        ret_val = FALSE;

        gen_bt_addr(&(p_btengine_var->device_addr));
        com_get_config_struct_inner(BTSTACK_DEVICE_NAME_HID, p_btengine_var->device_name, BT_MAC_NAME_LEN_MAX);
        p_btengine_var->device_name[BT_MAC_NAME_LEN_MAX] = 0;
        p_btengine_var->magic = VRAM_MAGIC(VM_BTENGINE_HID);

        //libc_memset(p_btengine_var->devs_cfg, 0x00, sizeof(bt_devs_cfg_t));
        p_btengine_var->burn_lock_flag = 0;
        sys_vm_write(p_btengine_var, VM_BTENGINE_HID, sizeof(btengine_var_t));
    }

    return ret_val;
}

void get_bt_stack_cfg_hid(btengine_param_t* bt_param)
{
    bt_param->bt_stack_cfg_info.sp_linkkey_miss_reject = (uint8) com_get_config_default_inner(BTSTACK_LINKKEY_MISS_REJECT);
    bt_param->bt_stack_cfg_info.sp_cont_link_info = (uint8) com_get_config_default_inner(BTSTACK_CONNECT_LINK_INFO);

#ifdef ENABLE_TRUE_WIRELESS_STEREO //TWS场景Sniff稳定性较差，暂时关闭Sniff
    if (com_get_config_default_inner(BTMANAGER_ENABLE_TWS) == TRUE)
    {
        bt_param->bt_stack_cfg_info.sp_sniff_enable = FALSE;
    }
    else
#endif
    {
        bt_param->bt_stack_cfg_info.sp_sniff_enable = (uint8) com_get_config_default_inner(BTSTACK_SNIFF_ENABLE);
    }

    bt_param->bt_stack_cfg_info.sp_ssp_enable = (uint8)com_get_config_default_inner(BTSTACK_SSP_ENABLE);
    bt_param->bt_stack_cfg_info.sp_sbc_audio_quality = (uint8)com_get_config_default_inner(BTSTACK_SBC_AUDIO_QUALITY);
    bt_param->bt_stack_cfg_info.sp_bqb_test = (uint16)com_get_config_default_inner(BTSTACK_BQB_TEST_ENALBE);
}

void get_bt_autoconnect_cfg_hid(btengine_param_t* bt_param)
{
    //回连相关的一些全局变量，放在此处初始化
    bt_param->bt_auto_connect_cfg.connect_count_timeout= (uint8) com_get_config_default_inner(BTMANAGER_AUTO_CONNECT_COUNT_TIMEOUT);
    bt_param->bt_auto_connect_cfg.connect_count_startup= (uint8) com_get_config_default_inner(BTMANAGER_AUTO_CONNECT_COUNT_STARTUP);
    bt_param->bt_auto_connect_cfg.connect_count_mode= (uint8) com_get_config_default_inner(BTMANAGER_AUTO_CONNECT_COUNT_MODE);
    bt_param->bt_auto_connect_cfg.connect_interval= (uint8) com_get_config_default_inner(BTMANAGER_AUTO_CONNECT_INTERVAL);
    bt_param->bt_auto_connect_cfg.connect_type = AUTO_CONNECT_TYPE_MODE;
    bt_param->bt_auto_connect_cfg.try_con_other_times = com_get_config_default_inner(BTMANAGER_RECONNECT_ANOTHER_TIMES);
}

void get_bt_manager_cfg_hid(btengine_param_t* bt_param, uint32 bt_att_mode)
{
    bt_param->bt_manager_cfg_info.sp_hid = com_get_config_default_inner(BTMANAGER_ENABLE_HID);
    bt_param->bt_manager_cfg_info.sp_auto_connect = com_get_config_default_inner(BTMANAGER_ENABLE_AUTO_CONNECT);
    bt_param->bt_manager_cfg_info.max_pair_list_num = 1;
    bt_param->bt_manager_cfg_info.sp_tws_link = FALSE;
    bt_param->bt_manager_cfg_info.sp_device_num = 1;
}

void get_bt_common_cfg_hid(btengine_param_t* bt_param, uint32 bt_att_mode)
{
    uint32 class_of_device;

    bt_param->bt_common_cfg.con_clbrpara_enable = 1;

    //是否打开BT Controller打印信息
    bt_param->bt_common_cfg.con_print_enable = (uint8) com_get_config_default_inner(SETTING_BT_CONTROLLER_PRINT_ENABLE);
    bt_param->bt_common_cfg.uart_print_enable = (uint8) com_get_config_default_inner(SETTING_UART_PRINT_ENABLE);
    //蓝牙协议栈打印选项
    bt_param->bt_common_cfg.stack_print_enable = (uint8) com_get_config_default_inner(SETTING_BT_ENGINE_PRINT_ENABLE);

    //RF发射功率
    bt_param->bt_common_cfg.rf_txpower_adjust = com_get_config_default_inner(BTSTACK_RF_TXPOWER_ADJUST);

    bt_param->bt_common_cfg.max_link_num= 1;

    bt_param->bt_common_cfg.ignore_uncall_sco= com_get_config_default_inner(BTCALL_IGNORE_UNCALLSCO);

    com_get_config_struct_inner(BTSTACK_CLASS_OF_DEVICE_HID, (void *) &class_of_device, 4);
    bt_param->bt_common_cfg.class_device=class_of_device;

    com_get_config_struct_inner(BTSTACK_FIX_PIN_CODE, bt_param->bt_common_cfg.pin_code, PIN_CODE_LEN);
}

