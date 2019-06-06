/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：COMMON API 接口表定义，注意需要与 common_api_def.h 的 API ID 一一对应。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_func_inner.h"

/*2.定义接口操作集结构体，并进行初始化*/
const uint32 common_op[] =
{
    //APPLIB
    (uint32) applib_init_inner,
    (uint32) applib_quit_inner,
    (uint32) applib_get_app_info_inner,
    (uint32) get_engine_type_by_appid_inner,
    (uint32) get_engine_appid_by_type_inner,
    (uint32) get_engine_type_inner,
    (uint32) get_bt_state_inner,
    (uint32) get_tws_mode_inner,
    (uint32) change_tws_mode_inner,
    (uint32) change_engine_state_inner,
    (uint32) change_app_state_inner,
    (uint32) get_last_app_id_inner,
    (uint32) get_cur_app_id_inner,
    (uint32) com_vram_merge_inner,
    //APP TIMER
    (uint32) set_app_timer_inner,
    (uint32) set_single_shot_app_timer_inner,
    (uint32) modify_app_timer_inner,
    (uint32) stop_app_timer_inner,
    (uint32) restart_app_timer_inner,
    (uint32) standby_restart_all_app_timer_inner,
    (uint32) kill_app_timer_inner,
    (uint32) init_app_timers_inner,
    (uint32) change_app_timer_tag_inner,
    //APP MSG
    (uint32) get_app_msg_inner,
    (uint32) message_redeal_app_msg_inner,
    (uint32) send_sync_msg_inner,
    (uint32) send_async_msg_inner,
    (uint32) broadcast_msg_inner,
    (uint32) reply_sync_msg_inner,
    (uint32) set_app_msg_dispatch_handle_inner,
    //FW CONFIG
    (uint32) com_get_config_struct_inner,
    (uint32) com_get_config_default_inner,


    //UI INTERFACE - KEY
    (uint32) get_gui_msg_inner,
    (uint32) message_redeal_gui_msg_inner,
    (uint32) com_double_click_enable_inner,
    (uint32) com_key_mapping_inner,
    (uint32) com_filter_key_hold_inner,
    (uint32) com_filter_key_up_inner,
    (uint32) com_filter_key_period_of_time_inner,
    (uint32) com_get_long_key_time_inner,
    (uint32) com_start_key_tone_inner,
    //UI INTERFACE - KEYTONE
    (uint32) keytone_set_on_off_inner,
    (uint32) keytone_play_inner,
    //UI INTERFACE - TTS
    (uint32) com_tts_state_play_inner,
    (uint32) com_tts_state_play_list_inner,
    (uint32) com_tts_state_play_ext_inner,
    (uint32) com_tts_state_play_control_inner,
    (uint32) com_sd_mp3_player_play_tts_inner,
    (uint32) com_sd_mp3_player_play_mp3_inner,
    (uint32) com_sd_mp3_player_stop_inner,
    (uint32) com_sd_mp3_player_is_stop_inner,
    //UI INTERFACE - SOUND
    (uint32) com_get_sound_volume_inner,
    (uint32) com_get_sound_volume_pc_inner,
    (uint32) com_reset_sound_volume_inner,
    (uint32) com_set_sound_volume_inner,
    (uint32) com_set_sound_volume_mode_inner,
    (uint32) com_set_sound_volume_gain_inner,
    (uint32) com_volume_add_inner,
    (uint32) com_volume_sub_inner,
    (uint32) com_volume_set_inner,
    (uint32) com_set_mute_inner,
    (uint32) com_set_mute_toggle_inner,
    (uint32) com_set_usbbox_volume_ctrl_inner,
    (uint32) com_switch_geq_inner,
    (uint32) is_speaker_on_inner,
    (uint32) com_speaker_on_inner,
    (uint32) com_speaker_off_inner,
    (uint32) com_speaker_ab_d_switch_inner,
    (uint32) com_set_dae_onoff_inner,
    (uint32) com_set_drc_onoff_inner,
    (uint32) com_switch_drc_mode_inner,
    //UI INTERFACE - LED
    (uint32) com_led_twinkle_init_inner,
    (uint32) com_led_light_on_inner,
    (uint32) com_led_light_off_inner,
    (uint32) com_led_light_dummy_inner,
    (uint32) com_led_breath_inner,
    //UI INTERFACE - LED SEGMENT
    (uint32) com_led_display,
    (uint32) com_led_display_icon,
    (uint32) com_led_clear_screen,
    (uint32) com_led_flash_screen,
    (uint32) com_led_enter_msg_show,
    (uint32) com_led_force_exit_msg_show,
    (uint32) com_led_twinkle_icon,
    //UI INTERFACE - OTHERS
    (uint32) com_message_box_inner,
    (uint32) sys_hot_detect_handle_inner,
    (uint32) com_save_sys_comval_inner,
    (uint32) com_restore_sys_comval_by_reset_inner,
    (uint32) com_aux_pa_init_inner,


    //ASET
    (uint32) aset_test_loop_deal_inner,
    (uint32) aset_switch_aux_mode_inner,


    //COMMON FUNC - AP SWITCH
    (uint32) com_ap_switch_deal_inner,
    (uint32) com_ap_switch_ask_next_func_inner,
    (uint32) com_ap_switch_check_inner,
    //COMMON FUNC - STUB
    (uint32) com_stub_tools_connect_inner,
    (uint32) com_get_stub_tools_type_inner,
    (uint32) com_stub_ack_ext_cmd_inner,
    (uint32) com_stub_read_ext_cmd_inner,
    (uint32) com_stub_write_ext_cmd_inner,
    (uint32) com_stub_read_cmd_inner,
    (uint32) com_stub_write_cmd_inner,
    //COMMON FUNC - FS FILE
    (uint32) com_file_open_inner,
    (uint32) com_file_close_inner,
    (uint32) com_file_read_inner,
    (uint32) com_file_write_inner,
    (uint32) com_file_seek_inner,
    (uint32) com_file_tell_inner,
    //COMMON FUNC - OTHERS
    (uint32) set_need_update_vm_inner,
    (uint32) get_need_update_vm_inner,
    (uint32) set_ic_pa_inited_inner,
    (uint32) get_ic_pa_inited_inner,
    (uint32) set_enter_dut_test_inner,
    (uint32) get_enter_dut_test_inner,
    (uint32) set_cur_func_id_inner,
    (uint32) get_card_state_inner,
    (uint32) change_card_state_inner,
    (uint32) get_uhost_state_inner,
    (uint32) get_linein_state_inner,
    (uint32) get_cable_state_inner,
    (uint32) get_adaptor_state_inner,
    (uint32) clear_fs_vram_info_inner,
    (uint32) com_adjust_sys_clk_inner,
    (uint32) common_init_inner,
    (uint32) common_fmdrv_standby_inner,

    //BT MANAGER
    (uint32) com_creat_bt_engine_inner,
    (uint32) com_close_bt_engine_inner,
    (uint32) com_bt_event_loop_inner,
    (uint32) com_get_btengine_status_inner,

    (uint32) com_btengine_set_visual_mode_inner,
    (uint32) com_btengine_connect_deal_inner,
    (uint32) com_btengine_set_sbc_play_inner,
    (uint32) com_btengine_set_play_pipe_filter_inner,
    (uint32) com_btengine_set_hfp_call_inner,
    (uint32) com_btengine_set_sco_pipe_filter_inner,
    (uint32) com_btengine_hid_take_photo_inner,
    (uint32) com_btengine_force_disable_sniff_inner,

    (uint32) com_enter_btdbg_flag_inner,
    (uint32) com_enter_duttest_check_inner,

    (uint32) bt_manager_blespp_data_trans_register_dispatch_callback_inner,
    (uint32) bt_manager_blespp_data_trans_send_inner,

    //UI INTERFACE - keytone
    (uint32) get_mmm_state_inner,
    (uint32) change_mmm_state_inner,
    (uint32) get_ktp_status_inner,
    (uint32) change_ktp_status_inner,

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    (uint32) com_btengine_tws_get_bt_cbk_inner,
    (uint32) com_btengine_tws_get_send_frame_cbk_inner,

    (uint32) get_tws_role_inner,
    (uint32) change_tws_role_inner,
    (uint32) get_tws_spk_pos_inner,
    (uint32) change_tws_spk_pos_inner,
    (uint32) write_tws_cmd_to_buffer_rw_inner,
    (uint32) read_tws_cmd_from_buffer_rw_inner,
    (uint32) com_tws_m2s_switch_pos_cmd_inner,
    (uint32) com_tws_m2s_switch_tws_mode_cmd_inner,
    (uint32) com_tws_s2m_tts_play_req_inner,
    (uint32) com_tws_slave_update_btplay_inner,

    (uint32) com_tts_state_play_tws_slave_inner,
#endif

    (uint32) com_get_input_msg_type_inner,

    (uint32) com_set_speech_peq_para_inner,
    (uint32) com_update_speech_peq_para_inner,

    (uint32) com_update_sound_energy_inner,
    (uint32) com_get_sound_energy_inner,

    (uint32) com_init_btengine_info_inner,

    //UI INTERFACE - RTC TIME ALARM
#if (SUPPORT_TIME_ALARM == 1)
    (uint32) com_rtc_time_entry_inner,
    (uint32) com_rtc_get_alarm_detail_inner,
    (uint32) com_rtc_update_alarm_inner,
#else
    (uint32) /*com_rtc_time_entry_inner*/ 0,
    (uint32) /*com_rtc_get_alarm_detail_inner*/ 0,
    (uint32) /*com_rtc_update_alarm_inner*/ 0,
#endif

    //DEVICE STATUS ASK
    (uint32) sys_card_detect_inner,
    (uint32) sys_usb_detect_inner,
    (uint32) sys_uhost_detect_inner,

#if (SUPPORT_SPI_LCD_DISPLAY == 1)
    //LCD UI Interface
    (uint32) ui_slider_inner,
#else
    (uint32) /*ui_slider_inner*/ 0,
#endif

#if (SUPPORT_RECORD_FUNC == 1)
    (uint32) com_record_disk_init_inner,
    (uint32) com_record_create_file_inner,
    (uint32) com_record_close_file_inner,
    (uint32) com_record_disk_exit_inner,
    (uint32) com_record_recover_record_file_inner, 
	(uint32) com_record_update_record_file_inner,
	(uint32) com_record_get_disk_type_inner,
	(uint32) com_record_update_record_status_inner,
#else
    (uint32) /*com_record_disk_init_inner*/ 0,
    (uint32) /*com_record_create_file_inner*/ 0,
    (uint32) /*com_record_close_file_inner*/ 0,
    (uint32) /*com_record_disk_exit_inner*/ 0,
    (uint32) /*com_record_recover_record_file_inner*/ 0,
    (uint32) /*com_record_update_record_file_inner*/ 0,
    (uint32) /*com_record_get_disk_type_inner*/ 0,
    (uint32) /*com_record_update_record_status_inner*/ 0,
#endif

    //OK MIC IDLE PLAYER
#if (SUPPORT_OK_MIC == 1)
    (uint32) com_ok_mic_idle_play_inner,
    (uint32) com_ok_mic_idle_stop_inner,
#else
    (uint32) /*com_ok_mic_idle_play_inner*/ 0,
    (uint32) /*com_ok_mic_idle_stop_inner*/ 0,
#endif

#if (SUPPORT_FAST_PREVIEW == 1)
    (uint32) com_fast_preview_cbk_inner,
    (uint32) com_fast_preview_update_ap_welcome_ui_inner,
    (uint32) com_fast_preview_get_status_inner,
    (uint32) com_fast_preview_next_app_inner,
#else
    (uint32) /*com_fast_preview_cbk_inner*/ 0,
    (uint32) /*com_fast_preview_update_ap_welcome_ui_inner*/ 0,
    (uint32) /*com_fast_preview_get_status_inner*/ 0,
    (uint32) /*com_fast_preview_next_app_inner*/ 0,
#endif
};

