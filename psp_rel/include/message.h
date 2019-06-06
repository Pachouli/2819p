/********************************************************************************
 *                              USDK(GL5110)
 *                            Module: KERNEL
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>      <time>           <version >             <desc>
 *      wuyueqian     2011-9-10 09:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     message.h
 * \brief    消息出来接口
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#define MSG_SYS_RTC2HZ               (0x0080)
#define MSG_SYS_BAT_V_CHG            (0x0081)
#define MSG_SYS_LOW_POWER            (0x0082)
#define MSG_SYS_RTCALARM             (0x0083)
#define MSG_SYS_POWER_OFF            (0x0084)  /*! 关机消息, 等待前台应用和后台应用退出后启动 config 应用进入关机流程 */
#define MSG_SYS_RECLAIM              (0x0085)

#define MSG_SYS_USB_STICK            (0x0090)
#define MSG_SYS_USB_UNSTICK          (0x0091)

#define MSG_SYS_ADAPTOR_IN           (0x00a0)  /*! 充电器 ADAPTOR 插入消息 */
#define MSG_SYS_ADAPTOR_OUT          (0x00a1)  /*! 充电器 ADAPTOR 拔出消息 */

#define MSG_SYS_SD_IN                (0x00b0)  /*插卡状态*/
#define MSG_SYS_SD_OUT               (0x00b1)  /*拨卡状态*/

#define MSG_SYS_UH_IN                (0x00c0)    /*U盘插入*/
#define MSG_SYS_UH_OUT               (0x00c1)    /*U盘拔出*/

#define MSG_SYS_LINEIN_IN            (0x00d0)  /*LineIn插入*/
#define MSG_SYS_LINEIN_OUT           (0x00d1)  /*LineIn拔出*/

//HY HDMI MSG, RESERVE 0xd4 ~ 0xef
#define MSG_SYS_POWER_ON             (0x00d4) /*! power on from CEC, and stay last play   */
#define MSG_SYS_POWER_HD1            (0x00d5) /*! power on from CEC, and enter into HDMI1 */
#define MSG_SYS_POWER_HD2            (0x00d6) /*! power on from CEC, and enter into HDMI2 */
#define MSG_SYS_POWER_HD3            (0x00d7) /*! power on from CEC, and enter into HDMI3 */
#define MSG_SYS_POWER_ARC            (0x00d8) /*! power on from CEC, and enter into ARC   */
#define MSG_SYS_MUTE_ON              (0x00d9) /*! mute on from CEC, and display           */
#define MSG_SYS_MUTE_OFF             (0x00da) /*! mute off from CEC, and display          */
#define MSG_SYS_VOL_ADD              (0x00db) /*! vol+ from CEC                           */
#define MSG_SYS_VOL_SUB              (0x00dc) /*! vol- from CEC                           */

#define MSG_SYS_HDMI0_IN             (0x00e0) /*! switch from CEC, and enter into HDMI1   */
#define MSG_SYS_HDMI1_IN             (0x00e1) /*! switch from CEC, and enter into HDMI2   */
#define MSG_SYS_HDMI2_IN             (0x00e2) /*! switch from CEC, and enter into HDMI3   */
#define MSG_SYS_ARC_IN               (0x00e3) /*! switch from CEC, and enter into ARC     */
#define MSG_SYS_HDMI_HDMI_IN         (0x00e4)

#define MSG_SYS_MUTE_ON_S            (0x00e5) /*! mute on from CEC, but don't display     */
#define MSG_SYS_MUTE_OFF_S           (0x00e6) /*! mute off from CEC, but don't display    */
//HY HDMI MSG END

#ifndef _ASSEMBLER_
#endif

#endif




