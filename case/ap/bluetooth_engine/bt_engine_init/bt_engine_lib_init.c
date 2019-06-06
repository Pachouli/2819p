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
 * \file     bt_engine_lib_init.c
 * \brief    global variable模块
 * \author   xiaomaky
 * \version  1.0
 * \date  2013/4/11
 *******************************************************************************/
#include "bluetooth_engine.h"
#include "bt_engine_passthrough.h"
#include "bt_engine_le.h"

//打印配对列表的蓝牙名称
void print_paired_list(void)
{
    uint8 i;
    if ((devs_cfg.tag != PAIRED_INFO_TAG) || (devs_cfg.pl_len == 0))
    {
        BTEG_PRINTF("paired list is empty\n");
    }
    else
    {
        BTEG_PRINTF("paired list len %d\n", devs_cfg.pl_len, 2);
        for (i = 0; i < bt_manager_cfg_info.max_pair_list_num; i++)
        {
            if (devs_cfg.paired_list[i].data_validity == 0)
            {
                continue;
            }

            BTEG_PRINTF("idx %d mru %d : %s\n", i, devs_cfg.paired_list[i].mru_num,
                    (uint8 * ) &devs_cfg.paired_list[i].dev_info.name);

        }
    }
}

void btengine_auto_connect(void)
{
    bool need_try;

    //acl连接达到最大
    if (btengine_global_variable.connect_lock_flag >= g_btengine_cfg_data.max_link_num)
    {
        return;
    }

    if (g_p_info->need_auto_connect != 0)
    {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (bt_manager_cfg_info.sp_tws_link == 1)
        {
            //标记当前进行回连的设备为手机设备，用于交替回连手机和音箱
            tws_status_info.cur_reconnect_dev = NO_TWS_DEV;
            //标志正在回连手机，仅表示进行回连
            phone_connect_flag = TRUE;
        }
#endif

        //自动回连期间关闭可见和可连接，仅可配对
        bt_set_visual_mode(GAP_VM_PAIR);

        //回连设备有优先顺序，如果没有以下支持的profile,说明配置出错，退出回连
        //调用profile连接接口，如果acl链路没有建立，底层会自动发起acl链路建立
        need_try = TRUE;
        if (((btengine_global_variable.g_cur_rmt_dev->profile & (uint8) HFP_SUPPORTED) != 0)
                && (bt_manager_cfg_info.sp_hfp == 1))
        {
            BTEG_PRINTF("btengine reconn hfp\n");
            HFConnectAG(btengine_global_variable.g_cur_rmt_dev);
            need_try = FALSE;
        }
        if (need_try == TRUE)
        {
            if ((((btengine_global_variable.g_cur_rmt_dev->profile & (uint8) A2DP_SUPPORTED) != 0)
                    || ((btengine_global_variable.g_cur_rmt_dev->profile & (uint8) AVRCP_SUPPORTED) != 0))
                    && (g_btengine_cfg_data.enable_a2dp_flag == 1))
            {
                BTEG_PRINTF("btengine reconn a2dp\n");
                A2dpConnect(0, btengine_global_variable.g_cur_rmt_dev->dev_info.bd_addr.bytes);
                need_try = FALSE;
            }
        }
        if (need_try == TRUE)
        {
            if (((btengine_global_variable.g_cur_rmt_dev->profile & (uint8) HID_SUPPORTED) != 0)
                    && (bt_manager_cfg_info.sp_hid == 1))
            {
                BTEG_PRINTF("btengine reconn hid\n");
                HID_connect(btengine_global_variable.g_cur_rmt_dev);
                need_try = FALSE;
            }
        }

        if (need_try == TRUE)
        {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
            //回连手机结束
            phone_connect_flag = FALSE;
#endif
            return; //QAC
        }
    }
}

