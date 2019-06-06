/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙引擎公共头文件，相关宏定义等。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _CASE_TYPE_H_
#define _CASE_TYPE_H_

#include "psp_includes.h"
#include "case_independent.h"
#include "mmm_mp.h"

/*!
 * \brief
 *  app_type_e 应用类型枚举类型
 */
typedef enum
{
    /*! 进程管理器 */
    APP_TYPE_PMNG = 0,
    /*! 前台应用（gui进程） */
    APP_TYPE_GUI = 1,
    /*! 后台应用（console进程） */
    APP_TYPE_CONSOLE = 2,
    /*! 后台应用（BTENGINE进程） */
    APP_TYPE_BTENGINE = 3,
} app_type_e;

/*!
 * \brief
 *  方案应用ID，最多支持32个应用，ID对应 app_name_ram 中的名字。
 */
#define APP_ID_MUSIC                (0x00)
#define APP_ID_LINEIN               (0x01)
#define APP_ID_RADIO                (0x02)
#define APP_ID_BTPLAY               (0x03)
#define APP_ID_BTCALL               (0x04)
#define APP_ID_USOUND               (0x05)
#define APP_ID_MANAGER              (0x06)
#define APP_ID_CONFIG               (0x07)
#define APP_ID_UDISK                (0x08)
#define APP_ID_MENGINE              (0x09)
#define APP_ID_LINEIN_EG            (0x0a)
#define APP_ID_FMENGINE             (0x0b)
#define APP_ID_BTPLAY_EG            (0x0c)
#define APP_ID_BTCALL_EG            (0x0d)
#define APP_ID_USOUND_EG            (0x0e)
#define APP_ID_BTENGINE             (0x0f)
#define APP_ID_UPGRADE              (0x10)
#define APP_ID_TEST                 (0x11)
#define APP_ID_BTHID                (0x12)
#define APP_ID_RECORD               (0x13)
#define APP_ID_ALARM                (0x14)
#define APP_ID_MAX                  (0x15)

#define APP_ID_THEENGINE            (0x80) //泛指引擎

/*!
 * \brief
 *  方案功能ID，最多支持32个功能
 */
//蓝牙推歌
#define APP_FUNC_BTPLAY             (0)
//播放卡
#define APP_FUNC_PLAYCARD           (1)
//播放linein
#define APP_FUNC_PLAYLINEIN         (2)
//fm radio
#define APP_FUNC_RADIO              (3)
//usb音箱
#define APP_FUNC_USOUND             (4)
//播放UHOST
#define APP_FUNC_PLAYUHOST          (5)
//u盘
#define APP_FUNC_UDISK              (6)
//蓝牙免提
#define APP_FUNC_BTCALL             (7)
//confige
#define APP_FUNC_CONFIG             (8)
//升级
#define APP_FUNC_UPGRADE            (9)
//自动化测试
#define APP_FUNC_TEST               (10)
//拍照应用
#define APP_FUNC_PHOTO              (11)
//卡录音
#define APP_FUNC_CARD_REC           (12)
//U盘录音
#define APP_FUNC_UHOST_REC          (13)
//卡回放
#define APP_FUNC_CARD_REPLAY        (14)
//U盘回放
#define APP_FUNC_UHOST_REPLAY       (15)
//播放主盘
#define APP_FUNC_PLAYMDISK          (16)
//闹铃播放应用
#define APP_FUNC_ALARM              (18)
#define APP_FUNC_ALARM_SDCARD       (19)
#define APP_FUNC_ALARM_UHOST        (20)
//SPDIF播放
#define APP_FUNC_SPDIF_PLAY         (21)
//I2SRX播放
#define APP_FUNC_I2SRX_PLAY         (22)
//HDMI播放
#define APP_FUNC_HDMI_HDMI          (23)
#define APP_FUNC_HDMI_HDMI1         (24)
#define APP_FUNC_HDMI_HDMI2         (25)
#define APP_FUNC_HDMI_ARC           (26)

#define APP_FUNC_MAX                (32)

#define LINEIN_MODE_AUX        0 //模拟输入

#define LINEIN_MODE_MIC_C      1 //MIC录音到卡
#define LINEIN_MODE_MIC_U      2 //MIC录音到U盘

#define LINEIN_MODE_SPDIF      3 //数字输入
#define LINEIN_MODE_I2SRX      4 //I2SRX输入
#define LINEIN_MODE_HDMI       5 //HDMI输入
#define LINEIN_MODE_HDMI1      6 //HDMI1输入
#define LINEIN_MODE_HDMI2      7 //HDMI2输入
#define LINEIN_MODE_ARC        8 //HDMI ARC输入

/*!
 * \brief
 *  应用后缀名 bitmap，bit<31> = 0h，与后缀名数组对应关系：bit[30] 对应后缀名数组 rom_ext_name 首元素，即 MP3。
 */
#if (SUPPORT_APE_TYPE == 1)
#define MUSIC_APE_BITMAP            EXT_NAME_APE
#else
#define MUSIC_APE_BITMAP            0
#endif

