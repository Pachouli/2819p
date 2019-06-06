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
 * \file     bt_engine_avrcp.c
 * \brief    global variableģ��
 * \author   xiaomaky
 * \version  1.0
 * \date  2013/4/11
 *******************************************************************************/
#include "./../bluetooth_engine.h"

void HFOngoingcallInd(void *hdl);

static UINT32 APP_SCODataInd(HANDLE hdl, void *context, struct BuffStru *buf, UINT32 op_ev);

static struct HCI_CbkLineStru sco_hci_table[] =
{
{ HCI_EVO_SCO_DATA_IND, APP_SCODataInd } };

/*******************************************************************
 *                                                                  *
 ********************************************************************/
static UINT32 APP_SCODataInd(HANDLE hdl, void *context, struct BuffStru *buf, UINT32 op_ev)
{
#if 0
    UINT16 data_len = DATASIZE(buf);
    UINT16 conn_hdl;
    UINT16 packet_len;
    UINT8 *p;

    /* The incoming buffer may contain multiple SCO packets */
    while (data_len> 0)
    {
        p = BUFDATA(buf); /* SCO packet begin with 3bytes HCI SCO Header */
        conn_hdl = ((UINT16) * p + ((UINT16) * (p + 1) << 8)) & 0x0FFF;/* SCO Connection Handle, Little Endian */
        packet_len = *(p + 2);/* SCO data size */
        if (data_len >= packet_len)
        {
            p += 3;
            /* TODO: Play the voice data */

            data_len -= packet_len + 3;
        }
        else
        {
            //printf("\nHF-->SCO packet error, connection handle = %04X, SizeTotal = %d, SizeExpected = %d\n",
            //       conn_hdl, data_len, packet_len);
            break;
        }
        BuffRes(buf, (INT16)(packet_len + 3));
    }
#endif
    HFP_DBG_PRINTF("SCODataInd\n");
    FREE(buf);

    return 0;
}

static bt_dev_t *HfpFindDevByHfHdl(HANDLE hf_hdl)
{
    uint8 idx;
    bt_dev_t *p_dev;

    p_dev = NULL;

    for (idx = 0; idx < g_btengine_cfg_data.max_link_num; idx++)
    {
        if ((btengine_global_variable.linked_dev_list[idx] != NULL)
                && (btengine_global_variable.linked_dev_list[idx]->p_dev_status != NULL)
                && (btengine_global_variable.linked_dev_list[idx]->p_dev_status->g_hf_handle == hf_hdl))
        {
            p_dev = btengine_global_variable.linked_dev_list[idx];
            break;
        }
    }
    return p_dev;
}

#ifdef ENABLE_SIRI
//*************************************************
//ƻ�������豸����Э�����

//ʹ���Զ���AT����
int8 BT_HfpCheckSiriStatus(void)
{
    uint8 temp_buf[12];
    if ((CURRENT_DEVICE != NULL) && (CURRENT_DEVICE_P_STATUS != NULL))
    {
        //libc_memcpy(temp_buf, "AT+BVRA?\r", 8);
        libc_memcpy(temp_buf, "AT+APLSIRI?\r", 12);
        HFP_ExtendCmd((uint8 * )btengine_global_variable.g_cur_rmt_dev->p_dev_status->g_hf_handle, temp_buf, 12, 0);
    }
    return 0;
}
#endif

/*******************************************************************
 *              when client                                                 *
 ********************************************************************/
void HFSppConnectCfm(struct HFP_ConnectEvStru *in)
{
    bt_dev_t *dev;

    dev = RmtFindDevByBD(in->bd);
    if ((dev != NULL) && (dev->p_dev_status != NULL))
    {
        /* Now the APP can cancel the connection setup procedure */
        dev->p_dev_status->g_hf_handle = (HANDLE) in->handle;
    }
}

/*******************************************************************
 *              when server.  we are.                                                   *
 ********************************************************************/
void HFSppConnectInd(struct HFP_ConnectEvStru *in)
{
    bt_dev_t *dev;

    dev = RmtFindDevByBD(in->bd);
    if ((dev != NULL) && (dev->p_dev_status != NULL))
    {
        /* Now the APP can cancel the connection setup procedure */
        dev->p_dev_status->g_hf_handle = (HANDLE) in->handle;
    }
}

/*******************************************************************
 *       hfp���ӳɹ�                                                           *
 ********************************************************************/
//�����¼�
//1.ȷ��SCO���ݴ���ص�������HFP������ɣ�ʹ��HFP�лص�����APP_SCODataInd��������gap�����ص�����Ϊgap_sco_cbk
//2.����profile����״̬��Ŀǰ������״̬�������ӵ�profile������Ҫ������profile�����˳���profile����Ҫ�˳���profile��
//3.����hfp��״̬
//4.������·Ŀǰ״̬
//5.��ʼ��hfp�Ͽ�ԭ��
//6.��ʼ���绰��״̬
//7.�豸�����ѱ䶯�����浱ǰ�豸��Ϣ
//8.��ʼ���绰��(���֧��)
//9.��������ӳɹ�ʱ��������HFP״̬Ϊ����ͨ�����Ҵ������������ô�л���ongoing
//10.BQB�������⴦��
//11.��������(���֧��)
//12.�ɼ��Դ���
void HFSlcEstablishInd(struct HFP_SLCConnectInfoStru *in)
{
    bt_dev_t *dev;

    HFP_PRINTF("slc connected\n");

    dev = RmtFindDevByBD(in->bd);
    if ((dev != NULL) && (dev->p_dev_status != NULL))
    {
        BTSRC_HCI_Cbk_Reg(sco_hci_table, sizeof(sco_hci_table) / sizeof(struct HCI_CbkLineStru));
        gap_unreg_sco_cbk();
        //�����Ϻ��ȡ���������Ľ�ɫ(master/slaver)
        //bt_get_cur_role(dev);
        //�����Ҫ������־
        dev->p_dev_status->need_auto_connect &= (uint8) HFP_DISCONNECTED;
        //��¼hfp�����Ѿ�����
        dev->p_dev_status->service_connect |= HFP_CONNECTED;
        if (dev->p_dev_status->hfp_sub_status == STATUS_WAIT_PAIRED)
        {
            dev->p_dev_status->hfp_sub_status = STATUS_LINKED;
        }

        dev->profile |= HFP_CONNECTED;

        //hfp���ӳɹ������ϼ��a2dp�Ƿ���Ҫ�����������Ҫ���趨��־λ����������߳������̻���a2dp,�ӿ������ٶ�
        if (((g_p_info->service_connect & A2DP_CONNECTED) == 0)
                && ((g_p_info->need_auto_connect & A2DP_CONNECTED) != 0))
        {
            btengine_global_variable.need_connect_a2dp_right_now = TRUE;
        }

        save_rmtdev_info();
        if (g_btengine_cfg_data.hfp_pb_mode == HFP_PB_ONLY_NUM)
        {
            libc_memset(dev->p_dev_status->phone_number, 0, MAX_PHONENUM);
            dev->p_dev_status->phone_status = PB_FIND_NAME;
            btengine_global_variable.g_btengine_info.phone_status = PB_FIND_NAME;
        }

#ifdef CONFIG_PHONEBOOK
        if (g_btengine_cfg_data.hfp_pb_mode != 0)
        {
            pb_init();
        }
#endif
        btengine_global_variable.hfp.disconnect_reason = ER_HFP_RFCOMM_SUCCESS;
        switch (get_cur_device_status(dev))
        {
            case STATUS_WAIT_PAIRED:
            set_cur_device_status(dev, STATUS_LINKED);
            bt_linking_flag = FALSE;
            //ʲôʱ������������������
            if (dev->p_dev_status->hfp_sub_status == STATUS_CALL_HFP)
            {
                HFOngoingcallInd((void *) dev->p_dev_status->g_hf_handle);
            }

            else if (dev->p_dev_status->hfp_sub_status == STATUS_CALL_PHONE)
            {
                if ((BQB_TEST_ENABLE & btengine_global_variable.bqb_test) != 0)
                {
                    HFP_DBG_PRINTF("FOR BQB HFP\n");
                    HFOngoingcallInd((void *) dev->p_dev_status->g_hf_handle);
                    /* it is also OK that not creating sco link here but until pressing switch-key(prev/next). */
                    HFP_HF_AudioConnTrans((uint8 * ) dev->p_dev_status->g_hf_handle);
                }
            }
            else
            {
                ; //for qac
            }
            break;
            case STATUS_LINKED:
            break;
            case STATUS_A2DP_PLAY:
            break;
            case STATUS_A2DP_PAUSE:
            break;
            case STATUS_CALL_INCOMING:
            break;
            case STATUS_CALL_OUTGOING:
            break;
            case STATUS_CALL_HFP:
            break;
            case STATUS_CALL_PHONE:
            break;
            case STATUS_NONE:
            break;
            default:
            break;
        }
#ifdef NOISE_CONTROL
        {
            if (dev->p_dev_status != NULL)
            {
                HFP_HF_DisableNREC((UINT8 *)(dev->p_dev_status->g_hf_handle));
            }
        }
#endif
        if (btengine_global_variable.connect_lock_flag >= g_btengine_cfg_data.max_link_num)
        {
            if (current_visual_mode != GAP_VM_PAIR)
            {
                bt_set_visual_mode(GAP_VM_PAIR);

            }
        }
        else
        {
            if (current_visual_mode != GAP_VM_NONDISCOV)
            {
                bt_set_visual_mode(GAP_VM_NONDISCOV);

            }
        }
    }
    else
    {
        HFP_PRINTF_WARNNING("slc connect err\n");
    }
}

