/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙管理器装载和卸载，固件升级后初次升级会生成随机地址。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_bt_inner.h"
#include "sys_info.h"
#include "mmm_sp.h"
#include "mmm_bp.h"

#define FIX_LOW3BYTE_ADDR           0
#define RANM_LOW3BYTE_ADDR          1

#define ADC_CHAN_DMA_CHAN  DMA_CHAN_DMA0

extern uint8 com_get_stub_tools_type_inner(void);
extern uint8 get_enter_dut_test_inner(void);

extern bool init_btengine_vram_info_hid(btengine_var_t *p_btengine_var);
extern void get_bt_manager_cfg_hid(btengine_param_t* bt_param, uint32 bt_att_mode);
extern void get_bt_common_cfg_hid(btengine_param_t* bt_param, uint32 bt_att_mode);
extern void get_bt_stack_cfg_hid(btengine_param_t * bt_param);
extern void get_bt_autoconnect_cfg_hid(btengine_param_t * bt_param);

/*! 是否开机进入蓝牙标志 */
static bool power_on_flag = TRUE;

test_state_t test_state;

btengine_info_t bt_info;
com_bt_env_t g_com_bt_env;
bt_hfp_info_t bt_hfp_info;
bt_a2dp_info_t bt_a2dp_info;

static uint8 __section__(".bss.random_lap_buffer") g_random_lap_buffer[RANDOM_LAP_BUF_SISE];

//生成蓝牙LAP域随机地址
//MIC ADC 采样环境声音运算得到随机数
uint32 com_gen_random_lap(void)
{
    dma_chan_setting_t dma_sett;
    ain_setting_t ain_sett;
    adc_setting_t adc_sett;
    int32 ramdom_value, i;
    short *adc_value;
    uint32 random_lap;
    uint32 adc_samples;

    ain_sett.ain_input_mode = AIN_INPUT_MIC0_L;
    ain_sett.op_gain.mic_op_gain = MIC_OP_26DB;
    ain_sett.ain_pa_mode = FALSE;
    enable_ain(AIN_SOURCE_MIC1, &ain_sett);

    adc_sett.sample_rate = 16;
    adc_sett.gain = 0;
    enable_adc(AIN_SOURCE_MIC1, &adc_sett);

    sys_reset_dma_chan(ADC_CHAN_DMA_CHAN);

    adc_samples = (RANDOM_LAP_BUF_SISE) / sizeof(short);

    libc_memset(&dma_sett, 0x00, sizeof(dma_chan_setting_t));
    dma_sett.dst_mem_addr = g_random_lap_buffer;
    dma_sett.frame_len = adc_samples;
    dma_sett.irq_cbk = NULL;
    dma_sett.src_type = DMA_SRC_ADC;
    dma_sett.dst_type = DMA_DST_MEM;
    dma_sett.hf_ie = FALSE;
    dma_sett.tc_ie = FALSE;
    dma_sett.reload = FALSE;
    dma_sett.start = TRUE;
    sys_set_dma_chan(ADC_CHAN_DMA_CHAN, &dma_sett);

    sys_wait_dma_chan_tc(ADC_CHAN_DMA_CHAN, WAIT_DMA_TC_ALWAYS);

    sys_reset_dma_chan(ADC_CHAN_DMA_CHAN);
    disable_adc(AIN_SOURCE_MIC1);
    disable_ain(AIN_SOURCE_MIC1);

    //计算随机数
    adc_value = (short *) (g_random_lap_buffer);
    for (i = 0; i < (adc_samples / 2); i++)
    {
        adc_value[i] = adc_value[i*2];
    }
    adc_samples = adc_samples/2;

    adc_value = (short *) (g_random_lap_buffer);
    ramdom_value = 1;
    for (i = 0; i < (adc_samples / 2); i += 2)
    {
        if (adc_value[i] != 0)
        {
            ramdom_value *= adc_value[i];
        }
        ramdom_value += adc_value[i + 1];
    }

    random_lap = (uint32) ramdom_value & 0xffffff;

    return random_lap;
}

