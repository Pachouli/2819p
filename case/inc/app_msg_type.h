/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：应用间消息类型枚举类型。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _APP_MSG_TYPE_H_
#define _APP_MSG_TYPE_H_

#include "psp_includes.h"

/*!
 * \brief
 *  msg_apps_type_e 应用消息枚举类型
 */
typedef enum
{
    /*! 空消息 */
    MSG_NULL = 0x0000,

    /****************************** shortcut事件 ****************************/
    /*! 快捷键占位符，保留空间 0x0020 ~ 0x003f */
    SHORTCUT_RESERVE_BEGIN = 0x001f,
    /*! MUTE 开关切换 */
    EVENT_SWITCH_MUTE,
    /*! 长按 PLAY 键关机事件 */
    EVENT_ENTER_SHUTOFF_PLAY,
    /*! 长按 PLAY 键几秒钟关机事件 */
    EVENT_ENTER_SHUTOFF_PLAY_SEC,
    /*! 短按 POWER 键关机事件 */
    EVENT_ENTER_SHUTOFF,

    /*! 音量加1事件 */
    EVENT_VOLUME_ADD,
    EVENT_VOLUME_ADD_HOLD,
    EVENT_VOLUME_ADD_NEXT,
    /*! 音量减1事件 */
    EVENT_VOLUME_SUB,
    EVENT_VOLUME_SUB_HOLD,
    EVENT_VOLUME_SUB_PREV,

    /*!TTS语言切换事件*/
    EVENT_TTS_LANGUAGE_SWITCH,
    /*!TTS开关切换*/
    EVENT_TTS_ENABLE_SWITCH,

    /*!进入蓝牙推歌*/
    EVENT_ENTER_BTPLAY,
    /*!进入MUSIC播歌*/
    EVENT_ENTER_MUSIC,
    /*!进入LINEIN播歌*/
    EVENT_ENTER_LINEIN,
    /*!进入fm收音*/
    EVENT_ENTER_RADIO,
    /*!进入插卡播歌*/
    EVENT_ENTER_MUSIC_CARD,
    /*!进入U盘播歌*/
    EVENT_ENTER_MUSIC_UHOST,
    /*!进入本地磁盘特殊目录*/
    EVENT_ENTER_FOLDER1,
    EVENT_ENTER_FOLDER2,

    EVENT_ENTER_CARD_REC,
    EVENT_ENTER_UHOST_REC,
    EVENT_ENTER_CARD_REPLAY,
    EVENT_ENTER_UHOST_REPLAY,
    EVENT_ENTER_REPLAY,

    EVENT_ENTER_RTC_TIME,
    EVENT_EXIT_RTC_TIME,
    EVENT_EXIT_RTC_TIME_HOLD,
    EVENT_SWITCH_RTC_SCENE,
    EVENT_RTC_VALUE_ADD,
    EVENT_RTC_VALUE_SUB,
    EVENT_RTC_VALUE_CONFORM,
    EVENT_ALARM_CLOSE,
    EVENT_ALARM_SNOOZE,
    EVENT_NUMBER_INPUT,

    EVENT_UI_VALUE_ADD,
    EVENT_UI_VALUE_SUB,
    EVENT_UI_CONFORM,
    EVENT_UI_CANCEL,

    EVENT_ENTER_MENU,

    EVENT_DUCKING_SWITCH,
    EVENT_REMOVAL_SWITCH,
    EVENT_RECORD_SWITCH,

    /******************************system message****************************/
    /*! system manager消息占位符，其中 0x0080 ~ 0x00ff 保留给系统*/
    SYSTEM_RESERVE_BEGIN = 0x007f,
    /* for qac 重定义为enum类型，消除 qac 警告 */
    MSG_RTC2HZ = MSG_SYS_RTC2HZ, /*0x80*/
    MSG_BAT_V_CHG = MSG_SYS_BAT_V_CHG, /*0x81*/
    MSG_LOW_POWER = MSG_SYS_LOW_POWER, /*0x82*/
    MSG_RTCALARM = MSG_SYS_RTCALARM, /*0x83*/
    MSG_POWER_OFF = MSG_SYS_POWER_OFF, /*0x84*/
    MSG_RECLAIM = MSG_SYS_RECLAIM, /*0x85*/

    MSG_FAST_PREVIEW_SWITCH_AP, /*0x86*/ /*快速预览切换应用*/

    MSG_USB_STICK = MSG_SYS_USB_STICK, /*0x90*/
    MSG_USB_UNSTICK = MSG_SYS_USB_UNSTICK, /*0x91*/

    MSG_ADAPTOR_IN = MSG_SYS_ADAPTOR_IN, /*0xa0*/
    MSG_ADAPTOR_OUT = MSG_SYS_ADAPTOR_OUT, /*0xa1*/

    MSG_SD_IN = MSG_SYS_SD_IN, /*0xb0*/
    MSG_SD_OUT = MSG_SYS_SD_OUT, /*0xb1*/

    MSG_UH_IN = MSG_SYS_UH_IN, /*0xc0*/
    MSG_UH_OUT = MSG_SYS_UH_OUT, /*0xc1*/

    MSG_LINEIN_IN = MSG_SYS_LINEIN_IN, /*0xd0*/
    MSG_LINEIN_OUT = MSG_SYS_LINEIN_OUT, /*0xd1*/

    //HY HDMI MSG, RESERVE 0xd4 ~ 0xef
    MSG_HDMI_POWER_ON = MSG_SYS_POWER_ON, /*0xd4*/
    MSG_HDMI_POWER_HD1 = MSG_SYS_POWER_HD1, /*0xd5*/
    MSG_HDMI_POWER_HD2 = MSG_SYS_POWER_HD2, /*0xd6*/
    MSG_HDMI_POWER_HD3 = MSG_SYS_POWER_HD3, /*0xd7*/
    MSG_HDMI_POWER_ARC = MSG_SYS_POWER_ARC, /*0xd8*/
    MSG_HDMI_MUTE_ON = MSG_SYS_MUTE_ON, /*0xd9*/
    MSG_HDMI_MUTE_OFF = MSG_SYS_MUTE_OFF, /*0xda*/
    MSG_HDMI_VOL_ADD = MSG_SYS_VOL_ADD, /*0xdb*/
    MSG_HDMI_VOL_SUB = MSG_SYS_VOL_SUB, /*0xdc*/

    MSG_HDMI_HDMI0_IN = MSG_SYS_HDMI0_IN, /*0xe0*/
    MSG_HDMI_HDMI1_IN = MSG_SYS_HDMI1_IN, /*0xe1*/
    MSG_HDMI_HDMI2_IN = MSG_SYS_HDMI2_IN, /*0xe2*/
    MSG_HDMI_ARC_IN = MSG_SYS_ARC_IN, /*0xe3*/
    MSG_HDMI_HDMI_IN = MSG_SYS_HDMI_HDMI_IN,/*0xe4*/

    MSG_HDMI_MUTE_ON_S = MSG_SYS_MUTE_ON_S, /*0xe5*/
    MSG_HDMI_MUTE_OFF_S = MSG_SYS_MUTE_OFF_S, /*0xe6*/
    //HY HDMI MSG END

    /*! 耳机（天线）插入 */
    MSG_EARPHONE_IN = 0x00f0,
    /*! 耳机（天线）拔出 */
    MSG_EARPHONE_OUT,
    /*! 充电已满消息 */
    MSG_FULL_CHARGE,
    /*! 电量较低，请充电 */
    MSG_LOW_BATTERY,
    /*! 蓝牙物理连接状态发生变化 */
    MSG_BT_CON_CHANGED,

    /*! 消息显示之后刷新LED数码管 */
    MSG_LED_REDRAW,

    /*! 切换相应的前台程序 */
    MSG_SWITCH_FRONT_APP,
    /*! 通知应用程序退出消息，应用程序接受到此消息后，进入退出流程，该消息只能由 manager.app 发送 */
    MSG_APP_QUIT,

    /*! 准备进入TTS播报 */
    MSG_TTS_START_SYNC,
    /*! TTS播报完成 */
    MSG_TTS_STOP_SYNC,

    /*! 请求播放软按键音 */
    MSG_SOFT_KEYTONE_PLAY_SYNC,

    /*! 发给引擎设置EQ参数 */
    MSG_SET_EQ_SYNC,

    /*! 保持LED数码管显示 */
    MSG_KEEP_LED_DISPLAY,

    /*! 录音开关 */
    MSG_REC_START_SYNC,
    MSG_REC_STOP_SYNC,
    MSG_REC_SAVE_RESTART_NEXT_SYNC,

    /*! 预先通知应用即将切换到后台应用（通话和闹钟起闹），让应用备份状态，以便返回时恢复状态 */
    MSG_READY_TO_BG_APP_SYNC,

    /*! 系统进入S2低功耗模式 */
    MSG_SYSTEM_ENTER_S2,
    /*! 前台发送给引擎和BT ENGINE告知进入S2低功耗模式 */
    MSG_SYSTEM_ENTER_S2_SYNC,

    /******************************manager message****************************/
    /*! process manager消息占位符*/
    MANAGER_BASE = 0x0200,
    /*! 异步创建应用程序消息 */
    MSG_CREAT_APP,
    /*! 同步创建应用程序消息 */
    MSG_CREAT_APP_SYNC,
    /*! 异步杀死应用程序消息 */
    MSG_KILL_APP,
    /*! 同步杀死应用程序消息 */
    MSG_KILL_APP_SYNC,

    /***************************Music engine message**************************/
    /*! Music engine消息占位符 */
    MENGINE_BASE = 0x0300,
    /*! 获取引擎信息，音乐播放界面定时获取引擎信息 */
    MSG_MENGINE_GET_ENGINE_INFO_SYNC,
    /*! 获取当前文件路径 */
    MSG_MENGINE_GET_FILEPATH_SYNC,
    /*! 设置播放模式 */
    MSG_MENGINE_SET_PLAYMODE_SYNC,
    /*! 设置显示模式 */
    MSG_MENGINE_SET_DISPMODE_SYNC,
    /*! 设置淡入淡出 */
    MSG_MENGINE_SET_FADE_SYNC,
    /*! 设置播放速度 */
    MSG_MENGINE_SET_PLAYSPEED_SYNC,
    /*! 播放音乐 */
    MSG_MENGINE_PLAY_SYNC,
    /*! 停止音乐 */
    MSG_MENGINE_STOP_SYNC,
    /*! 暂停音乐 */
    MSG_MENGINE_PAUSE_SYNC,
    /*! 恢复音乐 */
    MSG_MENGINE_RESUME_SYNC,
    /*! 跳到指定时间播放音乐 */
    MSG_MENGINE_SEEK_SYNC,
    /*! 设置音乐播放文件(文件选择器方式) */
    MSG_MENGINE_SET_FILEPATH_SYNC,
    /*! 设置音乐播放文件(playlist方式)  */
    MSG_MENGINE_SET_FILEINDEX_SYNC,
    /*! 切换到下一首 */
    MSG_MENGINE_PLAY_NEXT_SYNC,
    /*! 切换到上一首 */
    MSG_MENGINE_PLAY_PREV_SYNC,
    /*! 删除文件 */
    MSG_MENGINE_DELETE_FILE_SYNC,
    /*! 快进 */
    MSG_MENGINE_FFWD_SYNC,
    /*! 快退 */
    MSG_MENGINE_FBWD_SYNC,
    /*! 取消快进或快退 */
    MSG_MENGINE_CANCEL_FFB_SYNC,
    /*! 设置快进或快退步长 */
    MSG_MENGINE_SET_FFBSTEP_SYNC,
    /*! 设置快进或快退播放时间 */
    MSG_MENGINE_SET_FFBTIME_SYNC,
    /*! 设置上次播放文件 */
    MSG_MENGINE_SET_LAST_FILE_SYNC,
    /*! 清除错误状态: UI应用处理后清除 */
    MSG_MENGINE_CLEAR_ERR_SYNC,

    /*! 获取信息 */
    MSG_MENGINE_GET_ENGINE_CFG_SYNC,
    /*! 获取循环模式+shuffle */
    MSG_MENGINE_GET_PLAYMODE_SYNC,
    /*! 获取播放模式 */
    MSG_MENGINE_GET_ENGINE_PLAYMODE_SYNC,
    /*! 获取文件格式 */
    MSG_MENGINE_GET_FILEINFO_SYNC,
    /*! 获取当前播放状态 */
    MSG_MENGINE_GET_STATUS_SYNC,
    /*! 获取当前播放时间和比特率 */
    MSG_MENGINE_GET_PLAYINFO_SYNC,
    /*! 获取当前播放时间断点 */
    MSG_MENGINE_GET_BKINFO_SYNC,
    /*! 切换磁盘后插先播 */
    MSG_MENGINE_SWITCH_DISK_SYNC,
    /*! 数字点歌 */
    MSG_MENGINE_DIGITAL_SONG_SYNC,
    /*! folder切换下一首歌曲 */
    MSG_MENGINE_FOLFER_NEXT_SYNC,
    /*! folder切换上一首歌曲 */
    MSG_MENGINE_FOLFER_PREV_SYNC,

    /*! 获取id3信息 */
    MSG_MENGINE_GET_ID3INFO_SYNC,
    /*! 获取目录名信息 */
    MSG_MENGINE_GET_DIRNAME_SYNC,
    /*! 获取文件名称 */
    MSG_MENGINE_GET_FILENAME_SYNC,

    /*! 设置排序后的序号 */
    MSG_MENGINE_SET_SORTNUMBER_SYNC,

    /*! 获取当前序号 */
    MSG_MENGINE_GET_DISKNUMBER_SYNC,

    /****************************fm engine message****************************/
    /*! fm engine消息占位符*/
    FMENGINE_BASE = 0x0400,

    /* 初始化及卸载相关*/
    /* 安装FM drv ,  执行FM 引擎进程后即安装*/
    MSG_FMENGINE_OPEN,
    /* FM 驱动IC  初始化配置*/
    MSG_FMENGINE_INIT,
    /* FM 驱动IC 进入Standby 状态*/
    MSG_FMENGINE_STANDBY,

    /* 设置相关*/
    /* 设置频点播放( 异步) */
    MSG_FMENGINE_SETFREQ,
    /* 设置频点播放( 同步)  */
    MSG_FMENGINE_SETFREQ_SYNC,
    /* 设置静音或取消静音*/
    MSG_FMENGINE_SETMUTE,
    /* 设置搜台门限值*/
    MSG_FMENGINE_SETTHRED,
    /* 启动软件搜台*/
    MSG_FMENGINE_SET_SOFTSEEK,

    /* 获取信息相关*/
    /* 获取FM收听状态信息*/
    MSG_FMENGINE_GET_FM_STATUS,
    /* 获取播放状态信息 */
    MSG_FMENGINE_GET_PLAY_STATUS,
    /* 获取当前频率 */
    MSG_FMENGINE_GET_FREQ,
    /* 暂停收听 */
    MSG_FMENGINE_PAUSE_SYNC,
    /* 恢复收听 */
    MSG_FMENGINE_RESUME_SYNC,

    /****************************linein engine message****************************/
    /*! linein engine消息占位符*/
    LINEINEG_BASE = 0x0480,
    /*! 获取当前播放状态 */
    MSG_LINEINEG_GET_STATUS_SYNC,
    /*! 暂停音乐 */
    MSG_LINEINEG_PAUSE_SYNC,
    /*! 恢复音乐 */
    MSG_LINEINEG_RESUME_SYNC,

    /****************************usond engine message****************************/
    /*! usound engine消息占位符*/
    USOUNDEG_BASE = 0x04a0,
    /*! 获取当前播放状态 */
    MSG_USOUNDEG_GET_STATUS_SYNC,
    /*! 暂停音乐 */
    MSG_USOUNDEG_PAUSE_SYNC,
    /*! 恢复音乐 */
    MSG_USOUNDEG_RESUME_SYNC,
    /*!切换下一曲*/
    MSG_USOUNDEG_NEXT_SONG_SYNC,
    /*!切换上一曲*/
    MSG_USOUNDEG_PREV_SONG_SYNC,
    /*!音量加*/
    MSG_USOUNDEG_VOL_ADD_SYNC,
    /*!音量减*/
    MSG_USOUNDEG_VOL_SUB_SYNC,

    /****************************bluetooth message****************************/
    /*! bluetooth engine消息占位符 */
    BTENGINE_BASE = 0x0500,

    /*!!!***** bt manager 相关命令信息*****!!!*/

    /*! 获取蓝牙状态 信息*/
    MSG_BTENGINE_GET_STATUS_SYNC,
    /*! 获取远端设备支持的profile*/
    MSG_BTENGINE_GET_PROFILE_SYNC,
    /*! 获取编码类型 */
    MSG_BTENGINE_GET_MEDIA_TYPE_SYNC,
    /*获取蓝牙已配对列表*/
    MSG_BTENGINE_GET_PAIRED_LIST_SYNC,
    /*清除蓝牙已配对列表*/
    MSG_BTENGINE_CLR_PAIRED_LIST_SYNC,
    /*获取蓝牙设备的可见性，可连接性等信息*/
    MSG_BTENGINE_GET_VISUALMODE_SYNC,
    /*根据蓝牙地址进行自动回连(必须是已配对列表中存在的地址)*/
    MSG_BTENGINE_SET_CONNECT_ADDR_SYNC,
    /*回复最后一次连接的设备*/
    MSG_BTENGINE_SET_CONNECT_LAST_SYNC,
    /*! 设置mode (包括设置可见性，可连接性)*/
    MSG_BTENGINE_MGR_SET_VISUALMODE_SYNC,
    /*! 强制蓝牙进入等待连接状态*/
    MSG_BTENGINE_FORCE_WAITPAIRED_SYNC,
    /*! 蓝牙停止回连 */
    MSG_BTENGINE_STOP_AUTO_CONNECT_SYNC,
    /*! TWS设备停止回连手机 */
    MSG_BTENGINE_SET_TWS_DEV_RECONNECT_PHONE_POLICY_SYNC,
    /*!音量同步完成*/
    MSG_BTENGINE_VOL_UPDATE_FINISH_SYNC,
    /*!需要同步上报电池电量给手机端*/
    MSG_BTENGINE_REPORT_BAT_SYNC,
    /*!申请读取手机时间，并查询当前时间是否可读*/
    MSG_BTENGINE_READ_PHONE_CLOCK_REQ_SYNC,
    /*!读取手机时间*/
    MSG_BTENGINE_READ_PHONE_CLOCK_SYNC,
    /*! transmit a specific DTMF code */
    MSG_BTENGINE_TRANSMIT_DTMF_SYNC,
    /*BQB测试*/
    MSG_BTENGINE_BQB_TEST_SYNC,
    /*enter sniff*/
    MSG_BTENGINE_ENTER_SNIFF_SYNC,
    /*exit sniff*/
    MSG_BTENGINE_EXIT_SNIFF_SYNC,
    /*重新启动来电铃声*/
    MSG_BTENGINE_RESTART_INCOMING_CALL_RING,

    /*! START/STOP SIRI*/
    MSG_BTENGINE_SET_SIRI_SYNC,
    /*hid take photo*/
    MSG_BTENGINE_HID_TAKE_PHOTO_SYNC,
    /*hid connect*/
    MSG_BTENGINE_HID_CONNECT_SYNC,
    /*hid disconnect*/
    MSG_BTENGINE_HID_DISCONNECT_SYNC,
    /*获取电话本信息*/
    MSG_BTENGINE_GET_PB_CONTENT_SYNC,
    /* 打开蓝牙可见性 */
    MSG_BTENGINE_OPEN_BT_VISIBLE_SYNC,
    /* app端禁止进入sniff */
    MSG_BTENGINE_DISABLE_ENTER_BT_SNIFF_SYNC,
    /* app端允许进入sniff */
    MSG_BTENGINE_ENABLE_ENTER_BT_SNIFF_SYNC,
    /* 获取当前激活的已连接手机名称 */
    MSG_BTENGINE_GET_CURRENT_ACTIVE_CONNECTED_PHONE_NAME_SYNC,

    // FOR BQB
    MSG_BTENGINE_SET_TO_LIMITED_DISCOVERABLE_MODE_SYNC = 0x5b0,
    MSG_BTENGINE_SET_TO_GENERAL_DISCOVERABLE_MODE_SYNC,

    /*!!!!*****蓝牙打电话相关命令信息*****!!!*/

    /*!告诉蓝牙协议栈蓝牙免提已开始*/
    MSG_BTENGINE_ENTER_HFP_CALL_SYNC = 0x0600,
    /*!告诉蓝牙协议栈蓝牙免提已停止*/
    MSG_BTENGINE_EXIT_HFP_CALL_SYNC,
    /*! 接听电话 */
    MSG_BTENGINE_ACCEPT_CALL_SYNC,
    /*! 拒接电话 */
    MSG_BTENGINE_REJECT_CALL_SYNC,
    /*! 挂断电话 */
    MSG_BTENGINE_HANDUP_CALL_SYNC,
    /*! 拨出电话 */
    MSG_BTENGINE_OUTGOING_CALL_SYNC,
    /*! 创建sco链路*/
    MSG_BTENGINE_MGR_SETSYN_SYNC,
    /*! 断开sco链路*/
    MSG_BTENGINE_MGR_RELEASESYN_SYNC,
    /*! 拔打指定的电话号码*/
    MSG_BTENGINE_DIAL_PHONE_NUMBER_SYNC,
    /*! 蓝牙免提数据过滤控制 */
    MSG_BTENGINE_SCO_PIPE_CTRL_SYNC,
    /*3方通话处理*/
    MSG_BTENGINE_3WAY_HANDLE_SYNC,

    /*!!!!*****蓝牙推歌相关命令信息*****!!!!*/

    /*! 告诉蓝牙协议栈蓝牙推歌已播放 */
    MSG_BTENGINE_ENTER_SBC_PLAY_SYNC = 0x0680,
    /*! 告诉蓝牙协议栈蓝牙推歌已停止 */
    MSG_BTENGINE_EXIT_SBC_PLAY_SYNC,
    /*! 播放/暂停音乐 */
    MSG_BTENGINE_PLAY_PAUSE_SYNC,
    /*! 切换到下一首 */
    MSG_BTENGINE_PLAY_NEXT_SYNC,
    /*! 切换到上一首 */
    MSG_BTENGINE_PLAY_PREV_SYNC,
    /*! 同步音箱端的音量到手机端 */
    MSG_BTENGINE_SET_VOLUME_SYNC,
    /*! 蓝牙推歌数据过滤控制 */
    MSG_BTENGINE_PLAY_PIPE_CTRL_SYNC,
    /*快进*/
    MSG_BTENGINE_FFWD_SYNC,
    /*快退*/
    MSG_BTENGINE_FBWD_SYNC,
    /*取消快进*/
    MSG_BTENGINE_FFWD_CANCEL_SYNC,
    /*取消快退*/
    MSG_BTENGINE_FBWD_CANCEL_SYNC,
    /*切换AVDTP数据源*/
    MSG_BTENGINE_SWITCH_AVDTP_SOURCE_SYNC,

    /*!!!***** spp/ble 数传模块相关命令信息*****!!!*/
    /*! 创建SPP连接命令 */
    MSG_BTENGINE_SPP_CREATE_CONNECTION_SYNC,
    /*! 断开SPP连接命令 */
    MSG_BTENGINE_SPP_DISCON_CONNECTION_SYNC,

    /*! 创建BLE透传服务命令 */
    MSG_BTENGINE_CREATE_BLE_PASSTHROUGH_SYNC,
    /*! 删除BLE透传服务命令 */
    MSG_BTENGINE_DELETE_BLE_PASSTHROUGH_SYNC,
     /*! BLE透传服务数据发送命令 */
    MSG_BTENGINE_BLE_PASSTHROUGH_DATA_SEND_SYNC,
     /*! BLE透传服务数据读取命令 */
    MSG_BTENGINE_BLE_PASSTHROUGH_DATA_READ_SYNC,
     /*! BLE透传服务状态查询读取命令 */
    MSG_BTENGINE_BLE_PASSTHROUGH_GET_INFO_SYNC,

    /*! 创建BLE透传服务命令 */
    MSG_BTENGINE_CREATE_SPP_PASSTHROUGH_SYNC,
    /*! 删除BLE透传服务命令 */
    MSG_BTENGINE_DELETE_SPP_PASSTHROUGH_SYNC,
     /*! BLE透传服务数据发送命令 */
    MSG_BTENGINE_SPP_PASSTHROUGH_DATA_SEND_SYNC,
     /*! BLE透传服务数据读取命令 */
    MSG_BTENGINE_SPP_PASSTHROUGH_DATA_READ_SYNC,
     /*! BLE透传服务状态查询读取命令 */
    MSG_BTENGINE_SPP_PASSTHROUGH_GET_INFO_SYNC,

    /***************自动化测试附加的命令消息*******************/
    /*! auto test消息占位符 */
    AUTOTEST_BASE = 0x0700,
    /*! 获取蓝牙名称，地址等配置值 */
    MSG_AUTOTEST_GET_BTINFO_SYNC,
    /*! 修改蓝牙名称，地址等配置值 */
    MSG_AUTOTEST_SET_BTINFO_SYNC,
    /*! 设置回连属性消息*/
    MSG_AUTOTEST_SET_RECONNECT_SYNC,
    /*！获取逻辑按键属性消息*/
    MSG_GET_LOGIC_KEY_VALUE_SYNC,
    /*！开始蓝牙测试消息*/
    MSG_AUTOTEST_START_BTTEST_SYNC,
    /*！开始蓝牙RF测试消息*/
    MSG_BTENGINE_RF_TEST_SYNC,
    /*！断开所有连接消息*/
    MSG_AUTOTEST_RELEASE_CONNECT_SYNC,

    /*! 设置可见性 */
    MSG_BTENGINE_MGR_SET_DISCOVERABLE_SYNC,
    /*! 主动连接AVDTP */
    MSG_BTENGINE_AVDTP_CONNECT_SYNC,
    /*! 主动断开AVDTP */
    MSG_BTENGINE_AVDTP_DISCONNECT_SYNC,
    /*! 主动连接HFP */
    MSG_BTENGINE_HFP_CONNECT_SYNC,
    /*! 主动断开HFP */
    MSG_BTENGINE_HFP_DISCONNECT_SYNC,
    /*! 清除错误号*/
    MSG_BTENGINE_CLEAR_ERR_SYNC,
    /*! 进行频偏测试时,将命令往host端发送*/
    MSG_BTENGINE_SET_ATT_CMD_SYNC,
    /*! 进行频偏测试时,获取host端的响应信息*/
    MSG_BTENGINE_GET_ATT_RSP_SYNC,
    /*! 频偏测试测试结束*/
    MSG_BTENGINE_ATT_TEST_END_SYNC,
    /*! 添加设备到配对列表中*/
    MSG_BTENGINE_ADD_DEV_TO_PAIRED_LIST_SYNC,
    /*! auto test消息结束符 */
    AUTOTEST_END,

    /***************蓝牙TWS相关命令消息*******************/
    MSG_BTENGINE_TWS_BASE = 0x780,

    MSG_BTENGINE_TWS_LINK_SYNC,
    MSG_BTENGINE_TWS_FORCE_UNLINK_SYNC,

    MSG_BTENGINE_TWS_GET_BT_CBK_SYNC,
    MSG_BTENGINE_TWS_GET_SEND_FRAME_CBK_SYNC,
    MSG_BTENGINE_TWS_SET_SCAN_MODE_SYNC,

    /***************蓝牙免提的命令消息*******************/
    MSG_BTCALLEG_BASE = 0x800,
    MSG_BTCALLEG_GET_STATUS_SYNC,
    MSG_BTCALLEG_SPEECH_PLAY_SYNC,
    MSG_BTCALLEG_SPEECH_STOP_SYNC,

    /***************蓝牙免提的命令消息*******************/
    MSG_BTPLAYEG_BASE = 0x900,
    MSG_BTPLAYEG_GET_STATUS_SYNC,
    //TWS暂停和恢复消息
    MSG_BTPLAYEG_TWS_PAUSE_SYNC,
    MSG_BTPLAYEG_TWS_RESUME_SYNC,
    //TWS确定单箱应用模式或对箱应用模式
    MSG_BTPLAYEG_TWS_CONFORM_MODE_SYNC,

    /****************************** play scene 事件 ****************************/
    /*! 播放场景事件占位 */
    EVENT_PLAYING_BASE = 0x2000,
    /*! 快进事件 */
    EVENT_PLAYING_FFOREWORD,
    /*! 快退事件 */
    EVENT_PLAYING_FBACKWORD,
    /*! 下一曲 or AB 事件 */
    EVENT_PLAYING_NEXTSONG,
    /*! 上一曲事件 */
    EVENT_PLAYING_PREVSONG,
    /*! option事件 */
    EVENT_PLAYING_OPTION,
    /*! 播放/暂停事件 */
    EVENT_PLAYING_PAUSE,
    /*! EQ设置事件 */
    EVENT_EQ_SET,
    /*! 向下切EQ事件 */
    EVENT_EQ_NEXT,
    /*! 向上切EQ事件 */
    EVENT_EQ_PREV,
    /*! 设置EQ时忽略事件 */
    EVENT_EQ_IGNORE,
    /*! 循环模式设置事件 */
    EVENT_LOOPMODE_SET,
    /*! mute开关事件 */
    EVENT_MUTE_SWITCH,
    /*! 模式(mode short)切换事件 */
    EVENT_MODE_SWITCH,
    /*! 切换到时间显示事件 */
    EVENT_ENTER_TIMESHOW,
    /*! 切换到时间设置事件 */
    EVENT_ENTER_TIMESET,
    /*! folder加事件 */
    EVENT_FOLDER_ADD,
    /*! folder减事件 */
    EVENT_FOLDER_SUB,
    /*! 数字点歌事件 */
    EVENT_DIGITAL_SONG,
    /*! 返回事件 */
    EVENT_PLAYING_RETURN,

    /*! 强制蓝牙进入等待连接事件*/
    EVENT_FORCE_WAITPAIRED,
    /*! 强制连接切换：断开连接或回连最后一个设备*/
    EVENT_SWITCH_CONNECT,
    /*! 拨打最近一次电话 */
    EVENT_CALL_LASTPHONE,
    /*! 蓝牙HID拍照 */
    EVENT_HID_TAKE_PHOTO,
    /*! HID BQB KEY */
    EVENT_HID_BQB_PREV,
    EVENT_HID_BQB_NEXT,

    /*! 发起TWS组队连接 */
    EVENT_TWS_PAIR_LINK,
    /*! 发起TWS组队断开 */
    EVENT_TWS_PAIR_UNLINK,
    /*! 切换TWS左右箱体 */
    EVENT_TWS_SWITCH_POS,
    /*! 打开蓝牙可见性 */
    EVENT_OPEN_BT_VISIBLE,

    /*! 按键进入SIRI */
    EVENT_SIRI_START,

    /****************************** linein 事件 ****************************/
    /*! linein事件占位 */
    EVENT_LINEIN_BASE = 0x2100,
    /*!line in next*/
    EVENT_LINEIN_NEXT,
    /*!line in prev*/
    EVENT_LINEIN_PREV,
    /*!line in play*/
    EVENT_LINEIN_PLAY,
    /*! 录音PLAY事件 */
    EVENT_RECORD_PLAY_PAUSE,
    /*! 录音保存事件 */
    EVENT_RECORD_SAVE,

    /****************************** radio 事件 ****************************/
    /*! radio 事件占位 */
    EVENT_RADIO_BASE = 0x2200,
    /*! 切换到上一个频率点事件 */
    EVENT_RADIO_PREV_FREQ,
    /*! 切换到下一个频率点事件 */
    EVENT_RADIO_NEXT_FREQ,
    /*! 切换到上一个预存电台事件 */
    EVENT_RADIO_PREV_PRESET,
    /*! 切换到下一个预存电台事件 */
    EVENT_RADIO_NEXT_PRESET,
    /*! 向前自动搜台事件*/
    EVENT_RADIO_SEEK_PREV,
    /*! 向后自动搜台事件 */
    EVENT_RADIO_SEEK_NEXT,
    /*! 自动搜台事件 */
    EVENT_RADIO_AUTO_SEARCH,
    /*! 暂停收听/恢复收听事件 */
    EVENT_RADIO_PLAY_PAUSE,
    /*! 退出搜台场景 */
    EVENT_RADIO_SEARCH_EXIT,
    /*! 忽略某些按键消息 */
    EVENT_RADIO_SEARCH_IGNORE,

    /****************************** record 播放事件 ****************************/
    EVENT_RECPLAY_BASE = 0x2600,
    /*! MODE事件 */
    EVENT_RECPLAY_MODE,
    /*! 从头开始播放事件 */
    EVENT_RECPLAY_PLAY,
    /*! 退出播放返回录音事件 */
    EVENT_RECPLAY_REC,
    /*! 魔音选择模式(变音的模式)*/
    EVENT_RECPLAY_SELMODE_ADD,
    /*! 魔音选择模式(变音的模式)*/
    EVENT_RECPLAY_SELMODE_SUB,

} msg_apps_type_e;

#endif
