/*******************************************************************************
 *                              US211A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       xiaomaky   2016-8-8      14:26:35     1.0             build this file
 *******************************************************************************/
/*!
 * \file     bt_engine_vram_info.c
 * \brief
 * \author   stevenluo
 * \version  1.0
 * \date  2016/08/08
 *******************************************************************************/
#include "bluetooth_engine.h"

const uint32 plist_vram_index[PAIRED_LIST_MAX] =
{
VM_BT_PLIST0,
VM_BT_PLIST1,
VM_BT_PLIST2,
VM_BT_PLIST3,
VM_BT_PLIST4,
VM_BT_PLIST5,
VM_BT_PLIST6,
VM_BT_PLIST7 };

const uint32 plist_vram_index_hid[PAIRED_LIST_MAX] =
{
VM_BT_PLIST0_HID,
VM_BT_PLIST0_HID,
VM_BT_PLIST0_HID,
VM_BT_PLIST0_HID,
VM_BT_PLIST0_HID,
VM_BT_PLIST0_HID,
VM_BT_PLIST0_HID,
VM_BT_PLIST0_HID };

void init_plist_info(void)
{
    uint32 v_idx;
    uint8 i;
    uint8 vram_read_ret;
    uint8 read_pl_len = 0;

    libc_memset(&devs_cfg, 0x0, sizeof(bt_devs_cfg_t));

    if (g_p_global_var->only_hid_mode == 0)
    {
        v_idx = VM_BT_DEV_CFG;
    }
    else
    {
        v_idx = VM_BT_DEV_CFG_HID;
    }

    //if vram info invaild ,init vram info
    if (sys_vm_read(&vram_devs_cfg, v_idx, sizeof(vram_devs_cfg_t)) < 0)
    {
        vram_devs_cfg.magic = 0xffff;
        BTEG_PRINTF_WARNNING("bt paired list vm read failed,v_idx %d\n", v_idx);
    }

    if (vram_devs_cfg.magic != VRAM_MAGIC(v_idx))
    {
        libc_memset(&vram_devs_cfg, 0x0, sizeof(vram_devs_cfg_t));
        vram_devs_cfg.magic = VRAM_MAGIC(v_idx);
        sys_vm_write(&vram_devs_cfg, v_idx, sizeof(vram_devs_cfg_t));
    }

    //check whether the pl_len is match to data_validity 
    for (i = 0; i < bt_manager_cfg_info.max_pair_list_num; i++)
    {
        if (vram_devs_cfg.devs_status[i].data_validity == 1)
        {
            read_pl_len++;
        }
    }

    if (read_pl_len != vram_devs_cfg.pl_len)
    {
        BTEG_PRINTF_WARNNING("pl_len not match,clean plist info\n");
        clear_plist_info(CLEAR_ALL_DEV_PAIRED_INFO);
    }

    //if vram has paired dev info list
    if (vram_devs_cfg.pl_len != 0)
    {
        devs_cfg.tag = vram_devs_cfg.tag;
        vram_read_ret = read_plist_from_vram(devs_cfg.paired_list, bt_manager_cfg_info.max_pair_list_num);
#if (SUPPORT_BT_VISIBLE_CONFIG == 1)
        //if get vaild dev info,then close visual.else open the visual
        if (vram_read_ret == TRUE)
        {
            disable_visible();
        }
        else
        {
            enable_visible();
        }
#endif
    }

    //read the paired dev info from vram to dev_cfg
    for (i = 0; i < bt_manager_cfg_info.max_pair_list_num; i++)
    {
        devs_cfg.paired_list[i].p_dev_status = NULL;
        devs_cfg.paired_list[i].profile = vram_devs_cfg.devs_status[i].profile;
        devs_cfg.paired_list[i].mru_num = vram_devs_cfg.devs_status[i].mru_num;
        devs_cfg.paired_list[i].data_validity = vram_devs_cfg.devs_status[i].data_validity;
    }

    updata_paired_list_length(&devs_cfg);

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (bt_manager_cfg_info.sp_tws_link == 1)
    {
        //read the tws paired dev info from vram to dev_cfg
        TWS_GetPairedInfo_Form_Vram();

        //check whether the snk audiobox can connect phone
        slave_connect_phone_flag = FALSE;
        if ((tws_status_info.have_paired_dev == TRUE) && (devs_cfg.tws_paired_list.is_source == 0))
        {
            if (bt_auto_connect_cfg.enable_slave_connect_phone == 1)
            {

                if ((vram_devs_cfg.paired_count == 255) && (tws_status_info.paired_count == 0))
                {
                    ; //

                }
                else if ((vram_devs_cfg.paired_count == 0) && (tws_status_info.paired_count == 255))
                {
                    slave_connect_phone_flag = TRUE;
                }
                else
                {
                    if (vram_devs_cfg.paired_count > tws_status_info.paired_count)
                    {

                        slave_connect_phone_flag = TRUE;
                    }
                    else
                    {
                        ; //
                    }

                }
            }
        }
    }
#endif
}