void gen_bt_addr(t_bdaddr *device_addr)
{
    uint32 temp_rand = 0x00000000, tmp32;

    if (com_get_config_default_inner(BTSTACK_BDADD_GEN_METHOD) == RANM_LOW3BYTE_ADDR)
    {
        temp_rand = com_gen_random_lap();
    }
    else
    {
        com_get_config_struct_inner(BTSTACK_BDADD_LOW3BYTES, (void *) &temp_rand, 4);
    }
    device_addr->bytes[0] = (uint8) (temp_rand);
    device_addr->bytes[1] = (uint8) (temp_rand >> 8);
    device_addr->bytes[2] = (uint8) (temp_rand >> 16);
    com_get_config_struct_inner(BTSTACK_BDADD_HIGH3BYTES, (void *) &tmp32, 4);
    device_addr->bytes[3] = (uint8) (tmp32); //0xFD
    device_addr->bytes[4] = (uint8) (tmp32 >> 8); //0x4E
    device_addr->bytes[5] = (uint8) (tmp32 >> 16);//0xF4
}

/*
    接口说明：
        1.检查BT_ENGINE的vram信息是否正确。
        2.从VRAM中获取bt_addr,如果失败，从配置项中获取。
        3.从Vram中获取bt_dev_name，如果失败，从配置项中获取
        4.量产后，量产工具会备份bt_addr/bt_dev_name信息到fw_info，并且烧录完成后恢复到vram中
*/
bool init_btengine_vram_info(btengine_var_t *p_btengine_var)
{
    bool ret_val = TRUE;
    uint8 i;

    if (sys_vm_read(p_btengine_var, VM_BTENGINE, sizeof(btengine_var_t)) < 0)
    {
        p_btengine_var->magic = 0xffff;
    }

    //从vram中读取失败，初始化bt_device_info
    if (p_btengine_var->magic != VRAM_MAGIC(VM_BTENGINE))
    {
        uint8 save_bt_data = 0;

        ret_val = FALSE;

        //检查在量产时是否保存有蓝牙地址
        sys_get_fw_info(&save_bt_data, FW_INFO_BT_SAVE_DATA, FW_INFO_BT_SAVE_DATA_LEN);

        if (save_bt_data != SAVE_BT_ADDR)
        {
            //generate random bt addr
            gen_bt_addr(&(p_btengine_var->device_addr));
        }
        else
        {
            //check get addr whether it is valid or not
            for (i = 0; i < 6; i++)
            {
                if (p_btengine_var->device_addr.bytes[i] != 0)
                {
                    break;
                }
            }

            if (i == 6)
            {
                gen_bt_addr(&(p_btengine_var->device_addr));
            }
        }

        if (save_bt_data != SAVE_BT_NAME)
        {
            libc_memset(p_btengine_var->device_name, 0, (BT_MAC_NAME_LEN_MAX+1));
            //get bt dev name form config text
            com_get_config_struct_inner(BTSTACK_DEVICE_NAME, p_btengine_var->device_name, BT_MAC_NAME_LEN_MAX);
            p_btengine_var->device_name[BT_MAC_NAME_LEN_MAX] = 0;
        }
        else
        {
            //check get dev name whether it is valid or not
            if (p_btengine_var->device_name[0] == 0)
            {
                libc_memset(p_btengine_var->device_name, 0, (BT_MAC_NAME_LEN_MAX+1));
                com_get_config_struct_inner(BTSTACK_DEVICE_NAME, p_btengine_var->device_name, BT_MAC_NAME_LEN_MAX);
                p_btengine_var->device_name[BT_MAC_NAME_LEN_MAX] = 0;
            }
        }

        libc_printf("<bt clasee name> %s\n",p_btengine_var->device_name);

        //libc_memset(p_btengine_var->devs_cfg, 0x00, sizeof(bt_devs_cfg_t));
        p_btengine_var->burn_lock_flag = 0;
        p_btengine_var->magic = VRAM_MAGIC(VM_BTENGINE);

        sys_vm_write(p_btengine_var, VM_BTENGINE, sizeof(btengine_var_t));

    }

    return ret_val;
}

