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
 * \brief    global variableģ��
 * \author   xiaomaky
 * \version  1.0
 * \date  2013/4/11
 *******************************************************************************/
#include "./../bluetooth_engine.h"

uint8 a2dp_sc_prefer_channelmode[A2DP_SC_PREFER_CHANNELMODE_NUMBER] =
{ A2DP_SBC_SF_44100 | A2DP_SBC_CHMODE_JOINT_STEREO, /* such as HTC-G8.  */
A2DP_SBC_SF_44100 | A2DP_SBC_CHMODE_STEREO, A2DP_SBC_SF_48000 | A2DP_SBC_CHMODE_JOINT_STEREO, /* such as xiaomi4huilian.  */
A2DP_SBC_SF_48000 | A2DP_SBC_CHMODE_STEREO };

//���������豸�б���(������TWS)����Stream������ҵ�ǰ�豸
bt_dev_t * DevFineByStreamHdl(void *stream_hdl)
{
    uint8 idx;
    bt_dev_t *dev = NULL;

    btengine_global_variable.g_cur_rmt_dev_paried_index = 0;
    for (idx = 0; idx < g_btengine_cfg_data.max_link_num; idx++)
    {
        if ((btengine_global_variable.linked_dev_list[idx] != NULL)
                && (btengine_global_variable.linked_dev_list[idx]->p_dev_status != NULL)
                && (btengine_global_variable.linked_dev_list[idx]->p_dev_status->a2dp_source.stream_handle == stream_hdl))
        {
            btengine_global_variable.g_cur_rmt_dev_paried_index = idx;
            dev = btengine_global_variable.linked_dev_list[idx];
            break;
        }
    }

    return dev;
}

//���������豸�б���(����TWS)����Stream������ҵ�ǰ�豸
bt_dev_t *A2dpFindDevByStreamHdl(void *stream_hdl)
{
    bt_dev_t *p_dev;

    p_dev = NULL;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (bt_manager_cfg_info.sp_tws_link == 1)
    {
        p_dev = TWS_FindByStreamHdl(stream_hdl);
        if (p_dev != NULL)
        {
            return p_dev;
        }
        else
        {
            if (stream_hdl == tws_status_info.a2dp_stream_status.stream_hdl)
            {
                tws_status_info.a2dp_stream_status.status = A2DP_STREAM_DONE;
                tws_status_info.a2dp_stream_status.stream_hdl = NULL;
            }
        }
    }
#endif

    p_dev = DevFineByStreamHdl(stream_hdl);

    return p_dev;
}

//���Զ������豸�б���ͨ��Stream����ҵ���Ӧ���豸��Ϣ
bt_dev_t *A2dpFindDevInAutoConnectList(void *stream_hdl)
{
    uint8 idx;
    bt_dev_t *p_dev;

    p_dev = NULL;

    for (idx = 0; idx < g_btengine_cfg_data.max_link_num; idx++)
    {
        if ((btengine_global_variable.autoconnect_dev_list[idx] != NULL)
                && (btengine_global_variable.autoconnect_dev_list[idx]->p_dev_status != NULL)
                && (btengine_global_variable.autoconnect_dev_list[idx]->p_dev_status->a2dp_source.stream_handle
                        == stream_hdl))
        {
            p_dev = btengine_global_variable.autoconnect_dev_list[idx];
            break;
        }
    }
    return p_dev;
}

//����A2DP SNK Sied�ڵ�
void save_snk_seid(struct A2DP_SEIDStru *seid)
{
    uint8 i;

    A2DP_DBG_PRINTF("SNK seid\n");

    for (i = 0; i < (g_btengine_cfg_data.max_link_num * 2); i++)
    {
        if ((g_btengine_cfg_data.s_local_snk_seid[i]) == NULL)
        {
            g_btengine_cfg_data.s_local_snk_seid[i] = seid;
            break;
        }
    }
    if (i == (g_btengine_cfg_data.max_link_num * 2))
    {
        A2DP_PRINTF_ERROR("SEP id over\n");
        while (1)
        {
            ; //for qac
        }
    }
}

#ifdef ENABLE_TRUE_WIRELESS_STEREO
void tws_save_src_seip(struct A2DP_SEIDStru *seid)
{
    A2DP_TWS_PRINTF("SRC seid\n");

    if (g_btengine_cfg_data.s_local_src_seid != NULL)
    {
        FREE(g_btengine_cfg_data.s_local_src_seid);
    }
    g_btengine_cfg_data.s_local_src_seid = seid;
}
#endif

