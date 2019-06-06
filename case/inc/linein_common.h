/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：LINEIN播歌相关宏定义，枚举类型，结构体定义等，为LINEIN播歌前台和引擎共享。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef __LINEIN_COMMON_H__
#define __LINEIN_COMMON_H__

#include "psp_includes.h"
#include "record_common.h"

#define SUPPORT_AUX_AA_STEREO  1 //使用是否AIN-PA直通实现立体声

#define AUX_MODE_ADDA          0 //先混合，ADC后数字处理再DAC出去，不支持立体声
#define AUX_MODE_AA_STEREO     1 //AIN-PA直通立体声

#define ENTER_AUX_CHAN         0 //AUX通道

#define ENTER_MIC_CHAN_C       1 //MIC通道 TF
#define ENTER_MIC_CHAN_U       2 //MIC通道 U盘

#define ENTER_SPDIF_CHAN       3 //SPDIF通道
#define ENTER_I2SRX_CHAN       4 //I2SRX通道
#define ENTER_HDMI_CHAN        5 //HDMI通道
#define ENTER_HDMI1_CHAN       6 //HDMI1通道
#define ENTER_HDMI2_CHAN       7 //HDMI2通道
#define ENTER_ARC_CHAN         8 //ARC通道

/*!
 * \brief
 *  当前播放状态
 */
typedef enum
{
    /*! 停止 */
    StopSta = 0,
    /*! 暂停 */
    PauseSta,
    /*! 播放 */
    PlaySta,
} play_status_e;

/*!
 * \brief
 *  引擎状态
 */
typedef struct
{
    /*! 播放状态  */
    play_status_e play_status;
    /*! 录音状态 */
    record_status_e record_sta;
    /*! 低能量持续时间，持续一段时间后可以进入省电模式 */
    int32 lowpw_time;
    /*! 当前的录音时间*/
    int32 rec_time;
} lineineg_status_t;

#endif
