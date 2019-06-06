/*!
 * \file      upgrade_app_main.c
 * \brief     升级应用主体程序
 * \details   
 * \author    Tommy Wang
 * \date      
 * \copyright Actions
 */

#include "upgrade_app.h"
#include "upgrade_seg_led.h"


/* 应用任务优先级定义
 */
INT8U  prio = AP_FRONT_LOW_PRIO;


/* 应用全局数据结构
 */
upgrade_app_context_t  upgrade_app_context;


u32_t checksum_32(void* buf, uint_t len)
{
    u32_t   sum = 0;
    uint_t  i;

    for (i = 0; i < (len / sizeof(u32_t)); i++)
    {
        sum += ((u32_t*)buf)[i];
    }

    return sum;
}


#if (SUPPORT_LED_DISPLAY == 1)

void upgrade_seg_led_display(int percent)
{
    upgrade_app_context_t*  upg = upgrade_app_get_context();
    
    u8_t  disp_data[5];

    /* 开始升级
     */
    if (percent == 0)
    {
        disp_data[0] = SEG_LED_CHAR_U;
        disp_data[1] = SEG_LED_CHAR_P;
        disp_data[2] = SEG_LED_CHAR_G;
        disp_data[3] = SEG_LED_CHAR_D;
    }
    /* 升级中
     */
    else if (percent > 0 && percent < 100)
    {
        disp_data[0] = SEG_LED_CHAR_U;
        disp_data[1] = 0;
        disp_data[2] = seg_led_number[percent / 10];
        disp_data[3] = seg_led_number[percent % 10];
    }
    /* 升级成功
     */
    else if (percent == 100)
    {
        disp_data[0] = SEG_LED_CHAR_S;
        disp_data[1] = SEG_LED_CHAR_U;
        disp_data[2] = SEG_LED_CHAR_C;
        disp_data[3] = SEG_LED_CHAR_C;
    }
    /* 升级失败
     */
    else if (percent < 0)
    {
        disp_data[0] = SEG_LED_CHAR_F;
        disp_data[1] = SEG_LED_CHAR_A;
        disp_data[2] = SEG_LED_CHAR_I;
        disp_data[3] = SEG_LED_CHAR_L;
    }

    disp_data[4] = 0;

    if (upg->upg_params.upg_disk == DISK_H)
    {
        disp_data[4] = SEG_LED_ICON_SD;
    }
    else if (upg->upg_params.upg_disk == DISK_U)
    {
        disp_data[4] = SEG_LED_ICON_USB;
    }

    seg_led_display(disp_data);
}
#endif


#if (SUPPORT_LED_LIGHT == 1)

void upgrade_led_light_display(int percent)
{
    bool  on_off = FALSE;
    
    /* 开始升级
     */
    if (percent == 0)
    {
        on_off = TRUE;
    }
    /* 升级中
     */
    else if (percent > 0 && percent < 100)
    {
        static bool  state = 1;

        state ^= 1;
        on_off = state;
    }
    /* 升级成功
     */
    else if (percent == 100)
    {
        on_off = TRUE;
    }
    /* 升级失败
     */
    else if (percent < 0)
    {
        on_off = FALSE;
    }

    led_light_ctrl(on_off);
}
#endif


#if (SUPPORT_HW_KEYTONE == 1)

void upgrade_keytone_play(int percent)
{
    upgrade_app_context_t*  upg = upgrade_app_get_context();
    
    /* 升级成功
     */
    if (percent == 100)
    {
        sys_enable_reboot((upg->upg_mode == UPGRADE_MODE_FACTORY) ? FALSE : TRUE);

        while (1)
        {
            keytone_play(KT_FREQ_500HZ, KT_PERIOD_80);

            ROM_SYS_MDELAY(500);
        }
    }
    /* 升级失败
     */
    else if (percent < 0)
    {
        sys_enable_reboot((upg->upg_mode == UPGRADE_MODE_FACTORY) ? FALSE : TRUE);

        while (1)
        {
            keytone_play(KT_FREQ_1000HZ, KT_PERIOD_48);

            ROM_SYS_MDELAY(100);
        }
    }
}
#endif


/* 升级进度显示回调函数
 *
 * 注意: 升级过程涉及 Flash 代码擦写, 该函数内只能调用常驻内存代码以及 upgrade_api 接口函数!
 */
void upgrade_show_progress(int percent)
{
    upgrade_app_context_t*  upg = upgrade_app_get_context();
    
    upgrade_api_t*  api = &upg->upg_params.api;

    if (api->printf != NULL)
    {
        api->printf("<I> %s, %d\n", __FUNCTION__, percent);
    }

    #if (SUPPORT_LED_DISPLAY == 1)
    {
        upgrade_seg_led_display(percent);
    }
    #endif
    
    #if (SUPPORT_LED_LIGHT == 1)
    {
        upgrade_led_light_display(percent);
    }
    #endif
    
    #if (SUPPORT_HW_KEYTONE == 1)
    {
        upgrade_keytone_play(percent);
    }
    #endif
    
    /* 升级失败?
     */
    if (percent < 0)
    {
        /* 厂商升级模式不重启?
         */
        sys_enable_reboot((upg->upg_mode == UPGRADE_MODE_FACTORY) ? FALSE : TRUE);

        while (1);
    }
}


void upgrade_load_and_run(uint8 disk)
{
    upgrade_app_context_t*  upg = upgrade_app_get_context();

    upgrade_params_t*  params = &upg->upg_params;
    upgrade_header_t   hdr;
    
    log_info("0x%x", disk);

    sys_os_sched_lock();

    /* 打开升级文件
     */
    if (upgrade_file_open(disk) != _OK)
        goto err;

    if (upgrade_file_read(&hdr, 0, sizeof(hdr)) != _OK)
        goto err;

    libc_printhex((void*)&hdr, sizeof(hdr), 0);

    if (libc_memcmp(hdr.format, "UPG", 4) != 0)
        goto err;

    /* 加载升级驱动程序
     */
    if (upgrade_file_read((void*)hdr.load_addr, 0, hdr.load_size) != _OK)
        goto err;

    com_file_close(0);

    if (checksum_32((void*)(hdr.load_addr + 4), hdr.load_size - 4) != hdr.checksum)
        goto err;

    ROM_SYS_MDELAY(100);

    /* 禁止中断
     */
    sys_local_irq_save();

    /* 配置参数
     */
    params->cfg_uart.ctrl_no    = PRINT_UART_TX_IDX;
    params->cfg_uart.tx_gpio_no = PRINT_UART_TX_PIN  - PAD_GPIO0;
    params->cfg_uart.tx_mfp_sel = PRINT_UART_TX_FUNC - PAD_FUNC_MFP;
    params->cfg_uart.baudrate   = 115200;

    params->upg_name = upg->upg_name;
    params->upg_mode = upg->upg_mode;
    params->upg_disk = disk;
    
    params->show_progress = upgrade_show_progress;

    /* 执行升级驱动程序
     */
    ((void (*)(void*))hdr.main_func)(params);

err:
    log_error("FAIL");

    upgrade_show_progress(-1);
}


/* 应用主体程序入口
 */
int main(int argc, const char* argv[])
{
    u8_t  disk = (u8_t)(u32_t)argv[0];
    
    g_p_esd_bk_info->flag = 0;
    
    #if (SUPPORT_HW_KEYTONE == 1)
    {
        com_speaker_on(SPK_PLAY);
    }
    #endif
    
    upgrade_load_and_run(disk);

    return 0;
}