/*******************************************************************
 *                                                                  *
 ********************************************************************/
//�¼�����
//1.ȷ��SCO���ݴ���ص�������HFP������ɣ�ʹ��HFP�лص�����APP_SCODataInd��������gap�����ص�����Ϊgap_sco_cbk
//����profile����״̬
//���µ�ǰ�豸״̬
void HFSlcReleaseInd(struct HFP_SLCConnectInfoStru *in)
{
    bt_dev_t *dev;

    HFP_PRINTF("slc disconnected\n");

    dev = RmtFindDevByBD(in->bd);
    if (dev != NULL)
    {
        //ж��HCI����sco���ݷ�����ʱ��HFP�ص�������
        BTSRC_HCI_Cbk_UnReg(sco_hci_table);
        //ע��HCI��SCO���ӽ����ͶϿ��Ļص�����
        gap_reg_sco_cbk();
        btengine_global_variable.recv_quit_flag &= (uint8) HFP_DISCONNECTED;
        if (dev->p_dev_status != NULL)
        {
            dev->p_dev_status->service_connect &= (uint8) HFP_DISCONNECTED;
            dev->p_dev_status->hfp_sub_status = STATUS_WAIT_PAIRED;
            if ((dev->p_dev_status->acl_handle != 0) && (dev->p_dev_status->service_connect != 0))
            {
                if (dev->p_dev_status->a2dp_sub_status == STATUS_A2DP_PLAY)
                {
                    set_cur_device_status(dev, STATUS_A2DP_PLAY);
                }
                else
                {
                    set_cur_device_status(dev, STATUS_LINKED);
                }
            }
        }
    }
}

//�¼�����
//1.��ȡhfp server hdl
void HFServerStartCfm(struct HFP_RegServerStru *in, void *hdl)
{
    if ((in->role & HFP_ROLE_HF_SERVER) != 0)
    {
        // HF Server
        g_btengine_cfg_data.g_hf_server_hdl = (HANDLE) hdl;
    }
    else
    {
        // HS Server
        g_btengine_cfg_data.g_hs_server_hdl = (HANDLE) hdl;
    }
}

//�¼�����
//1.��ȡhfp server hdl
void HFUpperDecideEstablishScoInd(struct HFP_PreSCOConnIndStru *in)
{

    //δ����BQB Test
    if ((BQB_TEST_ENABLE & btengine_global_variable.bqb_test) == 0)
    {
        return;
    }

    in->pkt_type = HCI_PACKET_TYPE_DEFAULT2_SYNCHRONOUS;
    in->blocked = HCI_RETRANSMISSION_EFFORT_DONOT_CARE;
    in->result = HFP_CONN_SCO_DEFAULT;
}

void HFSppEstablishInd(struct HFP_SLCConnectInfoStru *in)
{
}

//�����¼�
//1.��ȡHFP�Ͽ�ԭ��
//2.ɾ��sco����ʱ��
//3.���sco buf
//4.���̻���
//5.����hfp���
//6.��ȷ��RFCOMM�Ѿ��Ͽ�ʱ�����浱ǰ�豸״̬���
void HFSppDisconnectInd(struct HFP_ConnectEvStru *in)
{
    bt_dev_t *dev;

    //in->result��hfp_ui.h�е�HFP_Connection_Done reason
    HFP_PRINTF("spp discon reason %d\n", (uint8 ) in->result);

    btengine_global_variable.hfp.disconnect_reason = (uint8) in->result;

    dev = RmtFindDevByBD(in->bd);
    if (dev != NULL)
    {
        if (dev->p_dev_status != NULL)
        {
            dev->p_dev_status->g_hf_handle = 0;

        }

        if (btengine_global_variable.hfp.sco.create_sco_timer_id != -1)
        {
            kill_app_timer(THIS_APP_TIMER_GROUP, btengine_global_variable.hfp.sco.create_sco_timer_id);
            btengine_global_variable.hfp.sco.create_sco_timer_id = -1;
        }

        hfp_standby_deal();
        if (((g_p_info->need_auto_connect & 0x0f) == 0) && (ER_HFP_MULTICONNECT_BETWEEN_TWOENDS == in->result))
        {
            HFP_PRINTF("multi connect dis");
            if ((dev->profile & HFP_CONNECTED) != 0)
            {
                reconnect_last_device(btengine_global_variable.g_cur_rmt_dev);
            }
        }
        if (dev->p_dev_status == NULL)
        {
            return;
        }

        /* for BQB, not to modify profile, or else cannot reconnect HFP. */
        if ((BQB_TEST_ENABLE & btengine_global_variable.bqb_test) == 0)
        {
            if (((in->result == ER_HFP_RFCOMM_CONNECT_FAILED) || (in->result == ER_HFP_RFCOMM_DISCONNECT_COMPLETE))
                    && (get_cur_device_status(dev) != STATUS_WAIT_PAIRED))
            {
                dev->profile &= (uint8) HFP_DISCONNECTED;
                save_rmtdev_info();
            }
        }
    }
}

//�����¼�
//1.���sco buff
void hfp_standby_deal(void)
{
    clear_sco_buf();
}

