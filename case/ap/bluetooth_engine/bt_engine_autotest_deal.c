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
 * \brief    MainMenu��main����ģ��
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "bluetooth_engine.h"

extern void RmtAddDevToAutoConnectList(bt_dev_t *p_dev);

uint8 g_start_bttest; //����ģʽ����ʼͨ��SCO���в���ʹ�绰����

bool bt_test_active(void)
{
    if (g_start_bttest)
    {
        return TRUE;
    }

    return FALSE;

}

void set_bt_test_active_st(bool status)
{
    g_start_bttest = status;
}

bool increase_bt_addr(uint8 *addr_bytes)
{
    uint8 i;

    bool ret_val;

    i = 0;
    ret_val = TRUE;

    for (i = 0; i < 3; i++)
    {
        addr_bytes[i]++;

        if (addr_bytes[i] != 0)
        {
            break;
        }
    }

    return ret_val;
}
void btengine_get_btinfo(private_msg_t *pri_msg)
{
    uint8 name_len;

    btengine_setinfo_t *bt_info_temp = (btengine_setinfo_t *) (pri_msg->msg.content.addr);

    name_len = (uint8) libc_strlen(g_btengine_var.device_name);
    if (name_len > BT_MAC_NAME_LEN_MAX)
    {
        name_len = BT_MAC_NAME_LEN_MAX;
    }
    libc_memcpy(bt_info_temp->bt_name, g_btengine_var.device_name, name_len);
    g_btengine_var.device_name[BT_MAC_NAME_LEN_MAX + 1] = 0;
    libc_memcpy(bt_info_temp->bt_addr, g_btengine_var.device_addr.bytes, BD_ADDR_LEN);
}

void btengine_set_btinfo(private_msg_t *pri_msg)
{
    uint8 i;
    uint8 result;
    uint8 name_len;
    bool write_vram = FALSE;
    bool write_bt_addr = FALSE;
    bool write_bt_name = FALSE;
    btengine_var_t temp_btengine_val;

    btengine_setinfo_t *bt_info_temp = (btengine_setinfo_t *) (pri_msg->msg.content.addr);

    name_len = (uint8) libc_strlen(g_btengine_var.device_name);
    if (name_len > BT_MAC_NAME_LEN_MAX)
    {
        name_len = BT_MAC_NAME_LEN_MAX;
    }

    //���Ʋ�һ�£��޸���������
    if (((bt_info_temp->modify_mode == BT_MODIFY_NAME) || (bt_info_temp->modify_mode == BT_MODIFY_ALL))\

            && (bt_info_temp->bt_name_len != 0))
    {
        if (name_len != bt_info_temp->bt_name_len)
        {
            libc_memset(g_btengine_var.device_name, 0, sizeof(g_btengine_var.device_name));
            if (bt_info_temp->bt_name_len > BT_MAC_NAME_LEN_MAX)
            {
                libc_memcpy(g_btengine_var.device_name, bt_info_temp->bt_name, BT_MAC_NAME_LEN_MAX);
                g_btengine_var.device_name[BT_MAC_NAME_LEN_MAX] = 0;
            }
            else
            {
                libc_memcpy(g_btengine_var.device_name, bt_info_temp->bt_name, bt_info_temp->bt_name_len);
            }
            write_vram = TRUE;
            write_bt_name = TRUE;
        }
        else
        {
            for (i = 0; i < name_len; i++)
            {
                if (g_btengine_var.device_name[i] != bt_info_temp->bt_name[i])
                {
                    break;
                }
            }

            if (i < name_len)
            {
                libc_memset(g_btengine_var.device_name, 0, sizeof(g_btengine_var.device_name));
                libc_memcpy(g_btengine_var.device_name, bt_info_temp->bt_name, bt_info_temp->bt_name_len);
                write_vram = TRUE;
                write_bt_name = TRUE;
            }
        }
    }

    //ֻ��������ǿ��дģʽ�����ߵ�ǰ�ǵ�һ���޸�������ַ���Ż����������ַ���޸�
    //burn_lock_flagΪ������־λ������һ�������̼�ʱ���ñ�־λΪ0���޸���������ַ
    //���־λ��Ϊ1��������֤������ַֻ���޸�һ��
    //����ͻ���Ҫ����޸ģ���ȥ������������
    if ((bt_info_temp->modify_mode == BT_MODIFY_ADDR) || (bt_info_temp->modify_mode == BT_MODIFY_ALL)
            || (bt_info_temp->force_write == TRUE))
    {
        result = FALSE;
        for (i = 0; i < 6; i++)
        {
            if (g_btengine_var.device_addr.bytes[i] != bt_info_temp->bt_addr[i])
            {
                result = TRUE;
                break;
            }
        }

        if (result == TRUE)
        {
            for (i = 0; i < 6; i++)
            {
                g_btengine_var.device_addr.bytes[i] = bt_info_temp->bt_addr[i];
            }

            write_vram = TRUE;

            write_bt_addr = TRUE;
        }
    }

    if (write_vram == TRUE)
    {
        if (g_p_global_var->only_hid_mode == 0)
        {
            sys_vm_write(&g_btengine_var, VM_BTENGINE, sizeof(btengine_var_t));
        }
        else
        {
            sys_vm_write(&g_btengine_var, VM_BTENGINE_HID, sizeof(btengine_var_t));
        }

        //���¼��� btengine_info
        com_init_btengine_info(&temp_btengine_val);

        if (libc_memcmp(&temp_btengine_val, &g_btengine_var, sizeof(temp_btengine_val)) != 0)
        {
            result = BTINFO_MODIFY_ERROR;
        }
        else
        {
            if (write_bt_addr == FALSE)
            {
                result = BTINFO_MODIFY_NAME_SUCCEED;
            }
            else
            {
                if (write_bt_name == FALSE)
                {
                    result = BTINFO_MODIFY_ADDR_SUCCEED;
                }
                else
                {
                    result = BTINFO_MODIFY_ALL_SUCCEED;
                }
            }
        }
    }
    else
    {
        result = BTINFO_MODIFY_SKIP;
    }

    //���÷��ؽ��
    *(uint8 *) (pri_msg->reply->content) = result;

    btengine_reply_msg(pri_msg, TRUE);

    return;
}

