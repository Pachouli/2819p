/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：COMMON模块API ID定义
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _COMMON_API_DEF_H
#define _COMMON_API_DEF_H

#include "psp_includes.h"
#include "case_independent.h"

typedef enum
{
    //APPLIB
    API_APPLIB_INIT = 0,
    API_APPLIB_QUIT,
    API_APPLIB_GET_APP_INFO,
    API_GET_ENGINE_TYPE_BY_APPID,
    API_GET_ENGINE_APPID_BY_TYPE,
    API_GET_ENGINE_TYPE,
    API_GET_BT_STATE,
    API_GET_TWS_MODE,
    API_CHANGE_TWS_MODE,
    API_CHANGE_ENGINE_STATE,
    API_CHANGE_APP_STATE,
    API_GET_LAST_APP_ID,
    API_GET_CUR_APP_ID,
    API_COM_VRAM_MERGE,
    //APP TIMER
    API_SET_APP_TIMER = 14, /*NOTE!! HY HDMI.a will call this API, FIX to 14 */
    API_SET_SINGLE_SHOT_APP_TIMER,
    API_MODIFY_APP_TIMER,
    API_STOP_APP_TIMER,
    API_RESTART_APP_TIMER,
    API_STANDBY_RESTART_ALL_APP_TIMER,
    API_KILL_APP_TIMER,
    API_INIT_APP_TIMERS,
    API_CHANGE_APP_TIMER_TAG,
    //APP MSG
    API_GET_APP_MSG,
    API_MESSAGE_REDEAL_APP_MSG,
    API_SEND_SYNC_MSG,
    API_SEND_ASYNC_MSG,
    API_BROADCAST_MSG,
    API_REPLY_SYNC_MSG,
    API_SET_APP_MSG_DISPATCH_HANDLE,
    //FW CONFIG
    API_COM_GET_CONFIG_STRUCT,
    API_COM_GET_CONFIG_DEFAULT,


    //UI INTERFACE - KEY
    API_GET_GUI_MSG,
    API_MESSAGE_REDEAL_GUI_MSG,
    API_COM_DOUBLE_CLICK_ENABLE,
    API_COM_KEY_MAPPING,
    API_COM_FILTER_KEY_HOLD,
    API_COM_FILTER_KEY_UP,
    API_COM_FILTER_KEY_PERIOD_OF_TIME,
    API_COM_GET_LONG_KEY_TIME,
    API_COM_START_KEY_TONE,
    //UI INTERFACE - KEYTONE
    API_KEYTONE_SET_ON_OFF,
    API_KEYTONE_PLAY,
    //UI INTERFACE - TTS
    API_COM_TTS_STATE_PLAY,
    API_COM_TTS_STATE_PLAY_LIST,
    API_COM_TTS_STATE_PLAY_EXT,
    API_COM_TTS_STATE_PLAY_CONTROL,
    API_COM_SD_MP3_PLAYER_PLAY_TTS,
    API_COM_SD_MP3_PLAYER_PLAY_MP3,
    API_COM_SD_MP3_PLAYER_STOP,
    API_COM_SD_MP3_PLAYER_IS_STOP,
    //UI INTERFACE - SOUND
    API_COM_GET_SOUND_VOLUME,
    API_COM_GET_SOUND_VOLUME_PC,
    API_COM_RESET_SOUND_VOLUME,
    API_COM_SET_SOUND_VOLUME,
    API_COM_SET_SOUND_VOLUME_MODE,
    API_COM_SET_SOUND_VOLUME_GAIN,
    API_COM_VOLUME_ADD,
    API_COM_VOLUME_SUB,
    API_COM_VOLUME_SET,
    API_COM_SET_MUTE,
    API_COM_SET_MUTE_TOGGLE,
    API_COM_SET_USBBOX_VOLUME_CTRL,
    API_COM_SWITCH_GEQ,
    API_IS_SPEAKER_ON,
    API_COM_SPEAKER_ON,
    API_COM_SPEAKER_OFF,
    API_COM_SPEAKER_AB_D_SWITCH,
    API_COM_SET_DAE_ONOFF,
    API_COM_SET_DRC_ONOFF,
    API_COM_SWITCH_DRC_MODE,
    //UI INTERFACE - LED
    API_COM_LED_TWINKLE_INIT,
    API_COM_LED_LIGHT_ON,
    API_COM_LED_LIGHT_OFF,
    API_COM_LED_LIGHT_DUMMY,
    API_COM_LED_BREATH,
    //UI INTERFACE - LED SEGMENT
    API_COM_LED_DISPLAY,
    API_COM_LED_DISPLAY_ICON,
    API_COM_LED_CLEAR_SCREEN,
    API_COM_LED_FLASH_SCREEN,
    API_COM_LED_ENTER_MSG_SHOW,
    API_COM_LED_FORCE_EXIT_MSG_SHOW,
    API_COM_LED_TWINKLE_ICON,
    //UI INTERFACE - OTHERS
    API_COM_MESSAGE_BOX,
    API_SYS_HOT_DETECT_HANDLE,
    API_COM_SAVE_SYS_COMVAL,
    API_COM_RESTORE_SYS_COMVAL_BY_RESET,
    API_COM_AUX_PA_INIT,


    //ASET
    API_ASET_TEST_LOOP_DEAL,
    API_ASET_SWITCH_AUX_MODE,


    //COMMON FUNC - AP SWITCH
    API_COM_AP_SWITCH_DEAL,
    API_COM_AP_SWITCH_ASK_NEXT_FUNC,
    API_COM_AP_SWITCH_CHECK,
    //COMMON FUNC - STUB
    API_COM_STUB_TOOLS_CONNECT,
    API_COM_GET_STUB_TOOLS_TYPE,
    API_COM_STUB_ACK_EXT_CMD,
    API_COM_STUB_READ_EXT_CMD,
    API_COM_STUB_WRITE_EXT_CMD,
    API_COM_STUB_READ_CMD,
    API_COM_STUB_WRITE_CMD,
    //COMMON FUNC - FS FILE
    API_COM_FILE_OPEN,
    API_COM_FILE_CLOSE,
    API_COM_FILE_READ,
    API_COM_FILE_WRITE,
    API_COM_FILE_SEEK,
    API_COM_FILE_TELL,
    //COMMON FUNC - OTHERS
    API_SET_NEED_UPDATE_VM,
    API_GET_NEED_UPDATE_VM,
    API_SET_IC_PA_INITED,
    API_GET_IC_PA_INITED,
    API_SET_ENTER_DUT_TEST,
    API_GET_ENTER_DUT_TEST,
    API_SET_CUR_FUNC_ID,
    API_GET_CARD_STATE,
    API_CHANGE_CARD_STATE,
    API_GET_UHOST_STATE,
    API_GET_LINEIN_STATE,
    API_GET_CABLE_STATE,
    API_GET_ADAPTOR_STATE,
    API_CLEAR_FS_VRAM_INFO,
    API_COM_ADJUST_SYS_CLK,
    API_COMMON_INIT,
    API_COMMON_FMDRV_STANDBY,


    //BT MANAGER
    API_COM_CREAT_BT_ENGINE,
    API_COM_CLOSE_BT_ENGINE,
    API_COM_BT_EVENT_LOOP,
    API_COM_GET_BTENGINE_STATUS,

    API_COM_BTENGINE_SET_VISUAL_MODE,
    API_COM_BTENGINE_CONNECT_DEAL,
    API_COM_BTENGINE_SET_SBC_PLAY,
    API_COM_BTENGINE_SET_PLAY_PIPE_FILTER,
    API_COM_BTENGINE_SET_HFP_CALL,
    API_COM_BTENGINE_SET_SCO_PIPE_FILTER,
    API_COM_BTENGINE_HID_TAKE_PHOTO,
    API_COM_BTENGINE_FORCE_DISABLE_SNIFF,

    API_COM_ENTER_BTDBG_FLAG,
    API_COM_ENTER_DUTTEST_CHECK,

    API_BT_MANAGER_BLESPP_DATA_TRANS_REGISTER_DISPATCH_CALLBACK,
    API_BT_MANAGER_BLESPP_DATA_TRANS_SEND,

    //UI INTERFACE - keytone
    API_GET_MMM_STATE,
    API_CHANGE_MMM_STATE,
    API_GET_KTP_STATUS,
    API_CHANGE_KTP_STATUS,

#ifdef ENABLE_TRUE_WIRELESS_STEREO
    //TWS INTERFACE
    API_COM_BTENGINE_TWS_GET_BT_CBK,
    API_COM_BTENGINE_TWS_GET_SEND_FRAME_CBK,

    API_GET_TWS_ROLE,
    API_CHANGE_TWS_ROLE,
    API_GET_TWS_SPK_POS,
    API_CHANGE_TWS_SPK_POS,
    API_WRITE_TWS_CMD_TO_BUFFER_RW,
    API_READ_TWS_CMD_FROM_BUFFER_RW,
    API_COM_TWS_M2S_SWITCH_POS_CMD,
    API_COM_TWS_M2S_SWITCH_TWS_MODE_CMD,
    API_COM_TWS_S2M_TTS_PLAY_REQ,
    API_COM_TWS_SLAVE_UPDATE_BTPLAY,

    API_COM_TTS_STATE_PLAY_TWS_SLAVE,
#endif
    API_COM_GET_INPUT_MSG_TYPE,
    API_COM_SET_SPEECH_PEQ_PARA,
    API_COM_UPDATE_SPEECH_PEQ_PARA,

    API_COM_UPDATE_SOUND_ENERGY,
    API_COM_GET_SOUND_ENERGY,

    API_COM_INIT_BTENGINE_INFO,

    //UI INTERFACE - RTC TIME ALARM
    API_COM_RTC_TIME_ENTRY,
    API_COM_RTC_GET_ALARM_DETAIL,
    API_COM_RTC_UPDATE_ALARM,

    API_SYS_CARD_DETECT,
    API_SYS_USB_DETECT,
    API_SYS_UHOST_DETECT,

    //LCD UI Interface
    API_UI_SLIDER,

    //RECORD
    API_RECORD_DISK_INIT,
    API_RECORD_CREATE_FILE,
    API_RECORD_CLOSE_FILE,
    API_RECORD_DISK_EXIT,
    API_RECORD_RECOVER_RECORD_FILE,
	API_RECORD_UPDATE_RECORD_FILE,
	API_RECORD_GET_DISK_TYPE,
	API_RECORD_UPDATE_RECORD_STATUS,

    //OK MIC
    API_COM_OK_MIC_IDLE_PLAY,
    API_COM_OK_MIC_IDLE_STOP,

    //fast preview
    API_COM_FAST_PREVIEW_CBK,
    API_COM_FAST_PREVIEW_UPDATE_AP_WELCOME_UI,
    API_COM_FAST_PREVIEW_GET_STATUS,
    API_COM_FAST_PREVIEW_NEXT_APP,
} common_api_e;


