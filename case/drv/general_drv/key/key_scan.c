/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������KEY��������ɨ��ӿڣ�������Ϣ���ͽӿڣ�֧��LRADC KEY��ONOFF KEY��UART SIM KEY��
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "key_inner.h"

#define KEY_MSG_DEPTH     8//gui ��Ϣ�������
#define DOWN_KEY_TIMER    2//����������Ϣʱ��Ϊ40ms
#define UP_KEY_TIMER      2//����̧����Ϣʱ��Ϊ40ms
#define LONG_KEY_TIMER    5//����������Ϣʱ��Ϊ40ms + (5-1)*HOLD_KEY_TIMER = 680ms
#define IR_LONG_KEY_TIMER 7//���ⰴ��������Ϣʱ��Ϊ60ms + (7-1)*110ms = 720ms��IR����hold��Ϣ���Ϊ110ms
#define HOLD_KEY_TIMER    8//��������hold��Ϣ���Ϊ160ms

#define NO_KEY            KEY_NULL

uint8 newkey = NO_KEY;
uint8 oldkey = NO_KEY;
uint8 tmpkey = NO_KEY;
uint8 tmp_count, hold_count;
uint8 wait_keyup = 0;
uint8 key_count = 0; //��ǰ������Ϣ�����Ĵ���
uint8 key_value = KEY_NULL; //��ǰ������ֵ

uint32 g_last_key_down_timer;

keymap_para_t g_keymap_para;

key_active_phy_type_e g_key_active_phy_type;

uint8 adckey_switch_lock; //�л���ȥ����ʱ��������Ҫ�ȴ�onoff�������²Ž�������
                          //0��ʾδ������1��ʾ��������2��ʾ���ڽ����ȴ�onoff̧��

int8 adckey_idx; //ADCKEY LRADC INDEX

#if (SUPPORT_FAST_PREVIEW == 1)
bool g_fast_preview_key_shot;
fast_preview_cbk g_fast_preview_cbk;
#endif

void __section__(".rcode") deal_no_key(void)
{
    hold_count = 0;
    oldkey = NO_KEY;
    tmpkey = NO_KEY;
    tmp_count = 0; //��ɨ�����
    PutSysMsg (TRUE, 0, KEY_PHY_TYPE_KEY); //��keyup��Ϣ
}

/******************************************************************************/
/*!
 * \par  Description:
 *    key scan of times
 * \param[in]    none
 * \param[in]    none
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \retval           0
 * \retval           1
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 * ����ɨ���Լ������ѯ��ÿ��20msɨ��һ�Σ������Ѿ�����ȥ������
 *******************************************************************************/