//
void boot_reconnect_deal(void)
{
    //蓝牙初始化后第一次自动回连标志
    bt_auto_connect_cfg.first_reconnect_after_btinit_flag = 1;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (bt_manager_cfg_info.sp_tws_link == 1)
    {
        //标记开机回连
        bt_auto_connect_cfg.tws_connect_type = AUTO_CONNECT_TYPE_STARTUP;
        //先获取需要回连的最后一次连接的手机信息
        tws_reconnect_lastphone_init(btengine_global_variable.g_cur_rmt_dev);

        //需要组队回连
        if (tws_status_info.have_paired_dev == TRUE)
        {
            //主箱开机可以立即回连，副箱需要等待半秒定时器启动回连
            if (devs_cfg.tws_paired_list.is_source == 1)
            {
                //如果主箱先回连组队或者没有需要已配对的手机记录，那么立刻回连主箱
                if ((bt_auto_connect_cfg.auto_connect_sequence == 1) || (devs_cfg.pl_len == 0))
                {
                    if (TWS_ManuPaired() != FALSE)
                    {
                        return;
                    }
                }
            }
            else
            {
                //稍后在半秒定时器中回连副箱
                tws_status_info.need_repaired = TRUE;
                return;
            }
        }
    }
#endif

    //warning ： 非测试模式下执行以下代码
    if (bt_test_active() == FALSE)
    {
        reconnect_last_device(btengine_global_variable.g_cur_rmt_dev);
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (bt_manager_cfg_info.sp_tws_link == 1)
        {
            //如果开机时先回连手机并且有已配对过的TWS设备，那么标记tws设备需要回连
            if (tws_status_info.have_paired_dev == TRUE)
            {
                tws_status_info.need_repaired = TRUE;
            }
        }
#endif
    }
}

/*
 Description:
 Confirmation to the asynchronous function call.
 Parameters:
 context     [in] Application defined data structure used to identify the request.
 result      [in] The result of the requested procedure. It may be:
 HCI_STATUS_OK - bonding is successful.
 HCI_STATUS_HOST_NOT_READY - The Host Stack is not initialized.
 HCI_STATUS_LOCAL_HOST_TIMEOUT - The supervision timer expires
 before receiving the expected complete event.
 HCI_STATUS_CONTROLLER_UNAVAILABLE - The Controller is not ready
 to execute any command.
 Other HCI error code returned by the Controller, e.g.
 HCI_STATUS_AUTHENTICATION_FAILURE.
 cfm_par     [in] Aditional information of the confirmation. Refer to the following
 specific confirmation callback definition. It is set to NULL if
 not specified additionally.
 */
void BT_GAP_SetLocalNameCfmCbk(void *context, UINT16 result, void *cfm_par)
{
    // general discoverable, page and inquiry scan enable, allow pairing
#ifdef TWS_SLAVE_NO_DISC_PAIRED_BEFORE
    if((tws_status_info.have_paired_dev == TRUE)&&(devs_cfg.tws_paired_list.is_source == 0))
    {
        bt_set_visual_mode (GAP_VM_NONDISCOV);
    }
    else
    {
        bt_set_visual_mode (GAP_VM_DEFAULT);
    }
#else
    bt_set_visual_mode(GAP_VM_DEFAULT); //设置设备名后再打开可见性
#endif

    if (result == HCI_STATUS_OK)
    {
        BTEG_PRINTF("wait paired\n");
        bt_SetFasterScanMode();
        print_paired_list();

        boot_reconnect_deal();
    }
    else
    {
        if (result == HCI_STATUS_HOST_NOT_READY)
        {
            BTEG_PRINTF_WARNNING("fsminst is null\n");
        }
        else if (result == HCI_STATUS_INVALID_PARAMETERS)
        {
            BTEG_PRINTF_WARNNING("device_name is null or name_len is too long\n");
        }
        else
        {
            ; //QAC need
        }
    }

}

