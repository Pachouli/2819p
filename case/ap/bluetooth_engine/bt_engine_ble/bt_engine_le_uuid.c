/*******************************************************************************
 *                              US281A
 *                 Copyright(c) 2014-2015 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 *      <author>       <time>
 *      huang anbang       2017-08-30
 *******************************************************************************/

#include "bt_engine_le.h"
#include "bt_engine_le_uuid.h"

#ifdef BT_SUPPORT_BLE //ATT 功能服务实现

/*********** UUID Operation ***********/
/*
 get uuid strings from 16bits uuid
 sample
 "00003838-0000-1000-8000-00805F9B34FB" to 0x3838;
 return value：0:not 16bit uuid  other:16bits uuid
 */
uint16 get_16bit_uuid_by_string(UINT8 *uuid)
{
    struct ATT_UUIDStru out;
    ATT_UUID_SetU16(&out, uuid);

    return ATT_UUID_GetU2(&out);
}

/*
 get 16bits uuid from 128bits uuid
 sample
 {0x00,0x00,0x37,0x38,0x00,0x00,0x10,0x00,0x80,0x00,0x00,0x80,0x5f,0x9B,0x34,0xFB} to 0x3738 ;

 get 16bits uuid from 16bits uuid
 sample
 {0x37,0x38} to 0x3738 ;
 */
uint16 get_16bit_uuid_by_16or128bit_uuid(UINT8 *uuid, UINT32 len)
{
    struct ATT_UUIDStru out;
    ATT_UUID_Get(&out, uuid, (UINT8) len);

    return ATT_UUID_GetU2(&out);
}

/*
 get uuid strings from 128bits uuid
 sample
 "00003838-0000-1000-8000-00805F9B34FB" to {0x00,0x00,0x38,0x38,0x00,0x00,0x10,0x00,0x80,0x00,0x00,0x80,0x5f,0x9B,0x34,0xFB};
 */
void get_128bit_uuid_by_string(struct ATT_UUIDStru *dst_uuid, UINT8 *src_str)
{
    ATT_UUID_SetU16(dst_uuid, src_str);
}

/*
 get 128bits uuid from 16bits uuid
 sample
 {0x00,0x00,0x38,0x38,0x00,0x00,0x10,0x00,0x80,0x00,0x00,0x80,0x5f,0x9B,0x34,0xFB} to 0x3838 ;
 */
uint16 get_16bit_uuid_by_128bit_uuid(struct ATT_UUIDStru *in_uuid)
{
    return ATT_UUID_GetU2(in_uuid);
}

/*
 get uuid strings from 16bits uuid
 sample
 0x3838 to {0x00,0x00,0x38,0x38,0x00,0x00,0x10,0x00,0x80,0x00,0x00,0x80,0x5f,0x9B,0x34,0xFB};
 */
void get_128bit_uuid_by_16bits_uuid(struct ATT_UUIDStru *dst_uuid, uint16 src_uuid)
{
    ATT_UUID_SET_U2(dst_uuid, src_uuid);
}

#endif //end of #ifdef BT_SUPPORT_BLE
