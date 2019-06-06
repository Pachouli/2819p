/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xiaomaky   2013-4-11 14:26:35     1.0             build this file
 *******************************************************************************/
/*!
 * \file     bt_engine_device_control.c
 * \brief    global variable模块
 * \author   xiaomaky
 * \version  1.0
 * \date  2013/4/11
 *******************************************************************************/
#include "./../bluetooth_engine.h"

role_switch_retry_st role_witch_retry[MAX_DEVICE_LINK];

uint8 __section__(".stack.ap_bt_con_high_stack_ptos") bt_high_stk_ptos[1]; //动态数组
extern uint16 _BT_HIGH_STK_SIZE;

uint8 __section__(".stack.ap_bt_con_low_stack_ptos") bt_low_stk_ptos[1]; //动态数组
extern uint16 _BT_LOW_STK_SIZE;

static uint8 BT_RESET[] =
{ 0x01, 0x03, 0x0c, 0x00 };

static uint8 BT_ENABLE_DUT[] =
{ 0x01, 0x03, 0x18, 0x00 };

static uint8 BT_ENABLE_ALLSCAN[] =
{ 0x01, 0x1a, 0x0c, 0x01, 0x03 };

static uint8 BT_AUTOACCEPT_CONNECT[] =
{ 0x01, 0x05, 0x0c, 0x03, 0x02, 0x00, 0x02 };
static uint8 host_buffer_size_cmd[] =
{ 0x01, 0x33, 0x0c, 0x07, 0xfd, 0x03, SCO_RECV_PAYLOAD_SIZE_MAX, 0x06, 0x00, 0x0a, 0x00 };
static uint8 errordata_report_cmd[] =
{ 0x01, 0x5b, 0x0c, 0x01, 0x01 };

bool bt_check_dev_exist(bt_dev_t* dev)
{
    if ((dev != NULL) && (dev->p_dev_status != NULL))
    {
        return TRUE;
    }
    return FALSE;
}

void creat_con_pthread(void)
{
    pthread_param_t btcont_thread_para;
    btcont_thread_para.start_rtn = bt_con_high_handle;
    btcont_thread_para.arg = NULL;
    btcont_thread_para.ptos = (void *) (bt_high_stk_ptos + 0x800);
    btcont_thread_para.stk_size = 0x800;
    if (libc_pthread_create(&btcont_thread_para, AP_BTCONTROLLER_HIGH_PRIO, CREATE_NOT_MAIN_THREAD) < 0)
    {
        while (1)
        {
            ; //创建hfp子线程失败
        }
    }

    btcont_thread_para.start_rtn = bt_con_low_handle;
    btcont_thread_para.arg = NULL;
    btcont_thread_para.ptos = (void *) (bt_low_stk_ptos + 0xa00);
    btcont_thread_para.stk_size = 0xa00;
    if (libc_pthread_create(&btcont_thread_para, AP_BTCONTROLLER_LOW_PRIO, CREATE_NOT_MAIN_THREAD) < 0)
    {
        while (1)
        {
            ; //创建hfp子线程失败
        }
    }
}

void monitor_tmpr(void)
{
    con_monitor_rf_temperature();
}

