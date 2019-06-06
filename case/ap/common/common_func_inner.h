/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：COMMON 一些公共接口内部头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _COMMON_FUNC_INNER_H_
#define _COMMON_FUNC_INNER_H_

#include "common_func.h"
#include "applib_inner.h"
#include "common_sys_inner.h"
#include "common_bt_inner.h"
#include "common_stub_inner.h"
#include "common_aset_inner.h"
#include "common_var.h"
#include "fm_tx_interface.h"

/*! 循环切换应用数目 */
#define MAX_FUNCTON_CYCLE   12

/*! 前台应用切换相关全局变量 */
typedef struct
{
    /*! 用于指示是否支持功能（应用模式），最多支持32个功能（应用模式） */
    uint32 app_func_support;
    //对应config.txt中SETTING_APP_SWITCH_SEQUENCE项的中括号里面的数字
    uint8 app_switch_seq[MAX_FUNCTON_CYCLE];
    //对应config.txt中SETTING_APP_SWITCH_SEQUENCE项的中括号里面的数字总数
    uint8 app_switch_count;
    //对应ap_switch_info中循环功能总数
    uint8 app_switch_cycle_count;
    //默认循环功能ID
    uint8 app_default_func_id;
    //默认循环功能ID在config.txt中SETTING_APP_SWITCH_SEQUENCE项的中括号里面的序号
    uint8 app_default_seq_index;
    //默认循环功能ID在ap_switch_info中的序号
    uint8 app_default_func_index;
} ap_switch_var_t;


extern void send_ap_switch_finish_msg(void);
extern void com_ap_switch_deal_inner(app_result_e app_retsult);
extern uint8 com_ap_switch_ask_next_func_inner(app_result_e app_result);
extern bool com_ap_switch_check_inner(uint8 app_func_id);

extern void set_need_update_vm_inner(uint8 flag);
extern uint8 get_need_update_vm_inner(void);
extern void set_ic_pa_inited_inner(bool inited);
extern bool get_ic_pa_inited_inner(void);
extern void set_enter_dut_test_inner(uint8 test);
extern uint8 get_enter_dut_test_inner(void);

extern uint8 get_bat_value_inner(void);

extern uint8 get_btdbg_ejtag_flag_inner(void);

extern void clear_fs_vram_info_inner(void);

extern freq_level_e com_adjust_sys_clk_inner(freq_level_e freq_level, bool sys_clk_work);

//前台磁盘文件读写
extern uint32 com_file_open_inner(uint8 disk_type, uint8 *file_name, uint8 open_mode);
extern bool com_file_close_inner(uint32 fhandle);
extern uint32 com_file_read_inner(uint8 *read_buf, uint32 byte_count, uint32 fhandle);
extern bool com_file_write_inner(uint8 *write_buf, uint32 byte_count, uint32 fhandle);
extern bool com_file_seek_inner(int32 file_offset, uint8 seek_type, uint32 fhandle);
extern uint32 com_file_tell_inner(uint32 fhandle);

extern void common_fmdrv_standby_inner(uint8 mode);

extern void com_update_sound_energy_inner(sound_energy_t *p_energy);
extern void com_get_sound_energy_inner(sound_energy_t *p_energy);

extern bool sys_card_detect_inner(void);
extern bool sys_usb_detect_inner(void);
extern bool sys_uhost_detect_inner(void);

extern uint8 com_record_create_file_inner(uint8 mode, record_param_t* p_param);
extern uint8 com_record_close_file_inner(uint8 mode);
extern uint8 com_record_disk_init_inner(uint8 diskno);
extern uint8 com_record_disk_exit_inner(void);
extern bool com_record_recover_record_file_inner(void);
extern void com_record_update_record_file_inner(bool force_update, record_file_update_t *p_file_update);
extern uint8 com_record_get_disk_type_inner(void);
extern void com_record_update_record_status_inner(record_status_e record_sta);

extern void com_record_switch_record(void);

extern bool com_ok_mic_idle_play_inner(void);
extern bool com_ok_mic_idle_stop_inner(void);
extern bool com_ok_mic_idle_eq_set(void);

extern bool com_hdmi_is_hdmi_func(uint8 func_id);
extern bool com_hdmi_is_arc_func(uint8 func_id);
extern HOST_SRC com_hdmi_get_host_src(uint8 func_id);
extern HDMI_SRC com_hdmi_get_hdmi_src(uint8 func_id);
extern uint8 hdmi_get_cur_func_id(void);
extern bool hdmi_get_cec_mute(void);
extern uint8 hdmi_get_sound_volume(void);

#endif
