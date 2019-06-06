
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
 * \file     TASK_MANAGER.h
 * \brief    APπ‹¿Ì
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/

#ifndef __TASK_MANAGER_H__
#define __TASK_MANAGER_H__

/* manager  */
#define AP_PROCESS_MANAGER_PRIO         (0xD)

/* ap front control*/
#define AP_FRONT_LOW_PRIO               (0xA)

/* ap back control*/
#define AP_BACK_LOW_PRIO                (0x6)

/* codec main */
#define AP_BACK_HIGH_PRIO               (0x3)

/* record write file */
#define AP_BACK_HIGH_REC_FS_PRIO        (0x5)

/* ok mic */
#define AP_BACK_HIGH_OK_MIC_PRIO        (0x2)

/* bt engine*/
#define AP_BTENGINE_PRIO                (0x7)

/* bt controller*/
#define AP_BTCONTROLLER_HIGH_PRIO       (0x1)

#define AP_BTCONTROLLER_LOW_PRIO        (0x4)

/* ap front sub thread*/
#define AP_FRONT_HIGH_PRIO              (0x9)


#endif