UINT16 GapStart(void)
{
    UINT16 result;
    UCHAR dev_class[3];
    //{ 0X04, 0X04, 0X24 }; // Handsfree fixed a2dp requirement
    uint32 tmp32;
    uint8 link_policy_flag = 0;
    //0x200404; 单HFP服务
    //      UCHAR dev_name[] = "AudioDemo_MKY";

    tmp32 = btengine_param.class_device;
    dev_class[0] = (uint8) (tmp32); //minor device
    dev_class[1] = (uint8) (tmp32 >> 8); //major device
    dev_class[2] = (uint8) (tmp32 >> 16); //major service

    //    if(g_btengine_cfg_data.enable_a2dp_flag == 0)
    //    {
    //        dev_class[2] = 0x20; //单HFP服务，图标显示电话
    //    }

    do
    {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (bt_manager_cfg_info.sp_tws_link == 1)
        {
            total_acl_pktnums = (HCI_Get_Acl_packet_num(0));
            tws_max_acl_pktnums = (total_acl_pktnums >> 1) + 1;
            BTEG_DBG_PRINTF("tws max acl %d\n", tws_max_acl_pktnums, 2);
            link_policy_flag |= HCI_LINK_POLICY_SETTINGS_ENABLE_ROLE_SWITCH;
            link_policy_flag |= HCI_LINK_POLICY_SETTINGS_ENABLE_HOLD_MODE;
        }
#endif
        if (bt_stack_cfg_info.sp_sniff_enable == FALSE)
        {
            /* disable sniff mode.   */
            link_policy_flag |= HCI_LINK_POLICY_SETTINGS_ENABLE_ROLE_SWITCH;
        }
        if (link_policy_flag != 0)
        {
            bt_set_default_link_policy(link_policy_flag); //TODO 该接口用法很奇怪，需要好好理解一下
        }

        // write the local device class
        BTSRC_GAP_SetLocalDeviceClassA(0, NULL, dev_class, NULL);

        // set the local device name
        //              BTSRC_GAP_SetLocalNameA(NULL, NULL, dev_name, sizeof(dev_name), BT_GAP_SetLocalNameCfmCbk);
        BTSRC_GAP_SetLocalNameA(0, NULL, g_btengine_var.device_name, libc_strlen(g_btengine_var.device_name) + 1,
                BT_GAP_SetLocalNameCfmCbk);

        // set the local IO Capability
        if (bt_stack_cfg_info.sp_ssp_enable == FALSE)
        {
            result = (UINT16) BTSRC_GAP_SetIOCapability((uint8 ) HCI_IO_CAPABILITY_KEYBOARDONLY); //HCI_IO_CAPABILITY_NOINPUTNOOUTPUT
        }
        else
        {
            result = (UINT16) BTSRC_GAP_SetIOCapability((uint8 ) HCI_IO_CAPABILITY_NOINPUTNOOUTPUT);
        }

        /* By default, security mode 4 is enabled for BT2.1 and later device;
         security mode 2 is enabled for pre-BT2.1 device.
         If the security mode 3 is required, the APP shall issue Authentication_Eanble_Command itself. */

#if (SUPPORT_SCO_ERR_REPORT == 1)
        //enable sco error report for PLC
        bt_set_default_errordata_report();
#endif
    } while (0);

    return result;
}