//�˳����硢ȥ�硢ͨ��ʱ�����¼�
//1.����绰��״̬(�ͳ�ʼ��һ����-Ӧ�������ó�ʼ��״̬)
//2.����ͨ������
//3.�������粥����Ϣ��״̬�������������δ��ɣ��ȴ����
//4.����hfp��״̬
//5.����ǰ�豸״̬.�鿴�Ƿ���Ҫ����
//6.�Ͽ�sco����
//7.ж��sco����ʱ��
//8.��ʼ��hf_clcc
void HFStandbyInd(void *hdl)
{
    bt_dev_t *dev;

    dev = HfpFindDevByHfHdl((HANDLE) hdl);
    if ((dev != NULL) && (dev->p_dev_status != NULL))
    {
        /* State changed indications */
        if (g_btengine_cfg_data.hfp_pb_mode == HFP_PB_ONLY_NUM)
        {
            libc_memset(dev->p_dev_status->phone_number, 0, MAX_PHONENUM);
            dev->p_dev_status->phone_status = PB_FIND_NAME;
            btengine_global_variable.g_btengine_info.phone_status = PB_FIND_NAME;
            //force_stop_tts_pb_deal(); //�ȴ�������Ϣ�������
        }

#ifdef SUPPORT_3_WAY_CALL
        btengine_global_variable.g_btengine_info.three_way_status = THREE_WAY_HOLD_NOT_EXIST;
#endif

#ifdef CONFIG_PHONEBOOK
        if (g_btengine_cfg_data.hfp_pb_mode != 0)
        {
            libc_memset(&pb_result, 0, sizeof(pb_search_result_t) );
            pb_result.validstatus = PB_FIND_NAME;
            btengine_global_variable.g_btengine_info.phone_status = 0;
            pb_info.pb_enable = 1;

            //force_stop_tts_pb_deal(); //�ȴ�������Ϣ�������
        }
#endif

        switch (get_cur_device_status(dev))
        {
            case STATUS_WAIT_PAIRED:
            break;
            case STATUS_LINKED:
            break;
            case STATUS_A2DP_PLAY:
            break;
            case STATUS_A2DP_PAUSE:
            break;
            case STATUS_CALL_INCOMING:
            case STATUS_CALL_OUTGOING:
            case STATUS_CALL_HFP:
            case STATUS_CALL_PHONE:
            dev->p_dev_status->hfp_sub_status = STATUS_LINKED;

            if (btengine_global_variable.g_sbc_thread_exist != 0)
            {
                set_cur_device_status(dev, STATUS_A2DP_PLAY);
            }
            else
            {
                set_cur_device_status(dev, STATUS_LINKED);
                /* ���tab3Զ�˹ҵ绰��A2DP_SEND_SEP_START_IND��standby��BUG00387932 */
                if (CURRENT_DEVICE_A2DP_SUB_STATUS == STATUS_A2DP_PLAY)
                {
                    set_exit_schedule_flag(TRUE);
                }
            }
            if (((dev->p_dev_status->service_connect) & A2DP_CONNECTED) != 0)
            {
                if (btengine_global_variable.g_cur_rmt_dev->p_dev_status->sco_handle_setup != 0)
                {
                    bt_engine_hfp_sco_disconnect_ind_handle();
                }
            }
            hfp_standby_deal();
            break;
            case STATUS_NONE:
            break;
            default:
            break;
        }
#ifdef BT_SP_GETCALLINFO
        unload_call_timer();
#endif
        libc_memset(&(g_btengine_cfg_data.hfp_clcc_info), 0x00, sizeof(g_btengine_cfg_data.hfp_clcc_info));
    }
    else
    {
        HFP_PRINTF_WARNNING("stanby err\n");

    }
}
//����ͨ�绰��
//�����¼�
//1.���õ绰��״̬(�ͳ�ʼ��Ҳһ����  Ӧ�������ó�ʼ��״̬)
//2.�������粥����Ϣ��״̬�������������δ��ɣ��ȴ����
//3.���ĵ�ǰ�豸״̬
//4.HFP��״̬
//5.����sco���ӣ����ö�ʱ������mic����
void HFOngoingcallInd(void *hdl)
{
    bt_dev_t *dev;

    HFP_PRINTF("ongoing call\n");

    dev = HfpFindDevByHfHdl((HANDLE) hdl);
    if ((dev != NULL) && (dev->p_dev_status != NULL))
    {
        if (dev->p_dev_status->sco_handle_setup == 0)
        {
            //sco��·û�н������������ֻ�����
            dev->p_dev_status->hfp_sub_status = STATUS_CALL_PHONE;
        }
        else
        {
            dev->p_dev_status->hfp_sub_status = STATUS_CALL_HFP;
        }
    }
    else
    {
        HFP_PRINTF_WARNNING("device null\n");
    }
    /* filter message before status is linked. hongmi note  */
    if ((dev != NULL) && (get_cur_device_status(dev) != STATUS_WAIT_PAIRED))
    {
        btengine_global_variable.g_cur_rmt_dev = dev;
        /* sangsung S8538 , maybe status is ongoing, but no call, when linking.  */
        //    if ( (g_p_info->service_connect & HFP_CONNECTED) == 0)
        //    {
        //        return;
        //    }
        //force_stop_tts_deal();
        if (g_btengine_cfg_data.hfp_pb_mode == HFP_PB_ONLY_NUM)
        {
            libc_memset(dev->p_dev_status->phone_number, 0, MAX_PHONENUM);
            dev->p_dev_status->phone_status = PB_FIND_NAME;
            btengine_global_variable.g_btengine_info.phone_status = PB_FIND_NAME;
            //force_stop_tts_pb_deal(); //�ȴ�������Ϣ�������
        }

#ifdef CONFIG_PHONEBOOK
        if (g_btengine_cfg_data.hfp_pb_mode != 0)
        {
            pb_info.pb_enable = 0;
            libc_memset(&pb_result, 0, sizeof(pb_search_result_t) );
            pb_result.validstatus = PB_FIND_NAME;
            btengine_global_variable.g_btengine_info.phone_status = 0;

            //force_stop_tts_pb_deal(); //�ȴ�������Ϣ�������
        }
#endif
        HFP_DBG_PRINTF("sco_handle %d\n", dev->p_dev_status->sco_handle_setup);
        if (dev->p_dev_status->sco_handle_setup == 0)
        {
            //sco��·û�н������������ֻ�����
            set_cur_device_status(dev, STATUS_CALL_PHONE);

        }
        else
        {
            set_cur_device_status(dev, STATUS_CALL_HFP);
            if (g_p_info->enter_hfp_flag == FALSE)
            {
                bt_engine_hfp_sco_connect_ind_handle();
            }
            load_micout_timer();

        }
    }
    else
    {
    }
}
//����״̬
//�����¼�
//1.��������豸Ϊ���豸
//2.���ĵ�ǰ�豸״̬
//3.����HFP��״̬
//4.�绰������
void HFIncomingcallInd(void *hdl)
{
    bt_dev_t *dev;

    HFP_PRINTF("incoming call\n");

    dev = HfpFindDevByHfHdl((HANDLE) hdl);
    if ((dev != NULL) && (dev->p_dev_status != NULL))
    {
        btengine_global_variable.g_cur_rmt_dev = dev;
        set_cur_device_status(dev, STATUS_CALL_INCOMING);
        dev->p_dev_status->hfp_sub_status = STATUS_CALL_INCOMING;
        //force_stop_tts_deal();

#ifdef CONFIG_PHONEBOOK
        if (g_btengine_cfg_data.hfp_pb_mode != 0)
        {
            pb_info.pb_enable = 0;
        }
#endif

    }
}
//�γ��绰��
//�����¼�
//1.��������豸Ϊ���豸
//2.���ĵ�ǰ�豸״̬
//3.����HFP��״̬
//4.����SCO����
void HFOutgoingcallInd(void *hdl)
{
    bt_dev_t *dev;

    HFP_PRINTF("outgoing call\n");
    dev = HfpFindDevByHfHdl((HANDLE) hdl);
    if ((dev != NULL) && (dev->p_dev_status != NULL))
    {
        btengine_global_variable.g_cur_rmt_dev = dev;
        set_cur_device_status(dev, STATUS_CALL_OUTGOING);
        dev->p_dev_status->hfp_sub_status = STATUS_CALL_OUTGOING;

        //force_stop_tts_deal();
        if ((dev->p_dev_status->sco_handle_setup) != 0)
        {
            bt_engine_hfp_sco_connect_ind_handle();
        }
#ifdef CONFIG_PHONEBOOK
        if (g_btengine_cfg_data.hfp_pb_mode != 0)
        {
            pb_info.pb_enable = 0;
        }
#endif
    }
}

void HFCallheldInd(void)
{
    HFP_PRINTF("call held\n");
}

//ͨ���ȴ�
//�����¼�
//1.��������豸Ϊ���豸
//2.���ĵ�ǰ�豸״̬
//3.����HFP��״̬
//4.����SCO����
//5.����ͨ������
void HFCallWatingInd(struct HFP_PhoneInfoStru *in, void *hdl)
{
    bt_dev_t *dev;

    HFP_PRINTF("call wait\n");
    dev = HfpFindDevByHfHdl((HANDLE) hdl);
    if (dev != NULL)
    {
#ifdef SUPPORT_3_WAY_CALL
        btengine_global_variable.g_btengine_info.three_way_status = THREE_WAY_NEW_CALL_IN;
#else
        //��֧������ͨ��
        if (g_p_info->enter_hfp_flag == TRUE)
        {
            //Reject the waiting call
            HFP_HF_3WayCallingHandler((uint8 *) dev->p_dev_status->g_hf_handle, 0, 0);
        }
#endif
    }
}