void __section__(".rcode") key_inner_key_scan(void)
{
    uint16 adcdat;
    uint8 onoff_hit;
    uint8 key_val, i;

#if (SUPPORT_VR_ADCKEY == 1)
    key_inner_vr_adc_sample();
    key_inner_vr_adc_mux_switch();
#endif

#if (SUPPORT_EXT_RTC == 1)
    //RTC INT ����·ת�������ߵ�ƽ��Ч
    if ((sRTC_get_rtc_int_level() == 1)&& (sRTC_get_alarm_flag() == TRUE))
    {
        libc_printf_info("EXT RTC Alarm!\n");
        if (g_ext_rtc_alram_isr != NULL)
        {
            g_ext_rtc_alram_isr();
        }
    }
#endif

#if (SUPPORT_FAST_PREVIEW == 1)
    g_fast_preview_key_shot = FALSE;
#endif

    key_val = KEY_NULL;

    onoff_hit = hal_pmu_read_onoff_key();

    if (onoff_hit == TRUE)
    {
        if (adckey_switch_lock == 1)
        {
            adckey_switch_lock = 2;
        }
        if (adckey_switch_lock == 0)
        {
            key_val = g_keymap_para.key_onoff;
        }
    }
    else
    {
        if (adckey_switch_lock == 2)
        {
            libc_printf_info("unlock!\n");
            adckey_switch_lock = 0;
        }

        //LRADC1/2 for adc key
        if (adckey_switch_lock == 0)
        {
            adcdat = hal_pmu_read_lradc(adckey_idx);
        }
        else
        {
            adcdat = LRADC_2_90_V;
        }

#if 0
        if (adcdat < LRADC_2_90_V)
        {
            libc_printf("adc:%d\n", adcdat);
        }
#endif

        for (i = 0; i < g_keymap_para.key_num; i++)
        {
            if (adcdat < g_keymap_para.key_adc_val[i])
            {
                key_val = g_keymap_para.key_val[i];
                break;
            }
        }

#if (SUPPORT_ROTARY_ENCODER == 1)
        //��ǰû����ͨ���������£����鿴�Ƿ���ת�����ж���
        if ((key_val == KEY_NULL) && (g_rotary_encode_info.EnStatus != EnKEY_STOP))//������ͨ����
        {
            if (g_rotary_encode_info.EnStatus == EnKEY_FORWARD)
            {
                key_val = KEY_ROTARY_ADD;
            }
            else
            {
                key_val = KEY_ROTARY_SUB;
            }

            PutSysMsg(SPECIAL_KEY_SHOUT_UP, key_val, KEY_PHY_TYPE_KEY);
            g_rotary_encode_info.EnStatus = EnKEY_STOP;
            return;
        }
#endif
    }

#if (SUPPORT_IR_KEY == 1)
    key_ir_check_status();
#endif

    //UARTģ�ⰴ��
#if (SUPPORT_UART_SIM_KEY == 1)
    if ((g_keymap_para.uart_sim_key_support == TRUE) && (key_val == KEY_NULL))
    {
        key_val = key_get_uart_sim_key();
    }
#endif

    if (key_val != oldkey) //�������ɨ��ֵ�������ϴΰ���ֵ
    {
        //���水��Ҳ��Ҫ��� hold_count
        hold_count = 0;

        if (key_val == tmpkey) //������μ�ֵ����ǰ��ɨ��ֵ
        {
            tmp_count++; //�ۼ�ɨ�����

            if (key_val != NO_KEY)
            {
                //ɨ�����Σ���ͬһ��ֵ��ȷ���а�������
                if ((tmp_count >= DOWN_KEY_TIMER)
                        //��Ϊscan�жϱ������ж����������¼�ʱ���ز�׼ȷ������Ѿ���ʱ�ͷ��Ͱ���DOWN��Ϣ
                        || ((sys_get_ab_timer() - g_last_key_down_timer) > (20 * DOWN_KEY_TIMER)))
                {
                    //PLAY����Ҫ����ShortUp��Ϣ
                    if (oldkey == KEY_PLAY)
                    {
                        PutSysMsg(TRUE, 0, KEY_PHY_TYPE_KEY);
                    }
                    oldkey = tmpkey;
                    PutSysMsg(FALSE, oldkey, KEY_PHY_TYPE_KEY);
                }
            }
            else
            {
                if (tmp_count >= UP_KEY_TIMER) //����̧���ж�
                {
                    deal_no_key();
                }
            }
        }
        else //�������ɨ��ֵ������ǰ��ɨ��ֵ�������ü�ֵ�����¼���
        {
            g_last_key_down_timer = sys_get_ab_timer();
            tmpkey = key_val;
            tmp_count = 0;
        }
    }
    else //������μ�ֵ�����ϴΰ���ֵ,�ۼ�8��(160ms)����һ�ΰ�����Ϣ
    {
        if (key_val != NO_KEY)
        {
            hold_count++;

            if (hold_count == HOLD_KEY_TIMER)
            {
                hold_count = 0;
                PutSysMsg(FALSE, oldkey, KEY_PHY_TYPE_KEY); //����ֵ��Ϣ
            }
        }
        else
        {
            tmpkey = key_val;
            hold_count = 0;
            tmp_count = 0;
        }
    }

#if (SUPPORT_FAST_PREVIEW == 1)
    if (g_fast_preview_key_shot == FALSE)
    {
        //����Ԥ��ģʽû�а����̰�̧��ʱιtime tick
        if (g_fast_preview_cbk != NULL)
        {
            g_fast_preview_cbk(NULL); //NULL - time tick
        }
    }
#endif
}