extern uint8 get_paired_list_length(bt_devs_cfg_t *p_dev_cfg);

void save_rmtdev_info(void)
{
    uint8 i;

    if (devs_cfg.pl_len != 0)
    {
        vram_devs_cfg.tag = devs_cfg.tag;
        vram_devs_cfg.pl_len = devs_cfg.pl_len;
        vram_devs_cfg.devs_status[btengine_global_variable.g_cur_rmt_dev_paried_index].profile =
                devs_cfg.paired_list[btengine_global_variable.g_cur_rmt_dev_paried_index].profile;
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (bt_manager_cfg_info.sp_tws_link == 1)
        {
            vram_devs_cfg.paired_count = tws_status_info.paired_count + 1;
        }
#endif
        for (i = 0; i < bt_manager_cfg_info.max_pair_list_num; i++)
        {
            if (devs_cfg.paired_list[i].data_validity == 1)
            {
                vram_devs_cfg.devs_status[i].mru_num = devs_cfg.paired_list[i].mru_num;
            }
            else
            {
                vram_devs_cfg.devs_status[i].mru_num = 0;
            }
            vram_devs_cfg.devs_status[i].data_validity = devs_cfg.paired_list[i].data_validity;
        }
        if (g_p_global_var->only_hid_mode == 0)
        {
            sys_vm_write(&vram_devs_cfg, VM_BT_DEV_CFG, sizeof(vram_devs_cfg_t));
        }
        else
        {
            sys_vm_write(&vram_devs_cfg, VM_BT_DEV_CFG_HID, sizeof(vram_devs_cfg_t));
        }
        BTEG_DBG_PRINTF("save dev info,pl_len %d\n", devs_cfg.pl_len, 2);
    }
}
//保存配对信息到vram中
void save_plist_to_vram(void)
{
    uint32 *p_v_idx;
    int32 ret = 0;

    if (g_p_global_var->only_hid_mode == 0)
    {
        p_v_idx = (uint32 *) plist_vram_index;
    }
    else
    {
        p_v_idx = (uint32 *) plist_vram_index_hid;
    }

    ret = sys_vm_write(&devs_cfg.paired_list[btengine_global_variable.g_cur_rmt_dev_paried_index],
            p_v_idx[btengine_global_variable.g_cur_rmt_dev_paried_index], sizeof(bt_dev_t));
    
    if (ret < 0)
    {
        BTEG_PRINTF_WARNNING("bt save dev info to vram ret %d\n",ret);
    }
    BTEG_DBG_PRINTF("save plist index %d\n", btengine_global_variable.g_cur_rmt_dev_paried_index, 2);
}

/*
 读取VRAM中已配对设备列表到指定位置
 输入：bt_dev_t *bt_dev   需要获取已配对设备列表的存放地址
 uint8 list_num  需要读取的列表长度
 */
