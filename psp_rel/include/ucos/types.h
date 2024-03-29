/********************************************************************************
 *                              USDK(1100)
 *                            Module: TYPES
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     wuyueqian     2011-9-10 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     types.h
 * \brief    数据类型定义
 * \author   wuyueqian
 * \version 1.0
 * \date  2011-9-10
 *******************************************************************************/
#ifndef   __OS_TYPES_H__
#define   __OS_TYPES_H__

//#include "libc/types.h"

#ifndef _ASSEMBLER_

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef char s8;
typedef short s16;
typedef int s32;

typedef char __s8;
typedef unsigned char __u8;

typedef short __s16;
typedef unsigned short __u16;

typedef int __s32;
typedef unsigned int __u32;

#ifndef PC
typedef long long s64;
typedef unsigned long long u64;
#else
typedef _int64 s64;
typedef _int64 u64;
#endif

typedef __u16 __le16;
typedef __u16 __be16;
typedef __u32 __le32;
typedef __u32 __be32;

typedef u32 size_t;

typedef u32 dev_t;
typedef u64 sector_t;

//#include <asm-mips/config.h>
#define BITS_PER_LONG _MIPS_SZLONG

#endif /*_ASSEMBLER_*/
#endif /* __OS_TYPES_H__ */