#define COMMON_ENTRY_ADDR(x)                (COMMON_ENTRY_VA_START + (4 * x))

//APPLIB
#define COM_APPLIB_INIT                     (*((uint32*)(COMMON_ENTRY_ADDR(API_APPLIB_INIT))))
#define COM_APPLIB_QUIT                     (*((uint32*)(COMMON_ENTRY_ADDR(API_APPLIB_QUIT))))
#define COM_APPLIB_GET_APP_INFO             (*((uint32*)(COMMON_ENTRY_ADDR(API_APPLIB_GET_APP_INFO))))
#define COM_GET_ENGINE_TYPE_BY_APPID        (*((uint32*)(COMMON_ENTRY_ADDR(API_GET_ENGINE_TYPE_BY_APPID))))
#define COM_GET_ENGINE_APPID_BY_TYPE        (*((uint32*)(COMMON_ENTRY_ADDR(API_GET_ENGINE_APPID_BY_TYPE))))
#define COM_GET_ENGINE_TYPE                 (*((uint32*)(COMMON_ENTRY_ADDR(API_GET_ENGINE_TYPE))))
#define COM_GET_BT_STATE                    (*((uint32*)(COMMON_ENTRY_ADDR(API_GET_BT_STATE))))
#define COM_GET_TWS_MODE                    (*((uint32*)(COMMON_ENTRY_ADDR(API_GET_TWS_MODE))))
#define COM_CHANGE_TWS_MODE                 (*((uint32*)(COMMON_ENTRY_ADDR(API_CHANGE_TWS_MODE))))
#define COM_CHANGE_ENGINE_STATE             (*((uint32*)(COMMON_ENTRY_ADDR(API_CHANGE_ENGINE_STATE))))
#define COM_CHANGE_APP_STATE                (*((uint32*)(COMMON_ENTRY_ADDR(API_CHANGE_APP_STATE))))
#define COM_GET_LAST_APP_ID                 (*((uint32*)(COMMON_ENTRY_ADDR(API_GET_LAST_APP_ID))))
#define COM_GET_CUR_APP_ID                  (*((uint32*)(COMMON_ENTRY_ADDR(API_GET_CUR_APP_ID))))
#define COM_COM_VRAM_MERGE                  (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_VRAM_MERGE))))
//APP TIMER
#define COM_SET_APP_TIMER                   (*((uint32*)(COMMON_ENTRY_ADDR(API_SET_APP_TIMER))))
#define COM_SET_SINGLE_SHOT_APP_TIMER       (*((uint32*)(COMMON_ENTRY_ADDR(API_SET_SINGLE_SHOT_APP_TIMER))))
#define COM_MODIFY_APP_TIMER                (*((uint32*)(COMMON_ENTRY_ADDR(API_MODIFY_APP_TIMER))))
#define COM_STOP_APP_TIMER                  (*((uint32*)(COMMON_ENTRY_ADDR(API_STOP_APP_TIMER))))
#define COM_RESTART_APP_TIMER               (*((uint32*)(COMMON_ENTRY_ADDR(API_RESTART_APP_TIMER))))
#define COM_STANDBY_RESTART_ALL_APP_TIMER   (*((uint32*)(COMMON_ENTRY_ADDR(API_STANDBY_RESTART_ALL_APP_TIMER))))
#define COM_KILL_APP_TIMER                  (*((uint32*)(COMMON_ENTRY_ADDR(API_KILL_APP_TIMER))))
#define COM_INIT_APP_TIMERS                 (*((uint32*)(COMMON_ENTRY_ADDR(API_INIT_APP_TIMERS))))
#define COM_CHANGE_APP_TIMER_TAG            (*((uint32*)(COMMON_ENTRY_ADDR(API_CHANGE_APP_TIMER_TAG))))
//APP MSG
#define COM_GET_APP_MSG                     (*((uint32*)(COMMON_ENTRY_ADDR(API_GET_APP_MSG))))
#define COM_MESSAGE_REDEAL_APP_MSG          (*((uint32*)(COMMON_ENTRY_ADDR(API_MESSAGE_REDEAL_APP_MSG))))
#define COM_SEND_SYNC_MSG                   (*((uint32*)(COMMON_ENTRY_ADDR(API_SEND_SYNC_MSG))))
#define COM_SEND_ASYNC_MSG                  (*((uint32*)(COMMON_ENTRY_ADDR(API_SEND_ASYNC_MSG))))
#define COM_BROADCAST_MSG                   (*((uint32*)(COMMON_ENTRY_ADDR(API_BROADCAST_MSG))))
#define COM_REPLY_SYNC_MSG                  (*((uint32*)(COMMON_ENTRY_ADDR(API_REPLY_SYNC_MSG))))
#define COM_SET_APP_MSG_DISPATCH_HANDLE     (*((uint32*)(COMMON_ENTRY_ADDR(API_SET_APP_MSG_DISPATCH_HANDLE))))
//FW CONFIG
#define COM_COM_GET_CONFIG_STRUCT           (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_GET_CONFIG_STRUCT))))
#define COM_COM_GET_CONFIG_DEFAULT          (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_GET_CONFIG_DEFAULT))))