void A2dpRegSbcSep(void)
{
    struct A2DP_SEPStru *sep = NULL;
    UINT8 len;
    UINT8 *p;
    uint8 seq_num;

    len = 10;

    if (((BQB_TEST_ENABLE | BQB_DELAY_REPORT_ENABLE) & btengine_global_variable.bqb_test)
            == (BQB_TEST_ENABLE | BQB_DELAY_REPORT_ENABLE))
    {
        len = len + 2;/*If Delay Reporting supported, two more bytes required. A2DP v1.3 */
    }

    if (bt_manager_cfg_info.sp_a2dp_cp == 2)
    {
        len = len + 4;
    }

    sep = BTSRC_List_NodeNew(sizeof(struct A2DP_SEPStru) + len);
    libc_memset(sep, 0, sizeof(struct A2DP_SEPStru) + len);
    sep->mask = APP_A2DP_SEPMASK_ISSNKSVC;
    sep->media_type = (A2DP_MEDIATYPE_AUDIO << 4) | (A2DP_TSEP_SNK << 3);
    sep->sc.len = len;

    p = sep->sc.service_capability;
    seq_num = 0;
    p[seq_num] = A2DP_SC_MEDIA_TRANSPORT;
    p[seq_num + 1] = 0;
    p[seq_num + 2] = A2DP_SC_MEDIA_CODEC;
    p[seq_num + 3] = 6;
    p[seq_num + 4] = A2DP_MEDIATYPE_AUDIO << 4;
    p[seq_num + 5] = A2DP_CODEC_SBC;
    p[seq_num + 6] = A2DP_SBC_SF_ALL | A2DP_SBC_CHMODE_ALL; /* some phone(XT319) need support all SF. */
    p[seq_num + 7] = A2DP_SBC_BLOCK_ALL | A2DP_SBC_SUBBAND_ALL | A2DP_SBC_ALLOCATION_ALL;
    p[seq_num + 8] = 2;
    p[seq_num + 9] = btengine_global_variable.sbc_max_bitpool;
    seq_num = seq_num + 10;

    if (((BQB_TEST_ENABLE | BQB_DELAY_REPORT_ENABLE) & btengine_global_variable.bqb_test)
            == (BQB_TEST_ENABLE | BQB_DELAY_REPORT_ENABLE))
    {
        p[seq_num] = A2DP_SC_DELAY_REPORTING;
        p[seq_num + 1] = 0;
        seq_num = seq_num + 2;
    }

    /* support Content Protection.  */
    if (bt_manager_cfg_info.sp_a2dp_cp == 2)
    {
        p[seq_num] = A2DP_SC_CONTENT_PROTECTION;
        p[seq_num + 1] = 0x02;
        p[seq_num + 2] = (uint8) (BTA_AV_CP_SCMS_T_ID);
        p[seq_num + 3] = (uint8) (BTA_AV_CP_SCMS_T_ID >> 8);
        seq_num = seq_num + 4;
    }
    /* Feature of A2DP v1.3 *//*some phone not support. */
    //sep->sc.service_capability[10] = A2DP_SC_DELAY_REPORTING;
    BTSRC_A2DP_Register_SEP(sep);
}

void A2dpRegAacSep(void)
{
    struct A2DP_SEPStru *sep = NULL;
    UINT8 len;
    UINT8 *p;

    //AAC as an option-codec, should register later, for some non-iphone
    if (bt_manager_cfg_info.sp_avdtp_aac == 1)
    {
        len = 12;

        if (bt_manager_cfg_info.sp_a2dp_cp == 2)
        {
            len = 16;
        }
        sep = BTSRC_List_NodeNew(sizeof(struct A2DP_SEPStru) + len);
        libc_memset(sep, 0, sizeof(struct A2DP_SEPStru) + len);
        sep->mask = APP_A2DP_SEPMASK_ISSNKSVC;
        sep->media_type = (A2DP_MEDIATYPE_AUDIO << 4) | (A2DP_TSEP_SNK << 3);
        sep->sc.len = len;
        p = sep->sc.service_capability;
        p[0] = A2DP_SC_MEDIA_TRANSPORT;
        p[1] = 0;
        p[2] = A2DP_SC_MEDIA_CODEC;
        p[3] = 8;
        p[4] = A2DP_MEDIATYPE_AUDIO << 4;
        p[5] = A2DP_CODEC_AAC;
        p[6] = 0xf0;
        p[7] = 0xff;
        p[8] = 0xff;
        p[9] = 0xff;
        p[10] = 0xff;
        p[11] = 0xff;

        /* support Content Protection.  */
        if (bt_manager_cfg_info.sp_a2dp_cp == 2)
        {
            p[12] = A2DP_SC_CONTENT_PROTECTION;
            p[13] = 0x02;
            p[14] = (uint8) (BTA_AV_CP_SCMS_T_ID);
            p[15] = (uint8) (BTA_AV_CP_SCMS_T_ID >> 8);
        }

        BTSRC_A2DP_Register_SEP(sep);
    }
}

void A2dpRegSep(void)
{
    A2dpRegSbcSep();
    A2dpRegAacSep();
}

void A2dpRegSnkService(void)
{

    if (!g_btengine_cfg_data.s_snk_svc_hdl)
    {
        g_btengine_cfg_data.s_snk_svc_hdl = BTSRC_A2DP_RegisterService(A2DP_TSEP_SNK, A2DP_PLAYER);
    }
    A2dpRegSep();
}