//ȷ��AT����
//�����¼�
//1.�绰������
//2.��������豸Ϊ���豸
//3.��鵱ǰ�豸�Ƿ����SIRI״̬�����ø���HFP��״̬
void HFATCommandCfm(struct HFP_ATCmdResultStru *in, void *hdl)
{
    bt_dev_t *dev;

    dev = HfpFindDevByHfHdl((HANDLE) hdl);
    if (dev != NULL)
    {
        /* Confirmation to the requests (AT Commands) from APP */
#ifdef CONFIG_PHONEBOOK
        if (g_btengine_cfg_data.hfp_pb_mode == HFP_PB_ALL)
        {
            if((pb_info.status == PB_CSCS_CFG) || (pb_info.status == PB_CPBS_SELECT))
            {
                pending = FALSE;
            }
        }
#endif

#ifdef ENABLE_SIRI
        //    if ( btengine_global_variable.g_check_siri_answer != 0 )

        {
            if ((in->cmd_code == HFP_HFC_BVRA_1) && (in->result_code == HFP_APPERR_OK))
            {
                HFP_PRINTF("SIRION!\n");
                set_cur_device_status(dev, STATUS_SIRI);
            }
            else if ((in->cmd_code == HFP_HFC_BVRA_0) && (in->result_code == HFP_APPERR_OK))
            {
                HFP_PRINTF("SIRIOFF!\n");
            }
            else
            {
                ; //QAC need
            }
        }
#endif
    }
}

void HFRingingInd(void *hdl, UINT8 in_band)
{
    HFP_PRINTF("HF ringing\n");

    bt_dev_t *dev;
    dev = HfpFindDevByHfHdl((HANDLE) hdl);

    set_cur_device_status(dev, STATUS_CALL_INCOMING);
    dev->p_dev_status->hfp_sub_status = STATUS_CALL_INCOMING;

    /* Local indications */
    if (in_band == 0)
    {
        ;/* TODO: Generate local ring tone */
    }
    else
    {
        ;/* else, in band ring tone */
    }
}

//����SCO��·
//�����¼�
//1.���õ�ǰ�豸Ϊ�����豸
//2.���µ�ǰdev״̬
//3.����hfp��״̬
//4.���hfp״̬Ϊ����ͨ��������ͨ������
//5.��ȡ��sco handle
//6.sco���ӽ�������
void HFAudioConnEstablishedInd(uint16 sco_handle, void *hdl)
{
    bt_dev_t *dev;

    HFP_PRINTF("sco audio conn ind\n");

    dev = HfpFindDevByHfHdl((HANDLE) hdl);
    if (dev != NULL)
    {
        /*  TODO: Switch local audio path to Bluetooth SCO/eSCO link */
        if (btengine_global_variable.g_cur_rmt_dev->p_dev_status->sco_handle_setup == 0)
        {
            btengine_global_variable.g_cur_rmt_dev = dev;
            dev->p_dev_status->sco_handle_setup = sco_handle;
            if (dev->p_dev_status->hfp_sub_status == STATUS_CALL_PHONE)
            {
                dev->p_dev_status->hfp_sub_status = STATUS_CALL_HFP;
                HFOngoingcallInd((void *) dev->p_dev_status->g_hf_handle);
            }
            bt_engine_hfp_sco_connect_ind_handle();
        }
        dev->p_dev_status->sco_handle_setup = sco_handle;
    }
}
//�ͷ�SCO��·
//�����¼�
//1.�¹���ǰ�豸Ϊ����״̬����ô��ж��sco����
void HFAudioConnReleasedInd(uint16 sco_handle, void *hdl)
{
    bt_dev_t *dev;

    HFP_PRINTF("sco audio ind\n");
    dev = HfpFindDevByHfHdl((HANDLE) hdl);
    if (dev != NULL)
    {
        /*  TODO: Swith local audio path to local audio adapter */
        if (dev->p_dev_status != NULL)
        {
            dev->p_dev_status->sco_handle_setup = 0;
        }
        if (btengine_global_variable.g_cur_rmt_dev == dev)
        {
            bt_engine_hfp_sco_disconnect_ind_handle();
        }
    }
}

void HFTerminateLocalringtoneInd(void)
{
    //HFP_DBG_PRINTF("Terminate local ring tone and un-mute the audio link\n");
    /* TODO: The application shall terminate its local ring tone here if it has been started before. */
}

void HFManufaturerIDInd(UINT8 *vendor_id)
{
    //HFP_DBG_PRINTF("AG Manufacturer ID retrieved\n");
}

void HFModelIDInd(UINT8 *vendor_id)
{
    //HFP_DBG_PRINTF("AG Modle ID retrieved\n");
}

//�����¼�
//1.����bvra
//2.����ǰ�豸״̬
void HFVoiceRecognitionActivatedInd(void *hdl)
{
    bt_dev_t *dev;

    HFP_PRINTF("VR active!\n");
    dev = HfpFindDevByHfHdl((HANDLE) hdl);
    if (dev != NULL)
    {
        /* Indications or responses from AG */
        g_btengine_cfg_data.g_hf_bvra_enable = 1;
#ifdef ENABLE_SIRI
        {
            set_cur_device_status(dev, STATUS_SIRI);
        }
#endif
    }
}
//�л�����/�˳�����ʶ��
//�����¼�
//1.
//2.����ǰ�豸״̬
//3.���ĵ�ǰ�豸״̬��
//4.�˳�siri
void HFVoiceRecognitionDeactivatedInd(void *hdl)
{
    bt_dev_t *dev;

    HFP_PRINTF("VR deactive\n");
    dev = HfpFindDevByHfHdl((HANDLE) hdl);
    if (dev != NULL)
    {
        g_btengine_cfg_data.g_hf_bvra_enable = 0;
#ifdef ENABLE_SIRI
        //    if ( btengine_global_variable.g_check_siri_answer == 0 )

        {
            if (get_cur_device_status(dev) == STATUS_SIRI)
            {
                btengine_global_variable.hfp.check_siri_answer = 0;
                set_cur_device_status(dev, STATUS_LINKED);
                if (btengine_global_variable.hfp.voice_recognition_status == ACTIVATE)
                {
                    hfp_standby_deal();
                    btengine_global_variable.hfp.voice_recognition_status = INACTIVE;
                }
                else
                {
                    btengine_global_variable.hfp.voice_recognition_status = ACTIVATE;
                }
            }
        }
#endif
    }
}

void HFNetwordAvailableInd(void)
{
    //HFP_DBG_PRINTF("Cellular Network is available\n");
}

void HFNetwordUnavailableInd(void)
{
    //HFP_DBG_PRINTF("Cellular Network is unavailable\n");
}

void HFRoamingResetInd(void)
{
    //HFP_DBG_PRINTF("Roaming is not active\n");
}

void HFRoamingActiveInd(void)
{
    //HFP_DBG_PRINTF("Roaming is active\n");
}

void HFSignalStrengthInd(UINT8 val)
{
    //HFP_DBG_PRINTF("The current signal strength\n");
}

void HFBatteryChargeInd(UINT8 val)
{
    //HFP_DBG_PRINTF("The battery level of AG\n");
}

void HFCallonChldholdActivatedInd(void *hdl)
{
    bt_dev_t *dev;

    HFP_PRINTF("HF-->An active call is put on hold\n");
    dev = HfpFindDevByHfHdl((HANDLE) hdl);
    if ((dev != NULL) && (dev->p_dev_status != NULL))
    {
#ifdef SUPPORT_3_WAY_CALL
        btengine_global_variable.g_btengine_info.three_way_status = THREE_WAY_HOLD_EXIST;
#else
        /* AT+CLCC to AG, and change current state in IVTLib.  */
        HFP_HF_GetCurrentCalls((uint8 *) dev->p_dev_status->g_hf_handle);
#endif
    }

}

