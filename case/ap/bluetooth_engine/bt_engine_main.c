/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     mainmenu_main.c
 * \brief    MainMenu的main函数模块
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "bluetooth_engine.h"
#include  "sys_info.h"

/*全局变量定义*/

/*main中要填充这个值，由系统人员分配*/
INT8U prio = AP_BTENGINE_PRIO;

typedef void (*handler_ker)(void);

//构造一个空printf函数用于关闭打印
void btengine_invalid_printf(const uint8 *fmt, ...)
{

}

/******************************************************************************/
/*!
 * \par  Description:
 *    MainMenu 读取应用VM全局变量,初始化全局变量
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
static void _read_var(void)
{
    //get bt dev name/addr
    if (com_init_btengine_info(&g_btengine_var) == FALSE)
    {
        //本地设备信息错误重新初始化，远程设备信息也同步清空重新初始化
        clear_plist_info(CLEAR_ALL_DEV_PAIRED_INFO);
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        clear_tws_info();
#endif
    }

    init_plist_info();

    print_bt_addr(g_btengine_var.device_addr.bytes);

}

/******************************************************************************/
/*!
 * \par  Description:
 *    applib初始化
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
void _app_init(void)
{
    /*读入VM变量*/
    _read_var();

    //初始化applib库，后台AP
    applib_init(APP_ID_BTENGINE, THIS_APP_TYPE);

    /*初始化软定时器*/
    init_app_timers(THIS_APP_TIMER_GROUP, 5);

}

/******************************************************************************/
/*!
 * \par  Description:
 *    applib退出
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
void _app_exit(void)
{

    /*执行applib库的注销操作*/
    applib_quit(THIS_APP_TYPE);
}

void btengine_cfg_data_init(bt_common_cfg_t *p_common_cfg)
{
    //是否支持A2DP服务
    g_btengine_cfg_data.enable_a2dp_flag = p_common_cfg->support_a2dp_flag;

    //如果不支持btplay,那么默认关闭A2DP
    if (p_common_cfg->support_btplay_flag == 0)
    {
        g_btengine_cfg_data.enable_a2dp_flag = 0;
    }

    g_btengine_cfg_data.max_link_num = p_common_cfg->max_link_num;

    //获取电话本相关配置
    g_btengine_cfg_data.hfp_pb_mode = HFP_PB_ONLY_NUM; //仅支持电话号码播报

}

#if (SUPPORT_BT_VISIBLE_CONFIG == 1)
void btengine_visual_cfg_mode_init(uint8 mode)
{
    //可见性配置
    //global_visible控制全局可见性，这里初始化默认值。开启可见条件为长按CONENCT键，关闭可见条件为已连接。
    BTEG_DBG_PRINTF("\nvisual_cfg_mode:%d\n",mode);//bt_manager_cfg_info.visual_cfg_mode

    if(mode == VMODE_DEFAULT_VISIBLE)
    {
        btengine_global_variable.global_visible = 1;
    }
    else if (mode == VMODE_ONLY_FIRST_VISIBLE)
    {
        btengine_global_variable.global_visible = 1;
    }
    else if (mode == VMODE_DEFAULT_INVISIBLE)
    {
        btengine_global_variable.global_visible = 0;
    }
    else
    {
        btengine_global_variable.global_visible = 1;
    }
}
#endif

