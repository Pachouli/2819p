/*****************************************************************************************************
 *                              US281B
 *                 Copyright(c) 2014-2020 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 *      <author>       <time>
 *      huang anbang   2018/05/31
 *****************************************************************************************************/
#ifndef __BT_ENGINE_SPP_TEST_H__
#define __BT_ENGINE_SPP_TEST_H__

#include "bluetooth_engine.h"

#define SPP_PASSTHROUGH_TEST_ENABLE 0

#if (SPP_PASSTHROUGH_TEST_ENABLE == 1)

#define SPP_PASS_TEST_BUFFER_LEN    (1024)

enum
{
    SPP_PASSTH_INIT, SPP_PASSTH_READY, SPP_PASSTH_CONNECTED,
};

typedef struct
{
    uint8 status;
    uint32 op_hdl;
    buffer_rw_t rcv_buf;
} spp_passth_test_st;

extern spp_passth_test_st spp_pass_test;
extern bool spp_passth_start(void);
extern void spp_passth_end(void);
extern void spp_pass_test_loop(void);

#endif //end of #if (SPP_PASSTHROUGH_TEST_ENABLE == 1)
#endif //end of #ifndef __BT_ENGINE_SPP_TEST_H__

