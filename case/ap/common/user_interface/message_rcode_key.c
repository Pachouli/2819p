/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������������Ϣ���սӿڣ�֧����ʱ���尴����Ϣ�����յ�������Ϣ��ص����Ӻ�����
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

key_deal_infor_t g_key_deal_infor;

/*!
 *  \brief
 *  g_buffer_gui_msg ����gui��Ϣ
 */
static input_gui_msg_t g_buffer_gui_msg;

extern bool com_gui_msg_hook_slave(input_gui_msg_t *input_msg);
extern bool com_gui_msg_hook(input_gui_msg_t *input_msg);

static void decompress_gui_msg(input_gui_msg_t *input_msg, uint32 key_data);

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    ���� gui ��Ϣ������������Ϣ��
 * \param[in]    none
 * \param[out]   input_msg�����ؽ��յ��� gui ��Ϣ
 * \return       bool
 * \retval           TRUE ���� gui ��Ϣ�ɹ���
 * \retval           FALSE ���� gui ��Ϣʧ�ܡ�
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
 ����1��gui ��Ϣ���¼���ѭ��
 input_gui_msg_t input_msg;
 msg_apps_type_e gui_event;
 bool ret;

 ret = get_gui_msg_inner(&input_msg);
 if(ret == TRUE)
 {
 if(com_key_mapping_inner(&input_msg, &gui_event, key_map_list) == TRUE)
 {
 switch(gui_event)
 {
 case EVENT_DO_SOMETHING:
 break;

 default:
 result = com_message_box_inner(gui_event);
 break;
 }
 }
 }
 else
 {
 gui ��Ϣ���¼���������ϣ����������Դ���Ӧ��˽����Ϣ��
 }
 * \endcode
 * \note
 *******************************************************************************/

static void __dc_state_reset(void)
{
    g_key_deal_infor.dc_fail_sta = DC_FAIL_STATUS_NULL;
    g_key_deal_infor.dc_fail_reason = DC_FAIL_NO_FAIL;
    g_key_deal_infor.double_click_fail_flag = FALSE;
    g_key_deal_infor.dc_sta = DC_KEY_STATUS_NULL;
}

uint16 g_vr_adc_value_last[2];