/******************************************************************************/
/*!
 * \par  Description:
 *    Post key package message to OS
 * \param[in]    *input_msg
 * \param[in]    none
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \retval           0
 * \retval           1
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 * ��װ������Ϣ�������͸���Ϣ����
 *******************************************************************************/
bool __section__(".rcode") post_key_msg(input_gui_msg_t *input_msg)
{
    uint32 key_data;

    //��Ϣѹ��
    key_data = (uint32) input_msg->type; //byte 0
    if (input_msg->type == INPUT_MSG_KEY)
    {
        key_data |= ((uint32) input_msg->data.kmsg.val << 8); //byte 1
        key_data |= ((uint32) input_msg->data.kmsg.type << 16); //byte 2-3
    }

    if ((int) sys_mq_send(MQ_ID_GUI, (void *) &key_data) == 0)
    {
        //libc_printf("post key msg:0x%x\n", key_data);
        return TRUE;
    }
    else
    {
        libc_printf("key drop:0x%x\n", key_data);
    }

    //��Ϣ��������������false
    return FALSE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    Key message deal
 * \param[in]    key
 * \param[in]    none
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      key drvier
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 * �����̰���������������Ӧ������Ϣ
 *******************************************************************************/
//�����ﴦ���е���������ң�صĳ�ͻ��ϵ
//short_up 0-DOWN,1-SHORT_UP,2-SPECIAL_KEY_SHOUT_UP
void __section__(".rcode") PutSysMsg(uint8 short_up, uint8 key, key_phy_type_e phy_type)
{
    input_gui_msg_t input_msg;
    input_msg.type = INPUT_MSG_KEY;
    uint8 long_key_timer, key_count_bak;

    if (g_key_active_phy_type == KEY_ACTIVE_PHY_TYPE_NULL)
    {
        if (phy_type == KEY_PHY_TYPE_KEY)
        {
            g_key_active_phy_type = KEY_ACTIVE_PHY_TYEP_KEY;
        }
        else
        {
            g_key_active_phy_type = KEY_ACTIVE_PHY_TYEP_IR;
        }
    }
    else if (g_key_active_phy_type == KEY_ACTIVE_PHY_TYEP_KEY)
    {//��е�������ں��Ժ��ⰴ��
        if (phy_type != KEY_PHY_TYPE_KEY)
        {
            //libc_printf("ignore ir key %d\n", key);
            return;
        }
    }
    else
    {//���ⰴ�����ں��Ի�е����
        if (phy_type != KEY_PHY_TYPE_IR)
        {
            //libc_printf("ignore phy key %d\n", key);
            return;
        }
    }

    key_count_bak = key_count;
    if (phy_type == KEY_PHY_TYPE_KEY)
    {
        long_key_timer = LONG_KEY_TIMER;
    }
    else
    {
        long_key_timer = IR_LONG_KEY_TIMER;
    }

    if (short_up == TRUE)
    {
        key_count = 0;
        /*key_value������ı�*/
        input_msg.data.kmsg.val = key_value;
        input_msg.data.kmsg.type = KEY_TYPE_SHORT_UP;
        key_value = KEY_NULL;
    }
    else if (short_up == SPECIAL_KEY_SHOUT_UP)
    {
        key_count = 0;
        /*key_value������ı�*/
        input_msg.data.kmsg.val = key;
        input_msg.data.kmsg.type = KEY_TYPE_SHORT_UP;
        key_value = KEY_NULL;
    }
    else
    {
        if (key == key_value)
        {
            key_count++;

            /*��ס����ʱ��160ms����һ����ͬ������Ϣ*/
            if (key_count == long_key_timer)
            {
                input_msg.data.kmsg.val = key_value;
                input_msg.data.kmsg.type = KEY_TYPE_LONG;
            }
            else if (key_count > long_key_timer)
            {
                input_msg.data.kmsg.val = key_value;
                input_msg.data.kmsg.type = KEY_TYPE_HOLD;
            }
            else
            {
                return;
            }
        }
        else
        {
            /*�°�������*/
            key_value = key;
            input_msg.data.kmsg.val = key;
            input_msg.data.kmsg.type = KEY_TYPE_DOWN;
            key_count = 1;
        }
    }

    //����Ԥ��ģʽ�����̰��ص���������Ҫ�����Ӿ��л�Ӧ�ô���
#if (SUPPORT_FAST_PREVIEW == 1)
    if (g_fast_preview_cbk != NULL)
    {
        g_fast_preview_cbk(&input_msg);
        if (input_msg.data.kmsg.type == KEY_TYPE_NULL)
        {
            //��������
            goto fast_preview_key_drop;
        }
    }
#endif

    post_key_msg_retry:
    if (post_key_msg(&input_msg) == FALSE)
    {
        if (short_up == TRUE)
        {
            //�������HOLD��Ϣ������ȡ��һ���ٷ� SHORT_UP ��������֤ SHORT_UP ��Ϣ�ܹ����ͳɹ�
            if (key_count_bak > long_key_timer)
            {
                //�Ƚ���gui��Ϣ����֤gui��Ϣ���з���
                uint32 key_data;
                if (sys_mq_receive(MQ_ID_GUI, (void *) &key_data) == 0)
                {
                    libc_printf("key recv&drop:0x%x\n", key_data);
                }
                goto post_key_msg_retry;
            }
        }
    }

    fast_preview_key_drop:

    if (short_up == TRUE)
    {
        g_key_active_phy_type = KEY_ACTIVE_PHY_TYPE_NULL;
    }

    return;
}

void key_inner_scan_init(keymap_para_t *p_keymap_para)
{
    libc_memcpy(&g_keymap_para, p_keymap_para, sizeof(keymap_para_t));

#if (SUPPORT_UART_SIM_KEY == 1)
    if (g_keymap_para.uart_sim_key_support == TRUE)
    {
        //��ʼ��UART RX
        key_init_uart_sim_key();
    }
#endif

    //LRADC1/2 for adc key
    adckey_idx = set_pad_function(g_keymap_para.lradc_pin, PAD_FUNC_ANOLOG, NULL, 0);
    if (adckey_idx >= 0)
    {
        pmu_enable_lradc(adckey_idx, TRUE);
        if (adckey_idx == 1)
        {
            pmu_enable_lradc1_resistance(g_keymap_para.lradc_res_enable);
        }
    }

#if (SUPPORT_ROTARY_ENCODER == 1)
    key_inner_rotary_encoder_init();
#endif

#if (SUPPORT_VR_ADCKEY == 1)
    key_inner_vr_adc_init();
#endif
}

void key_inner_adckey_switch(void)
{
    if (g_keymap_para.lradc_pin == PAD_KEY_LRADC1)
    {
        libc_printf_info("wait press onoff key to unlock!\n");
        adckey_switch_lock = 1;
        //LRADC1 disable
        pmu_enable_lradc(adckey_idx, FALSE);
        set_pad_function(g_keymap_para.lradc_pin, PAD_FUNC_GPIOIN, NULL, 0);
        adckey_idx = set_pad_function(PAD_GPIO20, PAD_FUNC_ANOLOG, NULL, 0);
        pmu_enable_lradc(adckey_idx, TRUE);
        g_keymap_para.lradc_pin = PAD_GPIO20;
    }
}

#if (SUPPORT_FAST_PREVIEW == 1)
void key_inner_set_fast_preview_cbk(fast_preview_cbk cbk)
{
    if ((cbk == NULL) && (g_fast_preview_cbk != NULL))
    {
        g_fast_preview_cbk((input_gui_msg_t *)1); //��ʾ����fast preview
    }
    g_fast_preview_cbk = cbk;
}
#endif