//UI INTERFACE - KEY
#define COM_GET_GUI_MSG                     (*((uint32*)(COMMON_ENTRY_ADDR(API_GET_GUI_MSG))))
#define COM_MESSAGE_REDEAL_GUI_MSG          (*((uint32*)(COMMON_ENTRY_ADDR(API_MESSAGE_REDEAL_GUI_MSG))))
#define COM_COM_DOUBLE_CLICK_ENABLE         (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_DOUBLE_CLICK_ENABLE))))
#define COM_COM_KEY_MAPPING                 (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_KEY_MAPPING))))
#define COM_COM_FILTER_KEY_HOLD             (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_FILTER_KEY_HOLD))))
#define COM_COM_FILTER_KEY_UP               (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_FILTER_KEY_UP))))
#define COM_COM_FILTER_KEY_PERIOD_OF_TIME   (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_FILTER_KEY_PERIOD_OF_TIME))))
#define COM_COM_GET_LONG_KEY_TIME           (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_GET_LONG_KEY_TIME))))
#define COM_COM_START_KEY_TONE              (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_START_KEY_TONE))))
//UI INTERFACE - KEYTONE
#define COM_KEYTONE_SET_ON_OFF              (*((uint32*)(COMMON_ENTRY_ADDR(API_KEYTONE_SET_ON_OFF))))
#define COM_KEYTONE_PLAY                    (*((uint32*)(COMMON_ENTRY_ADDR(API_KEYTONE_PLAY))))
//UI INTERFACE - TTS
#define COM_COM_TTS_STATE_PLAY              (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_TTS_STATE_PLAY))))
#define COM_COM_TTS_STATE_PLAY_LIST         (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_TTS_STATE_PLAY_LIST))))
#define COM_COM_TTS_STATE_PLAY_EXT          (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_TTS_STATE_PLAY_EXT))))
#define COM_COM_TTS_STATE_PLAY_CONTROL      (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_TTS_STATE_PLAY_CONTROL))))
#define COM_COM_SD_MP3_PLAYER_PLAY_TTS      (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_SD_MP3_PLAYER_PLAY_TTS))))
#define COM_COM_SD_MP3_PLAYER_PLAY_MP3      (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_SD_MP3_PLAYER_PLAY_MP3))))
#define COM_COM_SD_MP3_PLAYER_STOP          (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_SD_MP3_PLAYER_STOP))))
#define COM_COM_SD_MP3_PLAYER_IS_STOP       (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_SD_MP3_PLAYER_IS_STOP))))
//UI INTERFACE - SOUND
#define COM_COM_GET_SOUND_VOLUME            (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_GET_SOUND_VOLUME))))
#define COM_COM_GET_SOUND_VOLUME_PC         (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_GET_SOUND_VOLUME_PC))))
#define COM_COM_RESET_SOUND_VOLUME          (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_RESET_SOUND_VOLUME))))
#define COM_COM_SET_SOUND_VOLUME            (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_SET_SOUND_VOLUME))))
#define COM_COM_SET_SOUND_VOLUME_MODE       (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_SET_SOUND_VOLUME_MODE))))
#define COM_COM_SET_SOUND_VOLUME_GAIN       (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_SET_SOUND_VOLUME_GAIN))))
#define COM_COM_VOLUME_ADD                  (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_VOLUME_ADD))))
#define COM_COM_VOLUME_SUB                  (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_VOLUME_SUB))))
#define COM_COM_VOLUME_SET                  (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_VOLUME_SET))))
#define COM_COM_SET_MUTE                    (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_SET_MUTE))))
#define COM_COM_SET_MUTE_TOGGLE             (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_SET_MUTE_TOGGLE))))
#define COM_COM_SET_USBBOX_VOLUME_CTRL      (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_SET_USBBOX_VOLUME_CTRL))))
#define COM_COM_SWITCH_GEQ                  (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_SWITCH_GEQ))))
#define COM_IS_SPEAKER_ON                   (*((uint32*)(COMMON_ENTRY_ADDR(API_IS_SPEAKER_ON))))
#define COM_COM_SPEAKER_ON                  (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_SPEAKER_ON))))
#define COM_COM_SPEAKER_OFF                 (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_SPEAKER_OFF))))
#define COM_COM_SPEAKER_AB_D_SWITCH         (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_SPEAKER_AB_D_SWITCH))))
#define COM_COM_SET_DAE_ONOFF               (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_SET_DAE_ONOFF))))
#define COM_COM_SET_DRC_ONOFF               (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_SET_DRC_ONOFF))))
#define COM_COM_SWITCH_DRC_MODE             (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_SWITCH_DRC_MODE))))
//UI INTERFACE - LED
#define COM_COM_LED_TWINKLE_INIT            (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_LED_TWINKLE_INIT))))
#define COM_COM_LED_LIGHT_ON                (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_LED_LIGHT_ON))))
#define COM_COM_LED_LIGHT_OFF               (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_LED_LIGHT_OFF))))
#define COM_COM_LED_LIGHT_DUMMY             (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_LED_LIGHT_DUMMY))))
#define COM_COM_LED_BREATH                  (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_LED_BREATH))))
//UI INTERFACE - LED SEGMENT
#define COM_COM_LED_DISPLAY                 (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_LED_DISPLAY))))
#define COM_COM_LED_DISPLAY_ICON            (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_LED_DISPLAY_ICON))))
#define COM_COM_LED_CLEAR_SCREEN            (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_LED_CLEAR_SCREEN))))
#define COM_COM_LED_FLASH_SCREEN            (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_LED_FLASH_SCREEN))))
#define COM_COM_LED_ENTER_MSG_SHOW          (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_LED_ENTER_MSG_SHOW))))
#define COM_COM_LED_FORCE_EXIT_MSG_SHOW     (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_LED_FORCE_EXIT_MSG_SHOW))))
#define COM_COM_LED_TWINKLE_ICON            (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_LED_TWINKLE_ICON))))
//UI INTERFACE - OTHERS
#define COM_COM_MESSAGE_BOX                 (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_MESSAGE_BOX))))
#define COM_SYS_HOT_DETECT_HANDLE           (*((uint32*)(COMMON_ENTRY_ADDR(API_SYS_HOT_DETECT_HANDLE))))
#define COM_COM_SAVE_SYS_COMVAL             (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_SAVE_SYS_COMVAL))))
#define COM_COM_RESTORE_SYS_COMVAL_BY_RESET (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_RESTORE_SYS_COMVAL_BY_RESET))))
#define COM_COM_AUX_PA_INIT                 (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_AUX_PA_INIT))))


