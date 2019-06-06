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
 * \file     sysdef.h
 * \brief    ?�̨�3?����?
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/
#ifndef _SYSDEF_H
#define _SYSDEF_H




//#define DEBUG_GPIO_IRQ_THREAD           1

#define FPGA                            (0)     //TEST FPGA�汾
#define IC_EVB                          (1)     //IC EVB�汾

#define SYS_BOARD_TYPE                  (IC_EVB)

#define UART_TX_GPIO3                   0
#define UART_TX_GPIO13                  1
#define UART_TX_PIN_SEL                 UART_TX_GPIO13

//for system profile
#define PROFILE_DISABLE                 (0)
#define UART_PROFILE                    (1)
#define USB_PROFILE                     (2)
#define PROFILE_SWITCH                  (PROFILE_DISABLE)


//BT test device type
#define BT_TEST_DEV_DUT                 (0)
#define BT_TEST_DEV_UPT                 (1)
#define BT_TEST_DEV_MODE                (BT_TEST_DEV_DUT)

#ifndef _ASSEMBLER_
#define BACKDOOR_DISUSB_DETECT          (1 << 6)
#define BACKDOOR_DISUHOST_DETECT        (1 << 7)
#define BACKDOOR_USB_PLUGIN             (1 << 8)
#define BACKDOOR_UHOST_PLUGIN           (1 << 9)
#define BACKDOOR_USB_STATUS_CHANGED     (1 << 10)
#define BACKDOOR_SYSCLK_LOCKED          (1 << 11)
#define BACKDOOR_DISCARD_DETECT         (1 << 12)




#define  SAVE_BT_NONE (0)
#define  SAVE_BT_ALL  (1)
#define  SAVE_BT_NAME (2)
#define  SAVE_BT_ADDR (3)
#endif  /*_ASSEMBLER_*/

#endif  /*_SYSDEF_H*/



