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
#include "./../bluetooth_engine.h"
#include "hci_stru.h"
#include "bt_engine_le.h"

//关于FAST SCAN设置中参数说明
//  _______     _______     _______     _______
//_| window|___| window|___| window|___| window|___
// | interval  | interval  | interval  |
//scan的参数参考上图可知
//window影响了发现和连接的随机性，窗口越小，发现和连接的时间分步越分散
//interval决定了连接的速度，间隔约小，连接速度越快，休眠时间也越小，功耗也越大
//window取值不可超过interval,当window取值非常接近interval时，系统将无法进入休眠
const fast_scan_param_list_st fastscan_param_list[] =
{
{ 0x12, 0x1000, 0x12, 0x800 }, //core spec default scan param
        { 0x12, 0x800, 0x12, 0x800 }, //fast scan reference param1
        { 0x40, 0x200, 0x40, 0x200 }, //fast scan reference param2
        { 0x28, 0x400, 0x28, 0x400 }, //line in tws normal scan param
        { 0x40, 0x400, 0x40, 0x400 }, //bt_play tws normal scan param
        };

void bt_read_inquiry_page_scan_activity(void)
{
    uint8 reset_buf[11];
    reset_buf[0] = 0x01;
    reset_buf[1] = 0x1B;
    reset_buf[2] = 0x0c;
    reset_buf[3] = 0x00;
    Send(0, reset_buf, 4);

    reset_buf[0] = 0x01;
    reset_buf[1] = 0x1d;
    reset_buf[2] = 0x0c;
    reset_buf[3] = 0x00;
    Send(0, reset_buf, 4);
}

void bt_write_inquiry_scan_activity(uint16 window, uint16 interval)
{
    uint8 reset_buf[11];
    reset_buf[0] = 0x01;
    reset_buf[1] = 0x1E;
    reset_buf[2] = 0x0c;
    reset_buf[3] = 0x04;
    reset_buf[4] = (uint8) (interval & 0xff);
    reset_buf[5] = (uint8) (interval >> 8);
    reset_buf[6] = (uint8) (window & 0xff);
    reset_buf[7] = (uint8) (window >> 8);
    Send(0, reset_buf, 8);
}

void bt_write_page_scan_activity(uint16 window, uint16 interval)
{
    uint8 reset_buf[11];

    reset_buf[0] = 0x01;
    reset_buf[1] = 0x1C;
    reset_buf[2] = 0x0c;
    reset_buf[3] = 0x04;
    reset_buf[4] = (uint8) (interval & 0xff);
    reset_buf[5] = (uint8) (interval >> 8);
    reset_buf[6] = (uint8) (window & 0xff);
    reset_buf[7] = (uint8) (window >> 8);
    Send(0, reset_buf, 8);
}

void bt_Write_Inquiry_Scan_Type(uint8 mode)
{
    uint8 reset_buf[11];

    reset_buf[0] = 0x01;
    reset_buf[1] = 0x43;
    reset_buf[2] = 0x0c;
    reset_buf[3] = 0x01;
    reset_buf[4] = mode;
    Send(0, reset_buf, 5);
}

void bt_Write_Page_Scan_Type(uint8 mode)
{
    uint8 reset_buf[11];

    reset_buf[0] = 0x01;
    reset_buf[1] = 0x47;
    reset_buf[2] = 0x0c;
    reset_buf[3] = 0x01;
    reset_buf[4] = mode;
    Send(0, reset_buf, 5);
}

void bt_SetScanParam(uint8 param_num)
{
    const fast_scan_param_list_st *scan_param;
    scan_param = &fastscan_param_list[param_num];

    bt_write_inquiry_scan_activity(scan_param->inq_scan_win, scan_param->inq_scan_itv);
    bt_write_page_scan_activity(scan_param->page_scan_win, scan_param->page_scan_itv);
}

/*
 加快蓝牙被发现的速度和被连接的速度
 */
void bt_SetFasterScanMode(void)
{
    if (btengine_global_variable.fast_scan_status == ENABLE_FAST_SCAN)
    {
        GAP_PRINTF("already open\n");
        return;
    }
    if ((get_cur_device_status(btengine_global_variable.g_cur_rmt_dev) != STATUS_WAIT_PAIRED)
            && (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev) != STATUS_NONE))
    {
        return;
    }
    GAP_PRINTF("open fast scan\n");

#if (FAST_SCAN_MODE == 0)           
    bt_SetScanParam(FAST_SCAN_PARAM1);
#else
    bt_SetScanParam(FAST_SCAN_PARAM2);
#endif
    bt_Write_Inquiry_Scan_Type(HCI_INQUIRY_SCAN_TYPE_INTERLACED_SCAN); //0:Standard Scan  1:Interlaced Scan  2~0xff:Reserved
    bt_Write_Page_Scan_Type(HCI_PAGE_SCAN_TYPE_OPTIONAL_INTERLACED_SCAN); //0:Standard Scan  1:Interlaced Scan  2~0xff:Reserved
    btengine_global_variable.fast_scan_status = ENABLE_FAST_SCAN;
}

/*
 还原蓝牙被发现的默认速度和被连接的默认速度
 */
void bt_RestoreNormalScanMode(void)
{
    if (btengine_global_variable.fast_scan_status == DISABLE_FAST_SCAN)
    {
        return;
    }
    GAP_PRINTF("close fast scan\n");
    //BT_GetHciEventInfo(&hci_event_info);
    bt_SetScanParam(NORMAL_SCAN_PARAM_LINEIN_TWS);
    bt_Write_Inquiry_Scan_Type(HCI_INQUIRY_SCAN_TYPE_STANDARD_SCAN_DEFAULT);
    bt_Write_Page_Scan_Type(HCI_PAGE_SCAN_TYPE_MANDATORY_STANDARD_SCAN_DEFAULT);
    btengine_global_variable.fast_scan_status = DISABLE_FAST_SCAN;
}

UINT32 gap_sco_cbk(HANDLE hdl, void *context, void *par, UINT32 EventIndex);

struct HCI_CbkLineStru gap_sco_table[] =
{
{ (UINT16) HCI_EVI_SYNCHRONOUS_CONNECTION_COMPLETE, gap_sco_cbk },
{ (UINT16) HCI_EVI_DISCONNECTION_COMPLETE, gap_sco_cbk } };

bt_dev_t *HfpFindDevByScoHdl(uint16 sco_hdl)
{
    uint8 idx;
    bt_dev_t *p_dev;

    p_dev = NULL;

    for (idx = 0; idx < g_btengine_cfg_data.max_link_num; idx++)
    {
        if ((btengine_global_variable.linked_dev_list[idx] != NULL)
                && (btengine_global_variable.linked_dev_list[idx]->p_dev_status != NULL)
                && (btengine_global_variable.linked_dev_list[idx]->p_dev_status->sco_handle_setup == sco_hdl))
        {
            p_dev = btengine_global_variable.linked_dev_list[idx];
            break;
        }
    }
    return p_dev;
}

