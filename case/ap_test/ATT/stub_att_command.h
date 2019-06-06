/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：自动化测试：ATT协议命令ID定义头文件。
 * 作者：zhouxl
 ********************************************************************************/

#ifndef __STUB_ATT_COMMAND_H
#define __STUB_ATT_COMMAND_H

#include <psp_includes.h>


//ATT工具命令字

#define STUB_CMD_ATT_START              (0x0400)

#define STUB_CMD_ATT_STOP               (0x0401)

#define STUB_CMD_ATT_PAUSE              (0x0402)

#define STUB_CMD_ATT_RESUME             (0x0403)

#define STUB_CMD_ATT_GET_TEST_ID        (0x0404)

#define STUB_CMD_ATT_GET_TEST_ARG       (0x0405)

#define STUB_CMD_ATT_REPORT_TRESULT     (0x0406)

#define STUB_CMD_ATT_PRINT_LOG          (0x0407)

#define STUB_CMD_ATT_FREAD              (0x0408)

#define STUB_CMD_ATT_READ_TESTINFO      (0x0409)

#define STUB_CMD_ATT_CHECK_SDK_VERSION  (0x040a)

#define STUB_CMD_ATT_SWTICH_FW          (0x040b)

#define STUB_CMD_ATT_CFO_TX_BEGIN       (0x0456)

#define STUB_CMD_ATT_CFO_TX_STOP        (0x0457)

#define STUB_CMD_ATT_NAK                (0x04fd)

#define STUB_CMD_ATT_ACK                (0x04fe)

#define STUB_CMD_ATT_POLL               (0x04ff)

#define STUB_CMD_ATT_HCI_WRITE_CMD      (0x0450)

#define STUB_CMD_ATT_HCI_EVENT_READ     (0x0451)

#define STUB_CMD_ATT_RESET_CONTROLLER   (0x0452)

#define STUB_CMD_ATT_DOWNLOAD_PATCH     (0x0453)

#define STUB_CMD_ATT_WRITE_TRIM_CAP     (0x0454)

#define STUB_CMD_ATT_READ_TRIM_CAP      (0x0455)

#define STUB_CMD_ATT_RETRUN_CFO_VAL     (0x0456)

#endif

