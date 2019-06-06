/*****************************************************************************************************
 *                              US281B
 *                 Copyright(c) 2014-2020 Actions (Zhuhai) Microelectronics Co., Limited,
 *                            All Rights Reserved.
 *
 *      <author>       <time>
 *      dengcong        2018/09/12
 *****************************************************************************************************/

#include "bt_engine_le_hrs.h"

#if (BT_SUPPORT_BLE_HRS == 1)

static unsigned char heart_rate = 100;
static  UINT16 accumulated_energy;
struct GATT_ServiceStru *hrs_service;
static UINT8 hrs_blsc;

/* Heart Rate Service Declaration */
static struct le_gatt_attr hrs_attrs[] =
{
    BTSRV_LE_GATT_PRIMARY_SERVICE(GATT_UUID_HEART_RATE, UUID_TYPE_16,
                APP_GATT_HRS_Callback),
    BTSRV_LE_GATT_CHARACTERISTIC(GATT_UUID_HEART_RATE_MEASUREMENT, UUID_TYPE_16,
                NULL, 1, ATT_NOTIFY),
    BTSRV_LE_GATT_CLIENT_CHRC_CONFIG(NULL),
    BTSRV_LE_GATT_CHARACTERISTIC(GATT_UUID_BODY_SENSOR_LOCATION, UUID_TYPE_16, &hrs_blsc,
                sizeof(hrs_blsc), ATT_READ),
    BTSRV_LE_GATT_CHARACTERISTIC(GATT_UUID_HEART_RATE_CONTROL_POINT, UUID_TYPE_16,
                NULL, 1, ATT_WRITE),
    BTSRV_LE_GATT_ATTRS_END(),
};
void hrs_service_pend_ind(void *arg)
{
    struct ATT_PendIndStru *in = arg;
    struct GATT_CharacteristicStru *c = le_find_char_in_service(hrs_service,
                    GATT_UUID_HEART_RATE_CONTROL_POINT, UUID_TYPE_16);

    if (accumulated_energy && (in->op == WRITE_REQ || in->op == WRITE_CMD)
                    && in->it.hdl == c->value_hdl)
    {
        if(in->it.val.value[0] == 0x01)
        {
            accumulated_energy = 0;
        }
    }
}
void APP_GATT_HRS_Callback(void *context, UINT8 ev, void *arg)
{
    BLE_DBG_PRINTF("GATT HRS Callback ev %d\n",ev);
    switch (ev)
    {
    //GATT任务接收远端设备属性操作事件
    case GATT_EV_PEND_IND: /* struct ATT_PendIndStru *in */
        hrs_service_pend_ind(arg);
        GATT_Server_PendIND_Response(arg);
        break;

    //GATT注册服务完成事件(返回HDL)
    case GATT_EV_HDL_TREE: /* Local is server */
        {
            BLE_DBG_PRINTF("HRS HDL_TREE\n");
            BTSRC_List_AddTail(&g_bteg_le_env.app_attroot->service_list, arg);
            hrs_service = (struct GATT_ServiceStru *)arg;
        }
        break;
    default:
        APP_GATT_Callback(context, ev, arg);
        break;
    }
}
/*
@param blsc    the intended location of the heart rate measurement for the device
    case 0: "Other"
    case 1: "Chest"
    case 2: "Wrist"
    case 3: "Finger"
    case 4: "Hand"
    case 5: "Ear Lobe"
    case 6: "Foot"
*/
void APP_GATT_HRS_Start(UINT8 blsc)
{
    hrs_blsc = blsc;
    APP_LE_REGISTER_SERVICE(hrs_attrs);
}
void APP_GATT_HRS_Stop(void)
{
    hrs_service = NULL;
}
/*
rate measurement char value
    Field 0: Flags
        bit 0: Heart Rate Value Format bit(UINT8 set 0,UINT16 set 1)
        bit 1-2: The Sensor Contact Status bits
        bit 3: Energy Expended Status bit
        bit 4：RR-Interval bit
    Heart Rate Measurement Value Field
    Energy Expended Field
    RR-Interval Field
Transmission Interval：In typical applications,approximately 1 time per second.
                       if support Energy Expended Status,approximately 1 time per every 10 seconds

*/
void APP_GATT_HRS_Notify(void)
{
    struct GATT_CharacteristicStru *c;
    static UINT8 hrm[4];

	heart_rate++;
	if (heart_rate == 160) {
		heart_rate = 90;
	}
	if(accumulated_energy > (UINT16)(accumulated_energy + 1000))
	{
	    accumulated_energy = 0xffff;
	}
	else
	{
	    accumulated_energy += 1000;
	}
	 /* uint8, sensor contact,energy expended */
    hrm[0] = GATT_HRM_SCS_SUPPORTED_AND_DETECTED | GATT_HRM_ENERGY_EXPENDED_STATUS_BIT;
    hrm[1] = heart_rate;
    libc_memcpy(hrm + 2, &accumulated_energy, 2);

    if(hrs_service)
    {
        c = le_find_char_in_service(hrs_service, GATT_UUID_HEART_RATE_MEASUREMENT, UUID_TYPE_16);
        APP_GATT_Server_NotifyIndicate(c->value_hdl, hrm,  sizeof(hrm));
    }
}
#endif //end of (BT_SUPPORT_BLE_HRS == 1)