void get_bt_stack_cfg(btengine_param_t* bt_param)
{
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

    bt_param->bt_stack_cfg_info.sp_ssp_enable = (uint8) com_get_config_default_inner(BTSTACK_SSP_ENABLE);
    bt_param->bt_stack_cfg_info.sp_sbc_audio_quality = (uint8) com_get_config_default_inner(BTSTACK_SBC_AUDIO_QUALITY);
    bt_param->bt_stack_cfg_info.sp_bqb_test = (uint16) com_get_config_default_inner(BTSTACK_BQB_TEST_ENALBE);
}

void get_bt_autoconnect_cfg(btengine_param_t* bt_param)
{
    //回连相关的一些全局变量，放在此处初始化
    bt_param->bt_auto_connect_cfg.connect_count_timeout = (uint8) com_get_config_default_inner(BTMANAGER_AUTO_CONNECT_COUNT_TIMEOUT);
    bt_param->bt_auto_connect_cfg.connect_count_startup = (uint8) com_get_config_default_inner(BTMANAGER_AUTO_CONNECT_COUNT_STARTUP);
    bt_param->bt_auto_connect_cfg.connect_count_mode = (uint8) com_get_config_default_inner(BTMANAGER_AUTO_CONNECT_COUNT_MODE);
    bt_param->bt_auto_connect_cfg.connect_interval = (uint8) com_get_config_default_inner(BTMANAGER_AUTO_CONNECT_INTERVAL);
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    bt_param->bt_auto_connect_cfg.tws_connect_count_timeout= (uint8) com_get_config_default_inner(BTMANAGER_TWS_AUTO_CONNECT_COUNT_TIMEOUT);
    bt_param->bt_auto_connect_cfg.tws_connect_count_startup= (uint8) com_get_config_default_inner(BTMANAGER_TWS_AUTO_CONNECT_COUNT_STARTUP);
    bt_param->bt_auto_connect_cfg.tws_connect_count_before_search= (uint8) com_get_config_default_inner(BTMANAGER_TWS_AUTO_CONNECT_COUNT_BEFORE_SEARCH);
#endif
    bt_param->bt_auto_connect_cfg.auto_connect_sequence = com_get_config_default_inner(BTMANAGER_AUTO_CONNECT_SEQUENCE);
    bt_param->bt_auto_connect_cfg.enable_slave_connect_phone = com_get_config_default_inner(BTMANAGER_ENABLE_TWS_SLAVE_CONNECT_PHONE);

    if (power_on_flag == TRUE)
    {
        bt_param->bt_auto_connect_cfg.connect_type = AUTO_CONNECT_TYPE_STARTUP;
        power_on_flag = FALSE;
    }
    else
    {
        bt_param->bt_auto_connect_cfg.connect_type = AUTO_CONNECT_TYPE_MODE;
    }
}

