/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*
* Copyright (c) 1999-2013 IVT Corporation
*
* All rights reserved.
*
---------------------------------------------------------------------------*/

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Module Name:
    sysdep.h
Abstract:
    This file includes the definition of system dependent functions.
    Definition for different Operating system:
        CONFIG_OS_WIN32: For Windows 98/2000
        CONFIG_OS_LINUX: For Linux
        CONFIG_OS_WINCE: For Windows CE 2.12
        CONFIG_OS_WINKERNEL: For Windows Kernel module which is developed using VTOOLSD
        CONFIG_OS_REX:   For Rex
        CONFIG_OS_PSOS:  For PSOS+
        CONFIG_OS_VXWORK:  For Vxwork 5.3 and above
        CONFIG_OS_NUCLEUS:   For Nucleus
        CONFIG_OS_UC:    For uC/OS-II
        CONFIG_OS_NONE:  For no opeating system.

    Definition for other options:
        CONFIG_MEMORY_STATIC: use static memory allocation.
        CONFIG_ALIGN_ONEBYTE: one byte alignment for data structure.
        CONFIG_ALIGN_TWOBYTES: two byte alignment for data structure.
        CONFIG_LITTLE_ENDIAN: Using little endian encoding for integer.
---------------------------------------------------------------------------*/
#ifndef SYSTEMDEP_H
#define SYSTEMDEP_H

#include "autoconf.h"
#include "ctype_m4k.h"

#include "datatype.h"

#ifdef  CONFIG_MEMORY_STATIC
#include "btmem.h"
#endif

#define NEW(a) mem_malloc((int)(a))
#define FREE(a) mem_free((void*)(a))



#ifndef FUNC_EXPORT
#define FUNC_EXPORT
#endif



#endif

