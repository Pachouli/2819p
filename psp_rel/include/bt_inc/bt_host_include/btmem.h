/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* Copyright (c) 1999-2013 IVT Corporation
*
* All rights reserved.
*
---------------------------------------------------------------------------*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Module Name:
    btlist.c
Abstract:
    This file provides private memory management functions.
---------------------------------------------------------------------------*/
#ifndef BTMEM_H
#define BTMEM_H

#ifdef CONFIG_TYPE_BIT16_ONLY
#define HUGE huge
#else
#define HUGE
#endif

#define CONFIG_BLOCK_SIZE               0x10
#define CONFIG_BLOCK_NUMBER             (BT_ENGINE_HEAP_SIZE/CONFIG_BLOCK_SIZE)//0x700 (1792*16 = 28K)

#define BLK_IDXNUM  (CONFIG_BLOCK_NUMBER>>3)
#define MAX_BITS_NORMAl    (BLK_IDXNUM<<3)
#define MAX_POOL_SIZE   ((DWORD)CONFIG_BLOCK_NUMBER*CONFIG_BLOCK_SIZE)

#endif
