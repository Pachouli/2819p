/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：开机/关机应用入口，开机流程，关机流程；要考虑ESD重启的情况。
 * 作者：cailizhen
 ********************************************************************************/

#include "ap_config.h"
#include "fm_interface.h"

/*全局变量定义*/

//config应用变量
config_var_t g_config_var;
sys_poweron_info_t g_config_poweron_info;

/*main中要填充这个值，由系统人员分配*/
INT8U prio = AP_FRONT_LOW_PRIO;

/******************************************************************************/
/*!
 * \par  Description:
 *  读取应用VM全局变量,初始化全局变量
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
static void _read_var(void)
{
    bool first_boot = FALSE; //是否升级固件后第一次上电标志

    //初始化config的VM变量
    if (sys_vm_read(&g_config_var, VM_AP_CONFIG, sizeof(g_config_var)) < 0)
    {
        g_config_var.magic = 0xffff;
    }
    if (g_config_var.magic != VRAM_MAGIC(VM_AP_CONFIG))
    {
        first_boot = TRUE;
        g_config_var.magic = VRAM_MAGIC(VM_AP_CONFIG);
        g_config_var.ap_id = APP_ID_BTPLAY;
        sys_vm_write(&g_config_var, VM_AP_CONFIG, sizeof(g_config_var));
    }

    //升级固件后第一次上电
    if (first_boot == TRUE)
    {
        //清除ESD RESET flag
        g_p_esd_bk_info->flag = 0;
    }
}

static void _save_var(void)
{
    com_save_sys_comval();
    sys_vm_write(&g_config_var, VM_AP_CONFIG, sizeof(g_config_var));
}

/******************************************************************************/
/*!
 * \par  Description:
 *  applib初始化
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
bool _app_init(void)
{
    //初始化applib库，前台AP
    applib_init(APP_ID_CONFIG, THIS_APP_TYPE);

    //初始化软定时器
    init_app_timers(THIS_APP_TIMER_GROUP, 2);

    return TRUE;
}


//设置一些由客户配置的硬件参数，包括手机充电电流等，通过HARDWARE_SET接口设置，
//保存在HAL驱动中，其他模块可通过HAL驱动的HARDWARE_ GET接口获取到
void config_hardware_set(void)
{
    //设置手机充电电流
    hardware_set(HAL_SET_PHONE_CHARGING_CURRENT, (void*) (uint32) com_get_config_default(PHONE_CHARGING_CURRENT));
}

//VRAM自动化测试程序
void vram_autotest(void)
{
#if 0
    uint8 *p_buffer = sys_malloc(512);
    uint8 write_dat;
    uint32 VM_AP_TEST1 = 0x00180000;
    uint32 VM_AP_TEST2 = 0x00190000;
    uint32 VM_AP_TEST3 = 0x001a0000;
    uint32 VM_AP_TEST4 = 0x001b0000;

    libc_printf_info("VRAM TEST START...\n");

    for (write_dat = 1; write_dat < 201; write_dat++)
    {
        libc_memset(p_buffer, write_dat, 512);

        com_vram_merge();

        if (sys_vm_write(p_buffer, VM_AP_TEST1, 512) != 0)
        {
            libc_printf_error("VM_AP_TEST1 write fail!");
        }
        if (sys_vm_write(p_buffer, VM_AP_TEST2, 512) != 0)
        {
            libc_printf_error("VM_AP_TEST2 write fail!");
        }
        if (sys_vm_write(p_buffer, VM_AP_TEST3, 512) != 0)
        {
            libc_printf_error("VM_AP_TEST3 write fail!");
        }
        if (sys_vm_write(p_buffer, VM_AP_TEST4, 512) != 0)
        {
            libc_printf_error("VM_AP_TEST4 write fail!");
        }

        if (sys_vm_read(p_buffer, VM_AP_TEST1, 512) < 0)
        {
            libc_printhex(p_buffer, 64, 0);
            libc_printf_error("VM_AP_TEST1 read fail!");
            while (1);
        }
        else
        {
            int i;
            for (i = 0; i < 512; i++)
            {
                if (p_buffer[i] != write_dat)
                {
                    libc_printf_error("VM_AP_TEST1 data wrong! %x->%x", write_dat, p_buffer[i]);
                    while (1);
                }
            }
        }

        if (sys_vm_read(p_buffer, VM_AP_TEST2, 512) < 0)
        {
            libc_printf_error("VM_AP_TEST1 read fail!");
            while (1);
        }
        else
        {
            int i;
            for (i = 0; i < 512; i++)
            {
                if (p_buffer[i] != write_dat)
                {
                    libc_printf_error("VM_AP_TEST1 data wrong! %x->%x", write_dat, p_buffer[i]);
                    while (1);
                }
            }
        }

        if (sys_vm_read(p_buffer, VM_AP_TEST3, 512) < 0)
        {
            libc_printf_error("VM_AP_TEST1 read fail!");
            while (1);
        }
        else
        {
            int i;
            for (i = 0; i < 512; i++)
            {
                if (p_buffer[i] != write_dat)
                {
                    libc_printf_error("VM_AP_TEST1 data wrong! %x->%x", write_dat, p_buffer[i]);
                    while (1);
                }
            }
        }

        if (sys_vm_read(p_buffer, VM_AP_TEST4, 512) < 0)
        {
            libc_printf_error("VM_AP_TEST1 read fail!");
            while (1);
        }
        else
        {
            int i;
            for (i = 0; i < 512; i++)
            {
                if (p_buffer[i] != write_dat)
                {
                    libc_printf_error("VM_AP_TEST1 data wrong! %x->%x", write_dat, p_buffer[i]);
                    while (1);
                }
            }
        }
    }

    sys_free(p_buffer);

    libc_printf_info("VRAM TEST OK!\n");
#endif
}

//lcd驱动自动化测试程序
void lcd_autotest(void)
{
#if 0
    region_t str_region;
    uint8 i;

    lcd_show_image(0, 0, LOGO);
    lcd_update_screen(NULL);
    sys_mdelay(1000);

    lcd_clear_screen(NULL);
    lcd_update_screen(NULL);
    sys_mdelay(1000);

    str_region.x = 32;
    str_region.y = 16;
    str_region.width = 96;
    str_region.height = 16;
    for (i = 0; i < 4; i++)
    {
        lcd_clear_screen(&str_region);
        lcd_show_multi_string(32, 16, SLAN1 + i);
        lcd_update_screen(NULL);
        sys_mdelay(1000);
    }
#endif
}

//磁盘格式化自动化测试程序
void disk_format_autotest(void)
{
#if 0
    static const uint8 card_drv_name[] = "card.drv";
    static const uint8 uhost_drv_name[] = "uhost.drv";

    //format disk test -- sd card
    if (sys_drv_install(DRV_GROUP_STORAGE, SLOW_DRIVER, card_drv_name) >= 0)
    {
        sys_format_disk(STG_CARD, 0, FORMAT_FAT32);
        sys_drv_uninstall(DRV_GROUP_STORAGE);
    }

    //format disk test -- uhost
    if (sys_drv_install(DRV_GROUP_STORAGE, 0, uhost_drv_name) >= 0)
    {
        sys_format_disk(STG_UHOST, 0, FORMAT_FAT32);
        sys_drv_uninstall(DRV_GROUP_STORAGE);
    }
#endif
}

/******************************************************************************/
/*!
 * \par  Description:
 *    config应用的入口函数
 * \param[in]    0-开机，其他值：关机，低16bit代表关机时的前台应用ID，高16bit表示后台引擎的状态
 * \param[out]   none
 * \return       the result
 * \retval
 * \retval
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
int main(int argc, const void *argv[])
{
    app_result_e result = RESULT_NULL;
    uint8 config_enter_mode = (uint8) (uint32) argv[0];

    libc_printf_info("Enter Config AP : %d, @%d\n", config_enter_mode, sys_get_ab_timer());

    //应用变量初始化
    _read_var();

    //applib initial
    _app_init();

    //开机
    if (config_enter_mode == CONFIG_MODE_POWER_ON)
    {
        //开机时电量LED灯
        com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

        //开机LED数码管显示 ----
        led_display(NUMBER1, '-', TRUE);
        led_display(NUMBER2, '-', TRUE);
        led_display(NUMBER3, '-', TRUE);
        led_display(NUMBER4, '-', TRUE);

        //VRAM自动化测试程序，客户应该尽量使用vram merge时间较短的NOR，比如40ms左右(4KB Page Merge)
        vram_autotest();

#if (SUPPORT_SPI_LCD_DISPLAY == 1)
        //点阵屏逻辑驱动测试
        lcd_autotest();
        //磁盘格式化自动化测试程序
        disk_format_autotest();
#endif

        //开机配置一些硬件参数
        config_hardware_set();

        sys_get_poweron_info(&g_config_poweron_info);
        libc_printf("wakeup:0x%08x, 0x%08x\n", g_config_poweron_info.wake_pd, *(uint32 *)(&(g_config_poweron_info.wakeup_para)));

        //确保FM进入Standby状态，以降低功耗
        common_fmdrv_standby(FM_STANDBY_MODE);

        pa_thread_open(NULL);

        //开机阶段不要播放按键音
        keytone_set_on_off (FALSE);
        result = config_poweron_option();
        keytone_set_on_off (TRUE);

        libc_printf_info("Config poweron option:0x%x, @%d\n", result, sys_get_ab_timer());

        poweron_option_dispatch:

        config_poweron_dispatch(result);
    }
    else //以下是关机操作
    {
        g_config_var.ap_id = get_last_app_id();

        //闹钟响应完自动关机
        if (config_enter_mode == CONFIG_MODE_ALARM_OFF)
        {
            //无须任何TTS播报
        }
        //低功耗模式电量不足直接关机，不播报TTS
        else if (config_enter_mode == CONFIG_MODE_JUST_OFF)
        {
            //无须任何TTS播报
        }
        //低功耗模式USB唤醒进入空闲充电模式，不需要播报TTS
        else if (config_enter_mode == CONFIG_MODE_CHARGE_LOOP)
        {
            //无须任何TTS播报
        }
        else
        {
            //低电关机
            if (config_enter_mode == CONFIG_MODE_LOWPOWER)
            {
                //电量不足
                com_tts_state_play(TTS_BATTERY_LOW, TTS_MODE_KEY_NORECV | TTS_MODE_SYS_NORECV);
                sys_os_time_dly(50);
            }

            /*播报关机*/
            com_tts_state_play(TTS_POWEROFF, TTS_MODE_KEY_NORECV | TTS_MODE_SYS_NORECV);
        }

        if (get_bt_state() == BT_STATE_WORK)
        {
            //如果不支持后台蓝牙，所有非蓝牙应用都与 bt enging 冲突，需要杀死 bt engine
            if (com_get_config_default(SETTING_APP_SUPPORT_BT_INBACK) == 0)
            {
                com_close_bt_engine();
            }
        }

        //带USB线关机可进入充电空闲模式，以支持充电指示灯控制
        if ((get_cable_state() == CABLE_STATE_CABLE_IN) || (get_adaptor_state() == ADAPTOR_STATE_CABLE_IN))
        {
            if (com_get_config_default(SETTING_APP_SUPPORT_CHARGE_LOOP) == 1)
            {
                result = config_charge_loop(CHARGE_MODE_OFF);
                if ((result != RESULT_LOW_POWER) && (result != RESULT_POWER_OFF))
                {
                    goto poweron_option_dispatch;
                }
            }
        }

        //备份VRAM
        _save_var();

        config_power_off(); //断电关机，不再返回
    }

#if (SUPPORT_FAST_PREVIEW == 1)
    key_set_fast_preview_cbk(COM_COM_FAST_PREVIEW_CBK); //com_fast_preview_cbk
#endif

    com_led_light_off(LED_ID_STATUS, LED_SCENE_MAIN);

    //备份VRAM
    _save_var();

    //执行applib库的注销操作
    applib_quit(THIS_APP_TYPE);

    libc_printf_info("Config AP quit @%d\n", sys_get_ab_timer());

    return RESULT_NULL;
}
