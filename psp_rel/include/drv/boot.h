/********************************************************************************
*                              USDK281A
*                            Module: REGDEF
*                 Copyright(c) 2003-2016 Actions Semiconductor,
*                            All Rights Reserved.
*
* History:
*      <author>    <time>           <version >             <desc>
*      zhouxl     2015-9-10 15:00     1.0             build this file
********************************************************************************/
/*!
 * \file     boot.h
 * \brief    引导代码接口声明
 * \author   zhouxl
 * \version 1.0
 * \date  2015/9/10
*******************************************************************************/

#ifndef __BOOT_H__
#define __BOOT_H__

#include "types.h"

#ifndef _ASSEMBLER_

typedef enum
{
    ADFU_LAUNCHER = 3,
    CARD_LAUNCHER = 6,
}boot_op_cmd_e;


/*
    BOOT 向外提供多个函数指针, 以数组形式存放起来, 以下是这些函数指针地址
 */
#define BOOT_ENTRY_START                (0xBFC00290)
#define BOOT_INTERFACE_ENTRY_ADDR(x)    (BOOT_ENTRY_START + (4 * x))

#define ADFU_LAUNCHER_ENTRY             (*((uint32*)(BOOT_INTERFACE_ENTRY_ADDR(ADFU_LAUNCHER))))
#define CARD_LAUNCHER_ENTRY             (*((uint32*)(BOOT_INTERFACE_ENTRY_ADDR(CARD_LAUNCHER))))


/*
    BOOT 函数调用接口,通过函数指针方式调用
*/
#define boot_adfu_launcher()            ((void(*)(void))ADFU_LAUNCHER_ENTRY)()
#define boot_card_launcher()            ((void(*)(void))CARD_LAUNCHER_ENTRY)()

#endif
#endif  /*#ifndef __BOOT_H__*/


