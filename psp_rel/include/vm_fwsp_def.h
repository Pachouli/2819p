/*******************************************************************************
 *                              us211A
 *                            Module: case
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-10-24 16:20     1.0             build this file
 *******************************************************************************/
/*!
 * \file     vm_def.h
 * \brief    包含各个应用所占VM空间宏定义
 * \author   zhangxs
 * \version 1.0
 * \date  2011-10-24
 *******************************************************************************/
#ifndef _VM_FWSP_DEF_H
#define _VM_FWSP_DEF_H



//plist index
#define VM_PLIST_INDEX_START0           (0x00280000)    //0x28
#define VM_PLIST_INDEX_START1           (0x00290000)    //0x29
#define VM_PLIST_INDEX_START2           (0x002a0000)    //0x2a
#define VM_PLIST_INDEX_START3           (0x002b0000)    //0x2b
#define VM_PLIST_INDEX_START4           (0x002c0000)    //0x2c
#define VM_PLIST_INDEX_START5           (0x002d0000)    //0x2d
#define VM_PLIST_INDEX_START6           (0x002e0000)    //0x2e
#define VM_PLIST_INDEX_START7           (0x002f0000)    //0x2f

#define FS_VM_INDEX_START0              (0x00300000)    //0x30
#define FS_VM_INDEX_START1              (0x00310000)
#define FS_VM_INDEX_START2              (0x00320000)
#define FS_VM_INDEX_START3              (0x00330000)
#define FS_VM_INDEX_START4              (0x00340000)
#define FS_VM_INDEX_START5              (0x00350000)
#define FS_VM_INDEX_START6              (0x00360000)

#define FS_VM_HEAD_INFO                 (0x00360000)  //记录空簇的使用情况，

#define VM_BT_CLBRPARA                  (0x003e0000)    //0x3e
#define UDISK_VM                        (0x003f0000)    //0x3f

//#define VM_PLIST_INDEX_LEN            (0x1000)        //4k
//#define VM_FWSP_START                 (0x00300000)    //0x30
//#define VM_PLIST_LEN                  (0x1000)        //4k
//#define FS_VM_LEN                     (0x2000)        //0x10
//#define UDISK_VM                      (FS_VM+FS_VM_LEN)
//#define UDISK_VM_LEN                  (0x200)

#define  VRAM_MAGIC(vram_addr)  (0x55AA + (short)(vram_addr >> 16))

#endif
