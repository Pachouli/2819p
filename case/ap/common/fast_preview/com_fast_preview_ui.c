/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：快速预览模式UI部分实现。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_ui_inner.h"

#if (SUPPORT_FAST_PREVIEW == 1)

typedef struct
{
    uint16 magic;
    uint8 station_num;
    uint8 reserve;
    uint32 freq;
} radio_vram_info_t;

typedef struct
{
    /* 快速预览信息，注意这几个变量位置要保持与 com_fast_preview_ui.c 中的相对地址一致，上电后会保存到 fast_preview_info_t 里 */
    uint8 radio_station_num; //0和0xff表示不是有效电台，1-40表示有效电台号，对应VM_AP_RADIO[Byte 3]
    uint32 radio_freq; //频点信息，0表示未初始化，会显示为87.5，对应VM_AP_RADIO[Byte 4-7]
} fast_preview_ui_info_t;

static fast_preview_ui_info_t g_fast_preview_ui_info;

static void bt_show_welcome_ui(void)
{
    com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

    led_clear_screen(CLR_ALL);
    led_display(NUMBER2, 'b', TRUE);
    led_display(NUMBER3, 'L', TRUE);
}

//disk_index: 0-SD卡,1-Uhost,2-NOR
static void music_show_welcome_ui(uint8 disk_index)
{
    com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

    led_clear_screen(CLR_ALL);

    if (disk_index == 0)
    {
        led_display_icon(LCD_SD, TRUE);
    }
    else if (disk_index == 1)
    {
        led_display_icon(LCD_USB, TRUE);
    }

    //显示4个0
    led_display(NUMBER1, 0, TRUE);
    led_display(NUMBER2, 0, TRUE);
    led_display(NUMBER3, 0, TRUE);
    led_display(NUMBER4, 0, TRUE);
    led_display_icon(LCD_PAUSE, TRUE);
}

static void linein_show_welcome_ui(void)
{
    led_clear_screen(CLR_ALL);
    led_display(NUMBER1, 'L', TRUE);
    led_display(NUMBER2, 'I', TRUE);
    led_display(NUMBER3, 'N', TRUE);
    led_display(NUMBER4, 'E', TRUE);
    led_display_icon(LCD_AUX, TRUE);
    led_display_icon(LCD_PAUSE, TRUE);
}

static void spdif_show_welcome_ui(void)
{
    led_clear_screen(CLR_ALL);
    led_display(NUMBER1, 'S', TRUE);
    led_display(NUMBER2, 'P', TRUE);
    led_display(NUMBER3, 'D', TRUE);
    led_display(NUMBER4, 'F', TRUE);
    led_display_icon(LCD_PAUSE, TRUE);
}

static void hdmi_show_welcome_ui(void)
{
    led_clear_screen(CLR_ALL);
    led_display(NUMBER1, 'H', TRUE);
    led_display(NUMBER2, 'D', TRUE);
    led_display(NUMBER3, 'N', TRUE);
    led_display(NUMBER4, 'I', TRUE);
    
    led_display_icon(LCD_PAUSE, TRUE);
}

static void arc_show_welcome_ui(void)
{
    led_clear_screen(CLR_ALL);
    led_display(NUMBER1, 0, FALSE);
    led_display(NUMBER2, 'A', TRUE);
    led_display(NUMBER3, 'R', TRUE);
    led_display(NUMBER4, 'C', TRUE);
    
    led_display_icon(LCD_PAUSE, TRUE);
}

static void radio_show_welcome_ui(uint8 station_num, uint32 freq)
{
    com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

    led_clear_screen(CLR_ALL);
    led_display_icon(LCD_FM, TRUE);
    led_display_icon(LCD_PAUSE, FALSE);

    if ((station_num != 0) && (station_num != 0xff))
    {
        //CH
        led_display(NUMBER1, 'C', TRUE);
        led_display(NUMBER2, 'H', TRUE);
        //电台号
        led_display(NUMBER3, station_num/10, TRUE);
        led_display(NUMBER4, station_num%10, TRUE);
    }
    else
    {
        if (freq >= 100000)
        {
            led_display(NUMBER1, 1, TRUE);
        }
        else
        {
            led_display(NUMBER1, 0, FALSE);
        }
        freq = freq % 100000;
        led_display(NUMBER2, freq/10000, TRUE);
        freq = freq % 10000;
        led_display(NUMBER3, freq/1000, TRUE);
        led_display_icon(LCD_P1, TRUE);
        freq = freq % 1000;
        led_display(NUMBER4, freq/100, TRUE);
    }
}

static void usound_show_welcome_ui(void)
{
    com_led_light_on(LED_ID_STATUS, LED_SCENE_MAIN);

    //LED数码管显示
    led_clear_screen(CLR_ALL);
    led_display(NUMBER1, 'U', TRUE);
    led_display(NUMBER2, 'S', TRUE);
    led_display(NUMBER3, 'B', TRUE);
}

void com_fast_preview_show_ap_welcome_ui(uint8 func_id)
{
    com_led_force_exit_msg_show();

    switch (func_id)
    {
    case APP_FUNC_BTPLAY:
        bt_show_welcome_ui();
        break;

    case APP_FUNC_PLAYCARD:
        music_show_welcome_ui(0);
        break;

    case APP_FUNC_PLAYUHOST:
        music_show_welcome_ui(1);
        break;

    case APP_FUNC_PLAYMDISK:
        music_show_welcome_ui(2);
        break;

    case APP_FUNC_PLAYLINEIN:
        linein_show_welcome_ui();
        break;

    case APP_FUNC_SPDIF_PLAY:
        spdif_show_welcome_ui();
        break;

    case APP_FUNC_HDMI_HDMI:
        hdmi_show_welcome_ui();
        break;

    case APP_FUNC_HDMI_ARC:
        arc_show_welcome_ui();
        break;

    case APP_FUNC_RADIO:
        radio_show_welcome_ui(g_fast_preview_ui_info.radio_station_num, g_fast_preview_ui_info.radio_freq);
        break;

    case APP_FUNC_USOUND:
        usound_show_welcome_ui();
        break;

    default:
        break;
    }
}

void com_fast_preview_load_ap_welcome_ui(void)
{
    radio_vram_info_t radio_vram_info;

    if (sys_vm_read(&radio_vram_info, VM_AP_RADIO, sizeof(radio_vram_info_t)) >= 0)
    {
        com_fast_preview_update_ap_welcome_ui_inner(APP_FUNC_RADIO, &radio_vram_info);
    }
    else
    {
        g_fast_preview_ui_info.radio_station_num = 0;
        g_fast_preview_ui_info.radio_freq = 87500;
    }
}

void com_fast_preview_update_ap_welcome_ui_inner(uint8 func_id, void *para)
{
    if (func_id == APP_FUNC_RADIO)
    {
        radio_vram_info_t *p_radio_vram_info = para;

        g_fast_preview_ui_info.radio_station_num = p_radio_vram_info->station_num;
        g_fast_preview_ui_info.radio_freq = p_radio_vram_info->freq;
    }
}

#endif
