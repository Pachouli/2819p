/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：ASQT命令ID定义。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef _STUB_ASQT_COMMAND_H
#define _STUB_ASQT_COMMAND_H

#include  "psp_includes.h"

#define STUB_CMD_ASQT_READ_STATUS     (0x0100)
#define STUB_CMD_ASQT_READ_CONFIG     (0x0200)
#define STUB_CMD_ASQT_DOWNLOAD_SCOIN  (0x0300)

#define STUB_CMD_ASQT_DUMP_RESULT     (0x0e00)
#define STUB_CMD_ASQT_DUMP_RESULT_CP  (0x0500)
#define STUB_CMD_ASQT_DUMP_SCOIN      (0x0600)
#define STUB_CMD_ASQT_DUMP_SCOIN_CP   (0x0700)

#define STUB_CMD_ASQT_DUMP_RESULT_EXT (0x2e00)

#define STUB_CMD_ASQT_WRITE_CONFIG    (0x1000)

#define STUB_CMD_ASQT_READ_VOLUME     (0x0d00)

#define STUB_CMD_ASQT_READ_CONFIG_EXT (0x3200)
#define STUB_CMD_ASQT_READ_EQ1_PARA   (0x3300)
#define STUB_CMD_ASQT_READ_EQ2_PARA   (0x3400)

#endif