UINT32 gap_sco_cbk(HANDLE hdl, void *context, void *par, UINT32 EventIndex)
{
    bt_dev_t *dev;

    switch ((uint8) EventIndex)
    {
        case HCI_EVI_SYNCHRONOUS_CONNECTION_COMPLETE:
        {
            struct HCI_Synchronous_Connection_CompleteEvStru *p =
                    (struct HCI_Synchronous_Connection_CompleteEvStru *) par;
            GAP_DBG_PRINTF("sco connect complete\n");
            dev = RmtFindDevByBD(p->bd);
            if ((dev != NULL) && (dev->p_dev_status != NULL))
            {
                /*  TODO: Switch local audio path to Bluetooth SCO/eSCO link */
                /* if current device is not phone. */
                if (btengine_global_variable.g_cur_rmt_dev->p_dev_status->sco_handle_setup == 0)
                {
                    btengine_global_variable.g_cur_rmt_dev = dev;
                    dev->p_dev_status->sco_handle_setup = (uint16) p->connection_handle;
                    bt_engine_hfp_sco_connect_ind_handle();
                }
                else
                {
                    dev->p_dev_status->sco_handle_setup = (uint16) p->connection_handle;
                }
            }
            break;
        }

        case HCI_EVI_DISCONNECTION_COMPLETE:
        {
            struct HCI_Disconnection_CompleteEvStru *p = (struct HCI_Disconnection_CompleteEvStru *) par;
            GAP_PRINTF("sco disconnect complete\n");
            dev = HfpFindDevByScoHdl((uint16) p->connection_handle);
            if (dev != NULL)
            {
                /*  TODO: Swith local audio path to local audio adapter */
                /* not support two device phone on the same time.  */
                dev->p_dev_status->sco_handle_setup = 0;

                if (btengine_global_variable.g_cur_rmt_dev == dev)
                {
                    bt_engine_hfp_sco_disconnect_ind_handle();
                }
            }
            break;
        }

        default:
        break;
    }

    FREE(par);
    return 0;
}

/*
 注册一个hci cbk
 */
void gap_reg_sco_cbk(void)
{
    BTSRC_HCI_Cbk_Reg(gap_sco_table, sizeof(gap_sco_table) / sizeof(struct HCI_CbkLineStru));
}

void gap_unreg_sco_cbk(void)
{
    BTSRC_HCI_Cbk_UnReg(gap_sco_table);
}

bt_dev_t *BT_RmtFindDevByBD(uint8 *bdAddress)
{
    uint8 idx;
    bt_dev_t *p_dev;
    p_dev = NULL;
    btengine_global_variable.g_cur_rmt_dev_paried_index = 0;
    if (devs_cfg.tag == PAIRED_INFO_TAG)
    {
        for (idx = 0; idx < bt_manager_cfg_info.max_pair_list_num; ++idx)
        {
            if (devs_cfg.paired_list[idx].data_validity == 0)
            {
                continue;
            }

            if (!libc_memcmp(devs_cfg.paired_list[idx].dev_info.bd_addr.bytes, bdAddress, BD_ADDR_LEN))
            {
                p_dev = &devs_cfg.paired_list[idx];
                btengine_global_variable.g_cur_rmt_dev_paried_index = idx;
                break;
            }
        }
    }
    return p_dev;
}

/******************************************************************************/
/*!
 * \par  Description:
 * 根据设备地址查找配对列表中指定设备信息
 * \param[in]    bdAddress  要查找的设备的地址
 * \param[out]
 * \return
 * \note
 *******************************************************************************/
bt_dev_t *RmtFindDevByBD(uint8 *bdAddress)
{
    bt_dev_t *p_dev;
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (bt_manager_cfg_info.sp_tws_link == 1)
    {
        //先从tws的配对列表中匹配
        p_dev = TWS_FindPairedDevByAddr(bdAddress);
        if (p_dev != NULL)
        {
            return p_dev;
        }
    }
#endif
    //再从手机配对列表中匹配
    p_dev = BT_RmtFindDevByBD(bdAddress);
    return p_dev;
}

/******************************************************************************/
/*!
 * \par  Description:
 * update device MRU to latest.
 * \param[in]    in_dev
 * \param[out]
 * \return
 * \note
 *******************************************************************************/
void UpdateDevMRU(bt_dev_t *in_dev)
{
    uint8 idx;
    bt_dev_t *p_dev;

    /* if device is latest already.  */
    if (in_dev->mru_num == 1)
    {
        return;
    }

    idx = 0;
    p_dev = devs_cfg.paired_list;
    if (devs_cfg.tag == PAIRED_INFO_TAG)
    {
        for (idx = 0; idx < bt_manager_cfg_info.max_pair_list_num; idx++)
        {
            if (devs_cfg.paired_list[idx].data_validity == 0)
            {
                continue;
            }

            if (p_dev[idx].mru_num < (in_dev->mru_num))
            {
                p_dev[idx].mru_num++;
            }
        }
    }

    in_dev->mru_num = 1;
}

//获取devs_cfg.paired_list所有成员中最大的mru值
uint8 get_max_mru(bt_devs_cfg_t *p_dev_cfg)
{
    uint8 idx;
    uint8 max_mru = 0;

    for (idx = 0; idx < bt_manager_cfg_info.max_pair_list_num; idx++)
    {
        if (devs_cfg.paired_list[idx].data_validity == 0)
        {
            continue;
        }

        if (max_mru < p_dev_cfg->paired_list[idx].mru_num)
        {
            max_mru = p_dev_cfg->paired_list[idx].mru_num;
        }
    }
    return max_mru;
}

//获取已配对设备记录个数
//返回值：0~8，已配对的设备个数,0xff 已配对设备超出范围
uint8 updata_paired_list_length(bt_devs_cfg_t *p_dev_cfg)
{
    uint8 idx;
    uint8 paried_list_length = 0;

    for (idx = 0; idx < bt_manager_cfg_info.max_pair_list_num; idx++)
    {
        if (p_dev_cfg->paired_list[idx].data_validity == 1)
        {
            paried_list_length += 1;
        }
    }

    if (paried_list_length > bt_manager_cfg_info.max_pair_list_num)
    {
        BTEG_PRINTF_WARNNING("pl_len overflow warnning\n");
        paried_list_length = 0xff;
        p_dev_cfg->pl_len = bt_manager_cfg_info.max_pair_list_num;
    }
    else
    {
        p_dev_cfg->pl_len = paried_list_length;
        BTEG_DBG_PRINTF("updata pl_len %d\n", devs_cfg.pl_len);
    }

    return paried_list_length;

}

/******************************************************************************/
/*!
 * \par  Description:
 * 添加一个新设备到配对列表
 * \param[in]    bd_addr 新设备的地址
 * \param[out]
 * \return       none
 *******************************************************************************/
