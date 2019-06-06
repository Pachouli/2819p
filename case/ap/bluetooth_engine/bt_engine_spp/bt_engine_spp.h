/*****************************************************************************************************
 *                              US281B
 *                 Copyright(c) 2014-2020 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 *      <author>       <time>
 *      huang anbang   2018/05/31
 *****************************************************************************************************/
#ifndef __BT_ENGINE_SPP_H__
#define __BT_ENGINE_SPP_H__

#define SPP_MAX_MTU 200 //与BLE保持一致
#define SPP_MAX_WAITTING_SEND   1024

typedef struct
{
    uint32 send_len;
} spp_trans_ctrl_st;

extern spp_trans_ctrl_st spp_trans_ctrl;

extern UINT8 SppAppWriteData(UINT8 *src, UINT16 length);

#endif //end of #ifndef __BT_ENGINE_SPP_H__
