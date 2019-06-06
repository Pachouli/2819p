/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * ������COMMON ��ʼ���ӿڣ������ڵ���COMMON�κ������ӿ�ǰ���ã��� manager Ӧ�ó�ʼ��
 *       ʱ���á�
 * ���ߣ�cailizhen
 ********************************************************************************/

#include "common_func_inner.h"

static uint8 __section__(".bss.print_dma_cache") g_print_dma_cache[PRINT_DMA_CACHE_LEN];

extern void common_keydrv_init(void);
extern void common_debug_config(void);
extern void common_card_config(void);
extern void common_usb_config(void);
extern void volume_var_init(void);
extern void common_load_app_config(void);
extern void applib_var_init(void);
extern void app_state_var_init(void);
extern void led_var_init(void);
extern void keytone_var_init(void);
extern void tts_play_var_init(void);
extern void sys_counter_var_init(void);
extern void com_load_rtc_alarm_manager_info(void);

extern void common_spi1_set_mfp(void);

#if (SUPPORT_FAST_PREVIEW == 1)
extern void com_fast_preview_init(void);
#endif

void app_state_var_init(void)
{
    libc_memset(&g_app_info_state, 0x00, sizeof(g_app_info_state));

#if (SUPPORT_LINEIN_DETECT == DETECT_NO_SUPPORT)
    g_app_info_state.linein_state = LINEIN_STATE_LINEIN_IN;
#endif
    g_app_info_state.bat_value = BATTERY_GRADE_MAX;

    //for ap switch infor index invalid value
    g_app_info_state.cur_func_index = 0xff;

    g_app_last_state.last_func_index = 0;

    g_app_last_state.last_idle_flag = FALSE;
}

void print_para_setting(void)
{
    uart_print_info_t up_info;

    //����UART��ӡ����
    up_info.print_mode = UART_PRINT_MODE_DISABLE;
#if (SUPPORT_UART_PRINT == 1)
    if ((uint8) com_get_config_default_inner(SETTING_UART_PRINT_ENABLE) == 1)
    {
        up_info.print_mode = UART_PRINT_MODE_DMA; //ע�⣺�����ΪAHB��ӡ��������ĳЩ�����»���Ϊ�жϴ�ӡ�͸����ȼ��̴߳�ӡ̫�ർ��ϵͳ����
    }
#endif

#if (SUPPORT_SPI_LCD_DISPLAY == 1)
    up_info.print_mode = UART_PRINT_MODE_DISABLE; //����ʱ���Ը�Ϊ UART_PRINT_MODE_AHB��������ʱ��� UART_PRINT_MODE_DISABLE
#endif

    //����UART TX PIN
    if (up_info.print_mode == UART_PRINT_MODE_AHB)
    {
        set_pad_function(PRINT_UART_TX_PIN, PRINT_UART_TX_FUNC, NULL, 0);
        uart_tx_set(PRINT_UART_TX_IDX, 115200, UART_CLK_AHB);
    }
    else if (up_info.print_mode == UART_PRINT_MODE_DMA)
    {
        set_pad_function(PRINT_UART_TX_PIN, PRINT_UART_TX_FUNC, NULL, 0);
        uart_tx_set(PRINT_UART_TX_IDX, 115200, UART_CLK_DMA);
    }

    up_info.uart_idx = PRINT_UART_TX_IDX;
    up_info.dma_chan = PRINT_UART_TX_DMA_CHAN;
    up_info.cache_buf = g_print_dma_cache;
    up_info.cache_len = PRINT_DMA_CACHE_LEN;
    sys_set_system_info(SET_SYS_UART_PRINT_SWITCH, &up_info);
}