//ASET
#define COM_ASET_TEST_LOOP_DEAL             (*((uint32*)(COMMON_ENTRY_ADDR(API_ASET_TEST_LOOP_DEAL))))
#define COM_ASET_SWITCH_AUX_MODE            (*((uint32*)(COMMON_ENTRY_ADDR(API_ASET_SWITCH_AUX_MODE))))


//COMMON FUNC - AP SWITCH
#define COM_COM_AP_SWITCH_DEAL              (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_AP_SWITCH_DEAL))))
#define COM_COM_AP_SWITCH_ASK_NEXT_FUNC     (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_AP_SWITCH_ASK_NEXT_FUNC))))
#define COM_COM_AP_SWITCH_CHECK             (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_AP_SWITCH_CHECK))))
//COMMON FUNC - STUB
#define COM_COM_STUB_TOOLS_CONNECT          (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_STUB_TOOLS_CONNECT))))
#define COM_COM_GET_STUB_TOOLS_TYPE         (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_GET_STUB_TOOLS_TYPE))))
#define COM_COM_STUB_ACK_EXT_CMD            (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_STUB_ACK_EXT_CMD))))
#define COM_COM_STUB_READ_EXT_CMD           (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_STUB_READ_EXT_CMD))))
#define COM_COM_STUB_WRITE_EXT_CMD          (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_STUB_WRITE_EXT_CMD))))
#define COM_COM_STUB_READ_CMD               (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_STUB_READ_CMD))))
#define COM_COM_STUB_WRITE_CMD              (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_STUB_WRITE_CMD))))
//COMMON FUNC - FS FILE
#define COM_COM_FILE_OPEN                   (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_FILE_OPEN))))
#define COM_COM_FILE_CLOSE                  (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_FILE_CLOSE))))
#define COM_COM_FILE_READ                   (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_FILE_READ))))
#define COM_COM_FILE_WRITE                  (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_FILE_WRITE))))
#define COM_COM_FILE_SEEK                   (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_FILE_SEEK))))
#define COM_COM_FILE_TELL                   (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_FILE_TELL))))
//COMMON FUNC - OTHERS
#define COM_SET_NEED_UPDATE_VM              (*((uint32*)(COMMON_ENTRY_ADDR(API_SET_NEED_UPDATE_VM))))
#define COM_GET_NEED_UPDATE_VM              (*((uint32*)(COMMON_ENTRY_ADDR(API_GET_NEED_UPDATE_VM))))
#define COM_SET_IC_PA_INITED                (*((uint32*)(COMMON_ENTRY_ADDR(API_SET_IC_PA_INITED))))
#define COM_GET_IC_PA_INITED                (*((uint32*)(COMMON_ENTRY_ADDR(API_GET_IC_PA_INITED))))
#define COM_SET_ENTER_DUT_TEST              (*((uint32*)(COMMON_ENTRY_ADDR(API_SET_ENTER_DUT_TEST))))
#define COM_GET_ENTER_DUT_TEST              (*((uint32*)(COMMON_ENTRY_ADDR(API_GET_ENTER_DUT_TEST))))
#define COM_SET_CUR_FUNC_ID                 (*((uint32*)(COMMON_ENTRY_ADDR(API_SET_CUR_FUNC_ID))))
#define COM_GET_CARD_STATE                  (*((uint32*)(COMMON_ENTRY_ADDR(API_GET_CARD_STATE))))
#define COM_CHANGE_CARD_STATE               (*((uint32*)(COMMON_ENTRY_ADDR(API_CHANGE_CARD_STATE))))
#define COM_GET_UHOST_STATE                 (*((uint32*)(COMMON_ENTRY_ADDR(API_GET_UHOST_STATE))))
#define COM_GET_LINEIN_STATE                (*((uint32*)(COMMON_ENTRY_ADDR(API_GET_LINEIN_STATE))))
#define COM_GET_CABLE_STATE                 (*((uint32*)(COMMON_ENTRY_ADDR(API_GET_CABLE_STATE))))
#define COM_GET_ADAPTOR_STATE               (*((uint32*)(COMMON_ENTRY_ADDR(API_GET_ADAPTOR_STATE))))
#define COM_CLEAR_FS_VRAM_INFO              (*((uint32*)(COMMON_ENTRY_ADDR(API_CLEAR_FS_VRAM_INFO))))
#define COM_COM_ADJUST_SYS_CLK              (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_ADJUST_SYS_CLK))))
#define COM_COMMON_INIT                     (*((uint32*)(COMMON_ENTRY_ADDR(API_COMMON_INIT))))
#define COM_COMMON_FMDRV_STANDBY            (*((uint32*)(COMMON_ENTRY_ADDR(API_COMMON_FMDRV_STANDBY))))