//�����¼�
//21.����ǰ�豸״̬
void HFCallonChldholdReleasedInd(void *hdl)
{
    bt_dev_t *dev;

    HFP_PRINTF("held call is released\n");
    dev = HfpFindDevByHfHdl((HANDLE) hdl);
    if (dev != NULL)
    {
#ifdef SUPPORT_3_WAY_CALL
        btengine_global_variable.g_btengine_info.three_way_status = THREE_WAY_HOLD_NOT_EXIST;
#endif

        /* Whether the call is activated or released is determined by the indication previous to
         HFP_EV_CALLON_CHLDHOLD_RELEASED_IND. */
        switch (get_cur_device_status(dev))
        {
            case STATUS_WAIT_PAIRED:
            break;
            case STATUS_LINKED:
            break;
            case STATUS_A2DP_PLAY:
            break;
            case STATUS_A2DP_PAUSE:
            break;
            case STATUS_CALL_INCOMING:
            case STATUS_CALL_OUTGOING:
            case STATUS_CALL_HFP:
            case STATUS_CALL_PHONE:
            /* for mastone, no HFStandbyInd come when hangup sometimes.   */
            if ((g_btengine_cfg_data.hfp_cind_info.call) == 0)
            {
                if (btengine_global_variable.g_sbc_thread_exist != 0)
                {
                    set_cur_device_status(dev, STATUS_A2DP_PLAY);
                }
                else
                {
                    set_cur_device_status(dev, STATUS_LINKED);
                }
            }
            break;
            case STATUS_NONE:
            break;
            default:
            break;
        }
    }
}

//�����¼�
//1.����ǰ�豸״̬
void HFMicvolChangedInd(UINT8 vol)
{
    //HFP_DBG_PRINTF("New microphone gain is\n");

    /* TODO: Change the local microphone volume. */
    g_btengine_cfg_data.g_hf_mic_vol = vol;
    //�������ֵӦ����hfp�Լ������ϲ����ͨ���ӿڼ���Ƿ��и���
    btengine_global_variable.hfp.rmt_dev_vol = vol;
}

#ifdef ENABLE_HFP_VOLUME_SYNC
//�ñ�־λ�������ϲ�apͬ���ֻ�������������
void update_vol_to_ap(uint8 level)
{
    g_p_info->vol_syn_flag = 1;
    g_p_info->remote_volume = level;
}
#endif
//ͬ���ֻ��˵ĵ������������
//�����¼�
//1.����ͬ������
void HFSpkvolChangedInd(UINT8 vol, void *hdl)
{
    bt_dev_t *dev;

    HFP_PRINTF("VGS\n");
    dev = HfpFindDevByHfHdl((HANDLE) hdl);
    if (dev != NULL)
    {
#ifdef ENABLE_HFP_VOLUME_SYNC
        if (bt_manager_cfg_info.sp_hfp_vol_syn != 0)
        {
            if (btengine_global_variable.hfp.allow_sync_vol_to_ap_flag == FALSE)
            {
                return;
            }
            if ((g_p_info->enter_hfp_flag == 1) && ((dev->p_dev_status->service_connect & HFP_CONNECTED) != 0)) //����iphone����ʽͬ��

            {
                HFP_PRINTF("HFP VOL:%d\n", vol);
                g_btengine_cfg_data.g_hf_spk_vol = vol; //vol ��0��15
                update_vol_to_ap((uint8) vol);
            }
        }
#endif
    }
}
//��ȡ�绰��Ϣ
//�����¼�
//1.���粥������
void HFClipInd(struct HFP_PhoneInfoStru *in, void *hdl)
{
    bt_dev_t *dev;

    HFP_PRINTF("get call info\n");

    dev = HfpFindDevByHfHdl((HANDLE) hdl);
    if (dev != NULL)
    {
        if (g_btengine_cfg_data.hfp_pb_mode == HFP_PB_ONLY_NUM)
        {
            //ֻ�е�һ�β�ѯʱ����Ӧap�л������ҵ绰��Ϣ
            if (dev->p_dev_status->phone_status == PB_FIND_NAME)
            {

                /*some phone(such as mastone), number is null, +CLIP"",   */
                if (in->num_len != 0)
                {
                    libc_memset(dev->p_dev_status->phone_number, 0, MAX_PHONENUM);
                    if (in->num_len > MAX_PHONENUM)
                    {
                        libc_memcpy(dev->p_dev_status->phone_number, in->number, MAX_PHONENUM);
                    }
                    else
                    {
                        libc_memcpy(dev->p_dev_status->phone_number, in->number, (unsigned int ) (in->num_len));
                    }
                    //libc_printhex(dev->p_dev_status->phone_number, MAX_PHONENUM, 0);
                    dev->p_dev_status->phone_status = PB_NUM_VALID;
                    btengine_global_variable.g_btengine_info.phone_status = PB_NUM_VALID;
                    btengine_global_variable.g_btengine_info.phone_buf = dev->p_dev_status->phone_number;
                }
            }
        }
    }

#ifdef CONFIG_PHONEBOOK
    if (g_btengine_cfg_data.hfp_pb_mode != HFP_PB_DISABLE)
    {
        //ֻ�е�һ�β�ѯʱ����Ӧap�л������ҵ绰��Ϣ
        if(pb_result.validstatus == PB_FIND_NAME)
        {

            /*some phone(such as mastone), number is null, +CLIP"",   */
            if(in->num_len != 0)
            {
                libc_memset(pb_result.pb_data.phonenum, 0, MAX_NUM);
                if(in->num_len> MAX_NUM)
                {
                    libc_memcpy(pb_result.pb_data.phonenum, in->number, MAX_NUM);
                }
                else
                {
                    libc_memcpy(pb_result.pb_data.phonenum, in->number, (unsigned int)(in->num_len));
                }

                btengine_global_variable.g_btengine_info.phone_buf = pb_result.pb_data.phonenum;
                //libc_printhex(pb_result.pb_data.phonenum, MAX_NUM, 2);
                if(((g_btengine_cfg_data.hfp_pb_mode != HFP_PB_ONLY_NUM)) && (pb_search_bynum(&pb_result.pb_data) == 0))
                {
                    pb_result.validstatus = PB_NAME_VALID;
                    btengine_global_variable.g_btengine_info.phone_status = PB_NAME_VALID;
                    btengine_global_variable.g_btengine_info.phone_buf = pb_result.pb_data.name;
                    HFP_DBG_PRINTF("find call tell\n");
                }
                else
                {
                    pb_result.validstatus = PB_NUM_VALID;
                    btengine_global_variable.g_btengine_info.phone_status = PB_NUM_VALID;
                    btengine_global_variable.g_btengine_info.phone_buf = pb_result.pb_data.phonenum;
                }
            }
        }
    }
#endif
}

void HFCurrentCallsInd(struct HFP_CLCCInfoStru *in, void *hdl)
{
    bt_dev_t *dev;

    HFP_PRINTF("current call\n");

    dev = HfpFindDevByHfHdl((HANDLE) hdl);
    if (dev != NULL)
    {
        libc_memcpy(&(g_btengine_cfg_data.hfp_clcc_info), in, sizeof(g_btengine_cfg_data.hfp_clcc_info));HFP_DBG_PRINTF("clcc status %d\n", g_btengine_cfg_data.hfp_clcc_info.status);
#ifdef SUPPORT_3_WAY_CALL
        //5=waiting(incoming),��ʱΪ����ͨ��״̬������һ��ͨ�����У�һ��incoming���ڵȴ�����
        if (g_btengine_cfg_data.hfp_clcc_info.status == 5)
        {
            btengine_global_variable.g_btengine_info.three_way_status = THREE_WAY_NEW_CALL_IN;
        }
#endif        
    }
}

void HFNetworkOPInd(struct HFP_COPSInfoStru *in)
{
    if ((BQB_TEST_ENABLE & btengine_global_variable.bqb_test) != 0)
    {
        if (0 == libc_memcmp(in->operator_name, "Bluetooth SIG", (uint32 ) in->operator_len))
        {
            HFP_PRINTF_WARNNING("\nBluetooth SIG BQB\n");
        }
    }
}

void HFSubscriberNumberInd(struct HFP_PhoneInfoStru *in)
{
    //HFP_DBG_PRINTF("SubscriberNumber\n");
}

void HFVoicetagPhoneNumInd(struct HFP_PhoneInfoStru *in)
{
    //HFP_DBG_PRINTF("The phone number for voice tag\n");

    /* TODO: Attach the phone number to a voice tag. */
}

