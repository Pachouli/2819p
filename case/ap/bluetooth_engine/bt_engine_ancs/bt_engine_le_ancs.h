/********************************************************************************
 *        Copyright(c) 2018-2028 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：BLE ANCS支持头文件。
 * 作者：huanganbang
 ********************************************************************************/

#ifndef _BT_ENGINE_LE_ANCS_H_
#define _BT_ENGINE_LE_ANCS_H_

typedef struct
{
    uint8 drop_status; //whether need drop
    //next pky drop flag
    uint8 drop_next_pkg;
    //int8  drop_timer;//drop timer
    uint8 drop_len;    //drop length
    //total length
    uint8 att_pkg_len;
    //ble acl handle
    uint16 acl_hdl;
    //ancs notify handle
    uint16 ancs_ntf_hdl;
    //uint8 reserve;
    uint8 profilehdr[4];

} bt_ancs_drop_vars_t;

#endif