static bt_dev_t *mgr_add(uint8 *bd_addr)
{
    uint8 idx, instead_mru, instead_idx;
    bt_dev_t *p_dev;

    //标志表示devs_cfg的有效性
    if (devs_cfg.tag != PAIRED_INFO_TAG)
    {
        devs_cfg.tag = PAIRED_INFO_TAG;
    }

    idx = 0;
    instead_idx = 0;
    instead_mru = get_max_mru(&devs_cfg); //default replace the device info of the max mru
    p_dev = devs_cfg.paired_list;

    for (idx = 0; idx < bt_manager_cfg_info.max_pair_list_num; idx++)
    {
        //to replace the device info which data is not validity firstly
        if (p_dev[idx].data_validity == 0)
        {
            //prevent replace the validity data
            instead_mru += 1;
            if (instead_mru > bt_manager_cfg_info.max_pair_list_num)
            {
                instead_mru = bt_manager_cfg_info.max_pair_list_num;
            }
            //GAP_PRINTF("idx1 %d,mru %d\n",idx,instead_mru);
            break;
        }
        //get idx of the data info with the max mru,replace it when all data info is validity
        else if (p_dev[idx].mru_num == instead_mru)
        {
            instead_idx = idx;
        }
    }

    if (idx >= bt_manager_cfg_info.max_pair_list_num)
    {
        if (instead_idx < bt_manager_cfg_info.max_pair_list_num)
        {
            idx = instead_idx;
            GAP_DBG_PRINTF("need instead a dev info,instead dev idx %d,mru %d\n", idx, p_dev[idx].mru_num);
        }
        else
        {
            idx = 0;
            GAP_DBG_PRINTF("not fit mru\n");
        }
    }

    //初始化一个新记录
    libc_memset(&p_dev[idx], 0x00, sizeof(bt_dev_t));
    p_dev[idx].mru_num = instead_mru;

    //拷贝地址信息
    libc_memcpy(p_dev[idx].dev_info.bd_addr.bytes, bd_addr, BD_ADDR_LEN);
    /* maybe name is too long. */
    //蓝牙设备名全记0x55，是否不需要记录？
    libc_memset(p_dev[idx].dev_info.name, 0x55, MGR_MAX_REMOTE_NAME_LEN - 1);
    p_dev[idx].dev_info.name[MGR_MAX_REMOTE_NAME_LEN - 1] = 0;
    //记录linkKey
    p_dev[idx].profile = PROFILE_VALID; //保存了linkkey后再更新LINKKEY_VALID
    p_dev[idx].data_validity = 1;

    updata_paired_list_length(&devs_cfg);

    return &p_dev[idx];
}

/******************************************************************************/
/*!
 * \par  Description:
 * remove device from auto connect list when connect.
 * \param[in]    p_dev
 * \param[out]   none
 * \return
 *******************************************************************************/
void RmtRemoveDevFromAutoConnectList(bt_dev_t *p_dev)
{
    uint8 idx;

    if (p_dev != NULL)
    {
        for (idx = 0; idx < g_btengine_cfg_data.max_link_num; idx++)
        {
            if (btengine_global_variable.autoconnect_dev_list[idx] == p_dev)
            {
                btengine_global_variable.autoconnect_dev_list[idx] = NULL;
                break;
            }
        }
        BT_SetReconnectFlag(p_dev->dev_type, FALSE);
        //g_p_info->isneed_reconnect_flag = FALSE;
    }
}

void RmtRemoveAllDevFromAutoConnectList(void)
{
    uint8 idx;

    for (idx = 0; idx < g_btengine_cfg_data.max_link_num; idx++)
    {
        if (btengine_global_variable.autoconnect_dev_list[idx] != NULL)
        {
            if (btengine_global_variable.autoconnect_dev_list[idx]->p_dev_status != NULL)
            {
                FREE(btengine_global_variable.autoconnect_dev_list[idx]->p_dev_status);
                btengine_global_variable.autoconnect_dev_list[idx]->p_dev_status = NULL;
            }
            btengine_global_variable.autoconnect_dev_list[idx] = NULL;
            break;
        }
    }
}

bt_dev_t *BT_RmtAddDevice(uint8 *bdAddress)
{
    uint8 idx;
    bt_dev_t *p_dev;
    uint8 *p_temp;
    uint8 need_auto_connect;

    //检查是否已有设备的记录，包括tws，需重构，此时应该已经可以区分设备是否TWS设备（实际上只有主箱在查询副箱时才能判断，回连时无法判断）
    p_dev = RmtFindDevByBD(bdAddress);
    if (!p_dev)
    {
        //新增一条设备记录，顺便初始化配对信息，需重构去除
        p_dev = mgr_add(bdAddress);
    }

    //初始化设备状态
    /* p_dev_status is malloc.  */
    /* backup stream_handle for auto-connect a2dp only, because it is inited too early. 2015/9/21 19:25:02  */
    if (p_dev->p_dev_status == NULL)
    {
        p_dev->p_dev_status = NEW(sizeof(bt_dev_status_t));
        p_temp = NULL;
        need_auto_connect = 0x0;
    }
    else
    {
        p_temp = p_dev->p_dev_status->a2dp_source.stream_handle;
        need_auto_connect = p_dev->p_dev_status->need_auto_connect;

    }
    libc_memset(p_dev->p_dev_status, 0x00, sizeof(bt_dev_status_t));
    p_dev->p_dev_status->a2dp_source.stream_handle = p_temp;
    p_dev->p_dev_status->need_auto_connect = need_auto_connect;

    //取消需要自动回连标志
    GAP_PRINTF("<need_auto_connect|0x%x>\n", p_dev->p_dev_status->need_auto_connect);
    BT_SetReconnectFlag(p_dev->dev_type, FALSE);

    //设备是否已连接，为什么会出现设备已连接的情况？是否应该更早的做出检查？
    for (idx = 0; idx < g_btengine_cfg_data.max_link_num; idx++)
    {
        /* if device is linked already. */
        if (btengine_global_variable.linked_dev_list[idx] == p_dev)
        {
            GAP_PRINTF_WARNNING("device already added\n");
            return p_dev;
        }
    }

    //更新已连接设备列表
    /* update linked list. */
    for (idx = 0; idx < g_btengine_cfg_data.max_link_num; idx++)
    {
        if (btengine_global_variable.linked_dev_list[idx] == NULL)
        {
            btengine_global_variable.linked_dev_list[idx] = p_dev;
            break;
        }
    }

    //超过最大支持连接数，此时p_dev未保存入linked_dev_list
    if (idx == g_btengine_cfg_data.max_link_num)
    {
        GAP_PRINTF_WARNNING("too much device\n");
    }

    //将设备从自动回连中删除，此时连接已经成功，如果需要回连，那么要重新读取一边已配对列表到自动回连列表
    RmtRemoveDevFromAutoConnectList(p_dev);

    return p_dev;
}

/******************************************************************************/
/*!
 * \par  Description:
 * 每当有新连接产生时调用此函数添加新设备或者更新旧设备
 * \param[in]    bd_addr设备的地址
 * \param[out]   none
 * \return       bt_dev_t 添加或者更新后的设备信息
 *******************************************************************************/
bt_dev_t *RmtAddDevice(uint8 *bdAddress)
{
    bt_dev_t *p_dev;
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    //此函数做了三件事
    //1.判断设备是否TWS设备。
    //2.初始化p_dev的状态信息
    //3.初始化p_dev的配对信息
    if (bt_manager_cfg_info.sp_tws_link == 1)
    {
        p_dev = TWS_ConfigPairedInfo(bdAddress);
        if (p_dev != NULL)
        {
            return p_dev;
        }
    }
#endif
    p_dev = BT_RmtAddDevice(bdAddress);
    return p_dev;
}

