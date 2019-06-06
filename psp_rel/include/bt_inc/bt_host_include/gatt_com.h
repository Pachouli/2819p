#ifndef G_ATT_COM_H
#define G_ATT_COM_H

#include "att_ui.h"

#define DEFAULT_ATT_MTU 23

#define MAX_ATT_MTU 200//MAX IS 200

#define DEFAULT_BLE_MTU (DEFAULT_ATT_MTU - 3)
#define MAX_BLE_MTU (MAX_ATT_MTU - 3)

#define PACKET_TYPE_START      0x00
#define PACKET_TYPE_PACKETTING 0x01
#define PACKET_TYPE_END        0x02

#define PIPE_BUSY        1
#define PIPE_IDLE        0

struct Gatt_Packet_Info {
    UINT16 pack_info;//pack_type=00,total len;pack_type=01,current packet size;02,0000,reserved
    UINT8 pack_type;//00,start send packet;01,packettting;02,end send packet
    UINT8 reserved;
};


#endif