bool get_gui_msg_inner(input_gui_msg_t *input_msg)
{
    uint32 key_data;
    bool need_send_key = FALSE;
    bool ret_val = TRUE;

#if (SUPPORT_OK_MIC == 1)

    {
        static uint8 ok_mic_detect_time = 0; /*! OK MIC ������������ÿ�δ��10-20ms */
        bool mic_detect = TRUE; //TODO ����IO�ڼ��MIC�Ƿ����Ĵ���
        if (get_ok_mic_state() == FALSE)
        {
            //IO�ڼ�⵽MIC����
            if (mic_detect == TRUE)
            {
                ok_mic_detect_time++;
                if (ok_mic_detect_time >= 10)
                {
                    change_ok_mic_state(TRUE);
                    com_set_dae_ok_mic_mute(FALSE);
                }
            }
            else
            {
                ok_mic_detect_time = 0;
            }
        }
        else
        {
            //IO�ڼ�⵽MIC�γ�
            if (mic_detect == FALSE)
            {
                ok_mic_detect_time++;
                if (ok_mic_detect_time >= 3)
                {
                    change_ok_mic_state(FALSE);
                    com_set_dae_ok_mic_mute(TRUE);
                }
            }
            else
            {
                ok_mic_detect_time = 0;
            }
        }
    }

#if (SUPPORT_VR_ADCKEY == 1)
    {
        uint16 cur_vr_adc_value[2];
        uint16 temp_value, max_value, min_value;
        key_get_vr_adc_value(cur_vr_adc_value);
        if (cur_vr_adc_value[0] != g_vr_adc_value_last[0])
        {
            //echo decay
            uint8 echo_decay_index;
            echo_reverb_para_t tmp_echo_para;
            com_get_config_struct_inner(OK_MIC_ECHO_RATIO_ARRAY, (uint8 *) &tmp_echo_para, sizeof(echo_reverb_para_t));

            max_value = 0x3c0;
            min_value = 0;
            temp_value = (cur_vr_adc_value[0] > max_value) ? max_value : cur_vr_adc_value[0];
            echo_decay_index = (temp_value - min_value) * (tmp_echo_para.total - 1) / (max_value - min_value);

            if (echo_decay_index != g_p_com_dae_cfg->echo_ratio_index)
            {
                com_set_dae_reverb_depth_set(echo_decay_index);
            }
        }

        if (cur_vr_adc_value[1] != g_vr_adc_value_last[1])
        {
            //mic volume
            uint8 mic_volume;

            max_value = 0x3c0;
            min_value = 0;
            temp_value = (cur_vr_adc_value[1] > max_value) ? max_value : cur_vr_adc_value[1];
            mic_volume = (temp_value - min_value) * VOLUME_VALUE_MAX / (max_value - min_value);

            if (mic_volume != g_volume_infor.mic_volume_current)
            {
                com_set_mic_volume(mic_volume);
            }
        }

        libc_memcpy(g_vr_adc_value_last, cur_vr_adc_value, sizeof(g_vr_adc_value_last));
    }
#endif
#endif

    //��������
    keytone_play_deal();

    //��ȡ����ӡ����ֵ
    if (0)
    {
        sound_energy_t tmp_energy;
        com_get_sound_energy_inner(&tmp_energy);
        libc_printf("ENERGY:%d,%d\n", tmp_energy.play_cur_energy, tmp_energy.play_cur_energy_rms);
    }

    //˫����������Ƿ�ʱʧ��
    if ((g_key_deal_infor.double_click_enable == TRUE) && (g_key_deal_infor.double_click_fail_flag == FALSE))
    {
        switch(g_key_deal_infor.dc_sta)
        {
        case DC_KEY_STATUS_FIRST_DOWN:
            if ((sys_get_ab_timer() - g_key_deal_infor.dc_last_time) > DOUBLE_CLICK_TIME_SPACE_MAX)
            {
                g_key_deal_infor.dc_fail_sta = DC_FAIL_STATUS_NULL;
                g_key_deal_infor.dc_fail_reason = DC_FAIL_FIRSE_OVERTIME;
                g_key_deal_infor.double_click_fail_flag = TRUE;
            }
            break;

        case DC_KEY_STATUS_FIRST_UP:
            if ((sys_get_ab_timer() - g_key_deal_infor.dc_last_time) > DOUBLE_CLICK_TIME_SPACE_MAX)
            {
                g_key_deal_infor.dc_fail_sta = DC_FAIL_STATUS_NULL;
                g_key_deal_infor.dc_fail_reason = DC_FAIL_SECOND_NOT_DOWN;
                g_key_deal_infor.double_click_fail_flag = TRUE;
            }
            break;

        case DC_KEY_STATUS_SECOND_DOWN:
            if ((sys_get_ab_timer() - g_key_deal_infor.dc_last_time) > DOUBLE_CLICK_TIME_SPACE_MAX)
            {
                g_key_deal_infor.dc_fail_sta = DC_FAIL_STATUS_NULL;
                g_key_deal_infor.dc_fail_reason = DC_FAIL_SECOND_OVERTIME;
                g_key_deal_infor.double_click_fail_flag = TRUE;
            }
            break;

        default:
            break;
        }
    }

    //���˫������ȷ��ʧ�ܣ���Ҫ��������
    if ((g_key_deal_infor.double_click_enable == TRUE) && (g_key_deal_infor.double_click_fail_flag == TRUE))
    {
        need_send_key = TRUE;

        switch(g_key_deal_infor.dc_fail_sta)
        {
        case DC_FAIL_STATUS_NULL:
            input_msg->data.kmsg.val = g_key_deal_infor.dc_key_cur;
            input_msg->data.kmsg.type = KEY_TYPE_DOWN;
            g_key_deal_infor.dc_fail_sta = DC_FAIL_STATUS_SEND_FIRST_DOWN;
            break;

        case DC_FAIL_STATUS_SEND_FIRST_DOWN:
            if (g_key_deal_infor.dc_fail_reason == DC_FAIL_SECOND_NOT_DOWN)
            {
                input_msg->data.kmsg.val = g_key_deal_infor.dc_key_cur;
                input_msg->data.kmsg.type = KEY_TYPE_SHORT_UP;
                __dc_state_reset();
            }
            else if (g_key_deal_infor.dc_fail_reason == DC_FAIL_FIRSE_OVERTIME)
            {
                __dc_state_reset();
                need_send_key = FALSE;
            }
            else if (g_key_deal_infor.dc_fail_reason == DC_FAIL_FIRST_LONG)
            {
                input_msg->data.kmsg.val = g_key_deal_infor.dc_key_cur;
                input_msg->data.kmsg.type = KEY_TYPE_LONG;
                __dc_state_reset();
            }
            else
            {
                input_msg->data.kmsg.val = g_key_deal_infor.dc_key_cur;
                input_msg->data.kmsg.type = KEY_TYPE_SHORT_UP;
                g_key_deal_infor.dc_fail_sta = DC_FAIL_STATUS_SEND_FIRST_UP;
            }
            break;

        case DC_FAIL_STATUS_SEND_FIRST_UP:
            if (g_key_deal_infor.dc_fail_reason == DC_FAIL_SECOND_OTHER_KEY)
            {
                input_msg->data.kmsg.val = g_key_deal_infor.other_key;
                input_msg->data.kmsg.type = KEY_TYPE_DOWN;
                __dc_state_reset();
            }
            else
            {
                input_msg->data.kmsg.val = g_key_deal_infor.dc_key_cur;
                input_msg->data.kmsg.type = KEY_TYPE_DOWN;
                g_key_deal_infor.dc_fail_sta = DC_FAIL_STATUS_SEND_SECOND_DOWN;
            }
            break;

        case DC_FAIL_STATUS_SEND_SECOND_DOWN:
            if (g_key_deal_infor.dc_fail_reason == DC_FAIL_SECOND_OVERTIME)
            {
                __dc_state_reset();
                need_send_key = FALSE;
            }
            else
            {
                input_msg->data.kmsg.val = g_key_deal_infor.dc_key_cur;
                input_msg->data.kmsg.type = KEY_TYPE_LONG;
                __dc_state_reset();
            }
            break;

        default:
            need_send_key = FALSE;
            break;
        }

        //˫������ʧ����Ҫ����������Ϣ
        if (need_send_key == TRUE)
        {
            return TRUE;
        }
    }

    if (g_buffer_gui_msg.data.kmsg.val != KEY_NULL)
    {
        //������Ϣ��ֱ�ӿ���
        libc_memcpy(input_msg, &g_buffer_gui_msg, sizeof(input_gui_msg_t));
        g_buffer_gui_msg.data.kmsg.val = KEY_NULL;

        return TRUE;
    }

    if (sys_mq_receive(MQ_ID_GUI, (void *) &key_data) == 0)
    {
        decompress_gui_msg(input_msg, key_data);

        //�յ�gui��Ϣ����ʾ���û�����������һЩϵͳ�������磺
        //0.������������˵���������
        //1.����رռ�ʱ����Ļ������ʱ��ʡ��ػ���ʱ���������ڲ��ż�ʱ ��0
        //2.����������
        //3.����������
#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if ((get_tws_mode_inner() != TWS_MODE_SINGLE) && (get_tws_role_inner() == TWS_SLAVE))
        {
            //���������õ�ǰ����
            com_gui_msg_hook_slave(input_msg);

            if (input_msg->data.kmsg.type == KEY_TYPE_DOWN)
            {
                //û��Ƶ������Ǿ���״̬���Ұ�����ʹ�ܣ��ŷ���������
                com_start_key_tone_inner();
            }

            g_tws_slave_ui_info.key_data = key_data;
            if (com_tws_s2m_update_key(g_tws_slave_ui_info.key_data) == FALSE)
            {
                g_tws_slave_ui_info.has_key = TRUE;
            }
            ret_val = FALSE;
        }
        else
#endif
        {
            ret_val = com_gui_msg_hook(input_msg);
        }
    }
    else
    {
        ret_val = FALSE;

#ifdef ENABLE_TRUE_WIRELESS_STEREO
        if ((get_tws_mode_inner() != TWS_MODE_SINGLE) && (get_tws_role_inner() == TWS_MASTER))
        {
            if (sys_mq_receive(MQ_ID_GUI_TEST, (void *) &key_data) == 0)
            {
                decompress_gui_msg(input_msg, key_data);

                input_msg->type = INPUT_MSG_KEY_SLAVE;

                ret_val = com_gui_msg_hook(input_msg);
            }
        }
#endif
    }

    return ret_val;
}