/******************************************************************************/
/*!
 * \par  Description:
 * remove device from linked list when disconnect.
 * \param[in]    p_dev
 * \param[out]   none
 * \return
 *******************************************************************************/
void RmtRemoveDevFromLinkedList(bt_dev_t *p_dev)
{
    uint8 idx;

    /* update linked list. */
    for (idx = 0; idx < g_btengine_cfg_data.max_link_num; idx++)
    {
        if (btengine_global_variable.linked_dev_list[idx] == p_dev)
        {
            btengine_global_variable.linked_dev_list[idx] = NULL;
            break;
        }
    }
}

extern bool restore_paired_info_form_vram(bt_dev_t* p_dev);

/******************************************************************************/
/*!
 * \par  Description:
 * remove device from linked list when disconnect.
 * \param[in]    p_dev
 * \param[out]   none
 * \return
 *******************************************************************************/
void RmtRemoveDevice(bt_dev_t *p_dev)
{
    if (p_dev != NULL)
    {
        if (p_dev->p_dev_status != NULL)
        {
            FREE(p_dev->p_dev_status);
            p_dev->p_dev_status = NULL;
        }

        bt_clear_remote_phone_name(p_dev);

        /* if linkkey is not created yet, clear its info in list */
        if ((p_dev->profile & LINKKEY_VALID) != LINKKEY_VALID)
        {
            //如果连接失败，那么从VRAM中恢复之前的配对信息
            if (restore_paired_info_form_vram(p_dev) == FALSE)
            {
                GAP_PRINTF("restore paired info failed\n");

                //恢复失败，清空信息
                libc_memset(p_dev, 0x00, sizeof(bt_dev_t));
                p_dev->mru_num = bt_manager_cfg_info.max_pair_list_num;
                p_dev->data_validity = 0;
                updata_paired_list_length(&devs_cfg);
                save_rmtdev_info();
            }
        }
    }
}
void RmtRemovePairedDev(bt_dev_t *p_dev)
{

    uint8 idx;
    bt_dev_t *dev_pairlist;
    if (p_dev != NULL)
    {
        if (p_dev->p_dev_status != NULL)
        {
            FREE(p_dev->p_dev_status);
            p_dev->p_dev_status = NULL;
        }
        bt_clear_remote_phone_name(p_dev);
        idx = 0;
        dev_pairlist = devs_cfg.paired_list;
        if (devs_cfg.tag == PAIRED_INFO_TAG)
        {
            for (idx = 0; idx < bt_manager_cfg_info.max_pair_list_num; idx++)
            {
                if (devs_cfg.paired_list[idx].data_validity == 0)
                {
                    continue;
                }

                if (p_dev->mru_num < dev_pairlist[idx].mru_num)
                {
                    if (dev_pairlist[idx].mru_num > 0)
                    {
                        dev_pairlist[idx].mru_num--;
                    }
                }

                if (p_dev->mru_num == dev_pairlist[idx].mru_num)
                {
                    GAP_DBG_PRINTF("idx %d\n", idx);
                }
            }
            p_dev->data_validity = 0;
            p_dev->mru_num = bt_manager_cfg_info.max_pair_list_num;
            updata_paired_list_length(&devs_cfg);
            save_rmtdev_info();
        }

    }

}

uint8 check_linkedlist(uint8 *bd)
{
    uint8 idx, empty_flag = 0;

    for (idx = 0; idx < g_btengine_cfg_data.max_link_num; idx++)
    {
        if (btengine_global_variable.linked_dev_list[idx] != NULL)
        {
            if (libc_memcmp(btengine_global_variable.linked_dev_list[idx]->dev_info.bd_addr.bytes, bd, 6) == 0)
            {
                return TRUE;
            }
        }
        else
        {
            empty_flag = 1;
        }
    }
    return empty_flag;
}

/******************************************************************************/
/*!
 * \par  Description:
 * Find one Device In Linked List
 * \param[in]    bdAddress  要查找的设备的地址
 * \param[out]
 * \return
 * \note
 *******************************************************************************/
bt_dev_t *RmtFindDevInLinkedList(void)
{
    uint8 idx;
    bt_dev_t *p_dev;

    p_dev = NULL;

    for (idx = 0; idx < g_btengine_cfg_data.max_link_num; idx++)
    {
        if (btengine_global_variable.linked_dev_list[idx] != NULL)
        {
            p_dev = btengine_global_variable.linked_dev_list[idx];
            break;
        }
    }
    return p_dev;
}

/******************************************************************************/
/*!
 * \par  Description:
 * add device to auto connect list when disconnect.
 * \param[in]    p_dev
 * \param[out]   none
 * \return
 *******************************************************************************/
