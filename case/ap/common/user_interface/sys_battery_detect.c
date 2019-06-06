/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ���������ܵ�ص������ӿڣ�֧�ֳ������⣬֧�ֵ͵��⣻֧�ֵ�ص����������������ܡ�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

#define BAT_LOW_WARNING_CYCLE  120 //�͵�1���Ӳ���һ�Σ���������ǿ����͵磬����Ҫ������1���ӵ͵�Ż������͵粥��

/*!
 *  \brief
 *  g_sys_counter ϵͳ��ʱ���ṹ��
 */
extern sys_counter_t g_sys_counter;

/*! \cond COMMON_API */

void sys_battery_detect_var_init(void)
{
    g_sys_counter.bat_low_start = TRUE; //��������ʾ
    g_sys_counter.bat_low_cnt = BAT_LOW_WARNING_CYCLE - 20; //�ս���͵�10���Ӻ󲥱�һ�Σ���ֹ�ڿ���ʱ�����˵�
    g_sys_counter.bat_value_min = BATTERY_GRADE_MAX;

    //��ȡ��ص���
    g_app_info_state.bat_value = key_get_battery();
    if (g_app_info_state.bat_value > BATTERY_GRADE_MAX)
    {
        //�������
        g_app_info_state.bat_value = BATTERY_GRADE_MAX;
    }

    g_sys_counter.bat_value_win_min[0] = g_app_info_state.bat_value; //��ȷ��
    g_sys_counter.bat_value_cur_win_min = BATTERY_GRADE_MAX; //���Ƚ�
    g_sys_counter.bat_value_win_count = 1;
    g_sys_counter.bat_value_win_tick = 0;

    //����͵緢���͵�ϵͳ��Ϣ
    if (g_app_info_state.bat_value == 0)
    {
        uint16 msg_type = MSG_LOW_POWER;
        sys_mq_send(MQ_ID_HOTPLUG, &msg_type);
    }

    //��ʼ��ȥ������
    libc_memset(g_sys_counter.bat_values, g_app_info_state.bat_value, BAT_DET_VALUES_MAX);
}