#if (SUPPORT_FLAC_TYPE == 1)
#define MUSIC_FLAC_BITMAP           EXT_NAME_FLAC
#else
#define MUSIC_FLAC_BITMAP           0
#endif

#define MUSIC_BITMAP                (EXT_NAME_MP3|EXT_NAME_WMA|EXT_NAME_WAV|EXT_NAME_MP1|EXT_NAME_MP2|MUSIC_APE_BITMAP|MUSIC_FLAC_BITMAP)
#define RECORD_BITMAP               (EXT_NAME_MP3|EXT_NAME_WAV)
#define ALLALL_BITMAP               (EXT_NAME_ALL)
#define ALLFILE_BITMAP              (EXT_NAME_ALL_FILE)
#define ALLDIR_BITMAP               (EXT_NAME_ALL_DIR)
#define RECORD_WAV_BITMAP           (EXT_NAME_WAV)
#define RECORD_MP3_BITMAP           (EXT_NAME_MP3)


/*!
 * \brief
 *  app_result_e 应用层接口返回结果枚举类型
 */
typedef enum
{
    /*! 没有任何需要通知调用者的情况返回，调用者不需要做任何特殊处理 */
    RESULT_NULL = 0x00,
    /*! 忽略返回，类似于RESULT_NULL，调用者不需要做任何特殊处理*/
    RESULT_IGNORE,
    /*! 重绘UI返回，通常返回后需要重新绘制当前UI，但不需要退出当前消息循环*/
    RESULT_REDRAW,
    /*! 应用退出消息 */
    RESULT_APP_QUIT,
    /*! 其他错误返回*/
    RESULT_ERROR,

    _RESERVE_FOR_COMMON_RESULT_ = 0x8000,

    /*! 音乐文件播放卡文件消息*/
    RESULT_MUSIC_CPLAY,
    /*! 音乐文件播放u盘文件消息*/
    RESULT_MUSIC_UPLAY,
    /*! 音乐文件播放主盘文件消息*/
    RESULT_MUSIC_MDISKPLAY,
    /*! 进入line in */
    RESULT_ENTER_LINEIN,
    /*! 进入SPDIF播歌 */
    RESULT_ENTER_SPDIF,
    /*! 进入HDMI播歌 */
    RESULT_ENTER_HDMI,
    RESULT_ENTER_HDMI1,
    RESULT_ENTER_HDMI2,
    RESULT_ENTER_HDMI_ARC,
    /*! 进入I2SRX播歌 */
    RESULT_ENTER_I2SRX,
    /*!RADIO进入消息*/
    RESULT_RADIO,
    /*! 进入蓝牙音箱 */
    RESULT_BLUETEETH_SOUND,
    /*! 进入蓝牙HFP打电话 */
    RESULT_BLUETEETH_HFP,
    /*! 进入通话调试模式 */
    RESULT_ENTER_ASQT_TEST,
    /*! 进入usb音箱 */
    RESULT_USB_SOUND,
    /*! 按关机键关机*/
    RESULT_POWER_OFF,
    /*! 低电关机*/
    RESULT_LOW_POWER,
    /*! 闹钟响应 */
    RESULT_ENTER_ALARM,
    /*! 闹钟响应完自动关机 */
    RESULT_ALARM_POWER_OFF,

    /*! 通过ATD模式进入测试AP */
    RESULT_TESTAP_ATD_ENTER,
    /*! 通过插卡模式进入测试AP */
    RESULT_TESTAP_CARD_ENTER,

    /*! ESD RESET 断点恢复*/
    RESULT_ESD_RESET,

    /*! 快速预览切换应用，func id存放在 g_p_global_var->fp_switch_func_index */
    RESULT_FAST_PREVIEW_SWITCH_AP,

    /*! 进入RTC时钟显示 */
    RESULT_ENTER_RTC_TIME,
    RESULT_BACK_RTC_TIME,
    RESULT_SWITCH_RTC_SCENE,

    //定义因为与ap切换相关，请不要随意调换位置，终止
    /*!上一次播放进入消息*/
    RESULT_LASTPLAY,
    /*!下一个功能选项*/
    RESULT_NEXT_FUNCTION,
    /*! 进入upgrade*/
    RESULT_ENTER_CARD_UPGRADE,
    RESULT_ENTER_UHOST_UPGRADE,
    /*! 进入本地磁盘目录1*/
    RESULT_ENTER_FOLDER1,
    /*! 进入本地磁盘目录2*/
    RESULT_ENTER_FOLDER2,

    /*! USB选项选择数据传输模式 */
    RESULT_USB_TRANS,
    /*! 进入空闲充电模式 */
    RESULT_ENTER_CHARGE_LOOP,
    /*! 仅关机，不播报TTS */
    RESULT_JUST_POWER_OFF,

    /*! 进入拍照应用 */
    RESULT_ENTER_PHOTO,

    /* 开始FM 播放，直接进入播放场景*/
    RESULT_RADIO_START_PLAY,
    /* 进入自动搜台场景*/
    RESULT_RADIO_AUTO_SEARCH,
    /* 进入手动搜台场景-向后搜索*/
    RESULT_RADIO_MANUAL_UP_SEARCH,
    /* 进入手动搜台场景-向前搜索*/
    RESULT_RADIO_MANUAL_DOWN_SEARCH,

    RESULT_ENTER_CARD_REC,
    RESULT_ENTER_UHOST_REC,
    RESULT_ENTER_CARD_REPLAY,
    RESULT_ENTER_UHOST_REPLAY,
    RESULT_ENTER_REC_REWRITE_FILE,
    RESULT_MAIN_APP,
    /*! 场景调度退出 */
    RESULT_SCENE_EXIT,

    RESULT_UI_CONFIRM,
    RESULT_UI_CANCEL,
} app_result_e;

