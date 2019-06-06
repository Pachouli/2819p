/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：STUB通信协议读写接口头文件
 * 作者：cailizhen
 ********************************************************************************/

#ifndef __COMMON_STUB_H
#define __COMMON_STUB_H

#include "applib.h"
#include "stub_interface.h"
#include "stub_command.h"
#include "post_process.h"

#if 0
extern int32 com_stub_ack_ext_cmd_inner(uint16 cmd);
#endif
#define com_stub_ack_ext_cmd(a)         ((int32(*)(uint16))COM_COM_STUB_ACK_EXT_CMD)((a))

#if 0
extern int32 com_stub_read_ext_cmd_inner(uint16 cmd, void *data_buffer_rev_8bhead_2btail, uint32 data_len);
#endif
#define com_stub_read_ext_cmd(a,b,c)    ((int32(*)(uint16, void *, uint32))COM_COM_STUB_READ_EXT_CMD)((a),(b),(c))

#if 0
extern int32 com_stub_write_ext_cmd_inner(uint16 cmd, void *data_buffer_rev_8bhead_2btail, uint32 data_len);
#endif
#define com_stub_write_ext_cmd(a,b,c)   ((int32(*)(uint16, void *, uint32))COM_COM_STUB_WRITE_EXT_CMD)((a),(b),(c))

#if 0
extern int32 com_stub_read_cmd_inner(uint16 cmd, void *data_buffer, uint32 data_len);
#endif
#define com_stub_read_cmd(a,b,c)        ((int32(*)(uint16, void *, uint32))COM_COM_STUB_READ_CMD)((a),(b),(c))

#if 0
extern int32 com_stub_write_cmd_inner(uint16 cmd, void *data_buffer_rev_8bhead, uint32 data_len);
#endif
#define com_stub_write_cmd(a,b,c)       ((int32(*)(uint16, void *, uint32))COM_COM_STUB_WRITE_CMD)((a),(b),(c))

#if 0
extern app_result_e com_stub_tools_connect_inner(void);
#endif
#define com_stub_tools_connect()        ((app_result_e(*)(void))COM_COM_STUB_TOOLS_CONNECT)()

#if 0
extern uint8 com_get_stub_tools_type_inner(void);
#endif
#define com_get_stub_tools_type()       ((uint8(*)(void))COM_COM_GET_STUB_TOOLS_TYPE)()

#endif