//BT MANAGER
#define COM_COM_CREAT_BT_ENGINE             (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_CREAT_BT_ENGINE))))
#define COM_COM_CLOSE_BT_ENGINE             (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_CLOSE_BT_ENGINE))))
#define COM_COM_BT_EVENT_LOOP               (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_BT_EVENT_LOOP))))
#define COM_COM_GET_BTENGINE_STATUS         (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_GET_BTENGINE_STATUS))))

#define COM_COM_BTENGINE_SET_VISUAL_MODE         (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_BTENGINE_SET_VISUAL_MODE))))
#define COM_COM_BTENGINE_CONNECT_DEAL            (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_BTENGINE_CONNECT_DEAL))))
#define COM_COM_BTENGINE_SET_SBC_PLAY            (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_BTENGINE_SET_SBC_PLAY))))
#define COM_COM_BTENGINE_SET_PLAY_PIPE_FILTER    (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_BTENGINE_SET_PLAY_PIPE_FILTER))))
#define COM_COM_BTENGINE_SET_HFP_CALL            (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_BTENGINE_SET_HFP_CALL))))
#define COM_COM_BTENGINE_SET_SCO_PIPE_FILTER     (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_BTENGINE_SET_SCO_PIPE_FILTER))))
#define COM_COM_BTENGINE_HID_TAKE_PHOTO          (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_BTENGINE_HID_TAKE_PHOTO))))
#define COM_COM_BTENGINE_FORCE_DISABLE_SNIFF     (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_BTENGINE_FORCE_DISABLE_SNIFF))))

