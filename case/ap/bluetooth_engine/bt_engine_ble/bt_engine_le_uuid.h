/********************************************************************************
 *        Copyright(c) 2018-2028 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：BLE支持头文件。
 * 作者：huanganbang
 ********************************************************************************/
#ifndef __BT_ENGINE_LE_UUID_H__
#define __BT_ENGINE_LE_UUID_H__

extern uint16 get_16bit_uuid_by_string(UINT8 *uuid);
extern void get_128bit_uuid_by_string(struct ATT_UUIDStru *dst_uuid, UINT8 *src_str);
extern uint16 get_16bit_uuid_by_128bit_uuid(struct ATT_UUIDStru *in_uuid);
extern void get_128bit_uuid_by_16bits_uuid(struct ATT_UUIDStru *dst_uuid, uint16 src_uuid);
extern uint16 get_16bit_uuid_by_16or128bit_uuid(UINT8 *uuid, UINT32 len);

#endif //end of #ifndef __BT_ENGINE_LE_UUID_H__