void get_bt_manager_cfg(btengine_param_t* bt_param, uint32 bt_att_mode)
{

    bt_param->bt_manager_cfg_info.sp_a2dp_cp = com_get_config_default_inner(BTMANAGER_ENABLE_A2DP_CP);
    bt_param->bt_manager_cfg_info.sp_avdtp_aac = com_get_config_default_inner(BTMANAGER_ENABLE_AVDTP_AAC);
    bt_param->bt_manager_cfg_info.sp_avrcp = com_get_config_default_inner(BTMANAGER_ENABLE_AVRCP);

    //ASET音效调试模式不支持AVRCP音量同步
    if ((bt_param->bt_manager_cfg_info.sp_avrcp == 0) || (com_get_stub_tools_type_inner() == STUB_PC_TOOL_ASET_EQ_MODE))
    {
        bt_param->bt_manager_cfg_info.sp_avrcp_vol_syn = 0;
    }
    else
    {
        bt_param->bt_manager_cfg_info.sp_avrcp_vol_syn = com_get_config_default_inner(
                BTMANAGER_ENABLE_AVRCP_VOLUME_SYNC);
    }

    //自动化测试必须使能HFP服务
    if (bt_att_mode != 0)
    {
        bt_param->bt_manager_cfg_info.sp_hfp = 1;
    }
    else
    {
        //必须使能BTCALL应用才使能HFP服务；ASET工具不支持HFP服务，不支持通话功能，通话功能使用ASQT工具调试PEQ
        if ((com_get_config_default_inner(SETTING_APP_SUPPORT_BTCALL) == 0) || (com_get_stub_tools_type_inner() == STUB_PC_TOOL_ASET_EQ_MODE))
        {
            bt_param->bt_manager_cfg_info.sp_hfp = 0;
        }
        else
        {
            bt_param->bt_manager_cfg_info.sp_hfp = com_get_config_default_inner(BTMANAGER_ENABLE_HFP);
        }
    }

    //ASQT通话调试模式不支持HFP音量同步、电池电量同步、三方通话
    if ((bt_param->bt_manager_cfg_info.sp_hfp == 0) || (com_get_stub_tools_type_inner() == STUB_PC_TOOL_ASQT_MODE))
    {
        bt_param->bt_manager_cfg_info.sp_hfp_vol_syn = 0;
        bt_param->bt_manager_cfg_info.sp_hfp_bat_report = 0;
        bt_param->bt_manager_cfg_info.sp_hfp_3way_call = 0;
    }
    else
    {
        bt_param->bt_manager_cfg_info.sp_hfp_vol_syn = com_get_config_default_inner(BTMANAGER_ENABLE_HFP_VOLUME_SYNC);
        bt_param->bt_manager_cfg_info.sp_hfp_bat_report = com_get_config_default_inner(BTMANAGER_ENABLE_HFP_BAT_REPORT);
        bt_param->bt_manager_cfg_info.sp_hfp_3way_call = com_get_config_default_inner(BTMANAGER_SUPPORT_THREE_WAY_CALL);
    }

    bt_param->bt_manager_cfg_info.sp_hid = com_get_config_default_inner(BTMANAGER_ENABLE_HID);
    bt_param->bt_manager_cfg_info.sp_spp = com_get_config_default_inner(BTMANAGER_ENABLE_SPP);
    bt_param->bt_manager_cfg_info.sp_ble = com_get_config_default_inner(BTMANAGER_ENABLE_BLE);

    if (com_get_config_default(SETTING_APP_SUPPORT_TAKEPHOTO) == 1)
    {
        //如果使能独立HID拍照模式，那么蓝牙推歌打电话时是不需要使能HID的
        bt_param->bt_manager_cfg_info.sp_hid = 0;
    }

    bt_param->bt_manager_cfg_info.sp_auto_connect = com_get_config_default_inner(BTMANAGER_ENABLE_AUTO_CONNECT);
    //bt_param->bt_manager_cfg_info.try_con_other_times = com_get_config_default_inner(BTMANAGER_RECONNECT_ANOTHER_TIMES);
    bt_param->bt_manager_cfg_info.max_pair_list_num = com_get_config_default_inner(BTMANAGER_MAX_PAIRED_LIST_MEMBER);
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    bt_param->bt_manager_cfg_info.sp_tws_link = com_get_config_default_inner(BTMANAGER_ENABLE_TWS);
#else
    bt_param->bt_manager_cfg_info.sp_tws_link = FALSE;
#endif
#if (SUPPORT_ASQT_TEST == 1)
    //ASQT模式仅支持单链路
    if (com_get_stub_tools_type_inner() == STUB_PC_TOOL_ASQT_MODE)
    {
        bt_param->bt_manager_cfg_info.sp_tws_link = FALSE;
        bt_param->bt_manager_cfg_info.asqt_test_mode = 1;
    }
#endif
    bt_param->bt_manager_cfg_info.sp_device_num = 2;//已不再使用
    bt_param->bt_manager_cfg_info.tws_master_role_depending_on_key = com_get_config_default_inner(BTMANAGER_TWS_MASTER_ROLE_DEPENDING_ON_KEY);
    bt_param->bt_manager_cfg_info.visual_cfg_mode = (uint8) com_get_config_default_inner(BTMANAGER_VISUAL_MODE);

    bt_param->bt_stack_cfg_info.sp_linkkey_miss_reject = (uint8) com_get_config_default_inner(BTSTACK_LINKKEY_MISS_REJECT);
    bt_param->bt_stack_cfg_info.sp_cont_link_info = (uint8) com_get_config_default_inner(BTSTACK_CONNECT_LINK_INFO);

}
void get_bt_common_cfg(btengine_param_t* bt_param, uint32 bt_att_mode)
{
    uint8 dut_enable;
    uint32 class_of_device;

    dut_enable = (uint8) com_get_config_default_inner(SETTING_BT_CONTROLLER_DUT_ENABLE);
    if (dut_enable == 0)
    {
        bt_param->bt_common_cfg.con_test_mode = 0;
        if (get_enter_dut_test_inner() == 1)
        {
            bt_param->bt_common_cfg.con_test_mode = 1;
        }
    }
    else
    {
        bt_param->bt_common_cfg.con_test_mode = dut_enable;
    }

#if (BT_CON_CLBRPARA_WRITE == 1)
    bt_param->bt_common_cfg.con_clbrpara_enable = 1;
    if ((uint8) com_get_config_default_inner(SETTING_BT_CONTROLLER_CLBRPARA_WRITE) == 0)
    {
        bt_param->bt_common_cfg.con_clbrpara_enable = 0;
    }
#else
    bt_param->bt_common_cfg.con_clbrpara_enable = 0;
#endif
    //是否打开BT Controller打印信息
    bt_param->bt_common_cfg.con_print_enable = (uint8) com_get_config_default_inner(SETTING_BT_CONTROLLER_PRINT_ENABLE);
    bt_param->bt_common_cfg.uart_print_enable = (uint8) com_get_config_default_inner(SETTING_UART_PRINT_ENABLE);
    //蓝牙协议栈打印选项
    bt_param->bt_common_cfg.stack_print_enable = (uint8) com_get_config_default_inner(SETTING_BT_ENGINE_PRINT_ENABLE);

    //RF发射功率
    bt_param->bt_common_cfg.rf_txpower_adjust = com_get_config_default_inner(BTSTACK_RF_TXPOWER_ADJUST);

    bt_param->bt_common_cfg.support_a2dp_flag = com_get_config_default_inner(BTMANAGER_ENABLE_A2DP);

    bt_param->bt_common_cfg.support_btplay_flag = com_get_config_default_inner(SETTING_APP_SUPPORT_BTPLAY);

    bt_param->bt_common_cfg.sco_postdeal_enable = com_get_config_default_inner(BTSTACK_SCO_POSTDEAL_ENABLE);

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1) && (SUPPORT_BTCALL_RECORD_FUNC == 1))
    bt_param->bt_common_cfg.sco_postdeal_enable = FALSE;
