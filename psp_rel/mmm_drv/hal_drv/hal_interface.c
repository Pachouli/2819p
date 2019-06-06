/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：HAL驱动API接口表，必须与 hal_interface.h 中的API ID一一对应。
 * 作者：cailizhen
 ********************************************************************************/

#include "hal_inner.h"
#include "./share_code/share_code.h"

const uint32 __section__(".ram_op") hal_ram_op[] =
{
    (uint32) hal_clear_watch_dog,
    (uint32) hal_intc_is_irq_pending_inner,
    (uint32) hal_pmu_enter_low_power_mode_inner,
    (uint32) hal_pmu_exit_low_power_mode_inner,
    (uint32) hal_pmu_read_lradc_inner,
    (uint32) hal_pmu_read_batadc_inner,
    (uint32) hal_pmu_read_onoff_key_inner,
    (uint32) hal_set_gpio_out_level,
    (uint32) hal_get_gpio_in_level,
    (uint32) hal_get_sio_in_level,
};

/*使用kernal的空间*/
/* 定义接口操作集结构体，并进行初始化*/
const uint32 hal_op[] =
{
    //pa
    (uint32) hal_audio_enable_pa,//0
    (uint32) hal_audio_disable_pa,//1
    (uint32) hal_audio_mute_pa,//2

    //audio volume & clock
    (uint32) audio_set_audio_volume,//3
    (uint32) hal_audio_set_audio_aps,//4
    (uint32) hal_audio_get_audio_aps_info, //5

    //dac
    (uint32) hal_audio_enable_dac,//6
    (uint32) hal_audio_disable_dac,//7
    (uint32) hal_audio_set_dac_rate,//8
    (uint32) hal_audio_get_dac_rate,//9
    (uint32) hal_audio_wait_dacfifo_empty,//10

    //pcm buf
    (uint32) hal_audio_reset_pcmbuf,//11
    (uint32) hal_audio_set_pcmbuf,//12
    (uint32) hal_audio_get_pcmbuf_length,//13

    //ain
    (uint32) hal_audio_enable_ain,//14
    (uint32) hal_audio_disable_ain,//15

    //adc
    (uint32) hal_audio_set_adc_gain,//16
    (uint32) hal_audio_enable_adc,//17
    (uint32) hal_audio_disable_adc,//18
    (uint32) hal_audio_is_sim_adc_dual, //19

    //keytone
    (uint32) hal_audio_start_key_tone,//20
    (uint32) hal_audio_get_key_tone_status,//21

    //I2SRx
    (uint32) hal_audio_enable_i2srx,//22
    (uint32) hal_audio_disable_i2srx,//23

    //audio device reserve api
    (uint32) 0, //24
    (uint32) 0, //25
    (uint32) 0, //26
    (uint32) 0, //27
    (uint32) 0, //28
    (uint32) 0, //29

    //pad&sirq&fmclk&pwm reserve api
    (uint32) hal_set_pad_function,//30
    (uint32) hal_ioctl_pad,//31
    (uint32) hal_set_sirq_inner,//32
    (uint32) hal_disable_sirq_inner, //33
    (uint32) hal_set_fm_clock_inner, //34
    (uint32) hal_disable_fm_clock_inner,//35
    (uint32) hal_set_pwm_init,//36
    (uint32) hal_reset_pwm,//37

    //pad&sirq&fmclk&pwm reserve api
    (uint32) 0, //38
    (uint32) 0, //39
    (uint32) 0, //40
    (uint32) 0, //41
    (uint32) 0, //42
    (uint32) 0, //43

    //pmu
    (uint32) hal_pmu_en_wake_up,//44
    (uint32) hal_pmu_power_off,//45
    (uint32) hal_pmu_enable_lradc, //46
    (uint32) hal_pmu_enable_batadc, //47
    (uint32) hal_pmu_enable_lradc1_resistance, //48
    (uint32) 0, //49

    //twi interface
    (uint32) TWI_set_hw_info_sim,//50
    (uint32) hal_TWI_write_bytes,//51
    (uint32) hal_TWI_read_bytes,//52
    (uint32) TWI_master_set_hw_info,//53
    (uint32) hal_TWI_slave_set_hw_info,//54
    (uint32) 0, //55

    //LED interface
    (uint32) hal_led_phy_init, //56
    (uint32) hal_led_phy_exit, //57
    (uint32) hal_led_display,  //58
    (uint32) hal_led_adjust_light,//59
    (uint32) 0, //60
    (uint32) 0, //61

    //IRC interface
    (uint32) hal_irc_phy_init,//62
    (uint32) hal_irc_phy_exit,//63
    (uint32) hal_irc_correct_usercode,//64
    (uint32) 0,//65

    //RTC interface
    (uint32) hal_rtc_phy_init,//66
    (uint32) hal_rtc_set_time,//67
    (uint32) hal_rtc_get_time,//68
    (uint32) hal_rtc_set_date,//69
    (uint32) hal_rtc_get_date,//70
    (uint32) hal_rtc_set_alarm_time,//71
    (uint32) hal_rtc_get_alarm_time,//72
    (uint32) hal_rtc_enable_alarm,//73
    (uint32) hal_rtc_disable_alarm,//74
    (uint32) hal_rtc_get_status,//75
    (uint32) hal_rtc_bakdata_write,//76
    (uint32) hal_rtc_bakdata_read,//77
    (uint32) 0,//78

    //SPI interface
    (uint32) hal_spi1_trans_voer,//79
    (uint32) hal_spi1_init,//80
    (uint32) hal_spi1_read_data,//81
    (uint32) hal_spi1_write_data,//82
    (uint32) hal_spi1_set_cs,//83
    //SPINOR
    (uint32) hal_spi1nor_init,//84
    (uint32) hal_spi1nor_read_chipid,//85
    (uint32) hal_spi1nor_erase,//86
    (uint32) hal_spi1nor_read,//87
    (uint32) hal_spi1nor_write,//88
    (uint32) 0,//89

    //card hardware interface
    (uint32) hal_sdmmc_app_detect_with_mutex,//90
    (uint32) hal_sdmmc_app_phy_configure,//91
    (uint32) hal_sdmmc_app_reset_pad,//92
    (uint32) audio_device_set_card_pm_param,//93
    (uint32) hal_sdmmc_set_clk,//94
    (uint32) hal_sdmmc_app_select,//95
    (uint32) hal_sdmmc_app_deselect,//96
    (uint32) hal_sdmmc_app_start_phy_rw,//97
    (uint32) hal_sdmmc_app_wait_phy_rw_cmp,//98
    (uint32) hal_sdmmc_app_stop_transmission,//99
    (uint32) hal_sdmmc_app_get_status,//100
    (uint32) hal_sdmmc_get_cap,//101
    (uint32) 0,//102
    (uint32) 0,//103
    (uint32) 0,//104
    (uint32) 0,//105

    //usb device hardware interface
    (uint32) hal_usb_device_uhost_detect_switch_inner,//106
    (uint32) hal_set_usb_detect_inner,//107
    (uint32) hal_usb_detect_inner,//108
    (uint32) audio_usb_hardware_init,//109
    (uint32) audio_usb_hardware_exit,//110
    (uint32) hal_usb_set_in_ep,//111
    (uint32) hal_usb_set_out_ep,//112
    (uint32) hal_usb_rest_ep_toggle,//113
    (uint32) hal_usb_stall_ep,//114
    (uint32) hal_usb_clear_stall_ep,//115
    (uint32) hal_usb_send_ctrl_data,//116
    (uint32) hal_usb_is_cable_plug, //117
    (uint32) hal_get_usb_setup_data,//118
    (uint32) hal_usb_to_adfu,//119
    (uint32) hal_usb_connect_state,//120
    (uint32) hal_usb_hid_pro,//121
    (uint32) hal_usb_get_short_pk_data,//122
    (uint32) hal_usb_nak_ctrl, //123
    (uint32) hal_usb_recv_ctrl_data, //124
    (uint32) hal_usb_receive_data, //125
    (uint32) hal_usb_send_data, //126
    (uint32) 0,//127

    //uhost hardware interface
    (uint32) hal_uhost_detect_inner,//128
    (uint32) audio_device_host_bus_reset,//129
    (uint32) audio_device_host_ep_reset,//130
    (uint32) audio_device_host_controller_enable,//131
    (uint32) audio_device_host_controller_disable, //132
    (uint32) audio_device_host_clear_eperrorirq,//133
    (uint32) audio_device_host_clear_feature_ep_stall,//134
    (uint32) audio_device_host_setup_stage_transaction,//135
    (uint32) audio_device_host_control_in_data,//136
    (uint32) audio_device_host_status_stage_transaction_out,//137
    (uint32) audio_device_host_status_stage_transaction_in,//138
    (uint32) audio_device_host_set_endpoint,//139
    (uint32) audio_device_host_bulk_data,//140
    (uint32) hal_get_uhost_in_type, //141
    (uint32) hal_host_set_function_address,//142
    (uint32) hal_host_set_max_packet,//143
    (uint32) 0,//144
    (uint32) 0,//145
    (uint32) 0,//146

    //hardware set & get
    (uint32) hal_hardware_set,//147
    (uint32) hal_hardware_get,//148

    //uart interface, for print etc
    (uint32) hal_set_uart_tx_init,//149
    (uint32) hal_uart_send_bytes,//150
    (uint32) hal_set_uart_rx_init,//151
    (uint32) uart_recv_bytes_inner,//152
    (uint32) 0,//153

    //lib fs interface
    (uint32) hal_lib_read_data_inner,//154
    (uint32) hal_lib_fs_init_inner,//155
    (uint32) hal_lib_fopen_inner,//156
    (uint32) hal_lib_fclose_inner,//157
    (uint32) hal_lib_fseek_inner,//158
    (uint32) hal_lib_ftell_inner,//159
    (uint32) hal_lib_fread_inner,//160

    //hdmi communication interface  NOTE!!:HDMI API FIX TO 161 & 162
#if (SUPPORT_HDMI_MODULE == 1)
    (uint32) hal_iic_init,//161
    (uint32) hal_iic_sync,//162
#else
    (uint32) /*hal_iic_init*/ 0,//161
    (uint32) /*hal_iic_sync*/ 0,//162
#endif

    //share code api
    (uint32) update_PP_para_info_inner,//163
    (uint32) buffer_rw_write_inner,//164
    (uint32) buffer_rw_read_inner,//165
    (uint32) buffer_rw_update_inner,//166
    (uint32) buffer_rw_flush_inner,//167
    (uint32) energy_calc_inner,//168

    //other api

    //NOTE: HAL驱动API接口最多200个，如果超出需要修改PSP AFI.bin
};

int32 audio_device_init(void)
{
    g_p_sys_hal_cbks = sys_get_sys_hal_cbk();
    p_sys_global_info = sys_get_system_info(GET_SYS_GLOBAL_DATA_INFO);
    hal_timer_init();
    return 0;
}
int32 audio_device_exit(void)
{
    hal_timer_deinit();
    return 0;
}
#ifndef PC
module_init (audio_device_init)
module_exit(audio_device_exit)
#endif