#define COM_COM_ENTER_BTDBG_FLAG            (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_ENTER_BTDBG_FLAG))))
#define COM_COM_ENTER_DUTTEST_CHECK         (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_ENTER_DUTTEST_CHECK))))

#define COM_BT_MANAGER_BLESPP_DATA_TRANS_REGISTER_DISPATCH_CALLBACK  \
        (*((uint32*)(COMMON_ENTRY_ADDR(API_BT_MANAGER_BLESPP_DATA_TRANS_REGISTER_DISPATCH_CALLBACK))))
#define COM_BT_MANAGER_BLESPP_DATA_TRANS_SEND  \
        (*((uint32*)(COMMON_ENTRY_ADDR(API_BT_MANAGER_BLESPP_DATA_TRANS_SEND))))

//UI INTERFACE - keytone
#define COM_GET_MMM_STATE                   (*((uint32*)(COMMON_ENTRY_ADDR(API_GET_MMM_STATE))))
#define COM_CHANGE_MMM_STATE                (*((uint32*)(COMMON_ENTRY_ADDR(API_CHANGE_MMM_STATE))))
#define COM_GET_KTP_STATUS                  (*((uint32*)(COMMON_ENTRY_ADDR(API_GET_KTP_STATUS))))
#define COM_CHANGE_KTP_STATUS               (*((uint32*)(COMMON_ENTRY_ADDR(API_CHANGE_KTP_STATUS))))