/*! 后台应用（通话和闹钟起闹）状态*/
typedef enum
{
    BG_STATUS_NULL = 0, //非后台应用状态
    BG_STATUS_IN = 1,   //切换到后台应用
    BG_STATUS_BACK = 2, //从后台应用返回
} background_app_status_e;

#define ESD_BK_FLAG  0x3456
//ESD断点实时备份结构体
typedef struct
{
    uint16 flag;           //0x3456 表示有效，开始之后调到功能应用就会置为有效标志了
    /*NOTE! 以下变量位置不能调整，该变量会在welcome赋值，然后在应用层读取 */
    uint8  force_enable_udisk; //强制使能读卡器模式，即使配置项配置为不支持也可以进入，1表示强制使能
    uint8  reset_flag;     //ESD重启恢复断点标志，0表示正常进入，1表示ESD重启进入
    uint8  app_id;         //ESD重启时前台应用ID
    uint8  func_id;        //ESD重启时前台功能ID
    uint8  func_index;     //ESD重启时前台功能在ap_switch_info的下标，在 com_ap_switch_deal 中使用
    uint8  app_para;       //ESD重启时前台应用创建参数
    uint8  app_state;      //ESD重启时前台应用状态
    uint8  app_scene;      //ESD重启时前台场景
    uint8  engine_id;      //ESD重启时引擎应用ID
    uint8  engine_para;    //ESD重启时引擎应用创建参数
    uint8  engine_state;   //ESD重启时引擎应用状态
    uint8  play_status;    //ESD重启时引擎播放状态
    uint8  bt_state;       //ESD重启时蓝牙是否在工作
    uint8  bt_info_state;  //ESD重启时蓝牙状态机状态
    uint8  volume_mode;    //ESD重启时音量模式
    uint8  volume_value;   //ESD重启时音量值
    uint8  volume_value_call; //ESD重启时打电话音量值
    uint8  eq_type;        //ESD重启时EQ类型


    /*NOTE! 如果需要增加变量，只能在 bk_info 之前加，因为 bk_info 是一个不定长数组，后面的空间可能会被使用的 */
    uint32 bk_info[3];     //ESD重启时实时断点信息，用uint32是为了确保word对齐，各应用实际使用时请用强制类型转换为合适的断点结构体
    /*NOTE! 不能在 bk_info 之后增加变量，因为 bk_info 是一个不定长数组，后面的空间会被使用的 */
} esd_bk_infor_t; //max length is 0x50 bytes

//指向ESD断点实时备份缓冲区的指针
#define g_p_esd_bk_info ((esd_bk_infor_t *)(ESD_BK_INFOR_ADDR))

//应用层全局变量区，该片内存会在 common_init 进行 memset
typedef struct
{
    uint8  bg_app_sta;         //后台应用状态
    uint8  play_status_before; //记录切换后台应用（通话和闹钟起闹）之前的播放状态，以便返回时恢复状态
    uint8  only_hid_mode;      //仅作为HID设备，独立拍照应用模式
    uint8  att_flag;           //自动化测试标志
    uint8  slave_btplay_flag;  //副箱是否处于蓝牙推歌模式
    uint8  update_btplay_flag; //主箱是否收到副箱上报更新BTPLAY状态事件
    uint8  alarm_poweron_flag; //闹钟唤醒开机，关闭闹钟后自动软关机
    uint8  fp_switch_func_index;//快速预览切换应用func index
    uint8  fp_switch_jump_welcome;//快速预览切换应用，已经显示welcome界面，无需再显示
    uint16 record_result;      //录音返回后跳转应用
    uint32 display_time;       //从后台应用返回时显示时间点

    uint8 *pointer_1; //仅仅是例子，标案没有使用，可删除；需要大块内存，则可以从堆申请，保存地址即可使用。pointer_1 = sys_malloc(0x100);
} global_var_t; //max length is 0x30 bytes

//指向全局变量区的指针
#define g_p_global_var ((global_var_t *)(GLOBAL_VAR_ADDR))

#endif