void RmtAddDevToAutoConnectList(bt_dev_t *p_dev)
{
    uint8 idx;

    if (p_dev != NULL)
    {
        for (idx = 0; idx < g_btengine_cfg_data.max_link_num; idx++)
        {
            if (btengine_global_variable.autoconnect_dev_list[idx] == NULL)
            {
                btengine_global_variable.autoconnect_dev_list[idx] = p_dev;
                break;
            }
            if (btengine_global_variable.autoconnect_dev_list[idx] == p_dev)
            {
                break;
            }
        }

        if (p_dev->p_dev_status != NULL)
        {
            p_dev->p_dev_status->need_auto_connect = (p_dev->profile) & (uint8) (~(LINKKEY_VALID | PROFILE_VALID));
            GAP_PRINTF("<need_auto_connect|0x%x>\n", p_dev->p_dev_status->need_auto_connect);
            BT_SetReconnectFlag(p_dev->dev_type, TRUE);
            //g_p_info->isneed_reconnect_flag = TRUE;
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * Find one Device In AutoConnect List
 * \param[in]    bdAddress  要查找的设备的地址
 * \param[out]
 * \return
 * \note
 *******************************************************************************/
bt_dev_t *RmtFindDevInAutoConnectList(void)
{
    int8 idx;
    bt_dev_t *p_dev;

    p_dev = NULL;

    for (idx = g_btengine_cfg_data.max_link_num - 1; idx >= 0; idx--)
    {
        if (btengine_global_variable.autoconnect_dev_list[idx] != NULL)
        {
            p_dev = btengine_global_variable.autoconnect_dev_list[idx];
            break;
        }
    }
    return p_dev;
}

tGAP_IndCbk_Result RmtPinCodeReply(struct HCI_PIN_Code_RequestEvStru *in)
{
    struct HCI_PIN_Code_Request_ReplyStru reply;
    bt_dev_t *dev;
    tGAP_IndCbk_Result result = GAP_IND_RESULT_PENDING;

    dev = RmtFindDevByBD(in->bd);

    //倘若是手机取消配对后，音箱回连，根据配置决定是否继续连接
    if ((bt_stack_cfg_info.sp_linkkey_miss_reject == 1) && (g_p_info->need_auto_connect != 0))
    {
        GAP_PRINTF("link key miss,disconnect\n");
        if (dev != NULL)
        {
            bt_disconnect_acl(dev);
            BT_SetReconnectFlag(dev->dev_type, FALSE);
        }
        set_auto_connect_flag(0);
        //理论上只有手机可以拒接配对
        BT_SetReconnectFlag(NO_TWS_DEV, FALSE);

        result = GAP_IND_RESULT_REJECT;
    }
    else
    {
        GAP_PRINTF("device pin code reply\n");
        libc_memset(&reply, 0x00, sizeof(struct HCI_PIN_Code_Request_ReplyStru));
        reply.pin_code_len = (uint8) libc_strlen(btengine_param.pin_code);
        //reply.pin_code_len = (uint8)libc_strlen(FIXED_PIN);
        //libc_memset(reply.pin_code, 0x00, 16);
        libc_strncpy(reply.pin_code, btengine_param.pin_code, (uint32 ) reply.pin_code_len);
        libc_memcpy(reply.bd, in->bd, BD_ADDR_LEN);
        BTSRC_GAP_ExecuteCommandA(0, NULL, HCI_OPS_PIN_CODE_REQUEST_REPLY, &reply,
                sizeof(struct HCI_PIN_Code_Request_ReplyStru), NULL);
    }

    return result;
}

void RmtAddSearchedDev(struct GAP_RemoteDeviceInfoStru *in)
{
    struct HCI_EIR_DataNodeStru *node;
    struct ListStru list =
    { 0 };

    //GAP_PRINTF("device inquiry result\n");
    //dev->dev_cls = in->class_of_device;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (bt_manager_cfg_info.sp_tws_link == 1)
    {
        if (TWS_GetPairedStatus() == TWS_SEARCH)
        {
            if ((tws_connect_flag != TRUE) && (phone_connect_flag != TRUE))
            {
                bt_linking_flag = FALSE;
            }
        }
    }
#endif

    BTSRC_HCI_Extended_Inquiry_Response_Decode(&list, in->eir, in->eir + in->eir_size);
    node = BTSRC_List_RemoveHead(&list);
    while (node != NULL)
    {
        /* AD Node in a device */
        switch (node->eir_data_type)
        {
            case HCI_EIR_DATATYPE_COMPLETE_LOCAL_NAME:
            case HCI_EIR_DATATYPE_SHORTENED_LOCAL_NAME:
            //memcpy(dev->rmt_name, node->u.name.data, node->len - 1);
#ifdef ENABLE_TRUE_WIRELESS_STEREO
            if (bt_manager_cfg_info.sp_tws_link == 1)
            {
                TWS_MatchAvailableDevice(node, in);
            }
#endif
            break;
            default:
            break;
        }
        LFREE(node);
        node = BTSRC_List_RemoveHead(&list);
    }
}

void ReadRemoteNameCfmCbk(void *context, UINT16 result, struct GAP_ExecuteCommandCfmStru *cfm_par)
{
    bt_dev_t *dev;
    struct HCI_Remote_Name_Request_CompleteEvStru *ev = cfm_par->event_parameter;
    if ((result == HCI_STATUS_OK) && (ev->status == HCI_STATUS_OK))
    {
        dev = RmtFindDevByBD(ev->bd);
        if (dev != NULL)
        {
            if (bt_save_remote_phone_name(dev, ev->remote_name) == FALSE)
            {
                BTEG_PRINTF("save bt fullname failed!\n");
            }

            libc_memcpy(dev->dev_info.name, ev->remote_name, MGR_MAX_REMOTE_NAME_LEN);
            GAP_PRINTF("device Remote Name:%s\n", dev->dev_info.name);
#ifdef ENABLE_TRUE_WIRELESS_STEREO
            if (bt_manager_cfg_info.sp_tws_link == 1)
            {
                if (TWS_GetDevType(dev) == TRUE)
                {
                    if (TWS_GetPairedStatus() == TWS_PAIRED) //connect another tws dev
                    {
                        GAP_PRINTF_WARNNING("tws connected,disconnect\n");
                        dev->dev_type = NO_TWS_DEV;
                        bt_disconnect_acl(dev);
                        wrong_phone_dev = dev;
                    }
                    else
                    {
                        tws_status_info.acl_handle = dev->p_dev_status->acl_handle;

                        tws_status_info.tws_acl_link_time = 0; //刚连接起来默认中档
                        con_adjust_link_time(tws_status_info.acl_handle, tws_status_info.tws_acl_link_time);
                        tws_status_info.tws_acl_link_time_timer = 0;
                        TWS_PairedDev_Copyto_TWSDev(dev);
                    }
                    return;
                }
                else
                {
                    if ((CURRENT_DEVICE_STATUS != STATUS_WAIT_PAIRED) && (CURRENT_DEVICE_STATUS != STATUS_NONE)) //connect another phone
                    {
                        GAP_PRINTF_WARNNING("phone connected,tws not match\n");
                        bt_disconnect_acl(dev);
                        return;
                    }
                }
            }
#endif
            updata_paired_list_length(&devs_cfg);
            save_rmtdev_info();
            save_plist_to_vram();
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * 获取remote设备的名称
 * \param[in]    bd remote设备的地址
 * \param[out]   none
 * \return       none
 *******************************************************************************/
void RmtUpdateName(uint8 *bd)
{
    struct HCI_Remote_Name_RequestStru cmd;

    /* Update remote device name */
    libc_memset(&cmd, 0x0, sizeof(struct HCI_Remote_Name_RequestStru));
    libc_memcpy(cmd.bd, bd, BD_ADDR_LEN);
    cmd.clock_offset = 0;
    cmd.page_scan_repetition_mode = HCI_PAGE_SCAN_REPETITION_MODE_R2;
    cmd.reserved = 0;
    BTSRC_GAP_ExecuteCommandA(0, NULL, HCI_OPS_REMOTE_NAME_REQUEST, &cmd, sizeof(cmd), ReadRemoteNameCfmCbk);
}

void RmtDevConnected(struct GAP_ConnectionEventStru *in)
{
    bt_dev_t *dev;
    bool phone_dev_flag = TRUE;
    bool dev_not_record_flag = FALSE;

    dev = RmtFindDevByBD(in->addr.bd);
    if (in->connected == GAP_TRUE)
    {
        GAP_PRINTF("device connected,acl_hdl 0x%x\n", in->acl_hdl);
#ifdef BT_SUPPORT_BLE
        if (bt_manager_cfg_info.sp_ble == 1)
        {
            if (((in->addr.cap & HCI_ADDRESS_CAP_BREDR) == 0) || (in->link_type == GAP_LINK_TYPE_LE))
            {
                APP_GATTC_Link_Connected_Hook(in);
                return;
            }
        }
#endif
        bt_linking_flag = TRUE;
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (bt_manager_cfg_info.sp_tws_link == 1)
        {
            //收到连接请求，此时需要关闭设备scan
            TWS_CancelDeviceDiscovery();
            //如果手机已经连接，那么此次连接为TWS设备
            if ((CURRENT_DEVICE_STATUS != STATUS_WAIT_PAIRED) && (CURRENT_DEVICE_STATUS != STATUS_NONE))
            {
                //未在TWS配对列表中找到该设备
                if (dev == NULL)
                {
                    dev_not_record_flag = TRUE;
                    GAP_DBG_PRINTF("tws not record connect\n");

                }
            }
        }
#endif
        //记录设备信息
        dev = RmtAddDevice(in->addr.bd);
        dev->p_dev_status->acl_handle = (uint16) in->acl_hdl;

        //close faster mode
        bt_RestoreNormalScanMode();
//很混乱的一堆配置，置标志位以做后续的profile连接，更新一些环境变量
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (bt_manager_cfg_info.sp_tws_link == 1)
        {
            if (dev_not_record_flag == TRUE)
            {
                //如果不是回连，那么发起组队的为主箱
                dev->is_source = 1;

            }
            //处理当前连接设备不同类型时，主从箱回连、角色转换等逻辑
            tws_connect_deal(dev);

            phone_dev_flag = FALSE;
            //为什么要加上非信源的限定？
            if ((dev->is_source != 1) && (dev->dev_type != TWS_DEV))
            {
                phone_dev_flag = TRUE;
            }
        }
#endif

        if (phone_dev_flag == TRUE)
        {

            btengine_global_variable.g_cur_rmt_dev = dev;
            btengine_global_variable.hfp.disconnect_reason = ER_HFP_RFCOMM_SUCCESS;
            bt_auto_connect_cfg.connect_type = AUTO_CONNECT_TYPE_TIMEOUT;
        }
        //ACL链路有最大值限定
        btengine_global_variable.connect_lock_flag++;
        if (btengine_global_variable.connect_lock_flag >= g_btengine_cfg_data.max_link_num)
        {
            RmtRemoveAllDevFromAutoConnectList();
        }
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (bt_manager_cfg_info.sp_tws_link == 1)
        {
            phone_dev_flag = FALSE;
            if (dev->dev_type != TWS_DEV)
            {
                phone_dev_flag = TRUE;
            }
        }
#endif

        if (phone_dev_flag == TRUE)
        {
            //蓝牙初始化后第一次自动回连标志，连接成功后需要关闭
            bt_auto_connect_cfg.first_reconnect_after_btinit_flag = 0;
            //需要自动连接A2DP，起始不算才需要这个变量，有歧义，需重构消除
            not_reconnect_a2dp_flag = FALSE;
        }

    }
    else
    {

#ifdef BT_SUPPORT_BLE
        if (bt_manager_cfg_info.sp_ble == 1)
        {
            if (in->link_type == GAP_LINK_TYPE_LE)
            {
                APP_GATTC_Link_Loss_Hook(in);
                BLE_PRINTF("BLE disconnect,reason 0x%x\n", (uint8 )in->reason);
                return;
            }
        }
#endif    
        //断开连接需要重置此标志位
        bt_linking_flag = FALSE;

        if (dev == NULL)
        {
            GAP_PRINTF_WARNNING("dev is NULL\n");
            //如果样机收到连接请求后，立刻接收到断开连事件，此时样机有可能处于不可见并未连接状态，需要打开可见性
            bt_set_visual_mode(GAP_VM_DEFAULT);
            bt_SetFasterScanMode();
            return;
        }
        if (btengine_global_variable.connect_lock_flag != 0)
        {
            btengine_global_variable.connect_lock_flag--;
        }
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if ((bt_manager_cfg_info.sp_tws_link == 1) && (dev->dev_type == TWS_DEV))
        {
            GAP_PRINTF("TWS disconnect,reason 0x%x\n", (uint8 )in->reason)

            if (TWS_DisconnectDeal(dev, in) == TRUE)
            {
                return;
            }
        }
        else
#endif
        {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
            if (bt_manager_cfg_info.sp_tws_link == 1)
            {
                //TWS_DBG_PRINTF("READY FOR TWS1!!");
                phone_connect_flag = FALSE;
                if (snk_to_source_judge_flag == TRUE)
                {
                    snk_to_source_judge_flag = FALSE;
                    devs_cfg.tws_paired_list.is_source = 0;
                }
            }
#endif
            GAP_PRINTF("device disconnect,reason 0x%x\n", (uint8 ) in->reason);
        }

        set_exit_schedule_flag(TRUE);
        g_p_info->cur_role = ROLE_NULL;

        not_reconnect_a2dp_flag = FALSE;

        if (btengine_global_variable.connect_lock_flag < g_btengine_cfg_data.max_link_num)
        {
#ifdef TWS_SLAVE_NO_DISC_PAIRED_BEFORE
            if((tws_status_info.have_paired_dev == TRUE)&&(devs_cfg.tws_paired_list.is_source == 0))
            {
                bt_set_visual_mode (GAP_VM_NONDISCOV);
            }
            else
#else
            {
                bt_set_visual_mode(GAP_VM_DEFAULT);
            }
#endif
        }

        if (dev->p_dev_status != NULL)
        {
            set_cur_device_status(dev, STATUS_WAIT_PAIRED);
            dev->p_dev_status->acl_handle = 0;
            dev->p_dev_status->sniff_flag = 0;
            current_sniff_count = 0;
        }
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (bt_manager_cfg_info.sp_tws_link == 1)
        {
            if (dev->dev_type == TWS_DEV)
            {
                GAP_PRINTF("tws device reconnect,need repaired %d\n", tws_status_info.need_repaired);
                return;
            }
            else
            {
                if (TWS_GetPairedStatus() != TWS_PAIRED)
                {
                    bt_SetFasterScanMode();
                }

            }
            if (wrong_phone_dev == dev)
            {
                RmtRemoveDevFromLinkedList(dev);
                RmtRemovePairedDev(dev);
                wrong_phone_dev = NULL;

                tws_reconnect_lastphone_init(NULL);
                if (btengine_global_variable.g_cur_rmt_dev == dev)
                {
                    btengine_global_variable.g_cur_rmt_dev = RmtFindDevInLinkedList();
                    if (btengine_global_variable.g_cur_rmt_dev == NULL)
                    {
                        GAP_PRINTF("reconnect phone\n");
                        btengine_global_variable.g_cur_rmt_dev = RmtFindDevInAutoConnectList();

                        BT_SetReconnectFlag(NO_TWS_DEV, TRUE);
                    }
                }
                return;
            }
        }
        else
        {
            bt_SetFasterScanMode();
        }
#else
        bt_SetFasterScanMode();

#endif

        RmtRemoveDevFromLinkedList(dev);

        /* HCI_STATUS_AUTHENTICATION_FAILURE(0x5)------remote user not pair */
        /* HCI_STATUS_REMOTE_USER_TERMINATED_CONNECTION(0x13)------remote user initiate disconnection */
        /* HCI_STATUS_CONNECTION_TERMINATED_BY_LOCAL_HOST(0x16)---- local disconnect after remote RFCOMM reject   */
        /* HCI_STATUS_CONNECTION_TIMEOUT(0x8)------ */
        /* HCI_STATUS_PAGE_TIMEOUT(0x4)------ */
        /* HCI_STATUS_COMMAND_DISALLOWED(0xc)------ */
        switch (in->reason)
        {
            case HCI_STATUS_PAGE_TIMEOUT:
            //for att test
            test_disconnect_acl(dev);
            case HCI_STATUS_CONNECTION_TIMEOUT:
            case HCI_STATUS_COMMAND_DISALLOWED:
            //iphone通话过程中reset或掉电重启时会出现该错误，所以将该错误加入回连条件
            case HCI_STATUS_ACL_CONNECTION_ALREADY_EXISTS:
            case HCI_STATUS_LMP_OR_LL_RESPONSE_TIMEOUT:
            RmtAddDevToAutoConnectList(dev);
            break;
            default:
            //for att test
            if (test_add_to_connectList(dev) == 1)
            {
                break;
            }

            RmtRemoveDevice(dev);

            break;
        }

        if (btengine_global_variable.g_cur_rmt_dev == dev)
        {
            btengine_global_variable.g_cur_rmt_dev = RmtFindDevInLinkedList();
            if (btengine_global_variable.g_cur_rmt_dev == NULL)
            {
                GAP_PRINTF("no device is linked\n");
                btengine_global_variable.g_cur_rmt_dev = RmtFindDevInAutoConnectList();
            }
        }
    }
}

//返回值：0表示接受SSP并且已有LinkKey，1表示接受SSP但是没有LinkKey，2表示不支持SSP
void RmtLinkKeyReply(struct HCI_Security_Link_Key_Ask4Stru *in)
{
    bt_dev_t *dev;
    struct HCI_Security_Link_Key_Request_ReplyStru cmd;

    GAP_PRINTF("device link key reply\n");

    dev = RmtFindDevByBD(in->remote_bd);
    libc_memcpy(cmd.bd, in->remote_bd, BD_ADDR_LEN);
    if (bt_stack_cfg_info.sp_ssp_enable == FALSE)
    {
        GAP_PRINTF_WARNNING("link key reject\n");
        cmd.accept = 0;
        BTSRC_GAP_LinkKeyReply(0, &cmd);
        return;
    }
    if ((dev != NULL) && ((dev->profile & (PROFILE_VALID | LINKKEY_VALID)) == (PROFILE_VALID | LINKKEY_VALID)))
    {
        cmd.accept = 1;

        //更新MRU
        UpdateDevMRU(dev);
        print_link_key(dev->linkKey, LINKKEY_SIZE);
        libc_memcpy(cmd.link_key, dev->linkKey, LINKKEY_SIZE);
        cmd.key_type = dev->linkKeyType;
        //以防用户更改了remote设备的名字，还是更新吧
        RmtUpdateName(in->remote_bd);
        GAP_PRINTF("link key founded\n");
    }
    else
    {
        cmd.accept = 0;
        GAP_PRINTF("link key not founded\n");
    }
    BTSRC_GAP_LinkKeyReply(0, &cmd);
}

void RmtLinkKeyCreated(struct HCI_Security_Link_Key_SaveStru *in)
{
    bt_dev_t *dev;
    struct HCI_AddressStru addr;

    GAP_PRINTF("device link key create\n");

    //以下两句赋值貌似无效语句
    addr.atype = HCI_ADDRESS_CAP_BREDR;
    addr.cap = (uint8) HCI_OWN_ADDRESS_TYPE_PUBLIC_DEVICE_ADDRESS;
    libc_memcpy(addr.bd, in->remote_bd, BD_ADDR_LEN);
    //有新的连接添加或更新设备
    dev = RmtFindDevByBD(addr.bd);

    /* Update remote device link key */
    if (dev != NULL)
    {
        print_link_key(in->link_key, LINKKEY_SIZE);
        //保存LinkKey
        libc_memcpy(dev->linkKey, in->link_key, LINKKEY_SIZE);
        dev->linkKeyType = in->key_type;
        dev->profile |= LINKKEY_VALID;
        //更新MRU
        UpdateDevMRU(dev);
        RmtUpdateName(addr.bd);
        //save_rmtdev_info();
        //save_plist_to_vram();
    }
}

tGAP_IndCbk_Result RmtConnectionReply(struct HCI_Connection_RequestEvStru *in)
{
    bt_dev_t *dev;
    tGAP_IndCbk_Result ret = GAP_IND_RESULT_NOT_CARE;

    dev = RmtFindDevByBD(in->bd);

    switch (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev))
    {
        case STATUS_WAIT_PAIRED:
        case STATUS_LINKED:
        case STATUS_A2DP_PLAY:
        case STATUS_A2DP_PAUSE:
        case STATUS_CALL_INCOMING:
        case STATUS_CALL_OUTGOING:
        case STATUS_CALL_HFP:
        case STATUS_CALL_PHONE:
        {
            GAP_PRINTF("RmtConnectionReply\n");

            if (in->link_type == HCI_LINK_TYPE_ACL)
            {
                GAP_DBG_PRINTF("link_type acl\n");
            }
            else if ((in->link_type == HCI_LINK_TYPE_SCO) || (in->link_type == HCI_LINK_TYPE_ESCO))
            {
                GAP_DBG_PRINTF("link_type sco/Esco\n");
            }
            else
            {
                GAP_DBG_PRINTF("link_type %d\n", in->link_type);
            }

            if (in->link_type == HCI_LINK_TYPE_ACL)
            {
                //是否有风险？
                bt_linking_flag = TRUE;

                if (btengine_global_variable.connect_lock_flag >= g_btengine_cfg_data.max_link_num)
                {
                    GAP_PRINTF_WARNNING("double connection is already initiated\n");
                    ret = GAP_IND_RESULT_REJECT;
                }
                else
                {
                    //进入连接过程，临时提高频率
                    bt_adjust_clk(BT_LINK_WORK_FREQ);

                    ret = GAP_IND_RESULT_NOT_CARE;
#ifdef ENABLE_TRUE_WIRELESS_STEREO
                    if (bt_manager_cfg_info.sp_tws_link == 1)
                    {
                        ret = tws_connectionreply_deal(in->bd);
                    }
#endif
                    if (ret != GAP_IND_RESULT_REJECT)
                    {

                        bt_set_visual_mode(GAP_VM_PAIR);

                    }
                    else
                    {
                        //退出acl连接，蓝牙进入未连接等待状态需要重置此标志位
                        bt_linking_flag = FALSE;

                    }
                }
            }

            //if the connect request is for sco/esco link, no need to check connect lock flag.
            else if ((in->link_type == HCI_LINK_TYPE_SCO) || (in->link_type == HCI_LINK_TYPE_ESCO))
            {
                GAP_PRINTF("SCO/ESCO Request\n");
                sco_connect_request_flag = 1;
                sco_connect_request_time = sys_get_ab_timer();
#ifdef NOISE_CONTROL
                if ((dev != NULL) && (dev->p_dev_status != NULL)
                        && ((dev->p_dev_status->service_connect & HFP_CONNECTED) != 0))
                {
                    HFP_HF_DisableNREC((UINT8 *)(dev->p_dev_status->g_hf_handle));
                }
#endif
                if (bt_linking_flag == TRUE)
                {
                    //建立sco连接，此时蓝牙处于已连接状态（包括profile），标志位应该为false
                    bt_linking_flag = FALSE;
                }
                ret = GAP_IND_RESULT_ACCEPT;
#ifdef ENABLE_SIRI
                if ((dev != NULL) && (dev->p_dev_status != NULL)
                        && ((dev->p_dev_status->service_connect & HFP_CONNECTED) != 0))
                {
                    BT_HfpCheckSiriStatus();
                }
#endif
                break;
            }
            else
            {
            }
        }
        break;
        case STATUS_NONE: /* not accept connection before BTinit is over.  */
        ret = GAP_IND_RESULT_REJECT;
        break;
        default:
        break;
    }

    //如果有ACL连接请求,并且音箱接受该请求，则清除回连标志
    if ((in->link_type == HCI_LINK_TYPE_ACL) && (ret != GAP_IND_RESULT_REJECT))
    {
        RmtRemoveDevFromAutoConnectList(dev);

    }
    return ret;
}

void RmtInquiryComplete(void)
{
    GAP_PRINTF("Device search finished\n");
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (bt_manager_cfg_info.sp_tws_link == 1)
    {
        TWS_DevInquireComplete();
    }
#endif
}

void RmtAuthenticationFail(void)
{
    GAP_PRINTF("authentication failure\n");
}

void RmtRemoteDeviceNameUpdate(void)
{
    GAP_PRINTF("remote device name updated\n");

#if (SUPPORT_BT_VISIBLE_CONFIG == 1)
    // close global visiable wnhen conencted
    if (bt_manager_cfg_info.visual_cfg_mode != VMODE_DEFAULT_VISIBLE)
    {
        btengine_global_variable.global_visible = 0;
        GAP_DBG_PRINTF("global_visible change:%d\n",btengine_global_variable.global_visible);
    }
#endif
}

void RmtUserConfirmationRequest(void)
{
    GAP_PRINTF("cevice confirmation request\n");
}

void RmtUserPasskeyRequest(void)
{
    GAP_PRINTF("user passkey request\n");
}

void RmtSimplePairingComplete(struct HCI_Simple_Pairing_CompleteEvStru *in)
{
    if (in->status != HCI_STATUS_OK)
    {
        GAP_PRINTF("simple pairing complete fail\n");
    }
    else
    {
        GAP_PRINTF("simple pairing complete ok\n");
    }
}

void RmtUserPasskeyNotification(void)
{
    GAP_PRINTF("user passkey notification\n");
}

void RmtAuthenticationRequest(void)
{
    GAP_PRINTF("authorization request\n");
}

void RmtAuthenticationAbort(void)
{
    GAP_PRINTF("authorization abort\n");
}

void RmtModeChange(struct HCI_Mode_ChangeEvStru *in)
{
    bt_dev_t *dev;

    dev = BTFindDevByAclHdl((uint16) in->connection_handle);
    if ((CURRENT_DEVICE != NULL) && (CURRENT_DEVICE_P_STATUS != NULL))
    {
        if (in->current_mode == 0x00)
        {
            dev->p_dev_status->sniff_flag = 0;
            btengine_global_variable.bt_idle_cnt = 0;
            current_sniff_count = 0;
            GAP_PRINTF("Real Exit sniff\n");
        }
        else if (in->current_mode == 0x02)
        {
            dev->p_dev_status->sniff_flag = 1;
            GAP_PRINTF("Real Enter sniff\n");
        }
        else
        {

        }
    }
}

tGAP_IndCbk_Result AppGapCbk(HANDLE tl_handle, void *context, tGAP_Ind_Type ind_type, void *parameter, UINT32 size)
{
    tGAP_IndCbk_Result result = GAP_IND_RESULT_NOT_CARE;

    if (ind_type != GAP_IND_INQUIRY_RESULT)
    {
        GAP_DBG_PRINTF("event code 0x%x\n", ind_type);
    }

    switch (ind_type)
    {
        case GAP_IND_INQUIRY_COMPLETE:
        RmtInquiryComplete();
        break;
        case GAP_IND_INQUIRY_RESULT:
        RmtAddSearchedDev(parameter);
        break;
        case GAP_IND_CONNECTION_EVENT:
        RmtDevConnected(parameter);
        break;
        case GAP_IND_PIN_CODE_REQ:
        result = RmtPinCodeReply(parameter);
        //result = GAP_IND_RESULT_PENDING;
        break;
        case GAP_IND_LINK_KEY_REQUEST:
        RmtLinkKeyReply(parameter);
        result = GAP_IND_RESULT_PENDING;
        break;
        case GAP_IND_LINK_KEY_NOTIFICATION:
        RmtLinkKeyCreated(parameter);
        break;
        case GAP_IND_AUTHENTICATION_FAILURE:
        RmtAuthenticationFail();
        break;
        case GAP_IND_REMOTE_DEVICE_NAME_UPDATED:
        RmtRemoteDeviceNameUpdate();
        break;
        case GAP_IND_USER_CONFIRMATION_REQUEST:
        RmtUserConfirmationRequest();
        result = GAP_IND_RESULT_ACCEPT;
        break;
        case GAP_IND_USER_PASSKEY_REQUEST:
        RmtUserPasskeyRequest();
        break;
        case GAP_IND_SIMPLE_PAIRING_COMPLETE:
        RmtSimplePairingComplete(parameter);
        break;
        case GAP_IND_USER_PASSKEY_NOTIFICATION:
        RmtUserPasskeyNotification();
        break;
        case GAP_IND_AUTHORIZATION_REQUEST:
        RmtAuthenticationRequest();
        break;
        case GAP_IND_AUTHORIZATION_ABORT:
        RmtAuthenticationAbort();
        break;
        case GAP_IND_CONNECTION_REQUEST:
        result = RmtConnectionReply(parameter);
        break;
        case GAP_IND_MODE_CHANGE_EVENT:
        RmtModeChange(parameter);
        break;
        default:
        GAP_PRINTF("unknown ind_type\n")
        ;
        break;
    }

    return result;
}
uint8 get_cur_device_status(bt_dev_t *dev)
{

    if ((dev != NULL) && (dev->p_dev_status != NULL))
    {
        return dev->p_dev_status->status;
    }
    else
    {
        return STATUS_WAIT_PAIRED;
    }
}

void set_cur_device_status(bt_dev_t* dev, uint8 bt_status)
{
    if ((dev == NULL) || (dev->p_dev_status == NULL))
    {
        return;
    }

    GAP_PRINTF("bt status %d->%d\n", dev->p_dev_status->status, bt_status);
    dev->p_dev_status->status = bt_status;
}
void set_auto_connect_flag(uint8 flag)
{
    if ((CURRENT_DEVICE != NULL) && (CURRENT_DEVICE_P_STATUS != NULL))
    {
        btengine_global_variable.g_cur_rmt_dev->p_dev_status->need_auto_connect = flag;
    }
}
void deal_device_phone_status(void)
{
    if (g_btengine_cfg_data.hfp_pb_mode == HFP_PB_ONLY_NUM)
    {
        if ((CURRENT_DEVICE != NULL) && (CURRENT_DEVICE_P_STATUS != NULL))
        {
            libc_memset(btengine_global_variable.g_cur_rmt_dev->p_dev_status->phone_number, 0, MAX_PHONENUM);
            btengine_global_variable.g_cur_rmt_dev->p_dev_status->phone_status = PB_FIND_NAME;
        }
        btengine_global_variable.g_btengine_info.phone_status = PB_FIND_NAME;
    }
}

