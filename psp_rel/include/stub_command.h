/********************************************************************************
 *                              USDK281A
 *                            Module: KERNEL
 *                 Copyright(c) 2003-2016 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>      <time>           <version >             <desc>
 *      wuyufan     2015-9-10 09:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     stub_command.h
 * \brief    内核接口列表
 * \author   wuyufan
 * \version  1.0
 * \date  2015/9/10
 *******************************************************************************/


#ifndef _STUB_COMMAND_H
#define _STUB_COMMAND_H


//pc工具的类型
typedef enum
{
    STUB_PC_TOOL_ASQT_MODE = 1, /*ASQT*/
    STUB_PC_TOOL_ASET_MODE = 2,
    STUB_PC_TOOL_ASET_EQ_MODE = 3, /*ASET*/
    STUB_PC_TOOL_ATT_MODE = 4, /*ATT*/
    STUB_PC_TOOL_TK_PMODE = 5,
    STUB_PC_TOOL_PRINT_MODE = 6, /*PRINT*/
    STUB_PC_TOOL_BTT_MODE = 0x42,
} PC_stub_mode_e;


#define STUB_CMD_OPEN                   (0xff00)

#define STUB_CMD_NAK                    (0xFD)
#define STUB_CMD_ACK                    (0xFE)

//print command
#define STUB_CMD_PRINT_TOOL             (STUB_PC_TOOL_PRINT_MODE)

#endif