#ifdef ENABLE_TRUE_WIRELESS_STEREO
#define COM_COM_BTENGINE_TWS_GET_BT_CBK     (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_BTENGINE_TWS_GET_BT_CBK))))
#define COM_COM_BTENGINE_TWS_GET_SEND_FRAME_CBK  (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_BTENGINE_TWS_GET_SEND_FRAME_CBK))))

#define COM_GET_TWS_ROLE                    (*((uint32*)(COMMON_ENTRY_ADDR(API_GET_TWS_ROLE))))
#define COM_CHANGE_TWS_ROLE                 (*((uint32*)(COMMON_ENTRY_ADDR(API_CHANGE_TWS_ROLE))))
#define COM_GET_TWS_SPK_POS                 (*((uint32*)(COMMON_ENTRY_ADDR(API_GET_TWS_SPK_POS))))
#define COM_CHANGE_TWS_SPK_POS              (*((uint32*)(COMMON_ENTRY_ADDR(API_CHANGE_TWS_SPK_POS))))
#define COM_WRITE_TWS_CMD_TO_BUFFER_RW      (*((uint32*)(COMMON_ENTRY_ADDR(API_WRITE_TWS_CMD_TO_BUFFER_RW))))
#define COM_READ_TWS_CMD_FROM_BUFFER_RW     (*((uint32*)(COMMON_ENTRY_ADDR(API_READ_TWS_CMD_FROM_BUFFER_RW))))
#define COM_COM_TWS_M2S_SWITCH_POS_CMD      (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_TWS_M2S_SWITCH_POS_CMD))))
#define COM_COM_TWS_M2S_SWITCH_TWS_MODE_CMD (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_TWS_M2S_SWITCH_TWS_MODE_CMD))))
#define COM_COM_TWS_S2M_TTS_PLAY_REQ        (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_TWS_S2M_TTS_PLAY_REQ))))
#define COM_COM_TWS_SLAVE_UPDATE_BTPLAY     (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_TWS_SLAVE_UPDATE_BTPLAY))))