static void decompress_gui_msg(input_gui_msg_t *input_msg, uint32 key_data)
{
    //��Ϣ��ѹ��
    input_msg->type = (input_msg_type_e) (uint8) key_data; //byte 0
    if (input_msg->type == INPUT_MSG_KEY)
    {
        input_msg->data.kmsg.val = (uint8)(key_data >> 8); //byte 1
        input_msg->data.kmsg.type = (key_type_e)(key_data >> 16); //byte 2-3
    }
}

void message_redeal_gui_msg_inner(input_gui_msg_t *p_gui_msg)
{
    if (p_gui_msg == NULL)
    {
        libc_memset(&g_buffer_gui_msg, 0x00, sizeof(input_gui_msg_t));
    }
    else
    {
        libc_memcpy(&g_buffer_gui_msg, p_gui_msg, sizeof(input_gui_msg_t));
    }
}

void message_key_flush_inner(void)
{
    //��� gui ��Ϣ����
    sys_mq_flush (MQ_ID_GUI);
    message_redeal_gui_msg_inner(NULL);//��NULL��0

    //���˫������״̬���ͱ�־
    __dc_state_reset();
}

void com_double_click_enable_inner(bool enable, key_value_e dc_key_1, key_value_e dc_key_2)
{
    g_key_deal_infor.double_click_enable = enable;
    g_key_deal_infor.dc_key_1 = dc_key_1;
    g_key_deal_infor.dc_key_2 = dc_key_2;
    if ((dc_key_1 == KEY_NULL) && (dc_key_2 == KEY_NULL))
    {
        g_key_deal_infor.double_click_enable = FALSE;
    }
    g_key_deal_infor.dc_fail_sta = DC_FAIL_STATUS_NULL;
    g_key_deal_infor.dc_fail_reason = DC_FAIL_NO_FAIL;
    g_key_deal_infor.double_click_fail_flag = FALSE;
    g_key_deal_infor.dc_sta = DC_KEY_STATUS_NULL;
}

/*! \endcond */
