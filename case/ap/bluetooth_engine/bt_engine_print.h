/*******************************************************************************
 *                              us281B
 *                            Module: bt_engine
 *                 Copyright(c) 2018-2028 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>      <time>           <version >             <desc>
 *      huanganbang   2018-7-28 10:52     1.0             build this file
 *******************************************************************************/
/*
 * \file     bluetooth_engine_hfp_dec.h
 * \brief    cailizhen
 * \version  1.0
 * \date     2014-7-28
 *******************************************************************************/
#ifndef _BLUETOOTH_PRINT_H
#define _BLUETOOTH_PRINT_H

#include "bluetooth_engine.h"

#define ERR_END      "  FILE:%s LINE:%d"

#define ENABLE_PRINTF_CON_DBG   1

#define BTEG_CON_STR                 "\n<CON> "
#define BTEG_CON_WARNNING_STR        "\n<CON W> "
#define BTEG_CON_ERROR_STR           "\n<CON E> "

#define BTEG_CON_PRINTF(format,...)            btengine_global_variable.printf.log(BTEG_CON_STR format,##__VA_ARGS__);
#define BTEG_CON_PRINTF_WARNNING(format,...)   btengine_global_variable.printf.log(BTEG_CON_WARNNING_STR format,##__VA_ARGS__);
#define BTEG_CON_PRINTF_ERROR(format,...)      btengine_global_variable.printf.log(BTEG_CON_ERROR_STR format,##__VA_ARGS__);
#if (ENABLE_PRINTF_CON_DBG == 1)
#define BTEG_CON_DBG_PRINTF(format,...)        btengine_global_variable.printf.log_dbg(BTEG_CON_STR format,##__VA_ARGS__);
#else
#define BTEG_CON_DBG_PRINTF(format,...)
#endif

#define ENABLE_PRINTF_A2DP_DBG   0

#define A2DP_STR                 "\n<A2DP > "
#define A2DP_TWS_STR             "\n<A2DP T> "
#define A2DP_WARNNING_STR        "\n<A2DP W> "
#define A2DP_ERROR_STR           "\n<A2DP E> "

#define A2DP_PRINTF(format,...)             btengine_global_variable.printf.log(A2DP_STR format,##__VA_ARGS__);
#define A2DP_TWS_PRINTF(format,...)         btengine_global_variable.printf.log(A2DP_TWS_STR format,##__VA_ARGS__);
#define A2DP_PRINTF_WARNNING(format,...)    btengine_global_variable.printf.log(A2DP_WARNNING_STR format,##__VA_ARGS__);
#define A2DP_PRINTF_ERROR(format,...)       btengine_global_variable.printf.log(A2DP_ERROR_STR format,##__VA_ARGS__);
#if (ENABLE_PRINTF_A2DP_DBG == 1)
#define A2DP_DBG_PRINTF(format,...)         btengine_global_variable.printf.log_dbg(A2DP_STR format,##__VA_ARGS__);
#define A2DP_TWS_DBG_PRINTF(format,...)     btengine_global_variable.printf.log_dbg(A2DP_TWS_STR format,##__VA_ARGS__);
#else
#define A2DP_DBG_PRINTF(format,...)
#define A2DP_TWS_DBG_PRINTF(format,...)
#endif

#define ENABLE_PRINTF_AVRCP_DBG   0

#define AVRCP_STR                 "\n<AVRCP> "
#define AVRCP_WARNNING_STR        "\n<AVRCP W> "
#define AVRCP_ERROR_STR           "\n<AVRCP E> "

#define AVRCP_PRINTF(format,...)             btengine_global_variable.printf.log(AVRCP_STR format,##__VA_ARGS__);
#define AVRCP_PRINTF_WARNNING(format,...)    btengine_global_variable.printf.log(AVRCP_WARNNING_STR format,##__VA_ARGS__);
#define AVRCP_PRINTF_ERROR(format,...)       btengine_global_variable.printf.log(AVRCP_ERROR_STR format,##__VA_ARGS__);
#if (ENABLE_PRINTF_AVRCP_DBG == 1)
#define AVRCP_DBG_PRINTF(format,...)         btengine_global_variable.printf.log_dbg(AVRCP_STR format,##__VA_ARGS__);
#else
#define AVRCP_DBG_PRINTF(format,...)
#endif

#define ENABLE_PRINTF_BTEG_DBG   1

#define BTEG_STR                 "\n<BTEG> "
#define BTEG_WARNNING_STR        "\n<BTEG W> "
#define BTEG_ERROR_STR           "\n<BTEG E> "