//����Seid�ڵ���Ϣ
void A2dpRegSepInd(struct A2DP_SEIDStru *seid)
{
    uint8 a2dp_tsep_type;

    a2dp_tsep_type = ((seid->media_type) >> 3) & 0x01;

    //�������TWS�����У�������SNK��ɫ
    if (a2dp_tsep_type == A2DP_TSEP_SNK)
    {
        save_snk_seid(seid);
    }
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    //����ͬʱ����SNK��ɫ�Լ�SRC��ɫ
    else if (a2dp_tsep_type == A2DP_TSEP_SRC)
    {
        tws_save_src_seip(seid);
    }
    else
    {
        ; // do nothing
    }
#endif
}

//��ʼ�������ʽ��Ϣ��׼������ռ�
void init_codec_params(uint8 codec_mode)
{
    if (codec_mode == A2DP_CODEC_AAC)
    {
        if (btengine_global_variable.media_codec_type != A2DP_CODEC_AAC)
        {
            if (btengine_global_variable.avdtp_data_source == AVDTP_SOURCE_BT_A2DP)
            {
                g_a2dp_input_buf.raw_data = (uint8 *) (AAC_INPUT_BUF);
                g_a2dp_input_buf.capacity = AAC_INPUT_BUF_LENGTH;
                g_a2dp_input_buf.length = 0;
                g_a2dp_input_buf.write_ptr = 0;
                g_a2dp_input_buf.read_ptr = 0;
                g_a2dp_input_buf.full_error = FALSE;
            }
        }

        set_avdtp_frame_buf((uint8 *) AAC_DIRECT_INPUT_BUFFER, AVDTP_AAC_HEADER_LEN);

        btengine_global_variable.media_codec_type = A2DP_CODEC_AAC;
    }
    else
    {
        if (btengine_global_variable.media_codec_type != A2DP_CODEC_SBC)
        {
            if (btengine_global_variable.avdtp_data_source == AVDTP_SOURCE_BT_A2DP)
            {
                g_a2dp_input_buf.raw_data = (uint8 *) (SBC_INPUT_BUFFER_ADDRESS);
                g_a2dp_input_buf.capacity = A2DP_SBC_INPUT_BUFFER_LEN;
                g_a2dp_input_buf.length = 0;
                g_a2dp_input_buf.write_ptr = 0;
                g_a2dp_input_buf.read_ptr = 0;
                g_a2dp_input_buf.full_error = FALSE;
            }
        }

        set_avdtp_frame_buf((uint8 * ) SBC_DIRECT_INPUT_BUFFER, AVDTP_SBC_HEADER_LEN);

        btengine_global_variable.media_codec_type = A2DP_CODEC_SBC;
    }
}

//��ȡSBC������Ϣ
A2dpSbcInfoStru *A2dpGetSbcInfo(struct A2DP_ServiceCapStru *sc)
{
    UINT8 *p;
    A2dpSbcInfoStru *info = NULL;/* Invaild, AVDTP_CODEC_SBC */
    UINT8 i;
    UINT8 len = 0;
    UINT8 codec_mode = 0;

    if (sc != NULL)
    {
        i = sc->len;
        p = sc->service_capability;
        while (i >= 2)
        {
            /* Service Cagtegory(1) + LOSC(1) */
            if (*p == A2DP_SC_MEDIA_CODEC)
            {
                if (i >= (4 + len))
                {
                    codec_mode = *(p + 3);
                    init_codec_params(codec_mode);

                    info = (A2dpSbcInfoStru *) (p + 4);
                    break;
                }
            }
            else
            {
                /* Service Category not match, goto next */
                len = 2 + *(p + 1);
                i -= (uint8) MIN(i, len);
                p += len;
            }
        }
    }

    return info;
}

//��ȡ���ݱ��淽ʽ
uint8 A2dpGetCPInfo(struct A2DP_ServiceCapStru *sc)
{
    UINT8 *p;
    UINT8 i;
    UINT8 len = 0;

    if (sc != NULL)
    {
        i = sc->len;
        p = sc->service_capability;
        while (i >= 2)
        {
            /* Service Cagtegory(1) + LOSC(1) */
            if (*p == A2DP_SC_CONTENT_PROTECTION)
            {
                if (i >= BTA_AV_CP_INFO_LEN)
                {
                    if ((*(p + 1) == BTA_AV_CP_LOSC) && (*(p + 2) == (uint8) (BTA_AV_CP_SCMS_T_ID))
                            && (*(p + 3) == (uint8) (BTA_AV_CP_SCMS_T_ID >> 8)))
                    {
                        return BTA_AV_CP_SCMS_T_ID;
                    }

                    break;
                }
            }
            else
            {
                /* Service Category not match, goto next */
                len = 2 + *(p + 1);
                i -= (uint8) MIN(i, len);
                p += len;
            }
        }
    }

    return FALSE;
}