void deal_con_test_mode(void)
{
    int con_test_id;
    if ((btengine_param.con_test_mode != CON_LE_TEST) && (btengine_param.con_test_mode != CON_DUT_LE_TEST))
    {
        bt_set_address(); //不能发
    }
    if ((btengine_param.con_test_mode == CON_DUT_TEST) || (btengine_param.con_test_mode == CON_DUT_LE_TEST))
    {
        con_test_id = sys_set_irq_timer1(monitor_tmpr, 50);
        bt_dut_test(); //不能发
    }
    else if (btengine_param.con_test_mode == CON_LE_TEST)
    {
        con_test_id = sys_set_irq_timer1(monitor_tmpr, 50);
        while (1)
        {
            clear_watch_dog();
        }
    }
    else
    {
        ; //for qac
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *  启动蓝牙库
 * \param[in]    none
 * \param[out]   none
 * \return       设备句柄, NULL:蓝牙库初始化失败(因为按键或其他消息退出引擎)
 * \ingroup
 * \note
 *******************************************************************************/
app_result_e BtDev_Open(void)
{
    app_result_e ret_val = RESULT_NULL;
    uint32 temp_flag;

    bt_cfg_info_t bt_cfg_info;

    libc_memset(&bt_cfg_info, 0, sizeof(bt_cfg_info_t));

    //回调函数的注册需要先调用，否则bt drv中使用到的话会有问题。
    init_callback_func();
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (bt_manager_cfg_info.sp_tws_link == 1)
    {
        bt_cfg_info.support_tws = 1;
    }
    else
#endif
    {

        bt_cfg_info.support_tws = 0;
    }

    bt_cfg_info.sp_hfp = bt_manager_cfg_info.sp_hfp;
    bt_cfg_info.sp_cont_link_info = bt_stack_cfg_info.sp_cont_link_info;
    bt_cfg_info.con_clbrpara_enable = btengine_param.con_clbrpara_enable;
    bt_cfg_info.con_print_enable = btengine_param.con_print_enable;
    bt_cfg_info.con_test_mode = btengine_param.con_test_mode;
    bt_cfg_info.rf_txpower_adjust = btengine_param.rf_txpower_adjust;
    bt_cfg_info.uart_print_enable = btengine_param.uart_print_enable;
    bt_cfg_info.head = btengine_global_variable.bt_stack_init_param_cfg.btmempool_cfg.head;
    bt_cfg_info.total_size = btengine_global_variable.bt_stack_init_param_cfg.btmempool_cfg.total_size;
    bt_cfg_info.sco_rx_data_transfer_mode = btengine_global_variable.hfp.sco.sco_info.rx_data_transfer_mode; //FORMAT_ENCODE_TRANSMISSION;
    bt_cfg_info.sco_tx_data_transfer_mode = btengine_global_variable.hfp.sco.sco_info.tx_data_transfer_mode;
    bt_cfg_info.bt_page_scan_act_windows_size = btengine_global_variable.bt_stack_init_param_cfg.bt_page_scan_actwin;

    btengine_global_variable.bqb_test = bt_stack_cfg_info.sp_bqb_test;

    if ((BQB_TEST_ENABLE & btengine_global_variable.bqb_test) == 0)
    {
        bt_cfg_info.bqb_test_flag = 0x00;
    }
    else
    {
        bt_cfg_info.bqb_test_flag = 0x01;
    }

    if ((btengine_param.con_test_mode == CON_LE_TEST) || (btengine_param.con_test_mode == CON_DUT_LE_TEST))
    {
        //GPIO5-Uart0Rx/GPIO6-Uart1Tx
        set_pad_function(PAD_GPIO5, GPIO5_MFP_UART0_RX, NULL, 0);
        uart_rx_set(0, 115200, 0xff); //0xff表示不需要进行Uart控制器配置
        set_pad_function(PAD_GPIO6, GPIO6_MFP_UART0_TX, NULL, 0);
        uart_tx_set(0, 115200, 0xff); //0xff表示不需要进行Uart控制器配置

        bt_cfg_info.bqb_uart_idx = 0;
    }

    bt_init(&bt_cfg_info);

    temp_flag = sys_local_irq_save();
    if (btengine_global_variable.btdbg_sel_flag == 1)
    {
        con_set_debug_sel(btengine_global_variable.btdbg_sel_value);
    }
    btengine_global_variable.btdbg_sel_flag = 2;
    sys_local_irq_restore(temp_flag);

    creat_con_pthread();
    deal_con_test_mode();
    BtDev_LibInit();
    btengine_global_variable.reconnect_another_cnt = 0;
    btengine_global_variable.avrcp_disconnect_cnt = 0;
    btengine_global_variable.connect_lock_flag = 0;
    return ret_val;
}

/******************************************************************************/
/*!
 * \par  Description:
 *  关闭蓝牙库,并不进行模组断电操作，正常情况下模组一旦上电，就不会关闭
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup
 * \note
 *******************************************************************************/
void BtDev_Close(void)
{
    /* if not shut hardware, reset controller.  */
    //bt_reset_controller();
    //退出高优先级线程
    con_thread_close(1);
    //退出低优先级线程
    con_thread_close(0);

    bt_deinit();
}

/******************************************************************************/
/*!
 * \par  Description:
 *  蓝牙模组断电，并设置下载patchcode标志为FALSE，只有异常情况才会调用该函数
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup
 * \note
 *******************************************************************************/
void BtDev_PowerDown(void)
{
    //模组断电后延时100ms
    sys_os_time_dly(10);
    return;
}

//TX:01 06 04 03 32 00 13
void bt_disconnect_acl(bt_dev_t *dev)
{
    if (dev != NULL)
    {
        if (dev->p_dev_status != NULL)
        {
            if (dev->p_dev_status->sniff_flag != 0)
            {
                bt_exit_sniff(dev);
            }
            if (dev->p_dev_status->acl_handle != 0)
            {
                HCI_Disconnect(dev->p_dev_status->acl_handle);
                return;
            }
        }
        if (bt_linking_flag == TRUE)
        {
            cancel_create_connection(dev);
        }
    }
}

//for quit BT,  TX:01 03 0c 00
void bt_reset_controller(void)
{
    HCI_DBG_PRINTF("reset cmd\n");
    Send(0, BT_RESET, sizeof(BT_RESET));
}

//for modify sniff mode,  TX:01 0d 08 04 32 00 0f 00
void bt_set_link_policy(bt_dev_t *dev, uint8 link_policy_setting)
{
    uint8 link_policy_settings_cmd[8];
    if ((dev == NULL) || (dev->p_dev_status == NULL))
    {
        return;
    }
    if (dev->p_dev_status->acl_handle != 0)
    {
        link_policy_settings_cmd[0] = 0x01;
        link_policy_settings_cmd[1] = 0x0d;
        link_policy_settings_cmd[2] = 0x08;
        link_policy_settings_cmd[3] = 0x04;
        link_policy_settings_cmd[4] = (uint8) (dev->p_dev_status->acl_handle & 0xff);
        link_policy_settings_cmd[5] = (uint8) (dev->p_dev_status->acl_handle >> 8);
        link_policy_settings_cmd[6] = link_policy_setting;
        link_policy_settings_cmd[7] = 0x00;

        HCI_DBG_PRINTF("set_link_policy cmd\n");
        Send(0, link_policy_settings_cmd, 8);
    }
}

//for modify sniff mode,  TX:01 0f 08 02 03 00
void bt_set_default_link_policy(uint8 link_policy_setting)
{
    uint8 link_policy_settings_cmd[8];

    link_policy_settings_cmd[0] = 0x01;
    link_policy_settings_cmd[1] = 0x0f;
    link_policy_settings_cmd[2] = 0x08;
    link_policy_settings_cmd[3] = 0x02;
    link_policy_settings_cmd[4] = link_policy_setting;
    link_policy_settings_cmd[5] = 0x00;

    HCI_DBG_PRINTF("set_deafault link_policy cmd\n");
    Send(0, link_policy_settings_cmd, 6);
}

/******************************************************************************/
/*!
 * \par  Description:
 *      进入sniff
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/

void bt_enter_sniff(bt_dev_t *dev)
{
    uint8 sniff_cmd[14];
    if ((dev != NULL) && (dev->p_dev_status != NULL) && (bt_stack_cfg_info.sp_sniff_enable == TRUE)
            && (dev->p_dev_status->sniff_flag == 0) && (dev->p_dev_status->acl_handle != 0)
            && (current_sniff_count < MAX_SNIFF_COUNT))
    {
        HCI_DBG_PRINTF("send sniff msg\n");
        sniff_cmd[0] = 0x01;
        sniff_cmd[1] = 0x03;
        sniff_cmd[2] = 0x08;
        sniff_cmd[3] = 0x0a;
        sniff_cmd[4] = (uint8) (dev->p_dev_status->acl_handle);
        sniff_cmd[5] = (uint8) ((dev->p_dev_status->acl_handle) >> 8);
        sniff_cmd[6] = 0x20;
        sniff_cmd[7] = 0x03;
        sniff_cmd[8] = 0x20;
        sniff_cmd[9] = 0x03;
        sniff_cmd[10] = 0x04;
        sniff_cmd[11] = 0x00;
        sniff_cmd[12] = 0x01;
        sniff_cmd[13] = 0x00;

        Send(0, sniff_cmd, 14);
        current_sniff_count++;
    }

}

void bt_exit_sniff(bt_dev_t *dev)
{
    uint8 sniff_cmd[6];
    uint16 i;

    if ((dev != NULL) && (dev->p_dev_status != NULL) && (dev->p_dev_status->acl_handle != 0)
            && (dev->p_dev_status->sniff_flag != 0))
    {
        HCI_DBG_PRINTF("exit sniff\n");

        sys_mdelay(5);
        sniff_cmd[0] = 0x01;
        sniff_cmd[1] = 0x04;
        sniff_cmd[2] = 0x08;
        sniff_cmd[3] = 0x02;
        sniff_cmd[4] = (uint8) (dev->p_dev_status->acl_handle);
        sniff_cmd[5] = (uint8) ((dev->p_dev_status->acl_handle) >> 8);

        Send(0, sniff_cmd, 6);

        i = 500;

        //最多等待1s的时间要求退出sniff
        while (i > 0)
        {
            i--;

            BtDev_RxReadToHCI(NULL, 0);
            BtDev_LibExc();
            if (dev->p_dev_status->sniff_flag == 0)
            {
                break;
            }
            sys_mdelay(2);
        }
    }
}

void bt_dut_test(void)
{
    Send(0, BT_RESET, sizeof(BT_RESET));
    sys_mdelay(2000);
    Send(0, BT_ENABLE_ALLSCAN, sizeof(BT_ENABLE_ALLSCAN));
    sys_mdelay(2000);
    Send(0, BT_AUTOACCEPT_CONNECT, sizeof(BT_AUTOACCEPT_CONNECT));
    sys_mdelay(2000);
    Send(0, BT_ENABLE_DUT, sizeof(BT_ENABLE_DUT));
    sys_mdelay(2000);

    HCI_PRINTF("BT test mode Entered...\n");
    while (1)
    {
        clear_watch_dog();
    }
}

//enabled erroneous data reporting
void bt_set_default_errordata_report(void)
{
    HCI_DBG_PRINTF("set err data report cmd\n");
    Send(0, errordata_report_cmd, sizeof(errordata_report_cmd));
}

void bt_set_address(void)
{
    uint8 reset_buf[10];
    uint8 i;
    reset_buf[0] = 0x01;
    reset_buf[1] = 0x1a;
    reset_buf[2] = 0xfc;
    reset_buf[3] = 0x06;

    for (i = 0; i < 6; i++)
    {
        reset_buf[i + 4] = g_btengine_var.device_addr.bytes[i];
    }

    /* for BQB, fixed BT address. */
    if ((BQB_TEST_ENABLE & btengine_global_variable.bqb_test) != 0)
    {
        reset_buf[4] = 0x22;
        reset_buf[5] = 0x33;
        reset_buf[6] = 0x88;
    }

    HCI_PRINTF("set address\n");

    Send(0, reset_buf, 10);
}

//host buffer size
void bt_set_host_buffer_size(void)
{
    HCI_DBG_PRINTF("host buffer size cmd\n");
    Send(0, host_buffer_size_cmd, sizeof(host_buffer_size_cmd));
}

//获取当前role switch的角色(master/slave)
void bt_get_cur_role(bt_dev_t *dev)
{
    uint8 role_cmd[6];
    if ((dev != NULL) && (dev->p_dev_status != NULL) && (dev->p_dev_status->acl_handle != 0))
    {
        HCI_DBG_PRINTF("get cur role\n");
        role_cmd[0] = 0x01;
        role_cmd[1] = 0x09;
        role_cmd[2] = 0x08;
        role_cmd[3] = 0x02;
        role_cmd[4] = (uint8) (dev->p_dev_status->acl_handle);
        role_cmd[5] = (uint8) ((dev->p_dev_status->acl_handle) >> 8);
        Send(0, role_cmd, 6);
    }
}

#if (ROLE_SWITCH_RETRY_ENABLE == 1)

uint8 bt_get_role_sw_retry_id_by_dev(bt_dev_t *dev)
{
    uint8 i;
    for (i = 0; i < MAX_DEVICE_LINK; i++)
    {
        if (role_witch_retry[i].dev == dev)
        {
            break;
        }
    }
    return i;
}

uint8 bt_create_role_sw_retry(void)
{
    uint8 i;
    for (i = 0; i < MAX_DEVICE_LINK; i++)
    {
        if ((role_witch_retry[i].dev == NULL)
                || ((role_witch_retry[i].dev != btengine_global_variable.g_cur_rmt_dev)
                        && (role_witch_retry[i].dev != &devs_cfg.tws_paired_list)))
        {
            break;
        }
    }

    if (i >= MAX_DEVICE_LINK)
    {
        BTEG_PRINTF_ERROR("not recon dev\n");
        while (1)
        {
            ;
        }
    }

    return i;
}

void bt_role_switch_retry_deal(void)
{
    uint8 i;
    uint32 cur_time = sys_get_ab_timer();

    for (i = 0; i < MAX_DEVICE_LINK; i++)
    {
        if (role_witch_retry[i].dev == NULL)
        {
            continue;
        }

        if (role_witch_retry[i].need_switch == 0)
        {
            continue;
        }

        if ((cur_time - role_witch_retry[i].last_set_time) >= role_witch_retry[i].invl_time)
        {
            uint32 bak_last_set_time = role_witch_retry[i].last_set_time;
            role_witch_retry[i].last_set_time = cur_time;

            //如果已经转换成功，则退出retry
            if (role_witch_retry[i].switch_complete_cfm == 1)
            {
                role_witch_retry[i].need_switch = 0;
                continue;
            }

            //达到最大尝试次数后结束转换并且退出
            if (role_witch_retry[i].try_count >= role_witch_retry[i].try_times)
            {
                BTEG_PRINTF_WARNNING("role switch fail\n");
                role_witch_retry[i].switch_complete_cfm = 1;
                role_witch_retry[i].need_switch = 0;
                continue;
            }

            BTEG_PRINTF("role switch retry %d,cur_time %d,last_time %d,invl time %d", role_witch_retry[i].try_count,
                    role_witch_retry[i].last_set_time, bak_last_set_time, role_witch_retry[i].invl_time);
            bt_set_cur_role(role_witch_retry[i].dev, role_witch_retry[i].expect_role);
            role_witch_retry[i].try_count++;
        }
    }
}

void bt_role_change_ind(uint8 status, struct HCI_Switch_RoleStru *role_change)
{
#if (ROLE_SWITCH_RETRY_ENABLE == 1)
    uint8 id;
    bt_dev_t *dev = BT_RmtFindDevByBD(role_change->bd);
    id = bt_get_role_sw_retry_id_by_dev(dev);
    if (id >= MAX_DEVICE_LINK)
    {
        BTEG_PRINTF_WARNNING("not find dev in role switch retry\n");
        return;
    }

    if (status == HCI_STATUS_OK)
    {
        BTEG_PRINTF("role switch complete,new role %d\n", role_witch_retry[id].expect_role);
        role_witch_retry[id].switch_complete_cfm = 1;
        role_witch_retry[id].need_switch = 0;
        role_witch_retry[id].dev = NULL;
    }
#endif
}

//用带重复发送的方式设置当前role switch的角色问题(master/slave)
void bt_set_cur_role_with_retry(bt_dev_t *dev, uint8 role, uint16 retry_invl_ms, uint8 retry_times)
{
    uint8 id;
    bt_set_cur_role(dev, role);

    id = bt_get_role_sw_retry_id_by_dev(dev);
    if (id >= MAX_DEVICE_LINK)
    {
        id = bt_create_role_sw_retry();
    }

    role_witch_retry[id].dev = dev;
    role_witch_retry[id].expect_role = role;
    role_witch_retry[id].invl_time = retry_invl_ms;
    role_witch_retry[id].try_times = retry_times;
    role_witch_retry[id].switch_complete_cfm = 0;
    role_witch_retry[id].need_switch = 1;
    role_witch_retry[id].last_set_time = sys_get_ab_timer();
}
#endif

//设置当前role switch的角色问题(master/slave)
void bt_set_cur_role(bt_dev_t *dev, uint8 role)
{
    uint8 role_cmd[11];

    BTEG_PRINTF("dev %p,role %d\n", dev, role);

    if (dev != NULL)
    {
        HCI_DBG_PRINTF("set cur role\n");
        role_cmd[0] = 0x01;
        role_cmd[1] = 0x0b;
        role_cmd[2] = 0x08;
        role_cmd[3] = 0x07;
        libc_memcpy(&role_cmd[4], &dev->dev_info.bd_addr.bytes, 6);
        role_cmd[10] = role; //0-master;1-slave

        Send(0, role_cmd, 11);
    }
}
//取消创建连接
void cancel_create_connection(bt_dev_t *dev)
{
    uint8 role_cmd[10];

    HCI_PRINTF("cancel create connection\n");
    role_cmd[0] = 0x01;
    role_cmd[1] = 0x08;
    role_cmd[2] = 0x04;
    role_cmd[3] = 0x06;
    libc_memcpy(&role_cmd[4], &dev->dev_info.bd_addr.bytes, 6);
    Send(0, role_cmd, 10);
}

/*
 设置link loss time 5s
 */
void bt_write_link_supervision_timeout(uint16 aclhdl, uint16 timeout)
{
    uint8 link_supervision_cmd[8];

    HCI_DBG_PRINTF("set link supervision timeout\n");
    link_supervision_cmd[0] = 0x01;
    link_supervision_cmd[1] = 0x37;
    link_supervision_cmd[2] = 0x0c;
    link_supervision_cmd[3] = 0x04;
    link_supervision_cmd[4] = (uint8) aclhdl;
    link_supervision_cmd[5] = (uint8) (aclhdl >> 8);
    link_supervision_cmd[6] = timeout & 0xff;
    link_supervision_cmd[7] = timeout >> 8;

    Send(0, link_supervision_cmd, 8);
}

/* 
    获取远端设备的RSSI值
*/
void bt_get_rmt_rssi(uint16 aclhdl)
{
    uint8 get_rmt_rssi[6];

    HCI_DBG_PRINTF("get remote dev(%x) rssi\n",aclhdl);
    get_rmt_rssi[0] = 0x01;
    get_rmt_rssi[1] = 0x05;
    get_rmt_rssi[2] = 0x14;
    get_rmt_rssi[3] = 0x02;
    get_rmt_rssi[4] = (uint8) (aclhdl & 0xff);
    get_rmt_rssi[5] = (uint8) ((aclhdl >> 8) & 0xff);

    Send(0, get_rmt_rssi, 6);
}

#if 0
void get_link_supervision_timeout(bt_dev_t *dev)
{
    uint8 role_cmd[6];
    if ((dev != NULL) && (dev->p_dev_status->acl_handle != 0))
    {
        BTEG_PRINTF("\n!get link supervision timeout!\n");
        role_cmd[0] = 0x01;
        role_cmd[1] = 0x36;
        role_cmd[2] = 0x0c;
        role_cmd[3] = 0x02;
        role_cmd[4] = (uint8) (dev->p_dev_status->acl_handle);
        role_cmd[5] = (uint8)((dev->p_dev_status->acl_handle) >> 8);
        Send(0, role_cmd, 6);
    }
}
//配置acl链路断的超时时间,目前设置为5秒,该设置只有设备做master时设置才有效
void set_link_supervision_timeout(bt_dev_t *dev)
{
    uint8 role_cmd[8];
    uint16 timeout = 0x1f40; //time=timeout*0.625ms=0x1f40*0.625(ms)=5 s;
    if ((dev != NULL) && (dev->p_dev_status->acl_handle != 0))
    {
        BTEG_PRINTF("\n!set link supervision timeout!\n");
        role_cmd[0] = 0x01;
        role_cmd[1] = 0x37;
        role_cmd[2] = 0x0c;
        role_cmd[3] = 0x04;
        role_cmd[4] = (uint8) (dev->p_dev_status->acl_handle);
        role_cmd[5] = (uint8)((dev->p_dev_status->acl_handle) >> 8);
        role_cmd[6] = (uint8)timeout;
        role_cmd[7] = (uint8)(timeout >> 8);
        Send(0, role_cmd, 8);
    }
}
#endif
