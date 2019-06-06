/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������ϵͳ��ʱ��HANDLE������ʡ��ػ�����ص�����⣬����Ӧ��Ƶ��
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

/*!
 *  \brief
 *  g_sys_counter ϵͳ��ʱ���ṹ��
 */
sys_counter_t g_sys_counter;

extern void sys_battery_detect_var_init(void);
extern void sys_battery_detect_handle(void);

/*! \cond COMMON_API */

void sys_counter_var_init(void)
{
    libc_memset(&g_sys_counter, 0x00, sizeof(sys_counter_t));

    //ʡ��ػ�ʱ�� ��1����Ϊ��λ
    g_sys_counter.poweroff_enable = (uint8) com_get_config_default_inner(SETTING_ONOFF_POWEROFF_ENABLE);
    //NOTE! S2״̬��δ�������ƣ��������� TODO
    g_sys_counter.poweroff_mode = POWEROFF_MODE_S3;//(uint8) com_get_config_default_inner(SETTING_ONOFF_POWEROFF_MODE);
    g_sys_counter.poweroff_timer = (uint8) com_get_config_default_inner(SETTING_ONOFF_POWEROFF_TIMER);
    g_sys_counter.poweroff_timer_bt_wait = (uint8) com_get_config_default_inner(SETTING_ONOFF_POWEROFF_BT_WAITPAIR_TIMER);

    //LED���������ʱ�䣬1��Ϊ��λ��0��ʾ���ر�
    if ((uint8) com_get_config_default_inner(SETTING_DISPLAY_LED_SEGMENT_LIGHT_ENABLE) == 0)
    {
        g_sys_counter.light_timer = 0;
    }
    else
    {
        g_sys_counter.light_timer = com_get_config_default_inner(SETTING_DISPLAY_LED_SEGMENT_LIGHT_TIMER);
    }

    //��ص�������ʼ��
    sys_battery_detect_var_init();
}