void global_variable_init(void)
{
    btengine_cfg_data_init(&btengine_param);

    libc_memset(&btengine_global_variable, 0x0, sizeof(btengine_global_variable_struct));

    g_p_info = &(btengine_global_variable.g_btengine_info);
    libc_memset(g_p_info, 0x0, sizeof(btengine_info_t));
    g_p_info->cur_role = ROLE_NULL;

    g_p_avrcp = &(btengine_global_variable.btengine_avrcp_data);

    g_p_sco = &btengine_sco_deal;

    btengine_global_variable.filter_avdtp_sound_flag = TRUE;
    btengine_global_variable.hfp.sco.filter_sco_input_flag = DATA_FILTER;
    btengine_global_variable.hfp.disconnect_reason = ER_HFP_RFCOMM_SUCCESS;
    btengine_global_variable.hfp.allow_sync_vol_to_ap_flag = FALSE;
    btengine_global_variable.hfp.sco.data_deal_irq_timer_id = -1;
    btengine_global_variable.hfp.get_call_status_timer_id = -1;
    init_sco_pkg_count_insecond();

    btengine_global_variable.reconnect_status.halfsec_time_count = 0;
    btengine_global_variable.reconnect_status.reconn_interval = bt_auto_connect_cfg.connect_interval;
    btengine_global_variable.reconnect_status.reconn_times_count = 0;
    btengine_global_variable.linking_dev = NULL;

    //config btmempool
    if (bt_manager_cfg_info.asqt_test_mode == 1)
    {
        btengine_global_variable.bt_stack_init_param_cfg.btmempool_cfg.head = (uint32) BT_ENGINE_HEAP_ADDRESS;
        btengine_global_variable.bt_stack_init_param_cfg.btmempool_cfg.total_size = BT_ENGINE_ASQT_HEAP_SIZE;
    }
    else if (btengine_param.att_test_flag == TRUE)
    {
        btengine_global_variable.bt_stack_init_param_cfg.btmempool_cfg.head = (uint32) BT_ENGINE_HEAP_ADDRESS;
        btengine_global_variable.bt_stack_init_param_cfg.btmempool_cfg.total_size = TEST_POOL_SIZE;
    }
    else
    {
        btengine_global_variable.bt_stack_init_param_cfg.btmempool_cfg.head = (uint32) BT_ENGINE_HEAP_ADDRESS;
        btengine_global_variable.bt_stack_init_param_cfg.btmempool_cfg.total_size = BT_ENGINE_HEAP_SIZE;
    }
    btengine_global_variable.bt_stack_init_param_cfg.bt_page_scan_actwin = BT_PAGE_SCAN_ACT_WIN_200;

    //SBC编码音频质量水平：0 高质量，1 中等质量
    if (bt_stack_cfg_info.sp_sbc_audio_quality == 0)
    {
        btengine_global_variable.sbc_max_bitpool = SBC_MAX_BITPOOL_HIGH_QUALITY;
    }
    else
    {
        btengine_global_variable.sbc_max_bitpool = SBC_MAX_BITPOOL_MIDDLE_QUALITY;
    }

    btengine_global_variable.g_cur_rmt_dev = NULL;
    btengine_global_variable.hfp.sco.sco_info.sence = 0;
    btengine_global_variable.hfp.sco.sco_info.encode_type = SCO_DATA_ENCODE_FORMAT_CVSD;
    btengine_global_variable.hfp.sco.sco_info.rx_data_transfer_mode = TRANSPARENT_TRANSMISSION;
    if (btengine_param.sco_postdeal_enable == TRUE)
    {
        btengine_global_variable.hfp.sco.sco_info.tx_data_transfer_mode = TRANSPARENT_TRANSMISSION;
    }
    else
    {
        btengine_global_variable.hfp.sco.sco_info.tx_data_transfer_mode = FORMAT_ENCODE_TRANSMISSION;
    }

    btengine_global_variable.media_codec_type = A2DP_CODEC_NONA2DP;
    btengine_global_variable.local_device_support_profile = 0x00;

    if (g_btengine_cfg_data.enable_a2dp_flag == 1)
    {
        btengine_global_variable.local_device_support_profile |= A2DP_SUPPORTED;
        btengine_global_variable.local_device_support_profile |= AVRCP_SUPPORTED;
    }

    if (bt_manager_cfg_info.sp_hfp == 1)
    {
        btengine_global_variable.local_device_support_profile |= HFP_SUPPORTED;
    }

    if (bt_manager_cfg_info.sp_hid == 1)
    {
        btengine_global_variable.local_device_support_profile |= HID_SUPPORTED;
    }
    btengine_global_variable.hfp.sco.create_sco_timer_id = -1;

    //关键信息打印接口
    if (btengine_param.stack_print_enable == TRUE)
    {
        btengine_global_variable.printf.log = (pfunc_printf) LIBC_PRINTF_ENTRY;
    }
    else
    {
        btengine_global_variable.printf.log = btengine_invalid_printf;
    }

    //调试信息打印接口
#ifdef BT_DEBUG_PRINT
    if (btengine_param.stack_print_enable == TRUE)
    {
        btengine_global_variable.printf.log_dbg = btengine_global_variable.printf.log;
    }
    else
#endif
    {
        btengine_global_variable.printf.log_dbg = btengine_invalid_printf;
    }

#if (SUPPORT_BT_VISIBLE_CONFIG == 1)
    btengine_visual_cfg_mode_init(bt_manager_cfg_info.visual_cfg_mode);
#endif

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (bt_manager_cfg_info.sp_tws_link == 1)
    {
        TWS_global_var_init();
    }
#endif

    btengine_global_variable.disable_bt_sniff_by_app = FALSE;
    btengine_global_variable.enable_calib_bt_freq_offset = FALSE;

    btengine_global_variable.avdtp_data_source = AVDTP_SOURCE_BT_A2DP;

    btengine_global_variable.sbc_data_stop_trans_cnt = MAX_BT_SBC_DATA_TRANS;
}
//蓝牙推歌和免提input/output buffer管理器初始化
void buffer_manager_init(void)
{

    //创建蓝牙推歌Input Buffer管理结构体共享内存
    libc_memset(&g_a2dp_input_buf, 0x00, sizeof(buffer_rw_t));
    if (sys_shm_creat(SHARE_MEM_ID_BTPLAYPIPE, &g_a2dp_input_buf, sizeof(buffer_rw_t)) == -1)
    {
        while (1)
            ;
    }

    //创建蓝牙免提Input/Output Buffer管理结构体共享内存
    libc_memset(&g_sco_input_buf, 0x00, sizeof(buffer_rw_t));
    g_sco_input_buf.raw_data = (uint8 *) (SCO_RECEIVE_CACHE_ADDRESS);
    libc_memset(&g_sco_err_report_info, 0x00, sizeof(g_sco_err_report_info));
    g_sco_err_report_info.err_report_data = (sco_err_report_t *) (SCO_ERR_REPORT_BUF_ADDRESS);
    g_sco_err_report_info.count = SCO_ERR_REPORT_COUNT;
    g_sco_input_buf.ext_data = (uint8 *) (&g_sco_err_report_info);
    g_sco_input_buf.capacity = HFP_SCO_RECEIVE_CACHE_LEN;

    if (sys_shm_creat(SHARE_MEM_ID_SCO_INPIPE, &g_sco_input_buf, sizeof(buffer_rw_t)) == -1)
    {
        while (1)
            ;
    }

    //sco output
    libc_memset(&g_sco_output_buf, 0x00, sizeof(buffer_rw_t));
    g_sco_output_buf.raw_data = (uint8 *) (SCO_OUTPUT_CACHE_ADDRESS);
    g_sco_output_buf.capacity = HFP_SCO_OUTPUT_CACHE_LEN;

    if (sys_shm_creat(SHARE_MEM_ID_SCO_OUTPIPE, &g_sco_output_buf, sizeof(buffer_rw_t)) == -1)
    {
        while (1)
            ;
    }
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (bt_manager_cfg_info.sp_tws_link == 1)
    {
        tws_buffer_manager_init();
    }
#endif
}
//蓝牙推歌和免提所用到的buffer释放，销毁
void buffer_manager_destroy(void)
{

    //销毁蓝牙推歌Input Buffer管理结构体共享内存
    sys_shm_destroy(SHARE_MEM_ID_BTPLAYPIPE);

    //销毁蓝牙免提Input/Output Buffer管理结构体共享内存
    sys_shm_destroy(SHARE_MEM_ID_SCO_INPIPE);
    sys_shm_destroy(SHARE_MEM_ID_SCO_OUTPIPE);

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (bt_manager_cfg_info.sp_tws_link == 1)
    {
        tws_buffer_manager_destroy();
    }
#endif
}
void get_cfg_info(btengine_param_t *bt_param)
{
    //get bt_config_param form bt_engine create input
    libc_memcpy(&btengine_param, &bt_param->bt_common_cfg, sizeof(bt_common_cfg_t));
    libc_memcpy(&bt_manager_cfg_info, &bt_param->bt_manager_cfg_info, sizeof(bt_manager_cfg_info_t));
    libc_memcpy(&bt_stack_cfg_info, &bt_param->bt_stack_cfg_info, sizeof(bt_stack_cfg_info_t));
    libc_memcpy(&bt_auto_connect_cfg, &bt_param->bt_auto_connect_cfg, sizeof(bt_auto_connect_cfg_t));

    //对其中一些限制做检查处理
#ifndef ENABLE_HFP_VOLUME_SYNC
    bt_manager_cfg_info.sp_hfp_vol_syn = 0;
#endif

#ifndef ENABLE_BATTERY_REPORT
    bt_manager_cfg_info.sp_hfp_bat_report = 0;
#endif

    if (bt_manager_cfg_info.max_pair_list_num > PAIRED_LIST_MAX)
    {
        bt_manager_cfg_info.max_pair_list_num = PAIRED_LIST_MAX;
    }
    if (bt_manager_cfg_info.max_pair_list_num == 0)
    {
        bt_manager_cfg_info.max_pair_list_num = 1;
    }
}