//��ȡ��Դ�˵����˵���
//1.��ȡ�豸״̬�����a2dp���豸״̬Ϊ�գ���ô��ʼ��
//2.��־a2dp��������
//3.��ȡstream_handle
//4.step 1
void A2dpStreamCreateInd(struct A2DP_StreamCreateStru *in)
{
    bt_dev_t *dev;

    dev = RmtFindDevByBD(in->remote_bd);

    if (dev != NULL)
    {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (dev->dev_type == TWS_DEV)
        {
            A2DP_TWS_PRINTF("stream create,role 0x%x\n", in->mask); //bit0:master initiator  bit1:src  otherwise:snk
        }
        else
#endif
        {
            A2DP_PRINTF("stream create,role 0x%x\n", in->mask); //bit0:master initiator  bit1:src  otherwise:snk
        }

        /* p_dev_status is malloc. for auto-connect nexus7 only support a2dp. 2015/9/21 19:25:12  */
        if (dev->p_dev_status == NULL)
        {
            dev->p_dev_status = NEW(sizeof(bt_dev_status_t));
            libc_memset(dev->p_dev_status, 0x00, sizeof(bt_dev_status_t));
        }
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (bt_manager_cfg_info.sp_tws_link == 1)
        {
            if (dev->dev_type == TWS_DEV)
            {

                TWS_SetA2DPSource(dev, in->mask);
                dev->p_dev_status->a2dp_source.stream_handle = in->stream_handle;
                tws_status_info.a2dp_stream_status.status = A2DP_STREAM_CREATED;
                tws_status_info.a2dp_stream_status.stream_hdl = in->stream_handle;
                FREE(in);
                return;
            }
        }
#endif
        btengine_global_variable.a2dp_linking_flag = TRUE;
        if (dev->p_dev_status->a2dp_source.stream_handle == NULL)
        {
            dev->p_dev_status->a2dp_source.stream_handle = in->stream_handle;
        }
        else
        {
            A2DP_PRINTF_WARNNING("SNK Created conflict\n");
            //A2dpDisconnect(dev);
            dev->p_dev_status->a2dp_source.stream_handle = in->stream_handle;
        }
    }
    FREE(in);
}

//���ý���AVRCP������Ϣ
//1.��ȡԶ���豸������Ϣ������SBC�����ʽ�����ݱ�����ʽ
//2.step 2
void A2dpSetConfigInd(struct A2DP_SetConfigStru *in)
{
    A2dpSbcInfoStru *pSbcInfo;
    bt_dev_t *dev;
    bool need_GetSbcInfo = TRUE;

    dev = A2dpFindDevByStreamHdl(in->stream_handle);

    if (dev != NULL)
    {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (dev->dev_type == TWS_DEV)
        {
            A2DP_TWS_DBG_PRINTF("set config\n");
            TWS_MsgListInit();
            //���������ȡSbcInof�������ֻ����Ӽ���
            need_GetSbcInfo = FALSE;
            if ((dev->p_dev_status != NULL) && (dev->p_dev_status->a2dp_source.is_source == 0))
            {
                need_GetSbcInfo = TRUE;
            }
        }
        else
#endif
        {
            A2DP_DBG_PRINTF("set config\n");
        }

        if (need_GetSbcInfo == TRUE)
        {
            pSbcInfo = A2dpGetSbcInfo(&in->sc);
            dev->p_dev_status->a2dp_source.cp_active = A2dpGetCPInfo(&in->sc);
        }
    }
    FREE(in);

    //��avrcp����ʱ����ǰ��open codec�׶�
    //Avrcp_Connect_Req(dev);
}

//��ȡԶ���豸A2DP��������֧��
void A2dpGetConfigInd(struct A2DP_ServiceCapStru *in)
{
    FREE(in);
}

//Զ�˱������Ѿ�����֪ͨ
//1.����AVRCP
//2.step 3
void A2dpOpenCodecInfoInd(struct A2DP_OpenCodecInfoIndStru *in)
{
    bt_dev_t *dev;

    dev = A2dpFindDevByStreamHdl((void *) in->stream_handle);
    if (dev != NULL)
    {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (dev->dev_type == TWS_DEV)
        {
            A2DP_TWS_DBG_PRINTF("opencode\n");
        }
        else
#endif
        {
            A2DP_DBG_PRINTF("opencode\n");
        }

#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (bt_manager_cfg_info.sp_tws_link == 1)
        {
            if (dev->dev_type == TWS_DEV)
            {
                //����䲻��Ҫ֧��AVRCP����
                FREE(in);
                return;
            }
        }
#endif
        A2DP_PRINTF("connect avrcp\n");
        Avrcp_Connect_Req(dev);
    }
    FREE(in);
}