//��ص������
void sys_battery_detect_handle(void)
{
    uint16 msg_type;
    uint8 cur_bat_value;
    uint8 i, tmp_bat_value_min;
    bool bat_det_fall_flag, bat_det_rise_flag, bat_det_stable_flag;
    bool tmp_bat_full_flag = FALSE;
    uint8 last_bat_value = g_app_info_state.bat_value;
    bool finish_one_win = FALSE;
    uint8 tmp_bat_value_min_wins;

#if (BATTERY_VOL_DET_TYPE == BATTERY_VOL_DET_LRADC)
    if (g_com_comval.support_bat_lradc == FALSE)
    {
        cur_bat_value = BATTERY_GRADE_MAX;
    }
    else
#endif
    {
        cur_bat_value = key_get_battery();
    }

    /* ��ȡ��ص����������͵�*/
    if (cur_bat_value > BATTERY_GRADE_MAX)
    {
        //�������
        cur_bat_value = BATTERY_GRADE_MAX;
        tmp_bat_full_flag = TRUE;
    }

    if (g_sys_counter.bat_full_flag == FALSE)
    {
        if (((get_cable_state_inner() == CABLE_STATE_CABLE_IN) || (get_adaptor_state_inner() == ADAPTOR_STATE_CABLE_IN))
            && (tmp_bat_full_flag == TRUE))
        {
            g_sys_counter.bat_full_cnt++;
            //���������10������ʾ
            if (g_sys_counter.bat_full_cnt >= 1200)
            {
                msg_type = MSG_FULL_CHARGE;
                sys_mq_send(MQ_ID_SYS, &msg_type);
                g_sys_counter.bat_full_flag = TRUE;
            }
        }
        else
        {
            g_sys_counter.bat_full_cnt = 0;
        }
    }
    else
    {
        if (((get_cable_state_inner() == CABLE_STATE_CABLE_NULL) && (get_adaptor_state_inner() == ADAPTOR_STATE_CABLE_NULL))
            || (tmp_bat_full_flag == FALSE))
        {
            g_sys_counter.bat_full_flag = FALSE;
            g_sys_counter.bat_full_cnt = 0;
        }
    }

    //����ͻȻ��Ϊ0����Ҫ������Ӧ��������
    if (cur_bat_value == 0)
    {
        if (g_app_info_state.bat_value > 4)
        {
            g_app_info_state.bat_value = 4;
        }
        else if (g_app_info_state.bat_value > 0)
        {
            g_app_info_state.bat_value--;
        }

        libc_printf_info("bat_value down: %d -> %d\n", last_bat_value, g_app_info_state.bat_value);
        libc_memset(g_sys_counter.bat_values, g_app_info_state.bat_value, BAT_DET_VALUES_MAX);
        g_sys_counter.bat_value_counter = 0;
        g_sys_counter.bat_value_debounce = 0;
        g_sys_counter.bat_det_sta = BAT_DET_NULL; //�Զ��л�Ϊ����״̬
    }
    //��������0������ȥ����������
    else
    {
        //ȥ������ѭ������
        g_sys_counter.bat_values[g_sys_counter.bat_value_index] = cur_bat_value;
        g_sys_counter.bat_value_index++;
        g_sys_counter.bat_value_index %= BAT_DET_VALUES_MAX;

        bat_det_fall_flag = FALSE;
        bat_det_rise_flag = FALSE;
        bat_det_stable_flag = FALSE;
        tmp_bat_value_min = BATTERY_GRADE_MAX;
        for (i = 0; i < BAT_DET_VALUES_MAX; i++)
        {
            if (g_sys_counter.bat_values[i] < tmp_bat_value_min)
            {
                tmp_bat_value_min = g_sys_counter.bat_values[i];
            }
        }

        //ͳ�ƴ���
        if (tmp_bat_value_min < g_sys_counter.bat_value_cur_win_min)
        {
            g_sys_counter.bat_value_cur_win_min = tmp_bat_value_min;
        }
        g_sys_counter.bat_value_win_tick++;
        if (g_sys_counter.bat_value_win_tick >= BAT_DET_WIN_TIME)
        {
            if (g_sys_counter.bat_value_win_count < BAT_DET_WIN_COUNT)
            {
                g_sys_counter.bat_value_win_min[g_sys_counter.bat_value_win_count] = g_sys_counter.bat_value_cur_win_min;
                g_sys_counter.bat_value_win_count++;
            }
            else
            {
                for (i = 0; i < (BAT_DET_WIN_COUNT-1); i++)
                {
                    g_sys_counter.bat_value_win_min[i] = g_sys_counter.bat_value_win_min[i+1];
                }
                g_sys_counter.bat_value_win_min[BAT_DET_WIN_COUNT-1] = g_sys_counter.bat_value_cur_win_min;
            }

            g_sys_counter.bat_value_win_tick = 0;
            g_sys_counter.bat_value_cur_win_min = BATTERY_GRADE_MAX;

            finish_one_win = TRUE;
        }

        if (tmp_bat_value_min < g_app_info_state.bat_value)
        {
            bat_det_fall_flag = TRUE;

            bat_det_stable_flag = TRUE;
            for (i = 0; i < BAT_DET_VALUES_MAX; i++)
            {
                if ((g_sys_counter.bat_values[i] - tmp_bat_value_min) > 1)
                {
                    bat_det_stable_flag = FALSE;
                    break;
                }
            }

            if (g_sys_counter.bat_det_sta != BAT_DET_FALL)
            {
                g_sys_counter.bat_det_sta = BAT_DET_FALL;

                g_sys_counter.bat_value_min = BATTERY_GRADE_MAX;
                g_sys_counter.bat_value_counter = 0;
            }

            if (tmp_bat_value_min <= g_sys_counter.bat_value_min)
            {
                g_sys_counter.bat_value_min = tmp_bat_value_min;
                g_sys_counter.bat_value_debounce = 0;
            }
            else if (g_sys_counter.bat_det_sta == BAT_DET_FALL)
            {
                g_sys_counter.bat_value_debounce++;
                if (g_sys_counter.bat_value_debounce >= 10)
                {
                    g_sys_counter.bat_value_min++;
                    g_sys_counter.bat_value_debounce = 0;
                    g_sys_counter.bat_value_counter = 0;
                    if (g_sys_counter.bat_low_start == FALSE)
                    {
                        g_sys_counter.bat_low_cnt = 0;//�Ӹߵ�ѹ�½����͵�ѹҪ����1���ӵ͵�ſ�ʼ����
                    }
                }

                if (g_sys_counter.bat_value_min == g_app_info_state.bat_value)
                {
                    g_sys_counter.bat_det_sta = BAT_DET_NULL;
                }
            }
            g_sys_counter.bat_value_counter++;

            if (g_sys_counter.bat_det_sta == BAT_DET_FALL)
            {
                //�����ѹ���ȶ��������Ͻ��͵�ѹ��һ����λ
                if (bat_det_stable_flag == TRUE)
                {
                    g_app_info_state.bat_value = g_sys_counter.bat_value_min;
                    libc_printf_info("bat_value down: %d -> %d\n", last_bat_value, g_app_info_state.bat_value);
                    g_sys_counter.bat_value_counter = 0;
                    g_sys_counter.bat_value_debounce = 0;
                    if (g_app_info_state.bat_value == 1)//�͵��Һ��ȶ���Ӧ��������ʾ�͵�
                    {
                        g_sys_counter.bat_low_cnt = BAT_LOW_WARNING_CYCLE;
                    }
                    g_sys_counter.bat_det_sta = BAT_DET_NULL; //�Զ��л�Ϊ����״̬
                }
                //�����ѹ���ȶ�������Ҫ����30���Ӳ�������͵�������ֹ����
                else if (g_sys_counter.bat_value_counter >= BAT_DET_WIN_TIME)
                {
                    do
                    {
                        g_app_info_state.bat_value--;
                        libc_printf_info("bat_value down: %d -> %d\n", last_bat_value, g_app_info_state.bat_value);
                    } while ((g_app_info_state.bat_value > g_sys_counter.bat_value_min)
                        && (g_volume_infor.phy_limit_tbl[g_app_info_state.bat_value] >= 0)); //ʵ���Ͻ����������Ʒ��ȼ���

                    g_sys_counter.bat_value_counter = 0;
                    g_sys_counter.bat_value_debounce = 0;
                    g_sys_counter.bat_det_sta = BAT_DET_NULL; //�Զ��л�Ϊ����״̬
                }
            }
        }
        else if (tmp_bat_value_min > g_app_info_state.bat_value)
        {
            if ((finish_one_win == TRUE) && (g_sys_counter.bat_value_win_count == BAT_DET_WIN_COUNT))
            {
                tmp_bat_value_min_wins = BATTERY_GRADE_MAX;
                for (i = 0; i < BAT_DET_WIN_COUNT; i++)
                {
                    if (g_sys_counter.bat_value_win_min[i] < tmp_bat_value_min_wins)
                    {
                        tmp_bat_value_min_wins = g_sys_counter.bat_value_win_min[i];
                    }
                }
                //��Ҫ����5���Ӳ��������ߵ�������ֹ����
                if (tmp_bat_value_min_wins > g_app_info_state.bat_value)
                {
                    bat_det_rise_flag = TRUE;
                    g_sys_counter.bat_det_sta = BAT_DET_RISE;
                }

                //���ߵ���
                if (bat_det_rise_flag == TRUE)
                {
                    if (g_sys_counter.bat_low_start == FALSE)
                    {
                        g_sys_counter.bat_low_cnt = 0;//�Ӹߵ�ѹ�½����͵�ѹҪ����1���ӵ͵�ſ�ʼ����
                    }

                    g_app_info_state.bat_value = tmp_bat_value_min_wins;
                    libc_printf_info("bat_value up: %d -> %d\n", last_bat_value, g_app_info_state.bat_value);
                    g_sys_counter.bat_value_counter = 0;
                    g_sys_counter.bat_value_debounce = 0;
                    g_sys_counter.bat_det_sta = BAT_DET_NULL; //�Զ��л�Ϊ����״̬
                }
            }
        }
    }

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if ((get_tws_mode_inner() != TWS_MODE_SINGLE) && (get_tws_role_inner() == TWS_SLAVE))
    {
        if (g_app_info_state.bat_value != g_tws_slave_ui_info.bat_val)
        {
            g_tws_slave_ui_info.bat_val = g_app_info_state.bat_value;
            if (com_tws_s2m_update_bat(g_tws_slave_ui_info.bat_val) == FALSE)
            {
                g_tws_slave_ui_info.bat_val_update = TRUE;
            }
        }
    }
    else if ((get_tws_mode_inner() != TWS_MODE_SINGLE) && (get_tws_role_inner() == TWS_MASTER))
    {
        uint8 min_bat_val;

        min_bat_val = g_app_info_state.bat_value;
        if (g_app_info_state.slave_bat_value < g_app_info_state.bat_value)
        {
            min_bat_val = g_app_info_state.slave_bat_value;
        }

        if (g_volume_infor.phy_limit_current != g_volume_infor.phy_limit_tbl[min_bat_val])
        {
            com_set_volume_limit_inner(g_volume_infor.phy_limit_tbl[min_bat_val]);
            com_tws_m2s_set_vol_limit_cmd(g_volume_infor.phy_limit_tbl[min_bat_val]);
        }
    }
    else
    {
        //����volume_phy_limit����������仯��
        if (g_volume_infor.phy_limit_current != g_volume_infor.phy_limit_tbl[g_app_info_state.bat_value])
        {
            com_set_volume_limit_inner(g_volume_infor.phy_limit_tbl[g_app_info_state.bat_value]);
        }
    }
#else
    //����volume_phy_limit����������仯��
    if (g_volume_infor.phy_limit_current != g_volume_infor.phy_limit_tbl[g_app_info_state.bat_value])
    {
        com_set_volume_limit_inner(g_volume_infor.phy_limit_tbl[g_app_info_state.bat_value]);
    }
#endif

    if (g_app_info_state.bat_value <= 1)
    {
        if (g_app_info_state.bat_value == 0)
        {
            msg_type = MSG_LOW_POWER;
            sys_mq_send(MQ_ID_HOTPLUG, &msg_type);
        }
        else if (get_cur_app_id_inner() != APP_ID_CONFIG)//���ػ��ڼ�������ʾ�����������硱
        {
            bool need_bat_low_ui = TRUE;

#if (BATTERY_CHARGE_BY_DC5V == 1)
            if ((get_cable_state() == CABLE_STATE_CABLE_IN) || (get_adaptor_state() == ADAPTOR_STATE_CABLE_IN))
            {
                need_bat_low_ui = FALSE;
            }
#endif

            if (need_bat_low_ui == TRUE)
            {
                g_sys_counter.bat_low_cnt++;
                if (g_sys_counter.bat_low_cnt >= BAT_LOW_WARNING_CYCLE)
                {
                    g_sys_counter.bat_low_cnt = 0;

                    msg_type = MSG_LOW_BATTERY;
                    sys_mq_send(MQ_ID_SYS, &msg_type);

                    g_sys_counter.bat_low_start = TRUE;
                }
            }
            else
            {
                g_sys_counter.bat_low_start = TRUE; //��������ʾ
                g_sys_counter.bat_low_cnt = BAT_LOW_WARNING_CYCLE - 20; //�ε�DC5V��10������ʾ����������
            }
        }
    }
    else
    {
        g_sys_counter.bat_low_start = FALSE;
        g_sys_counter.bat_low_cnt = 0;//�Ӹߵ�ѹ�½����͵�ѹҪ����1���ӵ͵�ſ�ʼ����
    }
}

/*! \endcond */