//��ȡ��ǰͨ��״̬
//�����¼�
//1.��ӡ��ǰͨ��״̬
//2.���浱ǰͨ��״̬
void HFTransparentSubmitcievInd(uint8 indicator)
{
    HFP_PRINTF("ciev ind\n");
    if (indicator >= 40)
    {
        return;
    }
    else if (indicator >= 30)/* 30--no callheld,  31--active-hold,  32--onhold. */
    {
#ifdef SUPPORT_3_WAY_CALL
        if (indicator == 30)
        {
            HFP_PRINTF("no callheld\n");
        }
        else if (indicator == 31)
        {
            HFP_PRINTF("active-hold\n");
        }
        else if (indicator == 32)
        {
            HFP_PRINTF("onhold\n");
        }
        else
        {
        }
#endif
        g_btengine_cfg_data.hfp_cind_info.callheld = indicator - 30;
        //HFP_DBG_PRINTF("ciev ind\n");
    }
    else if (indicator >= 20)/* 20--no callsetup,  21--incoming,  22--outgoing,  23--outalert. */
    {
#ifdef SUPPORT_3_WAY_CALL
        if (indicator == 20)
        {
            HFP_PRINTF("no callsetup\n");
            if (btengine_global_variable.g_btengine_info.three_way_status != THREE_WAY_HOLD_EXIST)
            {
                btengine_global_variable.g_btengine_info.three_way_status = THREE_WAY_HOLD_NOT_EXIST;
            }
        }
        else if (indicator == 21)
        {
            HFP_PRINTF("21--incoming\n");
        }
        else if (indicator == 22)
        {
            HFP_PRINTF("22--outgoing\n");
        }
        else if (indicator == 23)
        {
            HFP_PRINTF("23--outalert\n");
        }
        else
        {
        }
#endif
        g_btengine_cfg_data.hfp_cind_info.callsetup = indicator - 20;
    }
    else if (indicator >= 10)/* 10--not active,  11--active. */
    {
#ifdef SUPPORT_3_WAY_CALL
        if (indicator == 10)
        {
            HFP_PRINTF("not active\n");
        }
        else if (indicator == 11)
        {
            HFP_PRINTF("active\n");
        }
        else
        {
        }
#endif
        g_btengine_cfg_data.hfp_cind_info.call = indicator - 10;
    }
    else
    {
    }
}

void HFExtResultCodeInd(void *in, UINT32 len)
{
    HFP_PRINTF("ex cmd:%s\n", in);

#ifdef CONFIG_PHONEBOOK
    if (g_btengine_cfg_data.hfp_pb_mode == HFP_PB_ALL)
    {
        //HFExtResultCodeInd����һ�����ǵ绰����صĻظ�������˫�����绰��
        pb_loop_deal_rsp(in, len);
    }
#endif
    parse_hfp_cclk_cmd_param(in);

}

UINT8 hfp_event_dbgprintf(void *hdl, UINT8 event, UINT8 *param, UINT32 len)
{
    switch (event)
    {
        /* Server Start Confirmation */
        case HFP_EV_SERVER_START_CFM:/* param: struct HFP_RegServerStru */
        HFP_DBG_PRINTF("-%d- HFP_EV_SERVER_START_CFM\n",event);
        break;
        /* Connect Start Confirmation */
        case HFP_EV_CONNECT_CFM:/* param: struct HFP_ConnectEvStru */
        HFP_DBG_PRINTF("-%d- HFP_EV_CONNECT_CFM\n",event);
        break;
        /* Connect Start indication */
        case HFP_EV_CONNECT_IND:/* param: struct HFP_ConnectEvStru */
        HFP_DBG_PRINTF("-%d- HFP_EV_CONNECT_IND\n",event);
        break;
        /* Ask app whether to establish SCO */
        case HFP_EV_UPPER_DECIDE_ESTABLISH_SCO_IND:
        HFP_DBG_PRINTF("-%d- HFP_EV_UPPER_DECIDE_ESTABLISH_SCO_IND\n",event);
        break;
        /* Connection Complete indication */
        case HFP_EV_SPP_ESTABLISHED_IND:/* param: struct HFP_SLCConnectInfoStru */
        HFP_DBG_PRINTF("-%d- HFP_EV_SPP_ESTABLISHED_IND\n",event);
        break;
        case HFP_EV_SLC_ESTABLISHED_IND:/* param: struct HFP_SLCConnectInfoStru */
        HFP_DBG_PRINTF("-%d- HFP_EV_SLC_ESTABLISHED_IND\n",event);
        break;
        /* Connection Terminated indication */
        case HFP_EV_SLC_RELEASED_IND:/* param: HFP_SLCConnectInfoStru */
        HFP_DBG_PRINTF("-%d- HFP_EV_SLC_RELEASED_IND\n",event);
        break;
        case HFP_EV_DISCONNECT_COMPLETE:/* param: struct HFP_ConnectEvStru */
        HFP_DBG_PRINTF("-%d- HFP_EV_DISCONNECT_COMPLETE\n",event);
        break;
        case HFP_EV_STANDBY_IND:
        HFP_DBG_PRINTF("-%d- HFP_EV_STANDBY_IND\n",event);
        break;
        case HFP_EV_ONGOINGCALL_IND:
        HFP_DBG_PRINTF("-%d- HFP_EV_ONGOINGCALL_IND\n",event);
        break;
        case HFP_EV_INCOMINGCALL_IND:
        HFP_DBG_PRINTF("-%d- HFP_EV_INCOMINGCALL_IND\n",event);
        break;
        case HFP_EV_OUTGOINGCALL_IND:
        HFP_DBG_PRINTF("-%d- HFP_EV_OUTGOINGCALL_IND\n",event);
        break;
        case HFP_EV_CALLHELD_IND:/* param: NULL */
        HFP_DBG_PRINTF("-%d- HFP_EV_CALLHELD_IND\n",event);
        break;
        case HFP_EV_CALL_WAITING_IND:/* param: struct HFP_PhoneInfoStru */
        HFP_DBG_PRINTF("-%d- HFP_EV_CALL_WAITING_IND\n",event);
        break;
        case HFP_EV_ATCMD_RESULT_IND:/* param: struct HFP_ATCmdResultStru */
        //HFP_DBG_PRINTF("-%d- HFP_EV_ATCMD_RESULT_IND\n",event);
        break;
        case HFP_EV_RINGING_IND:/* param: UINT8 in_band */
        HFP_DBG_PRINTF("-%d- HFP_EV_RINGING_IND\n",event);
        break;
        case HFP_EV_AUDIO_CONN_ESTABLISHED_IND:/* param: UINT16 sco_hdl */
        HFP_DBG_PRINTF("-%d- HFP_EV_AUDIO_CONN_ESTABLISHED_IND\n",event);
        break;
        case HFP_EV_AUDIO_CONN_RELEASED_IND:/* param: UINT16 sco_hdl */
        HFP_DBG_PRINTF("-%d- HFP_EV_AUDIO_CONN_RELEASED_IND\n",event);
        break;
        case HFP_EV_TERMINATE_LOCAL_RINGTONE_IND:
        HFP_DBG_PRINTF("-%d- HFP_EV_TERMINATE_LOCAL_RINGTONE_IND\n",event);
        break;
        case HFP_EV_HF_MANUFACTURERID_IND:/* param: UINT8 *vendor_id */
        HFP_DBG_PRINTF("-%d- HFP_EV_HF_MANUFACTURERID_IND\n",event);
        break;
        case HFP_EV_HF_MODELID_IND:/* param: UINT8 *vendor_id */
        HFP_DBG_PRINTF("-%d- HFP_EV_HF_MODELID_IND\n",event);
        break;
        case HFP_EV_VOICE_RECOGN_ACTIVATED_IND:
        HFP_DBG_PRINTF("-%d- HFP_EV_VOICE_RECOGN_ACTIVATED_IND\n",event);
        break;
        case HFP_EV_VOICE_RECOGN_DEACTIVATED_IND:
        HFP_DBG_PRINTF("-%d- HFP_EV_VOICE_RECOGN_DEACTIVATED_IND\n",event);
        break;
        case HFP_EV_NETWORK_AVAILABLE_IND:
        HFP_DBG_PRINTF("-%d- HFP_EV_NETWORK_AVAILABLE_IND\n",event);
        break;
        case HFP_EV_NETWORK_UNAVAILABLE_IND:
        HFP_DBG_PRINTF("-%d- HFP_EV_NETWORK_UNAVAILABLE_IND\n",event);
        break;
        case HFP_EV_ROAMING_RESET_IND:
        HFP_DBG_PRINTF("-%d- HFP_EV_ROAMING_RESET_IND\n",event);
        break;
        case HFP_EV_ROAMING_ACTIVE_IND:
        HFP_DBG_PRINTF("-%d- HFP_EV_ROAMING_ACTIVE_IND\n",event);
        break;
        case HFP_EV_SIGNAL_STRENGTH_IND:/* param: UINT8 val */
        HFP_DBG_PRINTF("-%d- HFP_EV_SIGNAL_STRENGTH_IND\n",event);
        break;
        case HFP_EV_BATTERY_CHARGE_IND:/* param: UINT8 val */
        HFP_DBG_PRINTF("-%d- HFP_EV_BATTERY_CHARGE_IND\n",event);
        break;
        case HFP_EV_CALLON_CHLDHOLD_ACTIVATED_IND:
        HFP_DBG_PRINTF("-%d- HFP_EV_CALLON_CHLDHOLD_ACTIVATED_IND\n",event);
        break;
        case HFP_EV_CALLON_CHLDHOLD_RELEASED_IND:
        HFP_DBG_PRINTF("-%d- HFP_EV_CALLON_CHLDHOLD_RELEASED_IND\n",event);
        break;
        case HFP_EV_MICVOL_CHANGED_IND:/* param: UINT8 vol */
        HFP_DBG_PRINTF("-%d- HFP_EV_MICVOL_CHANGED_IND\n",event);
        break;
        case HFP_EV_SPKVOL_CHANGED_IND:/* param: UINT8 vol */
        HFP_DBG_PRINTF("-%d- HFP_EV_SPKVOL_CHANGED_IND\n",event);
        break;
        case HFP_EV_CLIP_IND:/* param: struct HFP_PhoneInfoStru */
        HFP_DBG_PRINTF("-%d- HFP_EV_CLIP_IND\n",event);
        break;
        case HFP_EV_CURRENT_CALLS_IND:/* param: struct HFP_CLCCInfoStru */
        HFP_DBG_PRINTF("-%d- HFP_EV_CURRENT_CALLS_IND\n",event);
        break;
        case HFP_EV_NETWORK_OPERATOR_IND:/* param: struct HFP_COPSInfoStru */
        HFP_DBG_PRINTF("-%d- HFP_EV_NETWORK_OPERATOR_IND\n",event);
        break;
        case HFP_EV_SUBSCRIBER_NUMBER_IND:/* param: struct HFP_PhoneInfoStru */
        HFP_DBG_PRINTF("-%d- HFP_EV_SUBSCRIBER_NUMBER_IND\n",event);
        break;
        case HFP_EV_VOICETAG_PHONE_NUM_IND:/* param: struct HFP_PhoneInfoStru *info */
        HFP_DBG_PRINTF("-%d- HFP_EV_VOICETAG_PHONE_NUM_IND\n",event);
        break;
        case HFP_EV_TRANSPARENT_SUBMITCIEV_IND:/* param: UINT8 val */
        HFP_DBG_PRINTF("-%d- HFP_EV_TRANSPARENT_SUBMITCIEV_IND\n",event);
        break;

        /* Extended result codes from AG */
        case HFP_EV_EXTEND_CMD_IND: /* param: RAW data */
        HFP_DBG_PRINTF("-%d- HFP_EV_EXTEND_CMD_IND\n",event);
        break;
        default:
        break;
    }

    return 1;
}

