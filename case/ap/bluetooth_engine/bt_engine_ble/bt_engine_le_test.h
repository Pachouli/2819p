/********************************************************************************
 *        Copyright(c) 2018-2028 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：BLE支持头文件。
 * 作者：huanganbang
 ********************************************************************************/
#ifndef __BT_ENGINE_LE_TEST_H__
#define __BT_ENGINE_LE_TEST_H__

//是否开启BLE测试
#define BT_ENGINE_BLE_TEST_ENABLE 0

#define LETEST_BASE_INTERVAL_MIN (24)         //24*1.25ms = 30ms
#define LETEST_BASE_INTERVAL_MAX (36)         //36*1.25ms = 45ms
#define LETEST_BASE_SLAVE_LATENCY (0)          //
#define LETEST_BASE_TIMEOUT_MULTIPLIER (200) //200*10ms = 2s

#if (BT_ENGINE_BLE_TEST_ENABLE == 1)
//测试基础BLE功能
#define BLE_TEST_BASE_ENABLE 0
#if (BLE_TEST_BASE_ENABLE == 1)
#define LETEST_128BITS_UUID_SERVICE_ENABLE  0
#define LETEST_BASE_AUTONITIFY_ENABLE       0

#define LETEST_BASE_SERVICE 0x01
#define LETEST_BASE_WRITE_CHAR 0x02
#define LETEST_BASE_WRITE_NORESP_CHAR 0x04
#define LETEST_BASE_SIGNED_WRITE_CHAR 0x08
#define LETEST_BASE_READ_CHAR 0x10
#define LETEST_BASE_NOTIFY_CHAR 0x20
#define LETEST_BASE_NOTIFY_CHAR_CCC 0x40
#define LETEST_BASE_INDICATE_CHAR 0x80
#define LETEST_BASE_INDICATE_CHAR_CCC 0x100
#endif

//测试passthrough profile
#define BLE_TEST_PASSTHP_ENABLE 0

#else

//测试基础BLE功能
#define BLE_TEST_BASE_ENABLE 0
//测试passthrough profile
#define BLE_TEST_PASSTHP_ENABLE 0

#endif

typedef void (*le_speed_test_cbk)(int speed_value);
typedef void (*le_test_timer_cbk)(void);

typedef struct
{
    uint32 last_time;
    uint32 send_times;
    uint32 send_count;
    le_speed_test_cbk cbk;
} le_speed_test_st;

#if (BLE_TEST_PASSTHP_ENABLE == 1)
typedef struct
{
    uint32 op_hdl;
    uint8 notify_enable;
    le_speed_test_st tx_speed;
    le_speed_test_st rx_speed;
    buffer_rw_t ble_test_buf;
} passthp_st;
#endif

enum 
{
    GATT_NOTIFY_DISABLE,
    GATT_NOTIFY_ENABLE,
};

#if (BLE_TEST_BASE_ENABLE == 1)
typedef struct
{
    uint16 svctree_bmap;//service tree bits map
    uint8 notify_enable;
    uint8 indicate_enable;
    le_speed_test_st tx_speed_nty;
    le_speed_test_st tx_speed_idc;
    le_speed_test_st rx_speed_wr;
    le_speed_test_st rx_speed_wrnr;
    struct GATT_ServiceStru *service;
    struct GATT_CharacteristicStru *w_char;
    struct GATT_CharacteristicStru *wnr_char;
    struct GATT_CharacteristicStru *r_char;
    struct GATT_CharacteristicStru *nty_char;
    struct GATT_DescriptorStru *nty_char_ccc;
    struct GATT_CharacteristicStru *idc_char;
    struct GATT_DescriptorStru *idc_char_ccc;
} letest_base_st;
#endif
typedef struct
{
    //test_for_passthp
#if (BLE_TEST_PASSTHP_ENABLE == 1)
    passthp_st passthp;
#endif
#if (BLE_TEST_BASE_ENABLE == 1)
    letest_base_st letest_base16;
#endif
} le_test_env_st;

extern le_test_env_st le_test_env;

extern void le_start_test(void);
extern void le_stop_test(void);
extern void le_test_loop(void);

#if (BLE_TEST_BASE_ENABLE == 1)
void letest_base_test_start(void);
#endif

#ifdef NEW_ADV_INTERFACT_TEST
typedef struct
{
    uint8 company_identifier_code[2];
    uint8 ble_addr[BD_ADDR_LEN]; /* len - 1 - 2 octets */
}eir_manufacturer_data_st;

extern bool app_user_advertiser_info_set(void);
#endif //end of #ifdef NEW_ADV_INTERFAC＿TEST

#endif //end of #ifndef __BT_ENGINE_LE_TEST_H__