void APP_A2DP_Start(void)
{
    struct A2DP_RegCbkStru *in;

    in = NEW(sizeof(struct A2DP_RegCbkStru));
    in->media_type = A2DP_MEDIATYPE_AUDIO;
    in->cbk = A2dpIndCbk;
    BTSRC_A2DP_RegCbk(in);

    A2dpRegSnkService();
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (bt_manager_cfg_info.sp_tws_link == 1)
    {
        TWS_A2dpRegSrcService();
    }
#endif
}

void APP_A2DP_Stop(void)
{
    uint8 i;

    for (i = 0; i < (g_btengine_cfg_data.max_link_num * 2); i++)
    {
        if ((g_btengine_cfg_data.s_local_snk_seid != NULL) && (g_btengine_cfg_data.s_local_snk_seid[i] != NULL))
        {
            BTSRC_A2DP_UnRegister_SEP(g_btengine_cfg_data.s_local_snk_seid[i]->seid);
            FREE(g_btengine_cfg_data.s_local_snk_seid[i]);
            g_btengine_cfg_data.s_local_snk_seid[i] = NULL;
        }
    }

    if (g_btengine_cfg_data.s_snk_svc_hdl != 0)
    {
        BTSRC_A2DP_UnregisterService(g_btengine_cfg_data.s_snk_svc_hdl);
        g_btengine_cfg_data.s_snk_svc_hdl = 0;
    }
}

void APP_AVRCP_Start(void)
{
    BTSRC_AVRCP_ServerStart(AVRCP_CT, 0); /* CT support all features */
    if (bt_manager_cfg_info.sp_avrcp_vol_syn != 0)
    {
        BTSRC_AVRCP_ServerStart(AVRCP_TG, AVTG_CATEGORY_2); /* volume changed should support TG */
    }
    BTSRC_AVRCP_RegCbk((UINT8 *) APP_AVRCP_Cbk);
}

void APP_AVRCP_Stop(void)
{
    BTSRC_AVRCP_ServerStop();
}

void AppStart(void)
{
    GapStart();

    if (bt_manager_cfg_info.sp_hid == 1)
    {
        APP_HID_Start();
    }
    if (g_btengine_cfg_data.enable_a2dp_flag == 1)
    {
        APP_A2DP_Start();
        APP_AVRCP_Start();
    }

    if (bt_manager_cfg_info.sp_hfp == 1)
    {
        APP_HFP_Start();
    }
#ifdef CONFIG_PHONEBOOK
    if (g_btengine_cfg_data.hfp_pb_mode != HFP_PB_DISABLE)
    {
        pb_init();
    }
#endif

#ifdef BT_SUPPORT_BLE
    if (bt_manager_cfg_info.sp_ble == 1)
    {
        App_att_start();
    }
#endif

    if (bt_manager_cfg_info.sp_hid == 1)
    {
        APP_HID_REG();
    }

#ifdef BT_SUPPORT_SPP
    if (bt_manager_cfg_info.sp_spp == 1)
    {
        if (SppAppCbk != NULL)
        {
            App_Spp_StartServerReq(SppAppCbk, 7, 2); //SPP server start
        }
    }
#endif

    gap_reg_sco_cbk();
}