/*******************************************************************
 *                                                                  *
 ********************************************************************/
UINT8 APP_HFEventCbk(void *hdl, UINT8 event, UINT8 *param, UINT32 len)
{
    hfp_event_dbgprintf(hdl, event, param, len);

    switch (event)
    {
        /* Server Start Confirmation */
        case HFP_EV_SERVER_START_CFM:/* param: struct HFP_RegServerStru */
        HFServerStartCfm((struct HFP_RegServerStru *) param, hdl);
        break;
        /* Connect Start Confirmation */
        case HFP_EV_CONNECT_CFM:/* param: struct HFP_ConnectEvStru */
        HFSppConnectCfm((struct HFP_ConnectEvStru *) param);
        break;
        /* Connect Start indication */
        case HFP_EV_CONNECT_IND:/* param: struct HFP_ConnectEvStru */
        HFSppConnectInd((struct HFP_ConnectEvStru *) param);
        break;
        /* Ask app whether to establish SCO */
        case HFP_EV_UPPER_DECIDE_ESTABLISH_SCO_IND:
        HFUpperDecideEstablishScoInd((struct HFP_PreSCOConnIndStru *) param);
        break;
        /* Connection Complete indication */
        case HFP_EV_SPP_ESTABLISHED_IND:/* param: struct HFP_SLCConnectInfoStru */
        HFSppEstablishInd((struct HFP_SLCConnectInfoStru *) param);
        break;
        case HFP_EV_SLC_ESTABLISHED_IND:/* param: struct HFP_SLCConnectInfoStru */
        HFSlcEstablishInd((struct HFP_SLCConnectInfoStru *) param);
        break;
        /* Connection Terminated indication */
        case HFP_EV_SLC_RELEASED_IND:/* param: HFP_SLCConnectInfoStru */
        HFSlcReleaseInd((struct HFP_SLCConnectInfoStru *) param);
        break;
        case HFP_EV_DISCONNECT_COMPLETE:/* param: struct HFP_ConnectEvStru */
        HFSppDisconnectInd((struct HFP_ConnectEvStru *) param);
        break;
        case HFP_EV_STANDBY_IND:
        HFStandbyInd(hdl);
        break;
        case HFP_EV_ONGOINGCALL_IND:
        HFOngoingcallInd(hdl);
        break;
        case HFP_EV_INCOMINGCALL_IND:
        HFIncomingcallInd(hdl);
        break;
        case HFP_EV_OUTGOINGCALL_IND:
        HFOutgoingcallInd(hdl);
        break;
        case HFP_EV_CALLHELD_IND:/* param: NULL */
        HFCallheldInd();
        break;
        case HFP_EV_CALL_WAITING_IND:/* param: struct HFP_PhoneInfoStru */
        HFCallWatingInd((struct HFP_PhoneInfoStru *) param, hdl);
        break;
        case HFP_EV_ATCMD_RESULT_IND:/* param: struct HFP_ATCmdResultStru */
        HFATCommandCfm((struct HFP_ATCmdResultStru *) param, hdl);
        break;
        case HFP_EV_RINGING_IND:/* param: UINT8 in_band */
        HFRingingInd(hdl, *(UINT8 *) param);
        break;
        case HFP_EV_AUDIO_CONN_ESTABLISHED_IND:/* param: UINT16 sco_hdl */
        HFAudioConnEstablishedInd(*(UINT16 *) param, hdl);
        break;
        case HFP_EV_AUDIO_CONN_RELEASED_IND:/* param: UINT16 sco_hdl */
        HFAudioConnReleasedInd(*(UINT16 *) param, hdl);
        break;
        case HFP_EV_TERMINATE_LOCAL_RINGTONE_IND:
        HFTerminateLocalringtoneInd();
        break;
        case HFP_EV_HF_MANUFACTURERID_IND:/* param: UINT8 *vendor_id */
        HFManufaturerIDInd(param);
        break;
        case HFP_EV_HF_MODELID_IND:/* param: UINT8 *vendor_id */
        HFModelIDInd(param);
        break;
        case HFP_EV_VOICE_RECOGN_ACTIVATED_IND:
        HFVoiceRecognitionActivatedInd(hdl);
        break;
        case HFP_EV_VOICE_RECOGN_DEACTIVATED_IND:
        HFVoiceRecognitionDeactivatedInd(hdl);
        break;
        case HFP_EV_NETWORK_AVAILABLE_IND:
        HFNetwordAvailableInd();
        break;
        case HFP_EV_NETWORK_UNAVAILABLE_IND:
        HFNetwordUnavailableInd();
        break;
        case HFP_EV_ROAMING_RESET_IND:
        HFRoamingResetInd();
        break;
        case HFP_EV_ROAMING_ACTIVE_IND:
        HFRoamingActiveInd();
        break;
        case HFP_EV_SIGNAL_STRENGTH_IND:/* param: UINT8 val */
        HFSignalStrengthInd(*(UINT8 *) param);
        break;
        case HFP_EV_BATTERY_CHARGE_IND:/* param: UINT8 val */
        HFBatteryChargeInd(*(UINT8 *) param);
        break;
        case HFP_EV_CALLON_CHLDHOLD_ACTIVATED_IND:
        HFCallonChldholdActivatedInd(hdl);
        break;
        case HFP_EV_CALLON_CHLDHOLD_RELEASED_IND:
        HFCallonChldholdReleasedInd(hdl);
        break;
        case HFP_EV_MICVOL_CHANGED_IND:/* param: UINT8 vol */
        HFMicvolChangedInd(*(UINT8 *) param);
        break;
        case HFP_EV_SPKVOL_CHANGED_IND:/* param: UINT8 vol */
        HFSpkvolChangedInd(*(UINT8 *) param, hdl);
        break;
        case HFP_EV_CLIP_IND:/* param: struct HFP_PhoneInfoStru */
        HFClipInd((struct HFP_PhoneInfoStru *) param, hdl);
        break;
        case HFP_EV_CURRENT_CALLS_IND:/* param: struct HFP_CLCCInfoStru */
        HFCurrentCallsInd((struct HFP_CLCCInfoStru *) param, hdl);
        break;
        case HFP_EV_NETWORK_OPERATOR_IND:/* param: struct HFP_COPSInfoStru */
        HFNetworkOPInd((struct HFP_COPSInfoStru *) param);
        break;
        case HFP_EV_SUBSCRIBER_NUMBER_IND:/* param: struct HFP_PhoneInfoStru */
        HFSubscriberNumberInd((struct HFP_PhoneInfoStru *) param);
        break;
        case HFP_EV_VOICETAG_PHONE_NUM_IND:/* param: struct HFP_PhoneInfoStru *info */
        HFVoicetagPhoneNumInd((struct HFP_PhoneInfoStru *) param);
        break;
        case HFP_EV_TRANSPARENT_SUBMITCIEV_IND:/* param: UINT8 val */
        HFTransparentSubmitcievInd(*(UINT8 *) param);
        break;

        /* Extended result codes from AG */
        case HFP_EV_EXTEND_CMD_IND: /* param: RAW data */
        HFExtResultCodeInd(param, len);
        break;
        default:
        break;
    }

    return 1;
}

