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
 * \file     act_stdio.h
 * \brief    类型定义
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/
#ifndef _ACT_STDIO_H
#define _ACT_STDIO_H

#include <ucos_types.h>

#ifndef _ASSEMBLER_

//定义文件SEEK的方式
#define SEEK_SET    0   // 从文件首往文件尾定位，offset为正数
#define SEEK_CUR    1   //从当前位置往文件头或尾定位；正数表示向文件尾seek，负数表示向文件头seek
#define SEEK_END    2   // 从文件尾往文件首定位，offset为负数

#endif/* _ASSEMBLER_ */

#endif  /* _ACT_STDIO_H*/