void btengine_set_connect_info(private_msg_t *pri_msg)
{
    btengine_setinfo_t *bt_info_temp = (btengine_setinfo_t *) (pri_msg->msg.content.addr);

    libc_memset(&devs_cfg, 0, sizeof(bt_devs_cfg_t));

    devs_cfg.tag = PAIRED_INFO_TAG;
    devs_cfg.pl_len = 1;

    libc_memcpy(devs_cfg.paired_list[0].dev_info.bd_addr.bytes, bt_info_temp->bt_PairedAddr, 6);

    devs_cfg.paired_list[0].profile |= (uint8) (PROFILE_VALID | HFP_SUPPORTED | AVRCP_SUPPORTED | A2DP_SUPPORTED);
    btengine_global_variable.g_cur_rmt_dev = &devs_cfg.paired_list[0];
    RmtAddDevToAutoConnectList(btengine_global_variable.g_cur_rmt_dev);
    devs_cfg.paired_list[0].data_validity = 1;
    updata_paired_list_length(&devs_cfg);
    save_rmtdev_info();
    //sys_vm_write(&devs_cfg.paired_list[0], VM_BT_PLIST0, sizeof(bt_dev_t));
    set_bt_test_active_st(TRUE);

    return;
}
void check_connect_timeout(void)
{
    if (bt_test_active() == FALSE)
    {
        return;
    }
    if (bt_linking_flag == TRUE)
    {
        time_out_count++;
        if ((time_out_count > 12) && (time_out_count != 0xff))
        {
            time_out_count = 0xff;
            BT_AUTO_TEST_PRINTF_WARNNING("\ntest mode reconnect timeout\n");
            bt_disconnect_acl(btengine_global_variable.g_cur_rmt_dev);

        }
    }
}
void test_disconnect_acl(bt_dev_t *dev)
{
    if (bt_test_active() == TRUE)
    {
        bt_disconnect_acl(dev);
    }
}
uint8 test_add_to_connectList(bt_dev_t *dev)
{
    if ((bt_test_active() == TRUE) && (time_out_count == 0xff))
    {
        time_out_count = 0;
        RmtAddDevToAutoConnectList(dev);
        return 1;
    }
    return 0;
}
