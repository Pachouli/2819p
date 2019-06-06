/********************************************************************************
 *        Copyright(c) 2014-2015 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：btstack tws设备服务注册，搜索等。
 * 作者：kehaitao
 ********************************************************************************/
#include "./../bluetooth_engine.h"

#ifdef ENABLE_TRUE_WIRELESS_STEREO

void TWS_A2dpRegSrcService(void)
{
    struct A2DP_SEPStru *sep = NULL;
    //UINT8 len = 12; /* If Delay Reporting supported, two more bytes required. A2DP v1.3 */
    UINT8 len = 10;

    if (!g_btengine_cfg_data.s_src_svc_hdl)
    {
        g_btengine_cfg_data.s_src_svc_hdl = BTSRC_A2DP_RegisterService(A2DP_TSEP_SRC, A2DP_PLAYER);
    }

    if (!g_btengine_cfg_data.s_local_src_seid)
    {
        len = 10;
        sep = BTSRC_List_NodeNew(sizeof(struct A2DP_SEPStru) + len);
        libc_memset(sep, 0, sizeof(struct A2DP_SEPStru) + len);
        sep->mask = APP_A2DP_SEPMASK_ISSRCSVC;
        sep->media_type = (A2DP_MEDIATYPE_AUDIO << 4) | (A2DP_TSEP_SRC << 3);
        sep->sc.len = len;
        sep->sc.service_capability[0] = A2DP_SC_MEDIA_TRANSPORT;
        sep->sc.service_capability[1] = 0;
        sep->sc.service_capability[2] = A2DP_SC_MEDIA_CODEC;
        sep->sc.service_capability[3] = 6;
        sep->sc.service_capability[4] = A2DP_MEDIATYPE_AUDIO << 4;
        sep->sc.service_capability[5] = A2DP_CODEC_SBC;
        sep->sc.service_capability[6] = A2DP_SBC_SF_ALL | A2DP_SBC_CHMODE_ALL; /* some phone(XT319) need support all SF. */
        sep->sc.service_capability[7] = A2DP_SBC_BLOCK_ALL | A2DP_SBC_SUBBAND_ALL | A2DP_SBC_ALLOCATION_ALL;
        sep->sc.service_capability[8] = 2;
        sep->sc.service_capability[9] = btengine_global_variable.sbc_max_bitpool;
        //sep->sc.service_capability[10] = A2DP_SC_DELAY_REPORTING;   /* Feature of A2DP v1.3 */  /*some phone not support. */
        BTSRC_A2DP_Register_SEP(sep);
    }
}

uint8 bt_random(void)
{
    uint8 random_val;

    random_val = sys_random() % 6;

    TWS_DBG_PRINTF("random_val %d\n",random_val);

    return random_val;
}

void TWS_RmtSearchDev(void)
{
    struct GAP_DeviceDiscoveryReqStru req =
    { 0 };
    TWS_SetPairedStatus(TWS_SEARCH);
    tws_status_info.search_device_flag = 1;
    req.flag = GAP_DEVICE_DISCOVERY_BREDR | GAP_DEVICE_DISCOVERY_NAME_REQUIRED;
    //req.inquiry_len = (uint8)((200+bt_random()*100) / 128 + bt_random());
    req.inquiry_len = (uint8) (512 / 128);
    req.num_responses = 30;

    //TWS手动连接启动搜索，临时提高频率
    bt_adjust_clk(BT_LINK_WORK_FREQ);
    TWS_PRINTF("start searching...\n");
    bt_set_visual_mode(GAP_VM_PAIR);
    if (BTSRC_GAP_DeviceDiscovery((HANDLE)NULL, NULL, &req, NULL) != HCI_STATUS_OK)
    {
        TWS_PRINTF_WARNNING("Fail to initiate device search\n");
        if ((get_cur_device_status(btengine_global_variable.g_cur_rmt_dev) == STATUS_WAIT_PAIRED)
                || (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev) == STATUS_NONE))
        {
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
            bt_set_visual_mode(GAP_VM_DEFAULT);
#endif
        }
        TWS_SetPairedStatus(TWS_IDLE);
        tws_status_info.search_device_flag = 0;
    }
}

bool __section__(".rcode") tws_get_bt_clk_inner(bt_clock_t *p_bt_clock)
{
    uint16 acl_handle;

    if (tws_status_info.acl_handle != 0)
    {
        acl_handle = tws_status_info.acl_handle;
    }
    else
    {
        return FALSE;
    }

    con_get_bt_clk(acl_handle, p_bt_clock);

    return TRUE;
}

bool tws_set_bt_clk_inner(uint32 inform_clk)
{
    uint16 acl_handle;

    if (tws_status_info.acl_handle != 0)
    {
        acl_handle = tws_status_info.acl_handle;
    }
    else
    {
        return FALSE;
    }

    con_set_bt_clk(acl_handle, inform_clk);

    return TRUE;
}

void tws_register_bt_clk_sync_cbk_inner(f_bt_clk_sync_cbk bt_clk_sync_cbk)
{
    con_general_register_cbk(bt_clk_sync_cbk, NULL, NULL, 0, 3);
}