#define BTEG_PRINTF(format,...)             btengine_global_variable.printf.log(BTEG_STR format,##__VA_ARGS__);
#define BTEG_PRINTF_WARNNING(format,...)    btengine_global_variable.printf.log(BTEG_WARNNING_STR format,##__VA_ARGS__);
#define BTEG_PRINTF_ERROR(format,...)       btengine_global_variable.printf.log(BTEG_ERROR_STR format,##__VA_ARGS__);
#if (ENABLE_PRINTF_BTEG_DBG == 1)
#define BTEG_DBG_PRINTF(format,...)         btengine_global_variable.printf.log_dbg(BTEG_STR format,##__VA_ARGS__);
#else
#define BTEG_DBG_PRINTF(format,...)
#endif  

#define ENABLE_PRINTF_BT_AUTO_RECONN_DBG   0

#define BT_AUTO_RECONN_STR                 "\n<BT_AUTO_RECONN_> "
#define BT_AUTO_RECONN_WARNNING_STR        "\n<BT_AUTO_RECONN_ W> "
#define BT_AUTO_RECONN_ERROR_STR           "\n<BT_AUTO_RECONN_ E> "

#define BT_AUTO_RECONN_PRINTF(format,...)             btengine_global_variable.printf.log(BT_AUTO_RECONN_STR format,##__VA_ARGS__);
#define BT_AUTO_RECONN_PRINTF_WARNNING(format,...)    btengine_global_variable.printf.log(BT_AUTO_RECONN_WARNNING_STR format,##__VA_ARGS__);
#define BT_AUTO_RECONN_PRINTF_ERROR(format,...)       btengine_global_variable.printf.log(BT_AUTO_RECONN_ERROR_STR format,##__VA_ARGS__);
#if (ENABLE_PRINTF_BT_AUTO_RECONN_DBG == 1)
#define BT_AUTO_RECONN_DBG_PRINTF(format,...)         btengine_global_variable.printf.log_dbg(BT_AUTO_RECONN_STR format,##__VA_ARGS__);
#else
#define BT_AUTO_RECONN_DBG_PRINTF(format,...)
#endif 

#define ENABLE_PRINTF_BT_AUTO_TEST_DBG   0

#define BT_AUTO_TEST_STR                 "\n<BT_AUTO_TEST_> "
#define BT_AUTO_TEST_WARNNING_STR        "\n<BT_AUTO_TEST_ W> "
#define BT_AUTO_TEST_ERROR_STR           "\n<BT_AUTO_TEST_ E> "

#define BT_AUTO_TEST_PRINTF(format,...)             btengine_global_variable.printf.log(BT_AUTO_TEST_STR format,##__VA_ARGS__);
#define BT_AUTO_TEST_PRINTF_WARNNING(format,...)    btengine_global_variable.printf.log(BT_AUTO_TEST_WARNNING_STR format,##__VA_ARGS__);
#define BT_AUTO_TEST_PRINTF_ERROR(format,...)       btengine_global_variable.printf.log(BT_AUTO_TEST_ERROR_STR format,##__VA_ARGS__);
#if (ENABLE_PRINTF_BT_AUTO_TEST_DBG == 1)
#define BT_AUTO_TEST_DBG_PRINTF(format,...)         btengine_global_variable.printf.log_dbg(BT_AUTO_TEST_STR format,##__VA_ARGS__);
#else
#define BT_AUTO_TEST_DBG_PRINTF(format,...)
#endif 

#define ENABLE_PRINTF_HFP_DBG   0

#define HFP_STR                 "\n<HFP> "
#define HFP_WARNNING_STR        "\n<HFP W> "
#define HFP_ERROR_STR           "\n<HFP E> "

#define HFP_PRINTF(format,...)             btengine_global_variable.printf.log(HFP_STR format,##__VA_ARGS__);
#define HFP_PRINTF_WARNNING(format,...)    btengine_global_variable.printf.log(HFP_WARNNING_STR format,##__VA_ARGS__);
#define HFP_PRINTF_ERROR(format,...)       btengine_global_variable.printf.log(HFP_ERROR_STR format,##__VA_ARGS__);
#if (ENABLE_PRINTF_HFP_DBG == 1)
#define HFP_DBG_PRINTF(format,...)         btengine_global_variable.printf.log_dbg(HFP_STR format,##__VA_ARGS__);
#else
#define HFP_DBG_PRINTF(format,...)
#endif

#define ENABLE_PRINTF_HCI_DBG   0

#define HCI_STR                 "\n<HCI> "
#define HCI_WARNNING_STR        "\n<HCI W> "
#define HCI_ERROR_STR           "\n<HCI E> "

#define HCI_PRINTF(format,...)             btengine_global_variable.printf.log(HCI_STR format,##__VA_ARGS__);
#define HCI_PRINTF_WARNNING(format,...)    btengine_global_variable.printf.log(HCI_WARNNING_STR format,##__VA_ARGS__);
#define HCI_PRINTF_ERROR(format,...)       btengine_global_variable.printf.log(HCI_ERROR_STR format,##__VA_ARGS__);
#if (ENABLE_PRINTF_HCI_DBG == 1)
#define HCI_DBG_PRINTF(format,...)         btengine_global_variable.printf.log_dbg(HCI_STR format,##__VA_ARGS__);
#else
#define HCI_DBG_PRINTF(format,...)
#endif

