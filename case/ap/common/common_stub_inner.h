/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：STUB读写和连接接口声明。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef __COMMON_STUB_INNER_H
#define __COMMON_STUB_INNER_H

#include "applib_inner.h"

//inner function
extern int32 com_stub_ack_ext_cmd_inner(uint16 cmd);
extern int32 com_stub_read_ext_cmd_inner(uint16 cmd, void *data_buffer_rev_8bhead_2btail, uint32 data_len);
extern int32 com_stub_write_ext_cmd_inner(uint16 cmd, void *data_buffer_rev_8bhead_2btail, uint32 data_len);
extern int32 com_stub_read_cmd_inner(uint16 cmd, void *data_buffer, uint32 data_len);
extern int32 com_stub_write_cmd_inner(uint16 cmd, void *data_buffer_rev_8bhead, uint32 data_len);

extern app_result_e com_stub_tools_connect_inner(void);
extern void com_stub_tools_disconnect(void);
extern uint8 com_get_stub_tools_type_inner(void);
extern void com_set_stub_tools_type_inner(uint8 stub_tools_type);

#endif