void AppStop(void)
{
    if (g_btengine_cfg_data.enable_a2dp_flag == 1)
    {
        APP_A2DP_Stop();
        APP_AVRCP_Stop();
    }

    if (bt_manager_cfg_info.sp_hfp == 1)
    {
        APP_HFP_Stop();
    }

#ifdef BT_SUPPORT_BLE
    if (bt_manager_cfg_info.sp_ble == 1)
    {
        App_att_stop();
    }
#endif

#ifdef BT_SUPPORT_SPP
    if (bt_manager_cfg_info.sp_spp == 1)
    {
        SppStop();
    }
#endif

    gap_unreg_sco_cbk();
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 Description:
 FreeTransportLayer release all the resource related to the transport layer.

 Arguments:
 Return Value:
 ---------------------------------------------------------------------------*/
void FreeTransportLayer(void)
{
    BTSRC_GAP_UnregisterTransportLayerA(1, NULL);
}

/*
 Description:
 Confirmation to the GAP_ResetHardwareA.
 Parameters:
 context     [in] Application defined data structure used to identify the request.
 result      [in] The result of the last command executed. It may be:
 HCI_STATUS_OK - All the mandatory commands are executed
 successfully.
 HCI_STATUS_HOST_NOT_READY - The Host Stack is not initialized.
 HCI_STATUS_LOCAL_HOST_TIMEOUT - The supervision timer expires
 before receiving the expected complete event.
 HCI_STATUS_CONTROLLER_UNAVAILABLE - The Controller is not ready
 to execute any command.
 Other HCI error code returned by the Controller, e.g.
 HCI_STATUS_UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE.
 cfm_par     [in] Aditional information of the confirmation. It is reserved
 for future extension and shall be ignored.
 */
void BT_GAP_ResetHardwareCfmCbk(void *context, UINT16 result, void *cfm_par)
{
    uint32 temp_flag;

    if (result == HCI_STATUS_OK)
    {
        BTEG_PRINTF("reset hardware OK\n");

        // initializing the message thread
        BTSRC_InitRxLoop();

        AppStart();

        //可以配置DEBUG接口
        temp_flag = sys_local_irq_save();
        if (btengine_global_variable.btdbg_sel_flag == 1)
        {
            con_set_debug_sel(btengine_global_variable.btdbg_sel_value);
        }
        btengine_global_variable.btdbg_sel_flag = 2;
        sys_local_irq_restore(temp_flag);
    }
    else
    {
        if (result == HCI_STATUS_HOST_NOT_READY)
        {
            BTEG_PRINTF_WARNNING("fsminst is null\n");
        }
        BTEG_PRINTF_ERROR("reset hardware failed\n");
        //发生硬件错误
        g_status_bak = get_cur_device_status(btengine_global_variable.g_cur_rmt_dev);
        set_cur_device_status(btengine_global_variable.g_cur_rmt_dev, STATUS_ERROR);
        g_p_info->err_status = BT_ERR_HW;

        //模组断电
        BtDev_PowerDown();
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 *    初始化lib.
 * \param[in]    none.
 * \param[in]    none.
 * \param[out]   none
 * \return       none.
 * \retval       1 sucess
 * \retval       0 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note         called by BtDev_Open()
 *******************************************************************************/
int32 BtDev_LibInit(void)
{
#ifdef BT_SUPPORT_SPP
    if (bt_manager_cfg_info.sp_spp == 1)
    {
        SppStart();
    }
#endif

#if (BT_ENGINE_PASSTHROUTH_SUPPORT == 1)
    bt_engine_passthrough_init();
#endif    

    BTSRC_GAP_RegisterIndCbk(NULL, GAP_IND_ALL, AppGapCbk);
    // Register transport layer instance(s) to the stack
    //InitTransportLayer();

    /* Then Call HCI_Controller_Init() to initialize the Bluetooth hardware
     The port_name parameter is read from bttl.ini in this sample
     Local Controller init inside */
    if (bt_stack_cfg_info.sp_ssp_enable == FALSE)
    {
        BTSRC_GAP_ResetHardwareA(TRUE, NULL, BT_GAP_ResetHardwareCfmCbk);
    }
    else
    {
        BTSRC_GAP_ResetHardwareA(FALSE, NULL, BT_GAP_ResetHardwareCfmCbk);
    }
    return 1;
}

void BT_SetReconnectFlag(uint8 dev_type, uint8 flag)
{
    bool phone_dev_flag = TRUE;
    if (flag == g_p_info->isneed_reconnect_flag)
    {
        return;
    }
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (bt_manager_cfg_info.sp_tws_link == 1)
    {
        phone_dev_flag = FALSE;
        if (dev_type != TWS_DEV)
        {
            phone_dev_flag = TRUE;
        }
    }
#endif
    if (phone_dev_flag == TRUE)
    {
        g_p_info->isneed_reconnect_flag = flag;
        //超时回连时，先延时1s后立即发起回连，之后根据配置回连时间间隔发起回连
        if (flag == 1)
        {
            btengine_global_variable.reconnect_status.halfsec_time_count =
                    btengine_global_variable.reconnect_status.reconn_interval * COM_AUTOCONNECT_TICK_INTEVAL - 1;
        }
        BTEG_DBG_PRINTF("set rect flag %d\n", flag);
    }
}
