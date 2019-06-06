/********************************************************************************
 *        Copyright(c) 2018-2028 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：BLE HRS支持头文件。
 * 作者：dengcong
 ********************************************************************************/
#ifndef __BT_ENGINE_HRS_PROFILE_H__
#define __BT_ENGINE_HRS_PROFILE_H__

#include "bt_engine_le.h"

#if (BT_SUPPORT_BLE_HRS == 1)

enum {/* Heart Rate Measurement, 2A37 */

    /* Heart Rate Value Format bit */
    GATT_HRM_FORMAT_BIT = (1 << 0),/* Heart Rate Value Format is set to UINT8. Units: beats per minute (bpm)(0)/Heart Rate Value Format is set to UINT16. Units: beats per minute (bpm)(1), */

    /* Sensor Contact Status bits */
    GATT_HRM_SCS_NOT_SUPPORT = (0 << 1),
    GATT_HRM_SCS_SUPPORTED_BUT_NOT_DETECTED = (2 << 1),
    GATT_HRM_SCS_SUPPORTED_AND_DETECTED = (3 << 1),

    /* Energy Expended Status bit */
    GATT_HRM_ENERGY_EXPENDED_STATUS_BIT = (1 << 3),/* Energy Expended field is not present(0)/Energy Expended field is present. Units: kilo Joules(1), */

    /* RR-Interval bit */
    GATT_HRM_RR_INTERVAL_BIT = (1 << 4),/* RR-Interval values are not present.(0)/One or more RR-Interval values are present. Units: 1/1024 seconds(1), */
};

enum {/* Body Sensor Location, 2A38 */

    /* Body Sensor Location */
    GATT_BLSC_OTHER =                      0,
    GATT_BLSC_CHEST =                      1,
    GATT_BLSC_WRIST =                      2,
    GATT_BLSC_FINGER =                     3,
    GATT_BLSC_HAND =                       4,
    GATT_BLSC_EAR_LOBE =                   5,
    GATT_BLSC_FOOT =                       6,
    /* [ReservedForFutureUse ][start : 7][end : 255] */
};

void APP_GATT_HRS_Start(UINT8 blsc);
void APP_GATT_HRS_Stop(void);
void APP_GATT_HRS_Callback(void *context, UINT8 ev, void *arg);
void APP_GATT_HRS_Notify(void);

#endif //end of #if (BT_SUPPORT_BLE_HRS == 1)

#endif //end of #ifndef __BT_ENGINE_HRS_PROFILE_H__