#endif
#if (SUPPORT_OK_MIC == 1)
    bt_param->bt_common_cfg.sco_postdeal_enable = FALSE;
#endif

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if ((uint8) com_get_config_default_inner(BTMANAGER_ENABLE_TWS) == TRUE)
    {
        bt_param->bt_common_cfg.max_link_num = 2;
    }
    else
#endif
    {
        bt_param->bt_common_cfg.max_link_num = 1;
    }

#if (SUPPORT_ASQT_TEST == 1)
    //ASQT模式仅支持单链路
    if (com_get_stub_tools_type_inner() == STUB_PC_TOOL_ASQT_MODE)
    {
        bt_param->bt_common_cfg.max_link_num = 1;
    }
#endif

    bt_param->bt_common_cfg.ignore_uncall_sco = com_get_config_default_inner(BTCALL_IGNORE_UNCALLSCO);

    com_get_config_struct_inner(BTSTACK_CLASS_OF_DEVICE, (void *) &class_of_device, 4);
    bt_param->bt_common_cfg.class_device = class_of_device;

    com_get_config_struct_inner(BTSTACK_FIX_PIN_CODE, bt_param->bt_common_cfg.pin_code, PIN_CODE_LEN);
}
void bt_common_globe_var_init(void)
{
    libc_memset(&g_com_bt_env, 0x00, sizeof(com_bt_env_t));
    libc_memset(&bt_info, 0x00, sizeof(com_bt_env_t));
    g_com_bt_env.at_bat_value = 0xff;
    g_com_bt_env.last_status = STATUS_NONE;

    bt_hfp_info.hfp_volume_sync_enable = (bool) com_get_config_default_inner(BTMANAGER_ENABLE_HFP_VOLUME_SYNC);
    bt_hfp_info.hfp_start_flag = 0;
    bt_hfp_info.hfp_timer_id = -1;

    bt_a2dp_info.avrcp_volume_sync_enable = (bool) com_get_config_default_inner(BTMANAGER_ENABLE_AVRCP_VOLUME_SYNC);
    bt_a2dp_info.a2dp_start_flag = 0;
    bt_a2dp_info.a2dp_timer_id = -1;

    g_com_bt_env.last_tws_pair_status = TWS_IDLE;
    g_com_bt_env.last_dev_role = NORMAL_DEV;
}

