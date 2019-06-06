/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙免提通话引擎应用头文件。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _BTCALL_ENGINE_H
#define _BTCALL_ENGINE_H

#include  "common_engine_app_def.h"
#include  "bluetooth_common.h"
#include  "btcall_common.h"
#include  "mmm_sp.h"

extern void *sp_handle;
extern mmm_sp_status_t sp_status;
extern btengine_info_t btcall_bt_info;
extern btcall_info_t g_btcall_info;

extern btcall_speech_play_para_t g_play_para_backup;

extern bool g_stop_by_tts_flag;

extern record_param_t g_record_param;

extern void audio_setting_init_prms(speech_setting_t *p_audio_sett, asqt_configs_t *p_asqt_cfg);
extern void plc_init_prms(plc_para_t *p_plc_para, asqt_configs_t *p_asqt_cfg);
extern void preagc_init_prms(agc_para_t *p_agc_para, asqt_configs_t *p_asqt_cfg);
extern void postagc_init_prms(agc_para_t *p_agc_para, asqt_configs_t *p_asqt_cfg);
extern void actaec_init_sv_prms(actionsaec_prms_t *sv_prms, asqt_configs_t *p_asqt_cfg, bool att_flag);

extern app_result_e btcall_engine_tts_start(private_msg_t *msg_ptr);
extern app_result_e btcall_engine_tts_stop(private_msg_t *msg_ptr);

extern bool _play(bool oncall, bool att_flag, asqt_configs_t *p_asqt_config, asqt_dump_buf_t *p_asqt_dump_buf);
extern bool _stop(bool oncall);
extern bool _get_mmm_status(void);

extern app_result_e btcall_engine_control_block(void);

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_SPEECH_RECORD_FUNC == 1) && (SUPPORT_BTCALL_RECORD_FUNC == 1))
extern record_param_t g_record_param;
extern record_module_status_t g_rm_status;

extern bool _record_start(uint8 channel, uint8 record_source);
extern bool _record_stop(void);
extern bool _get_mmm_record_status(void);
extern bool _record_create_file(void);
extern bool _record_close_file(void);
extern app_result_e btcall_engine_record_start(private_msg_t *cur_msg);
extern app_result_e btcall_engine_record_stop(private_msg_t *cur_msg);
#endif

#endif