bool read_plist_from_vram(bt_dev_t *bt_dev, uint8 list_num)
{
    uint8 i;
    uint32 *p_v_idx;
    int32 ret_val = 0;

    if (g_p_global_var->only_hid_mode == 0)
    {
        p_v_idx = (uint32 *) plist_vram_index;
    }
    else
    {
        p_v_idx = (uint32 *) plist_vram_index_hid;
    }

    for (i = 0; i < list_num; i++)
    {
        ret_val = sys_vm_read(bt_dev + i, p_v_idx[i], sizeof(bt_dev_t));
        if (ret_val < 0)
        {
            //BTEG_PRINTF_WARNNING("bt paired list vm read failed,index_i %d\n",i);
            libc_memset(bt_dev + i, 0, sizeof(bt_dev_t));
        }
    }

    BTEG_PRINTF("read vram plist over\n");

    return TRUE;
}
void clear_plist_info(uint8 clear_mode)
{
    BTEG_PRINTF("clear plist! clear mode %d\n", clear_mode);

    switch (clear_mode)
    {
        case CLEAR_ALL_PHONE_PAIRED_INFO_ONLY:
        {
            devs_cfg.pl_len = 0;
            //初始化全局设备信息
            libc_memset(&devs_cfg.paired_list[0], 0x0, sizeof(bt_dev_t)*PAIRED_LIST_MAX);

            vram_devs_cfg.pl_len = 0;
            //初始化vram信息
            libc_memset(&vram_devs_cfg.devs_status[0], 0x0, sizeof(vram_devs_status_t)*PAIRED_LIST_MAX);
        }
        break;
        case CLEAR_TWS_DEV_PAIRED_INFO_ONLY:
        {
            libc_memset(&devs_cfg.tws_paired_list, 0x0, sizeof(bt_dev_t));
#ifdef ENABLE_TRUE_WIRELESS_STEREO
            clear_tws_info();
#endif
        }
        break;
        case CLEAR_ALL_DEV_PAIRED_INFO:
        {
            //初始化全局设备信息
            libc_memset(&devs_cfg, 0x0, sizeof(bt_devs_cfg_t));

            //初始化vram信息
            libc_memset(&vram_devs_cfg, 0x0, sizeof(vram_devs_cfg_t));
#ifdef ENABLE_TRUE_WIRELESS_STEREO
            clear_tws_info();
#endif
        }
        break;
        default:
        break;
    }

    if (g_p_global_var->only_hid_mode == 0)
    {
        vram_devs_cfg.magic = VRAM_MAGIC(VM_BT_DEV_CFG);
        sys_vm_write(&vram_devs_cfg, VM_BT_DEV_CFG, sizeof(vram_devs_cfg_t));
    }
    else
    {
        vram_devs_cfg.magic = VRAM_MAGIC(VM_BT_DEV_CFG_HID);
        sys_vm_write(&vram_devs_cfg, VM_BT_DEV_CFG_HID, sizeof(vram_devs_cfg_t));
    }
}

void clear_plist_info_async_start(uint8 clear_mode)
{
    btengine_clear_paired_list_async_deal_st* p_clpl_aync_info = &btengine_global_variable.clpl_async_info;

    if (clear_mode > CLEAR_ALL_DEV_PAIRED_INFO)
    {
        BTEG_PRINTF_ERROR("clear plist async failure! clear mode is not available\n");
        return;
    }

    //此时蓝牙仅仅是acl链路断开，后续服务断开的处理还需要依赖  该设备信息，所以配对设备信息
    //的清除必须在蓝牙完全断开后处理
    p_clpl_aync_info->need_clear_paired_list_async = TRUE;
    p_clpl_aync_info->clear_paired_list_timeout = CLEAR_PLIST_ASYNC_DEAL_TIMEOUT;
    p_clpl_aync_info->mode = clear_mode;

    switch (get_cur_device_status(btengine_global_variable.g_cur_rmt_dev))
    {
        case STATUS_LINKED:
        case STATUS_A2DP_PLAY:
        case STATUS_A2DP_PAUSE:
        case STATUS_CALL_INCOMING:
        case STATUS_CALL_OUTGOING:
        case STATUS_CALL_HFP:
        case STATUS_CALL_PHONE:
        BTEG_PRINTF("manual disconnect\n")
        ;

        app_msg_deal_force_waitpaired(0);
        break;

        default:
        break;
    }
}

