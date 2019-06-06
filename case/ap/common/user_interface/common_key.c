/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������������Ϣ���Ӻ�����֧�ְ�������֧�ְ������˴������ʡ��ػ���ϵͳ��ʱ����
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

/*! ���˵���������������Ҫ���˵İ��� */
static uint8 this_filter_key_value;
/*! ���˵��������������������� KEY_TYPE_HOLD & KEY_TYPE_SHORT_UP */
static uint8 filter_key;
/*! ���˵�����̧�������� KEY_TYPE_SHORT_UP */
static uint8 filter_key_up;
/*! ��ǰ�������ͣ������Ƿ�TWS���䰴�� */
static input_msg_type_e this_input_msg_type;
/*! ����ʱ�䰴����ʱ */
static uint32 long_key_time_start;
/*! ����һС��ʱ���ڵ����ⰴ��: ����ʱ�䳤�ȣ�0��ʾ������ */
static uint32 filter_time_length;
/*! ����һС��ʱ���ڵ����ⰴ��: ������ʼʱ��� */
static uint32 filter_time_start;

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    ��ʼ���Ű�������
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      common_func
 * \note
 *******************************************************************************/
void com_start_key_tone_inner(void)
{
    if ((uint8) com_get_config_default_inner(SETTING_SOUND_ENABLE_KEYTONE) == 1)
    {
#if (SUPPORT_SOFT_KEYTONE == 1)
        keytone_play_inner (KT_ID_KEYTONE);
#else
        keytone_play_inner (KT_MODE_KEYTONE);
#endif
    }
}