void sys_counter_reset(void)
{
    g_sys_counter.poweroff_counter = 0;
    g_sys_counter.ui_idle_counter = 0;
    g_sys_counter.light_counter = 0;

    if (get_ui_status() == UI_STATUS_IDLE)
    {
        if (get_sys_clk_work() == FALSE)
        {
            sys_adjust_clk(SYS_CLK_ON_BUSY, SET_SYS_CLK);
        }
        change_ui_status(UI_STATUS_BUSY);
    }

#if (SUPPORT_LED_DISPLAY != 0)
    if ((g_com_comval.support_led_segment != 0) && (get_backlight_state() == BACKLIGHT_STATE_OFF))
    {
        led_light_onoff(TRUE);
        change_backlight_state(BACKLIGHT_STATE_NORMAL);
    }
#endif
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ϵͳ��ʱ��ʱ���������̣�ʡ��ػ���˯�߹ػ�����ص�����⡣
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      misc_func
 * \note
 *******************************************************************************/
void sys_counter_handle(void)
{
    uint16 msg_type;
    bool cur_idle_flag;
    uint8 stub_tools_type = com_get_stub_tools_type_inner();

#if (SUPPORT_LED_DISPLAY != 0)
    if ((g_com_comval.support_led_segment != 0)
        && ((get_cable_state_inner() != CABLE_STATE_CABLE_IN) && (get_adaptor_state_inner() != ADAPTOR_STATE_CABLE_IN)))
    {
        /* �ر�LED����ܱ��� */
        if ((g_sys_counter.light_timer > 0) && (get_backlight_state() == BACKLIGHT_STATE_NORMAL))
        {
            g_sys_counter.light_counter++;
            if (g_sys_counter.light_counter >= (g_sys_counter.light_timer * 2))
            {
                led_light_onoff(FALSE);
                change_backlight_state(BACKLIGHT_STATE_OFF);
            }
        }
    }
#endif

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    if ((get_tws_mode_inner() != TWS_MODE_SINGLE) && (get_tws_role_inner() == TWS_SLAVE))
    {
        cur_idle_flag = FALSE;
    }
    else
#endif
    {
        cur_idle_flag = TRUE;

        if (get_ui_status() == UI_STATUS_BUSY)
        {
            cur_idle_flag = FALSE;
        }

        //�͹���ģʽҪ��������������ͱ������͹���ģʽ
        if ((g_sys_counter.poweroff_enable == TRUE) && (g_sys_counter.poweroff_mode == POWEROFF_MODE_S2))
        {
            if (get_bt_state_inner() == BT_STATE_WORK)
            {
                com_get_btengine_status_inner(NULL);
                if (bt_info.bt_in_idle == FALSE)
                {
                    cur_idle_flag = FALSE;
                }
            }
        }

        if ((get_cable_state_inner() == CABLE_STATE_CABLE_IN) || (get_adaptor_state_inner() == ADAPTOR_STATE_CABLE_IN))
        {
            cur_idle_flag = FALSE;
        }

        if ((get_engine_state() == ENGINE_STATE_PLAYING) || (get_app_state() != APP_STATE_NO_PLAY))
        { //��̨�����ǰ̨Ӧ�ô��ڲ���״̬
            cur_idle_flag = FALSE;
        }
        if ((stub_tools_type == STUB_PC_TOOL_ASET_EQ_MODE)
                || (stub_tools_type == STUB_PC_TOOL_ASQT_MODE)
                || (stub_tools_type == STUB_PC_TOOL_ATT_MODE))
        { //ASET/ASQT/ATT��������ʱ������͹���ģʽ
            cur_idle_flag = FALSE;
        }
    }

    /* ����ʡ��ػ���ʱ */
    if ((g_app_last_state.last_idle_flag == FALSE) && (cur_idle_flag == TRUE))
    {
        libc_printf_info("WORK -> IDLE\n");
        g_sys_counter.poweroff_counter = 0;
    }
    else if ((g_app_last_state.last_idle_flag == TRUE) && (cur_idle_flag == FALSE))
    {
        libc_printf_info("IDLE -> WORK\n");
        g_sys_counter.poweroff_counter = 0;
    }

    /* ʡ��ػ���ʱ�� */
    if ((g_sys_counter.poweroff_enable == TRUE) && (cur_idle_flag == TRUE))
    {
        bool need_poweroff = FALSE;

        g_sys_counter.poweroff_counter++;

        if ((get_cur_app_id_inner() == APP_ID_BTPLAY)
                && ((get_bt_state_inner() == BT_STATE_WORK) && (bt_info.status == STATUS_WAIT_PAIRED)))
        {
            if (g_sys_counter.poweroff_counter >= (g_sys_counter.poweroff_timer_bt_wait * 120))
            {
                need_poweroff = TRUE;
            }
        }
        else
        {
            if (g_sys_counter.poweroff_counter >= (g_sys_counter.poweroff_timer * 120))
            {
                need_poweroff = TRUE;
            }
        }

        if (need_poweroff == TRUE)
        {
            //�㲥�ػ���Ϣ
            if (g_sys_counter.poweroff_mode == POWEROFF_MODE_S3)
            {
                msg_type = MSG_POWER_OFF;
            }
            else
            {
                msg_type = MSG_SYSTEM_ENTER_S2;
            }
            sys_mq_send(MQ_ID_SYS, &msg_type);
        }
    }

    /* ��ص������*/
    sys_battery_detect_handle();

    /* ����һ��ʱ��ͽ���Ƶ�� */
    if (get_ui_status() == UI_STATUS_BUSY)
    {
        g_sys_counter.ui_idle_counter++;
        if (g_sys_counter.ui_idle_counter >= UI_IDLE_TIMER)
        {
            if (get_sys_clk_work() == FALSE)
            {
                sys_adjust_clk(SYS_CLK_ON_IDLE, SET_SYS_CLK);
            }
            change_ui_status(UI_STATUS_IDLE);
        }
    }

    /* �������״̬ */
    g_app_last_state.last_idle_flag = cur_idle_flag;

    /* STUBģʽ�¼��usb״̬�����usb�������˳�STUBģʽ */
    if ((stub_tools_type != 0) && (get_cable_state_inner() == CABLE_STATE_CABLE_NULL))
    {
        com_stub_tools_disconnect();
    }
}

void debug_detect(void)
{
#if (SUPPORT_MFP_EJTAG == 1)
    if (g_app_info_state.btdbg_ejtag_flag == 0)
    {
        uint16 debug_val = hal_pmu_read_lradc(DEBUG_LRADC_IDX);
        if ((debug_val >= LRADC_0_60_V) && (debug_val <= LRADC_0_80_V))
        {
            if (g_com_comval.support_led_segment == 1)
            {
                led_module_uninstall();
            }
            g_com_comval.support_led_segment = 0; //����ģʽ��LED�Ƴ�ͻ

            //����EJTAG 4��
            set_pad_function(MPAD_EJTAG_DEF, PAD_FUNC_DEFAULT, NULL, 0);
            g_app_info_state.btdbg_ejtag_flag = 2;

            libc_printf_info("switch to ejtag 4\n");
        }
    }
#endif
}

void hardware_timer_500ms_isr(void)
{
    //40PIN ICĬ�ϲ���ʹ��BT DEBUG��EJTAG DEBUG����Ҫ���Ӳ���źŴ���ʹ��
    debug_detect();
}

//ֻ���ڳ��μ�⵽����BT DEBUGģʽʱ�Ż᷵��TRUE
bool com_enter_btdbg_flag_inner(uint32 *btdbg_sel)
{
    bool enter_flag = FALSE;

#if (SUPPORT_MFP_EJTAG == 1)
#if (SUPPORT_MFP_BTDEBUG == 1)
    if (g_app_info_state.btdbg_ejtag_flag != 1)
    {
        uint16 debug_val = hal_pmu_read_lradc(DEBUG_LRADC_IDX);
        if ((debug_val >= LRADC_1_30_V) && (debug_val <= LRADC_1_50_V))
        {
            //�ص�GPIO3 LRADC��⹦�ܣ��Ա�ʹ��GPIO3��ΪDB��
            set_pad_function(DEBUG_LRADC_PIN, PAD_FUNC_GPIOIN, NULL, 0);

            if (g_com_comval.support_led_segment == 1)
            {
                led_module_uninstall();
            }
            g_com_comval.support_led_segment = 0; //����ģʽ��LED�Ƴ�ͻ

            //ADCKEY �� GPIO7 �л��� GPIO20
            key_adckey_switch();

            g_com_comval.support_led_light = 0; //����ģʽ��LED�Ƴ�ͻ
            g_com_comval.support_extern_pa = 0; //����ģʽ���ⲿ���ų�ͻ
            g_com_comval.support_fmradio = 0; //����ģʽ���ⲿ���ų�ͻ
            g_com_comval.support_aux_detect = 0; //����ģʽ��AUX����ͻ
            change_linein_state(LINEIN_STATE_LINEIN_IN);
#if (SUPPORT_IR_KEY == 1)
            if (g_com_comval.support_ir_key == 1)
            {
                ir_module_uninstall();
            }
            g_com_comval.support_ir_key = 0; //����ģʽ��IRң�س�ͻ
#endif

            //����EJTAG 4��
            set_pad_function(MPAD_EJTAG_DEF, PAD_FUNC_DEFAULT, NULL, 0);
            g_app_info_state.btdbg_ejtag_flag = 2;

            libc_printf_info("switch to ejtag 4\n");

            //����BT DEBUG, enable DB 3/4/5/6/7
            set_pad_function(MPAD_BTDEBUG, PAD_FUNC_DEFAULT, NULL, (void *) 0xf8); //enable DB 3/4/5/6/7

            g_app_info_state.btdbg_ejtag_flag = 1;

            libc_printf_info("switch to bt debug\n");

            *btdbg_sel = 0xDBB40;//enable DB 3/4/5/6/7
            enter_flag = TRUE;
        }
    }
#endif
#endif

    return enter_flag;
}

//��������Ƿ����DUT����
void com_enter_duttest_check_inner(void)
{
#if (SUPPORT_MFP_EJTAG == 1)
#if (SUPPORT_MFP_DUTDEBUG == 1)
    uint16 debug_val = hal_pmu_read_lradc(DEBUG_LRADC_IDX);
    if (debug_val <= LRADC_0_10_V)
    {
        libc_printf_info("enter dut test!!\n");
        set_enter_dut_test(1);
    }
#endif
#endif
}

void common_debug_config(void)
{
#if (SUPPORT_MFP_EJTAG == 1)
    set_pad_function(DEBUG_LRADC_PIN, PAD_FUNC_ANOLOG, NULL, 0);
    pmu_enable_lradc(DEBUG_LRADC_IDX, TRUE);
#endif
}

/*! \endcond */