bool com_creat_bt_engine_inner(uint32 bt_att_mode)
{
    msg_apps_t msg;
    msg_reply_t temp_reply;
    btengine_param_t btengine_param;

    if (get_bt_state_inner() == BT_STATE_WORK)
    {
        return TRUE;
    }

    bt_manager_blespp_data_trans_init();
    extern bool test_blespp_data_trans_init(void);
    extern bool test_blespp_data_trans_dispatch(buffer_rw_t* buf);
    test_blespp_data_trans_init();
    bt_manager_blespp_data_trans_register_dispatch_callback_inner(test_blespp_data_trans_dispatch);

    libc_memset(&btengine_param, 0x00, sizeof(btengine_param_t));

    btengine_param.bt_common_cfg.att_test_flag = bt_att_mode;

    if (g_p_global_var->only_hid_mode == 0)
    {
        get_bt_manager_cfg(&btengine_param, bt_att_mode);
        get_bt_common_cfg(&btengine_param,bt_att_mode);
        get_bt_stack_cfg(&btengine_param);
        get_bt_autoconnect_cfg(&btengine_param);
    }
    else
    {
        get_bt_manager_cfg_hid(&btengine_param, bt_att_mode);
        get_bt_common_cfg_hid(&btengine_param,bt_att_mode);
        get_bt_stack_cfg_hid(&btengine_param);
        get_bt_autoconnect_cfg_hid(&btengine_param);
    }


    bt_common_globe_var_init();

    msg.para[0] = APP_ID_BTENGINE;
    msg.para[1] = 0;
    msg.content.addr = &btengine_param;
    //消息类型(即消息名称)
    msg.type = MSG_CREAT_APP_SYNC;
    //发送同步消息
    return send_sync_msg_inner(APP_ID_MANAGER, &msg, &temp_reply, 0);
}

bool com_close_bt_engine_inner(void)
{
    msg_apps_t msg;

    if (get_bt_state_inner() == BT_STATE_NONE)
    {
        return TRUE;
    }

    msg.type = MSG_KILL_APP_SYNC;
    msg.para[0] = APP_ID_BTENGINE;
    //发送同步消息
    send_sync_msg_inner(APP_ID_MANAGER, &msg, NULL, 0);

    //销毁软件定时器
    if (bt_hfp_info.hfp_timer_id != -1)
    {
        kill_app_timer_inner(APP_TIMER_GROUP_COMMON, bt_hfp_info.hfp_timer_id);
        bt_hfp_info.hfp_timer_id = -1;
    }
    bt_a2dp_info.a2dp_start_flag = 0;
    if (bt_a2dp_info.a2dp_timer_id != -1)
    {
        kill_app_timer_inner(APP_TIMER_GROUP_COMMON, bt_a2dp_info.a2dp_timer_id);
        bt_a2dp_info.a2dp_timer_id = -1;
    }

    //有可能蓝牙马上关闭，common还没来得及完成tws pair exit，所以必须在此确认
    com_bt_check_tws_pair_exit();

    bt_manager_blespp_data_trans_exit();
    extern bool test_blespp_data_trans_exit(void);
    test_blespp_data_trans_exit();

    return TRUE;
}

bool com_init_btengine_info_inner(btengine_var_t *p_btengine_var)
{
    if (g_p_global_var->only_hid_mode == 0)
    {
        return init_btengine_vram_info(p_btengine_var);
    }
    else
    {
        return init_btengine_vram_info_hid(p_btengine_var);
    }
}