void btdbg_check_handle(void)
{
    uint32 btdbg_sel;

    if (com_enter_btdbg_flag(&btdbg_sel) == TRUE)
    {
        if (btengine_global_variable.btdbg_sel_flag == 2)
        {
            con_set_debug_sel(btdbg_sel);
        }
        else
        {
            btengine_global_variable.btdbg_sel_value = btdbg_sel;
            btengine_global_variable.btdbg_sel_flag = 1;
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    MainMenu 应用的入口函数和场景管理器模块
 * \param[in]    PARAM_FROM_XXX
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
int main(int argc, char **argv)
{
    app_result_e result = RESULT_NULL;
    int btdbg_check_timer_id = -1;

    /* step1 获取配置项内容、初始化蓝牙环境（bt_engine/bt_con/bt_host/bt_manage）*/
    get_cfg_info((btengine_param_t *) argv[0]); //配置参数详细配置请查看com_creat_bt_engine_inner()
    global_variable_init();
    bt_engine_freq_offset_config_init();
    bt_remote_dev_info_init();

    /* step2 ap初始化，分别执行从vram中还原bt_engine环境、app注册、定时器组初始化三件事*/
    _app_init();

    /* step3 数据缓冲空间分配以及初始化*/
    buffer_manager_init(); //蓝牙推歌和免提所使用的buf初始化管理

    /* step4 加载bt_hsot.drv,打开蓝牙*/
    bt_adjust_clk(BT_LINK_WORK_FREQ);      //打开蓝牙初始化阶段需要提频到104Hz，进入等待连接之后恢复为45MHz
    sys_drv_install(DRV_GROUP_BT_HOST, 0, "bt_host.drv");

    result = BtDev_Open();      //打开蓝牙
    if (result != RESULT_NULL)
    {
        goto quit_bt;
    }

    /* step5 设置定时检测任务*/
    btdbg_check_timer_id = sys_set_irq_timer1(btdbg_check_handle, 50);      //检测bt_con是否需要释放debug信号
    btdbg_check_handle(); //先主动执行一次

    half_second_timer_id = set_app_timer(THIS_APP_TIMER_GROUP, APP_TIMER_TAG_MAIN, APP_TIMER_ATTRB_CONTROL, 500,
            (timer_proc ) half_second_handle); //设置一个半秒检测定时器事件
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (bt_manager_cfg_info.sp_tws_link == 1)
    {
        send_data_10ms_timer = sys_set_irq_timer1(TWS_Send_10ms_Handle, 0x01); //主从箱同步信息实时性要求高，挂载在10ms中断执行
    }
#endif

    /* step6 应用消息事件处理循环，函数返回时表示退出此应用，result表示退出原因*/
    result = get_message_loop();

    /* step7 资源回收*/
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (bt_manager_cfg_info.sp_tws_link == 1)
    {
        sys_del_irq_timer1(send_data_10ms_timer);
        send_data_10ms_timer = -1;
    }
#endif
    kill_app_timer(THIS_APP_TIMER_GROUP, half_second_timer_id);
    half_second_timer_id = -1;

#ifdef CONFIG_PHONEBOOK
    if(btengine_global_variable.hfp.g_pbook_buf != NULL)
    {
        mem_free(btengine_global_variable.hfp.g_pbook_buf);
        btengine_global_variable.hfp.g_pbook_buf = NULL;
    }
#endif

    quit_bt:

    sys_del_irq_timer1((uint32 ) btdbg_check_timer_id);

    BtDev_Close();
    sys_drv_uninstall(DRV_GROUP_BT_HOST);
    bt_adjust_clk(FREQ_0M);
    //蓝牙推歌，免提所用到的buf释放，销毁
    buffer_manager_destroy();
    bt_remote_dev_info_deinit();

    /* step7 退出应用*/
    _app_exit();
    return result;
}