bool tws_has_avdtp_pkg_lost_inner(void)
{
    return has_avdtp_pkg_lost();
}
void tws_buffer_manager_init(void)
{
    //创建蓝牙免提Input/Output Buffer管理结构体共享内存
    libc_memset(&g_tws_mmm_m2s_pipe, 0x00, sizeof(buffer_rw_t));
    g_tws_mmm_m2s_pipe.raw_data = (uint8 *) (g_tws_mmm_m2s_buffer);
    g_tws_mmm_m2s_pipe.ext_data = NULL;
    g_tws_mmm_m2s_pipe.capacity = sizeof(g_tws_mmm_m2s_buffer);

    if (sys_shm_creat(SHARE_MEM_ID_TWS_MMM_M2S, &g_tws_mmm_m2s_pipe, sizeof(buffer_rw_t)) == -1)
    {
        while (1)
            ;
    }

    libc_memset(&g_tws_mmm_m2s_play_sync_cmd, 0x00, sizeof(tws_sync_cmd_t));
    if (sys_shm_creat(SHARE_MEM_ID_TWS_MMM_M2S_PLAY, &g_tws_mmm_m2s_play_sync_cmd, sizeof(tws_sync_cmd_t)) == -1)
    {
        while (1)
            ;
    }

    libc_memset(&g_tws_mmm_s2m_pipe, 0x00, sizeof(buffer_rw_t));
    g_tws_mmm_s2m_pipe.raw_data = (uint8 *) (g_tws_mmm_s2m_buffer);
    g_tws_mmm_s2m_pipe.ext_data = NULL;
    g_tws_mmm_s2m_pipe.capacity = sizeof(g_tws_mmm_s2m_buffer);

    if (sys_shm_creat(SHARE_MEM_ID_TWS_MMM_S2M, &g_tws_mmm_s2m_pipe, sizeof(buffer_rw_t)) == -1)
    {
        while (1)
            ;
    }

    libc_memset(&g_tws_engine_m2s_pipe, 0x00, sizeof(buffer_rw_t));
    g_tws_engine_m2s_pipe.raw_data = (uint8 *) (g_tws_engine_m2s_buffer);
    g_tws_engine_m2s_pipe.ext_data = NULL;
    g_tws_engine_m2s_pipe.capacity = sizeof(g_tws_engine_m2s_buffer);

    if (sys_shm_creat(SHARE_MEM_ID_TWS_EG_M2S, &g_tws_engine_m2s_pipe, sizeof(buffer_rw_t)) == -1)
    {
        while (1)
            ;
    }

    libc_memset(&g_tws_engine_s2m_pipe, 0x00, sizeof(buffer_rw_t));
    g_tws_engine_s2m_pipe.raw_data = (uint8 *) (g_tws_engine_s2m_buffer);
    g_tws_engine_s2m_pipe.ext_data = NULL;
    g_tws_engine_s2m_pipe.capacity = sizeof(g_tws_engine_s2m_buffer);

    if (sys_shm_creat(SHARE_MEM_ID_TWS_EG_S2M, &g_tws_engine_s2m_pipe, sizeof(buffer_rw_t)) == -1)
    {
        while (1)
            ;
    }

    libc_memset(&g_tws_ui_m2s_pipe, 0x00, sizeof(buffer_rw_t));
    g_tws_ui_m2s_pipe.raw_data = (uint8 *) (g_tws_ui_m2s_buffer);
    g_tws_ui_m2s_pipe.ext_data = NULL;
    g_tws_ui_m2s_pipe.capacity = sizeof(g_tws_ui_m2s_buffer);

    if (sys_shm_creat(SHARE_MEM_ID_TWS_UI_M2S, &g_tws_ui_m2s_pipe, sizeof(buffer_rw_t)) == -1)
    {
        while (1)
            ;
    }

    libc_memset(&g_tws_ui_s2m_pipe, 0x00, sizeof(buffer_rw_t));
    g_tws_ui_s2m_pipe.raw_data = (uint8 *) (g_tws_ui_s2m_buffer);
    g_tws_ui_s2m_pipe.ext_data = NULL;
    g_tws_ui_s2m_pipe.capacity = sizeof(g_tws_ui_s2m_buffer);

    if (sys_shm_creat(SHARE_MEM_ID_TWS_UI_S2M, &g_tws_ui_s2m_pipe, sizeof(buffer_rw_t)) == -1)
    {
        while (1)
            ;
    }
}
void tws_buffer_manager_destroy(void)
{
    sys_shm_destroy(SHARE_MEM_ID_TWS_MMM_M2S);
    sys_shm_destroy(SHARE_MEM_ID_TWS_MMM_S2M);

    sys_shm_destroy(SHARE_MEM_ID_TWS_EG_M2S);
    sys_shm_destroy(SHARE_MEM_ID_TWS_EG_S2M);

    sys_shm_destroy(SHARE_MEM_ID_TWS_UI_M2S);
    sys_shm_destroy(SHARE_MEM_ID_TWS_UI_S2M);

    sys_shm_destroy(SHARE_MEM_ID_TWS_MMM_M2S_PLAY);

}
#endif