#define ENABLE_PRINTF_HID_DBG   0

#define HID_STR                 "\n<HID> "
#define HID_WARNNING_STR        "\n<HID W> "
#define HID_ERROR_STR           "\n<HID E> "

#define HID_PRINTF(format,...)             btengine_global_variable.printf.log(HID_STR format,##__VA_ARGS__);
#define HID_PRINTF_WARNNING(format,...)    btengine_global_variable.printf.log(HID_WARNNING_STR format,##__VA_ARGS__);
#define HID_PRINTF_ERROR(format,...)       btengine_global_variable.printf.log(HID_ERROR_STR format,##__VA_ARGS__);
#if (ENABLE_PRINTF_HID_DBG == 1)
#define HID_DBG_PRINTF(format,...)         btengine_global_variable.printf.log_dbg(HID_STR format,##__VA_ARGS__);
#else
#define HID_DBG_PRINTF(format,...)
#endif

#define ENABLE_PRINTF_GAP_DBG   1

#define GAP_STR                 "\n<GAP> "
#define GAP_WARNNING_STR        "\n<GAP W> "
#define GAP_ERROR_STR           "\n<GAP E> "

#define GAP_PRINTF(format,...)             btengine_global_variable.printf.log(GAP_STR format,##__VA_ARGS__);
#define GAP_PRINTF_WARNNING(format,...)    btengine_global_variable.printf.log(GAP_WARNNING_STR format,##__VA_ARGS__);
#define GAP_PRINTF_ERROR(format,...)       btengine_global_variable.printf.log(GAP_ERROR_STR format,##__VA_ARGS__);
#if (ENABLE_PRINTF_GAP_DBG == 1)
#define GAP_DBG_PRINTF(format,...)         btengine_global_variable.printf.log_dbg(GAP_STR format,##__VA_ARGS__);
#else
#define GAP_DBG_PRINTF(format,...)
#endif

#define ENABLE_PRINTF_TWS_DBG   0

#define TWS_STR                 "\n<TWS> "
#define TWS_WARNNING_STR        "\n<TWS W> "
#define TWS_ERROR_STR           "\n<TWS E> "

#define TWS_PRINTF(format,...)             btengine_global_variable.printf.log(TWS_STR format,##__VA_ARGS__);
#define TWS_PRINTF_WARNNING(format,...)    btengine_global_variable.printf.log(TWS_WARNNING_STR format,##__VA_ARGS__);
#define TWS_PRINTF_ERROR(format,...)       btengine_global_variable.printf.log(TWS_ERROR_STR format,##__VA_ARGS__);
#if (ENABLE_PRINTF_TWS_DBG == 1)
#define TWS_DBG_PRINTF(format,...)         btengine_global_variable.printf.log_dbg(TWS_STR format,##__VA_ARGS__);
#else
#define TWS_DBG_PRINTF(format,...)
#endif

#define ENABLE_PRINTF_SPP_DBG   1

#define SPP_STR                 "\n<SPP> "
#define SPP_WARNNING_STR        "\n<SPP W> "
#define SPP_ERROR_STR           "\n<SPP E> "

#define SPP_PRINTF(format,...)             btengine_global_variable.printf.log(SPP_STR format,##__VA_ARGS__);
#define SPP_PRINTF_WARNNING(format,...)    btengine_global_variable.printf.log(SPP_WARNNING_STR format,##__VA_ARGS__);
#define SPP_PRINTF_ERROR(format,...)       btengine_global_variable.printf.log(SPP_ERROR_STR format,##__VA_ARGS__);
#if (ENABLE_PRINTF_SPP_DBG == 1)
#define SPP_DBG_PRINTF(format,...)         btengine_global_variable.printf.log_dbg(SPP_STR format,##__VA_ARGS__);
#else
#define SPP_DBG_PRINTF(format,...)
#endif

#define ENABLE_PRINTF_BLE_DBG   1

#define BLE_STR                 "\n<BLE> "
#define BLE_WARNNING_STR        "\n<BLE W> "
#define BLE_ERROR_STR           "\n<BLE E> "

#define BLE_PRINTF(format,...)             btengine_global_variable.printf.log(BLE_STR format,##__VA_ARGS__);
#define BLE_PRINTF_WARNNING(format,...)    btengine_global_variable.printf.log(BLE_WARNNING_STR format,##__VA_ARGS__);
#define BLE_PRINTF_ERROR(format,...)       btengine_global_variable.printf.log(BLE_ERROR_STR format,##__VA_ARGS__);
#if (ENABLE_PRINTF_BLE_DBG == 1)
#define BLE_DBG_PRINTF(format,...)         btengine_global_variable.printf.log_dbg(BLE_STR format,##__VA_ARGS__);
#else
#define BLE_DBG_PRINTF(format,...)
#endif

extern void print_bt_addr(uint8 *addr);

#endif

