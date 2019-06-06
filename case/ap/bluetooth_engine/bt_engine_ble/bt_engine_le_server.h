/********************************************************************************
 *        Copyright(c) 2018-2028 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：BLE支持头文件。
 * 作者：huanganbang
 ********************************************************************************/
#ifndef __BT_ENGINE_LE_SERVER_H__
#define __BT_ENGINE_LE_SERVER_H__

#define LE_TX_CAPACITY_RESERVE 30

//descriptor_report_ref
struct le_gatt_DRR
{
    unsigned char id; /* report id */
    unsigned char type; /* report type */
};

/** @brief Bluetooth UUID types */
typedef enum
{
    UUID_TYPE_16, UUID_TYPE_32, UUID_TYPE_128,
}UUID_TYPE;

struct le_gatt_attr
{
    /** Attribute UUID */
    unsigned short attr_uuid;
    /** UUID type*/
    unsigned char uuid_type;
    /** UUID */
    unsigned int uuid;
    /** Attribute init data */
    const void * init_data;
    /** Attribute data len*/
    unsigned short data_len;
    /** Attribute properties */
    unsigned short data_prop;
    /** service callback */
    void *cbk;
};

#define BTSRV_LE_GATT_PRIMARY_SERVICE(_uuid, _uuid_type, _cbk)		\
{													\
	.attr_uuid = 0x2800,							\
	.uuid = _uuid,									\
	.uuid_type = _uuid_type,                        \
	.cbk  = _cbk                                    \
}

#define BTSRV_LE_GATT_CHARACTERISTIC(_uuid, _uuid_type, _init_data, _data_len, _data_prop)	\
{													\
	.attr_uuid = 0x2803,							\
	.uuid = _uuid,									\
    .uuid_type = _uuid_type,                        \
	.init_data = _init_data,						\
	.data_len = _data_len,							\
	.data_prop = _data_prop,						\
}

#define BTSRV_LE_GATT_CLIENT_CHRC_CONFIG(_cfg)		\
{													\
	.attr_uuid = 0x2902,							\
	.init_data = (const void *)_cfg,				\
	.data_len = 2,									\
	.data_prop = ATT_READ 			\
				| ATT_WRITE,			\
}

#define BTSRV_LE_GATT_DESCRIPTOR_REPORT_REF(_init_data, _data_len, _data_prop)	\
{													\
	.attr_uuid = 0x2908,							\
	.init_data = _init_data,						\
	.data_len = _data_len,							\
	.data_prop = _data_prop,						\
}

#define BTSRV_LE_GATT_ATTRS_END()					\
{													\
	.attr_uuid = 0,									\
}

extern void APP_LE_Server_stop(UINT8 *arg);
void APP_LE_REGISTER_SERVICE(struct le_gatt_attr * le_services);
extern bool APP_GATT_Server_NotifyIndicate(UINT16 hdl,UINT8 *val, UINT8 len);

extern uint16 get_cur_rmt_client_mtu(void);

#endif //end of #ifndef __BT_ENGINE_LE_SERVER_H__