//ר���������õ�ǰ������
bool com_gui_msg_hook_slave(input_gui_msg_t *input_msg)
{
    key_value_e key_val = input_msg->data.kmsg.val;
    key_type_e key_type = input_msg->data.kmsg.type;

    //���µ�ǰ����ֵ
    if ((key_type == KEY_TYPE_SHORT_UP) //��������Ѿ�̧����ô���ù��˰�����Ч
    || (key_type == KEY_TYPE_LONG_UP))
    {
        this_filter_key_value = KEY_NULL;
    }
    else
    {
        this_filter_key_value = key_val;
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    �����������⴦����Ϊ gui ��Ϣ����Ļص�������
 * \param[in]    input_msg ��ȡ���� gui��Ϣָ��
 * \param[out]   input_msg ���ܻ��input_msg�����޸ģ����罫val�޸�ΪKEY_HOLD���ⰴ��
 * \return       bool
 * \retval           TRUE ��ȡ���İ�����Ч��û�б����˵���
 * \retval           FALSE ��ȡ���İ�����Ч�������˵��ˡ�
 * \ingroup      common_func
 * \note
 *******************************************************************************/
bool com_gui_msg_hook(input_gui_msg_t *input_msg)
{
    key_value_e key_val = input_msg->data.kmsg.val;
    key_type_e key_type = input_msg->data.kmsg.type;

    //���˵�һ��ʱ���ڵ����ⰴ�����������Ը���֧����ϰ�������ϰ������º�̧��ʱ��Ϊ�޷���֤ͬʱ̧�𣬻���һ��β����Ҫ�������
    if (filter_time_length != 0)
    {
        if ((sys_get_ab_timer() - filter_time_start) < filter_time_length)
        {
            return FALSE;
        }
        else
        {
            filter_time_length = 0;
        }
    }

    //1.������������˵�������������⵽����̧�𣬻��������������£���ð����������ڣ����������ɣ�
    if (key_val == filter_key)
    {
        //���˻�û��ɾ��յ�DOWN��Ϣ������SHORTUP��Ϣ���л�Ӧ��ʱ��������ˣ�
        //��ôDOWN��Ϣ�������µİ������ڵ���Ϣ����Ӧ�ñ�����
        if (key_type == KEY_TYPE_DOWN) //�յ��ð����������ڣ��������
        {
            filter_key = KEY_NULL;
        }
        else
        {
            //����LONG��HOLD����
            if ((key_type != KEY_TYPE_SHORT_UP)
                    && (key_type != KEY_TYPE_LONG_UP))
            {
                return FALSE;
            }
            else //������ɣ������ǲ���ӦSHORT_UP����
            {
                this_filter_key_value = KEY_NULL;
                filter_key = KEY_NULL;
                return FALSE;
            }
        }
    }
    else //�յ��°������������
    {
        filter_key = KEY_NULL;
    }

    //2.������������˵�����̧�𰴼�����⵽����̧�𣬻��������������£���ð����������ڣ����������ɣ�
    if (key_val == filter_key_up)
    {
        //���˻�û��ɾ��յ�DOWN��Ϣ������SHORTUP��Ϣ���л�Ӧ��ʱ��������ˣ�
        //��ôDOWN��Ϣ�������µİ������ڵ���Ϣ����Ӧ�ñ�����
        if (key_type == KEY_TYPE_DOWN) //�յ��ð����������ڣ��������
        {
            filter_key_up = KEY_NULL;
        }
        else
        {
            //������̧�𰴼�
            if ((key_type == KEY_TYPE_SHORT_UP)
                    || (key_type == KEY_TYPE_LONG_UP))
            {
                this_filter_key_value = KEY_NULL;
                filter_key_up = KEY_NULL;
                return FALSE;
            }
        }
    }
    else //�յ��°������������
    {
        filter_key_up = KEY_NULL;
    }

    //�°����������¼�ʱ
    if (key_type == KEY_TYPE_DOWN)
    {
        long_key_time_start = sys_get_ab_timer();
    }

    //���µ�ǰ����ֵ
    if ((key_type == KEY_TYPE_SHORT_UP) //��������Ѿ�̧����ô���ù��˰�����Ч
    || (key_type == KEY_TYPE_LONG_UP))
    {
        this_filter_key_value = KEY_NULL;
    }
    else
    {
        this_filter_key_value = key_val;
    }

    //3.�������������ڰ�������ʱ����������
    if ((key_type == KEY_TYPE_DOWN) && (input_msg->type == INPUT_MSG_KEY))
    {
        //û��Ƶ������Ǿ���״̬���Ұ�����ʹ�ܣ��ŷ���������
        com_start_key_tone_inner();
    }

    //4.ʡ��ػ���ʱ ��0
    sys_counter_reset();

    this_input_msg_type = input_msg->type;

    //�����յ�һ��������Ϣ������˫����������
    if (g_key_deal_infor.double_click_enable == TRUE)
    {
        switch(g_key_deal_infor.dc_sta)
        {
        case DC_KEY_STATUS_NULL:
            if (g_key_deal_infor.dc_key_1 != KEY_NULL)
            {
                if ((key_val == g_key_deal_infor.dc_key_1) && (key_type == KEY_TYPE_DOWN))
                {
                    g_key_deal_infor.dc_sta = DC_KEY_STATUS_FIRST_DOWN;
                    g_key_deal_infor.dc_last_time = sys_get_ab_timer();
                    g_key_deal_infor.dc_key_cur = key_val;
                }
            }
            if (g_key_deal_infor.dc_key_2 != KEY_NULL)
            {
                if ((key_val == g_key_deal_infor.dc_key_2) && (key_type == KEY_TYPE_DOWN))
                {
                    g_key_deal_infor.dc_sta = DC_KEY_STATUS_FIRST_DOWN;
                    g_key_deal_infor.dc_last_time = sys_get_ab_timer();
                    g_key_deal_infor.dc_key_cur = key_val;
                }
            }
            break;

        case DC_KEY_STATUS_FIRST_DOWN:
            if ((key_val == g_key_deal_infor.dc_key_cur) && (key_type == KEY_TYPE_SHORT_UP))
            {
                g_key_deal_infor.dc_sta = DC_KEY_STATUS_FIRST_UP;
                g_key_deal_infor.dc_last_time = sys_get_ab_timer();
            }
            else
            {
                if (key_val != g_key_deal_infor.dc_key_cur)
                {
                    libc_printf_warning("dc unexpect key val!!\n");
                    g_key_deal_infor.dc_sta = DC_KEY_STATUS_NULL;
                    return FALSE;
                }
                else
                {
                    g_key_deal_infor.dc_fail_reason = DC_FAIL_FIRST_LONG;
                }
                g_key_deal_infor.double_click_fail_flag = TRUE;
            }
            break;

        case DC_KEY_STATUS_FIRST_UP:
            if ((key_val == g_key_deal_infor.dc_key_cur) && (key_type == KEY_TYPE_DOWN))
            {
                g_key_deal_infor.dc_sta = DC_KEY_STATUS_SECOND_DOWN;
                g_key_deal_infor.dc_last_time = sys_get_ab_timer();
            }
            else
            {
                if (key_val != g_key_deal_infor.dc_key_cur)
                {
                    g_key_deal_infor.dc_fail_reason = DC_FAIL_SECOND_OTHER_KEY;
                    g_key_deal_infor.other_key = key_val;
                }
                else
                {
                    libc_printf_warning("dc unexpect key type!!\n");
                    g_key_deal_infor.dc_sta = DC_KEY_STATUS_NULL;
                    return FALSE;
                }
                g_key_deal_infor.double_click_fail_flag = TRUE;
            }
            break;

        case DC_KEY_STATUS_SECOND_DOWN:
            if ((key_val == g_key_deal_infor.dc_key_cur) && (key_type == KEY_TYPE_SHORT_UP))
            {
                //˫�������ɹ�
                input_msg->data.kmsg.type = KEY_TYPE_DOUBLE_CLICK; //���İ�������Ϊ KEY_TYPE_DOUBLE_CLICK
                g_key_deal_infor.dc_sta = DC_KEY_STATUS_NULL;
            }
            else
            {
                if (key_val != g_key_deal_infor.dc_key_cur)
                {
                    libc_printf_warning("unexpect key val!!\n");
                    g_key_deal_infor.dc_sta = DC_KEY_STATUS_NULL;
                    return FALSE;
                }
                else
                {
                    g_key_deal_infor.dc_fail_reason = DC_FAIL_SECOND_LONG;
                }
                g_key_deal_infor.double_click_fail_flag = TRUE;
            }
            break;

        default:
            break;
        }

        if (g_key_deal_infor.dc_sta != DC_KEY_STATUS_NULL)
        {
            return FALSE;
        }
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ���˵���ǰ������������������ֵͬ������Ϣ��
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      common_func
 * \note
 * \li  �ýӿ����ڹ��˵�ǰ����ֵ�ĺ���������������˵�����ָ����������ֱ�ӵ���
 *      change_filter_key ���á�
 * \li  �ڼ�⵽��ǰ����̧����Ϣ��������������Ϣ���� com_gui_msg_hook �Զ����ý����
 * \li  �����ʱû�а������£��ýӿڵ��ò�����������İ�������Ч����
 *******************************************************************************/
void com_filter_key_hold_inner(void)
{
    filter_key = this_filter_key_value;
}

void com_filter_key_up_inner(void)
{
    filter_key_up = this_filter_key_value;
}

void com_filter_key_period_of_time_inner(uint16 time_length)
{
    filter_time_start = sys_get_ab_timer();
    filter_time_length = time_length;
}

//������סʱ�䳤�ȣ����յ�DOWN��Ϣ��ʼ����
uint32 com_get_long_key_time_inner(void)
{
    return (sys_get_ab_timer() - long_key_time_start);
}

//��ȡ��ǰ���ڴ���İ�������
input_msg_type_e com_get_input_msg_type_inner(void)
{
    return this_input_msg_type;
}

/*! \endcond */