//�����豸״̬Ϊ����״̬������a2dp���ӳɹ�״̬
void A2dpConnectedDevStatusUpdata(bt_dev_t *dev)
{
    dev->p_dev_status->a2dp_sub_status = STATUS_LINKED;

    //para = NEW(sizeof(struct AVRCP_ConnectReqStru));
    //memcpy(para->bd_addr, btengine_global_variable.g_cur_rmt_dev->bdAddress.bytes, BD_ADDR_LEN);
    //para->browsing_support = TRUE; /* remote device supports browsing channel or not */
    //BTSRC_AVRCP_ConnectReq(para);
    //��¼a2dp�����Ѿ�����
    dev->p_dev_status->service_connect |= A2DP_CONNECTED;
    //�����Ҫ������־
    dev->p_dev_status->need_auto_connect &= (uint8) A2DP_DISCONNECTED;
    dev->profile |= A2DP_CONNECTED;
}

//������ʾa2dp�����ӳɹ�
//1.����dev״̬Ϊ a2dp������
//2.����stream_cid
//3.���浱ǰ�豸��Ϣ
//4.���¿ɼ���
//2.step 4
void A2dpStreamOpenInd(struct A2DP_OpenedStru *in)
{
    bt_dev_t *dev;

    dev = A2dpFindDevByStreamHdl(in->stream_handle);
    if ((dev != NULL) && (dev->p_dev_status != NULL))
    {
        //�����Ϻ��ȡ���������Ľ�ɫ(master/slaver)
        bt_get_cur_role(dev);
        A2dpConnectedDevStatusUpdata(dev);
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (dev->dev_type == TWS_DEV)
        {
            A2DP_TWS_PRINTF("connect complete\n");
        }
        else
#endif
        {
            A2DP_PRINTF("connect complete\n");
        }

        //get_stream_cid
        dev->p_dev_status->a2dp_source.stream_cid = in->cid;
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (bt_manager_cfg_info.sp_tws_link == 1)
        {
            if (dev->dev_type == TWS_DEV)
            {
                TWS_A2dpConnectComplete(in, dev);
                //�ཨ��һ������������ʱ���Ƶ��
                bt_adjust_clk(BT_LINK_WORK_FREQ);
                senddata_cnt = 0;
                bt_linking_flag = FALSE;
                FREE(in);

                return;
            }
        }
#endif

        //save rmtdev info
        save_rmtdev_info();

        //updata btengine status
        btengine_global_variable.a2dp_linking_flag = FALSE;
        if (get_cur_device_status(dev) == STATUS_WAIT_PAIRED)
        {
            bt_linking_flag = FALSE;
            set_cur_device_status(dev, STATUS_LINKED);
        }

    }

    FREE(in);

    //updata visual mode
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

//�Ƹ�ʱ����������ʼ����
//1.����dev�豸Ϊplay״̬
//2.���Ƶ��
//3.����ǰ�����豸����Ϊ����
//2.step 5
void A2dpStreamStartInd(struct A2DP_StartSuspendIndStru *in)
{
    bt_dev_t *dev;

    dev = A2dpFindDevByStreamHdl(in->stream_handle);
#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if (bt_manager_cfg_info.sp_tws_link == 1)
    {
        if (dev != NULL)
        {
            if (dev->dev_type == TWS_DEV)
            {
                //��Ҫ�ȴ���tws�豸���ֻ������䣩����������ſ���tws�豸�������븱�䣩�������
                FREE(in);
                return;
            }
        }
    }
#endif
    if ((dev != NULL) && (dev->p_dev_status != NULL))
    {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (dev->dev_type == TWS_DEV)
        {
            A2DP_TWS_PRINTF("stream start\n");A2DP_TWS_DBG_PRINTF("dev cur status %d\n", (uint8) get_cur_device_status(dev));
        }
        else
#endif
        {
            A2DP_PRINTF("stream start\n");A2DP_DBG_PRINTF("dev cur status %d\n", (uint8) get_cur_device_status(dev));
        }
        //��ʼA2DP���������䣬��ʱ���Ƶ��
        bt_adjust_clk(BT_LINK_WORK_FREQ);
        dev->p_dev_status->a2dp_sub_status = STATUS_A2DP_PLAY;

        /* for BQB TC_SNK_SYN_BV_01_C.  maybe influence other test, so maybe need test individually.  */
        if (((BQB_TEST_ENABLE | BQB_DELAY_REPORT_ENABLE) & btengine_global_variable.bqb_test)
                == (BQB_TEST_ENABLE | BQB_DELAY_REPORT_ENABLE))
        {
            A2DP_PRINTF("delay report\n");
            BTSRC_A2DP_Delayreporting(in->stream_handle);
        }

        switch (get_cur_device_status(dev))
        {
            case STATUS_A2DP_PLAY:
            case STATUS_LINKED:
            case STATUS_A2DP_PAUSE:
            if (get_cur_device_status(dev) != STATUS_A2DP_PLAY)
            {
                set_cur_device_status(dev, STATUS_A2DP_PLAY);
            }
            btengine_global_variable.g_cur_rmt_dev = dev;
            set_exit_schedule_flag(TRUE);
            break;
            default:
            break;
        }
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (bt_manager_cfg_info.sp_tws_link == 1)
        {
            //��Ҫ�ȴ���tws�豸���ֻ������䣩����������ſ���tws�豸�������븱�䣩�������
            TWS_A2dpStreamStart(dev->dev_type);
            tws_status_info.sequence_number = 1;
        }
#endif
    }
    else
    {
        A2DP_PRINTF_WARNNING("stream_handle ERR 0x%x\n", in->stream_handle);
    }

    FREE(in);
}

//��ͣ
void pause_music(void)
{
    if (btengine_global_variable.g_sbc_thread_exist == 1)
    {
        btengine_global_variable.g_sbc_thread_exist = 0;
    }
}

//�Ƹ�ʱ������������
//1.����devΪpause״̬���˳�����
void A2dpStreamSuspendInd(struct A2DP_StartSuspendIndStru *in)
{
    bt_dev_t *dev;

    dev = A2dpFindDevByStreamHdl(in->stream_handle);
    if (dev != NULL)
    {
        if (dev->p_dev_status != NULL)
        {
            dev->p_dev_status->a2dp_sub_status = STATUS_A2DP_PAUSE;
        }

#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (dev->dev_type == TWS_DEV)
        {
            A2DP_TWS_PRINTF("stream suspend\n");
        }
        else
#endif
        {
            A2DP_PRINTF("stream suspend\n");
        }

        switch (get_cur_device_status(dev))
        {
            case STATUS_A2DP_PLAY:
            case STATUS_A2DP_PAUSE:
            set_cur_device_status(dev, STATUS_A2DP_PAUSE);
            set_exit_schedule_flag(FALSE);
            if (btengine_global_variable.g_cur_rmt_dev == dev)
            {
                pause_music();
            }
            start_cnt = 0;
            break;
            default:
            break;
        }
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (bt_manager_cfg_info.sp_tws_link == 1)
        {
            TWS_A2dpStreamSuspend(dev->dev_type);
        }
#endif
    }
    FREE(in);
}

//�Ͽ�������()
//1.����ǰ�Ͽ��豸�����Զ������б�
//2.��־a2dp��δ����
//3.�˳�SBC����
//4.����a2dp�ڵ�/handle����Ϣ
//5.�Ͽ�AVRCP
//6.�����Զ�����
void A2dpStreamDoneInd(struct A2DP_Stream_DoneStru *in)
{
    bt_dev_t *dev;

    dev = A2dpFindDevByStreamHdl(in->stream_handle);
    /* maybe gap disconnect is earlier than a2dp disconnect,
     so maybe dev is in AutoConnectList or p_dev_status is null. */
    if (dev == NULL)
    {
        dev = A2dpFindDevInAutoConnectList(in->stream_handle);
    }

    if (dev != NULL)
    {
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (dev->dev_type == TWS_DEV)
        {
            A2DP_TWS_PRINTF("snk disconnect by reason 0x%x\n", in->reason);
        }
        else
#endif
        {
            A2DP_PRINTF("snk disconnect by reason 0x%x\n", in->reason);
        }

#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if (bt_manager_cfg_info.sp_tws_link == 1)
        {
            //if(TWS_A2dpSnkDisconnect(dev)==TRUE)
            if (dev->dev_type == TWS_DEV)
            {
                tws_status_info.a2dp_stream_status.status = A2DP_STREAM_DONE;
                tws_status_info.a2dp_stream_status.stream_hdl = NULL;
                tws_status_info.is_src_flag = FALSE;
                if (in->reason != 0x0a)
                {
                    dev->p_dev_status->service_connect &= (uint8) A2DP_DISCONNECTED;
                }
                FREE(in);
                return;
            }
        }
#endif
        btengine_global_variable.a2dp_linking_flag = FALSE;
        /* AVDTP_DONE_L2CAP_CONNECT_COMPLETE_FAIL----0  */
        /* AVDTP_DONE_L2CAP_DISCONNECT----1  */
        /* AVDTP_DONE_TRASCATION_RTX----2  */
        /* AVDTP_DONE_RECV_CLOSE_IND----3  */
        /* AVDTP_DONE_RECV_ABORT_IND----4  */
        /* AVDTP_DONE_RECV_REJECT----5  */
        /* AVDTP_DONE_RECV_ACCEPT----6  */
        /* AVDTP_DONE_SEP_KILL_STREAM----7  */
        /* AVDTP_DONE_REMOTE_KILL_STREAM----8  */
        /* AVDTP_DONE_STREAM_UNREF_REMOTE----9  */
        /* AVDTP_DONE_SEPSELECT_FAIL----a  */
        //in->reason =0xff,��ʾscrean_list.head=NULL,��reason!=AVDTP_DONE_L2CAP_DISCONNECT
        //����reason!=AVDTP_DONE_RECV_CLOSE_IND
        //ipad ���Ƹ�����йػ���Ȼ�����¿�����in->reason=8,������Ҫ���״̬
        //�������ִ���stream_handle��ͻ���Ӷ�����malloc ʧ��
        if (btengine_global_variable.g_sbc_thread_exist == 1)
        {
            btengine_global_variable.g_sbc_thread_exist = 0;
        }
        if (dev->p_dev_status != NULL)
        {
            if (in->reason != 0x0a)
            {
                dev->p_dev_status->a2dp_source.stream_handle = NULL;
                dev->p_dev_status->a2dp_source.stream_cid = 0;
                dev->p_dev_status->a2dp_sub_status = STATUS_WAIT_PAIRED;
            }

            /* SONY LT 22i, �ֻ�ȡ����Ժ������ӻ�ʧ��,������.   ���䲻�����Ͽ�AVRCP���Ӽ���.   */
            if (((dev->p_dev_status->avrcp_source.connected_flag) == TRUE)
                    && ((dev->p_dev_status->service_connect & A2DP_CONNECTED) == 0))
            {
                BTSRC_AVRCP_DisconnectReq(dev->dev_info.bd_addr.bytes);
            }
            if (in->reason != 0x0a)
            {
                dev->p_dev_status->service_connect &= (uint8) A2DP_DISCONNECTED;
            }
        }
        btengine_global_variable.recv_quit_flag &= (uint8) A2DP_DISCONNECTED;
        btengine_global_variable.media_codec_type = A2DP_CODEC_NONA2DP;

        if ((in->reason == 0x01) || (in->reason == 0x07) || (in->reason == 0x08))
        {
            not_reconnect_a2dp_flag = TRUE;
        }
    }
    else
    {
        if (in->reason == 0x01)
        {
            if (btengine_global_variable.g_sbc_thread_exist == 1)
            {
                btengine_global_variable.g_sbc_thread_exist = 0;
            }
            btengine_global_variable.recv_quit_flag &= (uint8) A2DP_DISCONNECTED;
            btengine_global_variable.media_codec_type = A2DP_CODEC_NONA2DP;
        }
        else if (in->reason == 0x0a)
        {
            A2DP_PRINTF_WARNNING("connect conflict\n");
        }
        else
        {
        }
    }
    FREE(in);
}

//�ýӿڲ���ʹ�ã��书�ܷŵ� read_avdtp_sound_hook �ӿ���
void A2dpStreamDataInd(struct A2DP_Stream_DataIndStru *in)
{
    FREE(in);
}

void A2dpEventDbgPrint(UINT8 event, void *param)
{
    switch (event)
    {
        case A2DP_EV_DATAIND: /* param: struct A2DP_Stream_DataIndStru */
        {
            struct A2DP_Stream_DataIndStru *info = param;
            A2DP_DBG_PRINTF("A2DP_EV_DATAIND:0x%x\n",info->stream_handle);
        }
        break;
        case A2DP_EV_REGSEP: /* param: struct A2DP_SEIDStru */
        {
            struct A2DP_SEIDStru *info = param;
            A2DP_DBG_PRINTF("A2DP_EV_REGSEP:0x%x\n",info->seid);
        }
        break;
        case A2DP_EV_STREAMCREATE: /* param: struct A2DP_StreamCreateStru */
        {
            struct A2DP_StreamCreateStru *info = param;
            A2DP_DBG_PRINTF("A2DP_EV_STREAMCREATE:0x%x\n",info->stream_handle);
        }
        break;
        case A2DP_EV_SETCONFIGURATION: /* param: struct A2DP_SetConfigStru */
        {
            struct A2DP_SetConfigStru *info = param;
            A2DP_DBG_PRINTF("A2DP_EV_SETCONFIGURATION:0x%x\n",info->stream_handle);
        }
        break;
        case A2DP_EV_GETCONFIGURATION: /* param: struct A2DP_ServiceCapStru */
        {
            A2DP_DBG_PRINTF("A2DP_EV_GETCONFIGURATION\n");
        }
        break;
        case A2DP_EV_OPENCODECINFO: /* param: struct A2DP_OpenCodecInfoIndStru */
        {
            struct A2DP_OpenCodecInfoIndStru *info = param;
            A2DP_DBG_PRINTF("A2DP_EV_OPENCODECINFO:0x%x\n",info->stream_handle);
        }
        break;
        case A2DP_EV_STREAMOPEN: /* param: struct A2DP_OpenedStru */
        {
            struct A2DP_OpenedStru *info = param;
            A2DP_DBG_PRINTF("A2DP_EV_STREAMOPEN:0x%x\n",info->stream_handle);
        }
        break;
        case A2DP_EV_STREAMSTART:/* param: struct A2DP_StartSuspendIndStru */
        {
            struct A2DP_StartSuspendIndStru *info = param;
            A2DP_DBG_PRINTF("A2DP_EV_STREAMSTART:0x%x\n",info->stream_handle);
        }
        break;
        case A2DP_EV_STREAMSUSPEND:/* param: struct A2DP_StartSuspendIndStru */
        {
            struct A2DP_StartSuspendIndStru *info = param;
            A2DP_DBG_PRINTF("A2DP_EV_STREAMSUSPEND:0x%x\n",info->stream_handle);
        }
        break;
        case A2DP_EV_STREAMDONE: /* param: struct A2DP_Stream_DoneStru */
        {
            struct A2DP_Stream_DoneStru *info = param;
            A2DP_DBG_PRINTF("A2DP_EV_STREAMDONE:0x%x\n",info->stream_handle);
        }
        break;
        default:
        FREE(param);
        break;
    }
}

void A2dpIndCbk(UINT8 event, void *param)
{
    A2dpEventDbgPrint(event, param);

    switch (event)
    {
        case A2DP_EV_DATAIND: /* param: struct A2DP_Stream_DataIndStru */
        A2dpStreamDataInd(param);
        break;
        case A2DP_EV_REGSEP: /* param: struct A2DP_SEIDStru */
        A2dpRegSepInd(param);
        break;
        case A2DP_EV_STREAMCREATE: /* param: struct A2DP_StreamCreateStru */
        A2dpStreamCreateInd(param);
        break;
        case A2DP_EV_SETCONFIGURATION: /* param: struct A2DP_SetConfigStru */
        A2dpSetConfigInd(param);
        break;
        case A2DP_EV_GETCONFIGURATION: /* param: struct A2DP_ServiceCapStru */
        A2dpGetConfigInd(param);
        break;
        case A2DP_EV_OPENCODECINFO: /* param: struct A2DP_OpenCodecInfoIndStru */
        A2dpOpenCodecInfoInd(param);
        break;
        case A2DP_EV_STREAMOPEN: /* param: struct A2DP_ReadyToStartStru */
        A2dpStreamOpenInd(param);

#if  (SUPPORT_A2DP_UNIT_TEST == 1)
        if (a2dp_unit_test_get_status() == STATUS_THREAD_NO_INIT)
        {
            a2dp_unit_test_init();
            //��Ԫ����ģʽ�£����δ���ý��룬��ô��Ҫ��avdtp�����ݹ��˿��ش�
            btengine_global_variable.filter_avdtp_sound_flag = 0;
        }
#endif
        break;
        case A2DP_EV_STREAMSTART:
        A2dpStreamStartInd(param); /* param: struct A2DP_StartSuspendIndStru */
        break;
        case A2DP_EV_STREAMSUSPEND:
        A2dpStreamSuspendInd(param); /* param: struct A2DP_StartSuspendIndStru */
        break;
        case A2DP_EV_STREAMDONE: /* param: struct A2DP_Stream_DoneStru */
        A2dpStreamDoneInd(param);

#if  (SUPPORT_A2DP_UNIT_TEST == 1)
        if (a2dp_unit_test_get_status() == STATUS_THREAD_READY)
        {
            a2dp_unit_test_deinit();
            //��Ԫ����ģʽ�£����δ���ý��룬��ô��Ҫ��avdtp�����ݹ��˿��عر�
            btengine_global_variable.filter_avdtp_sound_flag = 1;
        }
#endif
        break;
        default:
        FREE(param);
        break;
    }
}

void A2dpAddPrefer(void *prefer, uint8 *channel_mode, uint8 channel_mode_number)
{
    struct A2DP_ServiceCapStru *sc;
    UINT8 *p, i;
    UINT8 len;

    if (bt_manager_cfg_info.sp_avdtp_aac == 1)
    {
        //add aac codec
        len = 12;
        sc = BTSRC_List_NodeNew(sizeof(struct A2DP_ServiceCapStru) + len);
        sc->len = len;
        p = sc->service_capability;
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
        BTSRC_List_AddTail(prefer, sc);

        /* support Content Protection.  */
        if (bt_manager_cfg_info.sp_a2dp_cp == 2)
        {
            len = 16;
            sc = BTSRC_List_NodeNew(sizeof(struct A2DP_ServiceCapStru) + len);
            sc->len = len;
            p = sc->service_capability;
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
            p[12] = A2DP_SC_CONTENT_PROTECTION;
            p[13] = 0x02;
            p[14] = (uint8) (BTA_AV_CP_SCMS_T_ID);
            p[15] = (uint8) (BTA_AV_CP_SCMS_T_ID >> 8);
            BTSRC_List_AddTail(prefer, sc);
        }

    }

    i = 0;
    len = 10;
    for (i = 0; i < channel_mode_number; i++)
    {
        sc = BTSRC_List_NodeNew(sizeof(struct A2DP_ServiceCapStru) + len);
        sc->len = len;
        p = sc->service_capability;
        p[0] = A2DP_SC_MEDIA_TRANSPORT;
        p[1] = 0;
        p[2] = A2DP_SC_MEDIA_CODEC;
        p[3] = 6;
        p[4] = A2DP_MEDIATYPE_AUDIO << 4;
        p[5] = A2DP_CODEC_SBC;
        p[6] = *(channel_mode + i);
        p[7] = A2DP_SBC_BLOCK_16 | A2DP_SBC_SUBBAND_8 | A2DP_SBC_LOUDNESS;
        p[8] = 2;
        p[9] = btengine_global_variable.sbc_max_bitpool;
        BTSRC_List_AddTail(prefer, sc);
    }

    /* support Content Protection.  */
    if (bt_manager_cfg_info.sp_a2dp_cp == 2)
    {
        i = 0;
        len = 14;
        for (i = 0; i < channel_mode_number; i++)
        {
            sc = BTSRC_List_NodeNew(sizeof(struct A2DP_ServiceCapStru) + len);
            sc->len = len;
            p = sc->service_capability;
            p[0] = A2DP_SC_MEDIA_TRANSPORT;
            p[1] = 0;
            p[2] = A2DP_SC_MEDIA_CODEC;
            p[3] = 6;
            p[4] = A2DP_MEDIATYPE_AUDIO << 4;
            p[5] = A2DP_CODEC_SBC;
            p[6] = *(channel_mode + i);
            p[7] = A2DP_SBC_BLOCK_16 | A2DP_SBC_SUBBAND_8 | A2DP_SBC_LOUDNESS;
            p[8] = 2;
            p[9] = btengine_global_variable.sbc_max_bitpool;
            p[10] = A2DP_SC_CONTENT_PROTECTION;
            p[11] = 0x02;
            p[12] = (uint8) (BTA_AV_CP_SCMS_T_ID);
            p[13] = (uint8) (BTA_AV_CP_SCMS_T_ID >> 8);
            BTSRC_List_AddTail(prefer, sc);
        }
    }
}

bool A2dpConnect(uint8 type, uint8* bt_addr)
{
    bool ret = FALSE;
    struct A2DP_ConnectReqStru *in;

    A2DP_PRINTF("connect start\n");

    if (bt_addr == NULL)
    {
        return ret;
    }

    /* conn_obj->app_param is ignored. */
    in = NEW(sizeof(struct A2DP_ConnectReqStru));
    libc_memset(in, 0, sizeof(struct A2DP_ConnectReqStru));
    libc_memcpy(in->bd, bt_addr, BD_ADDR_LEN);
    in->src = type;
    A2dpAddPrefer(&in->perfer, a2dp_sc_prefer_channelmode, A2DP_SC_PREFER_CHANNELMODE_NUMBER);

    BTSRC_A2DP_Connect(in);
    return TRUE;
}

void A2dpDisconnect(bt_dev_t *dev)
{
    A2DP_PRINTF("disconnect\n");

    if (dev != NULL)
    {
        if ((dev->p_dev_status != NULL) && (dev->p_dev_status->a2dp_source.stream_handle != NULL))
        {
            BTSRC_A2DP_Disconnect(dev->p_dev_status->a2dp_source.stream_handle);
        }
    }
}