//COMMON ��ģ���ʼ��
bool common_init_inner(void)
{
    //Ĭ�Ϲر�EJTAG�ڣ�����GPIO30~GPIO35�ſ���������Ϊ������;
    set_pad_function(MPAD_EJTAG_DEF, PAD_FUNC_DEFAULT, NULL, 0xff);

    //STEP 1������config�������ļ�
    if (com_open_config_file() == FALSE)
    {
        libc_printf_error("open config bin fail!\n");
        while (1)
        {
            ; //for QAC
        }
    }

    //STEP 2��������ӡ
    print_para_setting();

    //STEP 3��Ӧ�ò�ȫ�ֱ�����ʼ��
    if (sizeof(esd_bk_infor_t) > ESD_BK_INFOR_LEN)
    {
        libc_printf_error("esd_bk_infor_t too length! 0x%x\n", sizeof(esd_bk_infor_t));
        while (1)
            ;
    }
    if (sizeof(global_var_t) > GLOBAL_VAR_LEN)
    {
        libc_printf_error("global_var_t too length! 0x%x\n", sizeof(global_var_t));
        while (1)
            ;
    }
    libc_memset(g_p_global_var, 0x00, sizeof(global_var_t));

    //STEP 3������ϵͳ����������ΪһЩģ��Ŀ��أ�����Ӧ�þ�����ǰ
    com_load_sys_comval();

    //STEP 4��װ��KEY����
    common_keydrv_init();

    //SETP 4.1��װ����KEY�����󼴿ɰ�װLED�����ģ��
    if (g_com_comval.support_led_segment == 1)
    {
        led_module_install();
    }

    //STEP 4.2: װ��FM��������
#if (SUPPORT_FM_TX == 1)
    fm_tx_module_install();
    fm_tx_open(g_com_env_var.fm_tx_freq);
#endif

    //STEP 4.3: ��ʼ��SPI1�ӿ�
#if ((SUPPORT_SPI_LCD_DISPLAY == 1) || (SUPPORT_SPI1NOR_STORAGE == 1))
    common_spi1_set_mfp();
#endif

    //STEP 4.4: ��ʼ������SPI1 NORFLASH
#if (SUPPORT_SPI1NOR_STORAGE == 1)
    spi1nor_init();
    libc_printf("nor1 id:0x%x\n", spi1nor_read_chipid());
    lib_fs_init();
#endif

    //STEP 4.5: װ��LCD����������
#if (SUPPORT_SPI_LCD_DISPLAY == 1)
    {
        lcd_module_install();

        //lcdӲ���ӿڲ���
#ifdef LCD_HDRV_TEST
        {
            region_t temp_region;
            uint8 i;

            temp_region.y = 4;
            temp_region.x = 0;
            temp_region.width = 128;
            temp_region.height = 24;
            lcd_update_screen(&temp_region);
            sys_mdelay(1000);

            libc_printf("lcd_test start:%d\n", sys_get_ab_timer_us());
            lcd_update_screen(NULL);
            libc_printf("lcd_test over:%d\n", sys_get_ab_timer_us());
            sys_mdelay(1000);

            for (i = 0; i < 16; i++)
            {
                lcd_set_contrast(i);
                sys_mdelay(1000);
            }
            lcd_set_contrast(10);

            lcd_standby_screen(FALSE);
            sys_mdelay(3000);
            lcd_standby_screen(TRUE);
            sys_mdelay(1000);
        }
#endif
    }
#endif

    //STEP 4.6: װ�����RTC����
#if (SUPPORT_EXT_RTC == 1)
    rtc_module_install();
#endif

    //STEP 5��LED��ģ���ʼ��
    if (g_com_comval.support_led_light == 1)
    {
        led_var_init();
    }

    //STEP 6��PIN ����
    //SETP 6.1��SD��PIN ����
    if (g_com_comval.hard_support_card == 1)
    {
        common_card_config();
    }

    //SETP 6.2��USB PIN ����
    if ((g_com_comval.support_uhostplay == 1)
            || (g_com_comval.support_usbbox == 1)
            || (g_com_comval.support_usbmsc == 1))
    {
        common_usb_config();
    }

    //SETP 6.3��DEBUG PIN����
    common_debug_config();

    //STEP 7: ��ʼ��APPLIB����&��Ϣͨ����ر���
    applib_var_init();

    //STEP 8����ʼ����ʱ��
    app_timer_var_init(APP_TIMER_GROUP_MAX);
    app_timer_common_group_init();

    //STEP 9����ʼ��ϵͳ״̬����
    app_state_var_init();

    //STEP 10��KEYTONE������ʼ����Ĭ��ʹ�ܰ�����
    keytone_var_init();

    //STEP 11��TTS������ʼ��
    tts_play_var_init();

    //STEP 12������DAE����
    g_p_com_dae_cfg = (dae_config_t *) sys_malloc(sizeof(dae_config_t));
    if (g_p_com_dae_cfg == NULL)
    {
        libc_printf("p_com_dae_cfg\n");
        while (1)
            ;
    }
    libc_memset(g_p_com_dae_cfg, 0x00, sizeof(dae_config_t));
    com_load_dae_config(g_p_com_dae_cfg);
    com_init_dae_config(g_p_com_dae_cfg);

    //STEP 13��������ʼ�������ʹ�����������ܻ��ڳ�ʼ��ʱ�޸� g_p_com_dae_cfg
    volume_var_init();

    //STEP 14����ʼ��ϵͳ��ر���
    sys_counter_var_init();

    //STEP 15��װ��COMMON��ʱ��������ϵͳ��ض�ʱ��
    sys_timer_init();

    //STEP 16�����ز���ʼ��Ӧ�ù���������
    common_load_app_config();

    //STEP 17���������Ӽ�¼������Ϣ
#if (SUPPORT_TIME_ALARM == 1)
    com_load_rtc_alarm_manager_info();
#endif

    //STEP 18: ��ʼ��HDMI��������Ҫ����2�������ʱ�������Ա����������
#if (SUPPORT_HDMI_MODULE == 1)
    {
        app_callback_t t_app_cbk;
        app_env_var_t t_env_var;

        hdmi_module_install();

        libc_memset(&t_app_cbk, 0x0, sizeof(t_app_cbk));
        t_app_cbk.is_hdmi_cbk = com_hdmi_is_hdmi_func;
        t_app_cbk.is_arc_cbk = com_hdmi_is_arc_func;
        t_app_cbk.get_host_src_cbk = com_hdmi_get_host_src;
        t_app_cbk.get_hdmi_src_cbk = com_hdmi_get_hdmi_src;
        t_app_cbk.get_cur_func_id_cbk = hdmi_get_cur_func_id;
        t_app_cbk.get_cec_mute_cbk = hdmi_get_cec_mute;
        t_app_cbk.get_sound_volume_cbk = hdmi_get_sound_volume;

        libc_memset(&t_env_var, 0x0, sizeof(t_env_var));
        t_env_var.max_volume = CEC_VOLUME_MAX;

        hdmi_startup(&t_app_cbk, &t_env_var);
    }
#endif

#if (SUPPORT_FAST_PREVIEW == 1)
    com_fast_preview_init();
#endif

    return TRUE;
}

int __section__(".my_entry") common_module_init(uint32 para)
{
    return 0;
}

int __section__(".my_entry") common_module_exit(void)
{
    return 0;
}

module_init(common_module_init)
module_exit(common_module_exit)