void clear_plist_info_async_deal(btengine_clear_paired_list_async_deal_st* p_clpl_aync_info)
{
    if (p_clpl_aync_info->need_clear_paired_list_async == FALSE)
    {
        return;
    }

    //配对信息的清除必须在蓝牙完全断开后处理，可以通过服务连接标识判断蓝牙是否完全断开
    if ((CURRENT_DEVICE_SERVICE_CONNECT == 0))
    {
        clear_plist_info(p_clpl_aync_info->mode);
        p_clpl_aync_info->need_clear_paired_list_async = FALSE;
    }

    //超时处理，防止进入死循环
    p_clpl_aync_info->clear_paired_list_timeout--;

    if (p_clpl_aync_info->clear_paired_list_timeout == 0)
    {
        p_clpl_aync_info->need_clear_paired_list_async = FALSE;
        p_clpl_aync_info->clear_paired_list_timeout = CLEAR_PLIST_ASYNC_DEAL_TIMEOUT;
    }
}

bool restore_paired_info_form_vram(bt_dev_t* p_dev)
{
    uint32 *p_v_idx;
    uint32 v_idx;
    int32 ret_val = 0;
    uint8 res_idx;

    //get plist idx with insterd mru
    if (g_p_global_var->only_hid_mode == 0)
    {
        v_idx = VM_BT_DEV_CFG;
    }
    else
    {
        v_idx = VM_BT_DEV_CFG_HID;
    }

    //if vram info invaild ,init vram info
    if (sys_vm_read(&vram_devs_cfg, v_idx, sizeof(vram_devs_cfg_t)) < 0)
    {
        vram_devs_cfg.magic = 0xffff;
        BTEG_PRINTF_WARNNING("bt paired list vm read failed,v_idx %d\n", v_idx);
    }

    if (vram_devs_cfg.magic != VRAM_MAGIC(v_idx))
    {
        libc_memset(&vram_devs_cfg, 0x0, sizeof(vram_devs_cfg_t));
        vram_devs_cfg.magic = VRAM_MAGIC(v_idx);
        sys_vm_write(&vram_devs_cfg, v_idx, sizeof(vram_devs_cfg_t));
        return FALSE;
    }

    res_idx = ((uint32) p_dev - (uint32) devs_cfg.paired_list) / (sizeof(bt_dev_t));

    if (res_idx >= bt_manager_cfg_info.max_pair_list_num)
    {
        BTEG_PRINTF_WARNNING("param err(p_dev err)\n");
        return FALSE;
    }

    if (vram_devs_cfg.devs_status[res_idx].data_validity == 0)
    {
        BTEG_PRINTF_WARNNING("paired info isnot validity\n");
        return FALSE;
    }

    //restore plist info
    if (g_p_global_var->only_hid_mode == 0)
    {
        p_v_idx = (uint32 *) plist_vram_index;
    }
    else
    {
        p_v_idx = (uint32 *) plist_vram_index_hid;
    }

    ret_val = sys_vm_read(p_dev, p_v_idx[res_idx], sizeof(bt_dev_t));
    if (ret_val < 0)
    {
        BTEG_PRINTF_WARNNING("bt paired list vm read failed,res_idx %d\n", res_idx, 2);
        return FALSE;
    }

    //此时从vram中恢复的p_dev_status无效
    if (devs_cfg.paired_list[res_idx].p_dev_status != NULL)
    {
        devs_cfg.paired_list[res_idx].p_dev_status = NULL;
    }

    //restore global variable
    devs_cfg.paired_list[res_idx].profile = vram_devs_cfg.devs_status[res_idx].profile;
    devs_cfg.paired_list[res_idx].mru_num = vram_devs_cfg.devs_status[res_idx].mru_num;
    devs_cfg.paired_list[res_idx].data_validity = vram_devs_cfg.devs_status[res_idx].data_validity;
    BTEG_PRINTF("\nrestore paired success\n");

    return TRUE;
}