#define COM_COM_TTS_STATE_PLAY_TWS_SLAVE    (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_TTS_STATE_PLAY_TWS_SLAVE))))
#endif
#define COM_COM_GET_INPUT_MSG_TYPE          (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_GET_INPUT_MSG_TYPE))))

#define COM_COM_SET_SPEECH_PEQ_PARA         (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_SET_SPEECH_PEQ_PARA))))
#define COM_COM_UPDATE_SPEECH_PEQ_PARA      (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_UPDATE_SPEECH_PEQ_PARA))))

#define COM_COM_UPDATE_SOUND_ENERGY         (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_UPDATE_SOUND_ENERGY))))
#define COM_COM_GET_SOUND_ENERGY            (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_GET_SOUND_ENERGY))))

#define COM_COM_INIT_BTENGINE_INFO          (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_INIT_BTENGINE_INFO))))

#define COM_COM_RTC_TIME_ENTRY              (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_RTC_TIME_ENTRY))))
#define COM_COM_RTC_GET_ALARM_DETAIL        (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_RTC_GET_ALARM_DETAIL))))
#define COM_COM_RTC_UPDATE_ALARM            (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_RTC_UPDATE_ALARM))))

#define COM_SYS_CARD_DETECT                 (*((uint32*)(COMMON_ENTRY_ADDR(API_SYS_CARD_DETECT))))
#define COM_SYS_USB_DETECT                  (*((uint32*)(COMMON_ENTRY_ADDR(API_SYS_USB_DETECT))))
#define COM_SYS_UHOST_DETECT                (*((uint32*)(COMMON_ENTRY_ADDR(API_SYS_UHOST_DETECT))))

//LCD UI Interface
#define COM_UI_SLIDER                       (*((uint32*)(COMMON_ENTRY_ADDR(API_UI_SLIDER))))

//RECORD
#define COM_RECORD_DISK_INIT                (*((uint32*)(COMMON_ENTRY_ADDR(API_RECORD_DISK_INIT))))
#define COM_RECORD_CREATE_FILE              (*((uint32*)(COMMON_ENTRY_ADDR(API_RECORD_CREATE_FILE))))
#define COM_RECORD_CLOSE_FILE               (*((uint32*)(COMMON_ENTRY_ADDR(API_RECORD_CLOSE_FILE))))
#define COM_RECORD_DISK_EXIT                (*((uint32*)(COMMON_ENTRY_ADDR(API_RECORD_DISK_EXIT))))
#define COM_RECORD_RECOVER_RECORD_FILE      (*((uint32*)(COMMON_ENTRY_ADDR(API_RECORD_RECOVER_RECORD_FILE))))
#define COM_RECORD_UPDATE_RECORD_FILE       (*((uint32*)(COMMON_ENTRY_ADDR(API_RECORD_UPDATE_RECORD_FILE))))
#define COM_RECORD_GET_DISK_TYPE            (*((uint32*)(COMMON_ENTRY_ADDR(API_RECORD_GET_DISK_TYPE))))
#define COM_RECORD_UPDATE_RECORD_STATUS     (*((uint32*)(COMMON_ENTRY_ADDR(API_RECORD_UPDATE_RECORD_STATUS))))

//OK MIC
#define COM_COM_OK_MIC_IDLE_PLAY            (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_OK_MIC_IDLE_PLAY))))
#define COM_COM_OK_MIC_IDLE_STOP            (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_OK_MIC_IDLE_STOP))))

#define COM_COM_FAST_PREVIEW_CBK            (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_FAST_PREVIEW_CBK))))
#define COM_COM_FAST_PREVIEW_UPDATE_AP_WELCOME_UI (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_FAST_PREVIEW_UPDATE_AP_WELCOME_UI))))
#define COM_COM_FAST_PREVIEW_COM_GET_STATUS (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_FAST_PREVIEW_GET_STATUS))))
#define COM_COM_FAST_PREVIEW_NEXT_APP       (*((uint32*)(COMMON_ENTRY_ADDR(API_COM_FAST_PREVIEW_NEXT_APP))))

#endif