//hf��������������ͱ���ͨ����������Ϣ
//��ȡ��ǰͨ��״̬
//�����¼�
//1.��ӡ��ǰͨ��״̬
void HFCmdInRing(bt_dev_t *dev, int32 choice)
{
    if ((dev != NULL) && (dev->p_dev_status != NULL))
    {
        switch (choice)
        {
            case HFP_EV_ANSWER_CALL_REQ: // Answer the incoming call
            HFP_PRINTF("answer incoming call\n")
            ;
            BTSRC_HFP_HF_AnswerCall((uint8 * ) dev->p_dev_status->g_hf_handle);
            break;
            case HFP_EV_CANCEL_CALL_REQ: // Reject the incoming call
#if 0            
            if (g_btengine_cfg_data.hfp_clcc_info.status == 0) /* callheld=0  */
            {
                HFP_PRINTF("cancel calling\n");
                BTSRC_HFP_HF_CancelCall((uint8 *) dev->p_dev_status->g_hf_handle);
            }
            else if (g_btengine_cfg_data.hfp_clcc_info.status == 1) /* callheld=1  */
            {
                HFP_PRINTF("3way calling\n");
                HFP_HF_3WayCallingHandler((uint8 *) dev->p_dev_status->g_hf_handle, 1,
                        (UINT8) (g_btengine_cfg_data.hfp_clcc_info.idx));
            }
            else
            {
                BTSRC_HFP_HF_CancelCall((uint8 *) dev->p_dev_status->g_hf_handle);
            }
#else
            //clcc status must be get in real time,and now 3waycalling can never enter this function,
            //so,just cancel call is ok
            HFP_PRINTF("cancel calling\n")
            ;
            BTSRC_HFP_HF_CancelCall((uint8 * ) dev->p_dev_status->g_hf_handle);
#endif
            break;
            case HFP_EV_HOLD_CALL_REQ: // BTRH hold the incoming call
            BTSRC_HFP_HF_HoldIncomingCall((uint8 * ) dev->p_dev_status->g_hf_handle);
            break;
            default:
            //printf("Invalid command.\nHF>");
            break;
        }
    }
}

//�Զ�����ʱ����ע��ͻ��˷�������
bool HFConnectAG(bt_dev_t *dev)
{
    if (dev == NULL)
    {
        return FALSE;
    }

    BTSRC_HFP_Client_Start((void * ) g_btengine_cfg_data.g_hf_server_hdl, dev->dev_info.bd_addr.bytes, NULL,
            HFP_ROLE_HF_CLIENT, APP_HFEventCbk);
    return TRUE;
}

//�Ͽ��ͻ��˷�������
void HFDisconnectAG(bt_dev_t *dev)
{
    if ((dev != NULL) && (dev->p_dev_status != NULL) && (dev->p_dev_status->g_hf_handle != 0))
    {
        BTSRC_HFP_Client_Stop((uint8 * ) dev->p_dev_status->g_hf_handle);
    }
}

void hfp_sco_connected_cfm(bt_dev_t *dev)
{
    if (bt_check_dev_exist(dev) == FALSE)
    {
        HFP_PRINTF_WARNNING("\nhfp device null\n");
        return;
    }

    if ((dev->p_dev_status->service_connect & HFP_CONNECTED) != 0)
    {
#ifdef NOISE_CONTROL
        HFP_HF_DisableNREC((UINT8 *)(dev->p_dev_status->g_hf_handle));
#endif
#ifdef ENABLE_SIRI
        BT_HfpCheckSiriStatus();
#endif
    }
}

void APP_HFP_Start(void)
{
    UINT32 sdp_hdl;
    UINT32 features;
    features = 0;

    /* Hands-free HF server */

#ifdef NOISE_CONTROL
    /*support noise reduction add*/
    features = features | HFP_BRSF_HF_NREC;
#endif

#ifdef SUPPORT_3_WAY_CALL
    /*support 3_way call add*/
    if (bt_manager_cfg_info.sp_hfp_3way_call == 1)
    {
        features = features | HFP_BRSF_HF_3WAYCALL;

    }
#endif

#ifdef ENABLE_HFP_VOLUME_SYNC
    /*support volume sync add*/
    if (bt_manager_cfg_info.sp_hfp_vol_syn == 1)
    {
        features = features | HFP_BRSF_HF_RMTVOLCTRL;
    }
#endif    

    features = features | HFP_BRSF_HF_CLIP | HFP_BRSF_HF_BVRA | HFP_BRSF_HF_ENHANCED_CALLSTATUS
            | HFP_BRSF_HF_ENHANCED_CALLCONTROL;

    //HFP_BRSF_HF_BVRA(����ʶ��):�ù��ܴ򿪺󣬶���С���ֻ�����������������󣬻Ὠ��SCO\ESCO��·��
    //�˳��������ֺ���ʱ�ֻ��˻�û�������Ͽ�sco��·�����Ե����Ƹ�ʱ�������ֻ��˳��֡�
    //����ù���û�д򿪣���û�и�����
    BTSRC_HFP_Server_Start(features, 1, &sdp_hdl, HFP_ROLE_HF_SERVER, g_btengine_cfg_data.max_link_num, APP_HFEventCbk);
}

void APP_HFP_Stop(void)
{
    BTSRC_HFP_Server_Stop((void * ) g_btengine_cfg_data.g_hf_server_hdl);
}

