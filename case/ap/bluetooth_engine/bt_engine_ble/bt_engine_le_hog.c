/*****************************************************************************************************
 *                              US281B
 *                 Copyright(c) 2014-2020 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 *      <author>       <time>
 *      dengcong        2018/08/23
 *****************************************************************************************************/

#include "bt_engine_le_hog.h"

#if (BT_SUPPORT_BLE_HOG == 1)

struct GATT_ServiceStru *hog_service;

#define BIT(n)  (1UL << (n))

struct hids_info
{
    unsigned short version; /* version number of base USB HID Specification */
    unsigned char code; /* country HID Device hardware is localized for. */
    unsigned char flags;
};

enum
{
    HIDS_REMOTE_WAKE = BIT(0),
    HIDS_NORMALLY_CONNECTABLE = BIT(1),
};
static struct hids_info info =
{
    .version = 0x0000,
    .code = 0x00,
    .flags = HIDS_NORMALLY_CONNECTABLE,
};
static const unsigned char hids_report_map[] =
{
    0x05, 0x01, /* Usage Page (Generic Desktop Ctrls) */
    0x09, 0x02, /* Usage (Mouse) */
    0xA1, 0x01, /* Collection (Application) */
    0x09, 0x01, /*   Usage (Pointer) */
    0xA1, 0x00, /*   Collection (Physical) */
    0x05, 0x09, /*     Usage Page (Button) */
    0x19, 0x01, /*     Usage Minimum (0x01) */
    0x29, 0x03, /*     Usage Maximum (0x03) */
    0x15, 0x00, /*     Logical Minimum (0) */
    0x25, 0x01, /*     Logical Maximum (1) */
    0x95, 0x03, /*     Report Count (3) */
    0x75, 0x01, /*     Report Size (1) */
    0x81, 0x02, /*     Input (Data,Var,Abs,No Wrap,Linear,...) */
    0x95, 0x01, /*     Report Count (1) */
    0x75, 0x05, /*     Report Size (5) */
    0x81, 0x03, /*     Input (Const,Var,Abs,No Wrap,Linear,...) */
    0x05, 0x01, /*     Usage Page (Generic Desktop Ctrls) */
    0x09, 0x30, /*     Usage (X) */
    0x09, 0x31, /*     Usage (Y) */
    0x15, 0x81, /*     Logical Minimum (129) */
    0x25, 0x7F, /*     Logical Maximum (127) */
    0x75, 0x08, /*     Report Size (8) */
    0x95, 0x02, /*     Report Count (2) */
    0x81, 0x06, /*     Input (Data,Var,Rel,No Wrap,Linear,...) */
    0xC0, /*   End Collection */
    0xC0, /* End Collection */
};

enum
{
    HIDS_INPUT = 0x01,
    HIDS_OUTPUT = 0x02,
    HIDS_FEATURE = 0x03,
};

static const struct le_gatt_DRR hids_drr =
{
    .id = 0x01,
    .type = HIDS_INPUT,
};

static struct le_gatt_attr hog_attrs[] =
{
    BTSRV_LE_GATT_PRIMARY_SERVICE(GATT_UUID_HUMAN_INTERFACE_DEVICE, UUID_TYPE_16,
            APP_GATT_Hog_Callback),
    BTSRV_LE_GATT_CHARACTERISTIC(GATT_UUID_HID_INFORMATION, UUID_TYPE_16,
            &info, sizeof(info), ATT_READ),
    BTSRV_LE_GATT_CHARACTERISTIC(GATT_UUID_REPORT_MAP, UUID_TYPE_16, hids_report_map,
            sizeof(hids_report_map), (ATT_READ)),
    BTSRV_LE_GATT_CHARACTERISTIC(GATT_UUID_REPORT, UUID_TYPE_16, NULL, 2,
            ATT_READ | ATT_ALLWRITE | ATT_NOTIFY),
    BTSRV_LE_GATT_CLIENT_CHRC_CONFIG(
            GATT_CLIENT_CHARACTERISTIC_CONFIGURATION_NOTIFICATIONS_ENABLED),
    BTSRV_LE_GATT_DESCRIPTOR_REPORT_REF(&hids_drr, sizeof(hids_drr), ATT_READ),
    BTSRV_LE_GATT_CHARACTERISTIC(GATT_UUID_HID_CONTROL_POINT, UUID_TYPE_16, NULL, 2,
            ATT_WRITE_NORSP),
    BTSRV_LE_GATT_ATTRS_END(),
};

void APP_GATT_Hog_Callback(void *context, UINT8 ev, void *arg)
{
    switch (ev)
    {
        //GATT任务接收远端设备属性操作事件
        case GATT_EV_PEND_IND: /* struct ATT_PendIndStru *in */
            GATT_Server_PendIND_Response(arg);
            break;

        //GATT注册服务完成事件(返回HDL)
        case GATT_EV_HDL_TREE: /* Local is server */
            {
                //注册本地的service时会返回，AP记录下来
                BLE_DBG_PRINTF("HOG HDL TREE\n");
                struct GATT_ServiceStru *service = arg;
                BLE_DBG_PRINTF("hog service hdl : %d~%d\n",service->hdl,service->endhdl);
                BTSRC_List_AddTail(&g_bteg_le_env.app_attroot->service_list, arg);
                hog_service = arg;
            }
            break;

        default:
            APP_GATT_Callback(context, ev, arg);
            break;
    }
}

void APP_GATT_Hog_Start(void)
{
    APP_LE_REGISTER_SERVICE(hog_attrs);
}

void APP_GATT_Hog_Stop(void)
{
    hog_service = NULL;
}

#endif //end of (BT_SUPPORT_BLE_HOG == 1)
