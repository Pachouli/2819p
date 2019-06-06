#include "ap_test_head.h"
#include "bank_resident/ap_autotest_resident.h"
#include "bank_bt_attr_modify/ap_autotest_bt_attr_modify_entry.h"
#include "bank_bt_attr_read/ap_autotest_bt_attr_read_entry.h"
#include "bank_bt_test/ap_autotest_bt_test_entry.h"
#include "bank_fm/ap_autotest_fm_entry.h"
#include "bank_gpio/ap_autotest_gpio_entry.h"
#include "bank_linein/ap_autotest_linein_entry.h"
#include "bank_channel/ap_autotest_channel_entry.h"
#include "bank_music/ap_autotest_music_entry.h"

/*
    各个测试项与test_item_bank.bin中的子bank_id的映射关系
*/
const test_id_to_bank_id_map_t test_id_to_bank_id_maps[]={
    {TESTID_MODIFY_BTNAME,    TEST_ITEM_BIN_ID      , TEST_ITEM_BANK_ID_BT_ATTR_MODIEY},

    {TESTID_MODIFY_BTADDR,    TEST_ITEM_BIN_ID      , TEST_ITEM_BANK_ID_BT_ATTR_MODIEY},

    {TESTID_READ_BTADDR,      TEST_ITEM_BIN_ID      , TEST_ITEM_BANK_ID_BT_ATTR_READ},

    {TESTID_READ_BTNAME,      TEST_ITEM_BIN_ID      , TEST_ITEM_BANK_ID_BT_ATTR_READ},

    {TESTID_BT_TEST,          TEST_ITEM_BIN_ID      , TEST_ITEM_BANK_ID_BT_TEST},

    {TESTID_FM_TEST,          TEST_ITEM_BIN_ID      , TEST_ITEM_BANK_ID_FM},

    {TESTID_LINEIN_TEST,      TEST_ITEM_BIN_ID      , TEST_ITEM_BANK_ID_LINEIN},

    {TESTID_SDCARD_TEST,      TEST_ITEM_BIN_ID      , TEST_ITEM_BANK_ID_MUSIC},

    {TESTID_UHOST_TEST,       TEST_ITEM_BIN_ID      , TEST_ITEM_BANK_ID_MUSIC},

    {TESTID_GPIO_TEST,        TEST_ITEM_BIN_ID_EXT      , TEST_ITEM_EXT_BANK_ID_GPIO},

    {TESTID_LINEIN_CH_TEST,   TEST_ITEM_BIN_ID_EXT      , TEST_ITEM_EXT_BANK_ID_CHANNEL},

    {TESTID_MIC_CH_TEST,      TEST_ITEM_BIN_ID_EXT      , TEST_ITEM_EXT_BANK_ID_CHANNEL},
};

/*
    各个测试项与测试函数的映射表
*/
const att_task_stru_ext_t autotest_ops_ext[] =
{
    {TESTID_MODIFY_BTNAME,          act_test_modify_bt_name_ext},

    {TESTID_MODIFY_BTADDR,          act_test_modify_bt_addr_ext},

    {TESTID_READ_BTADDR,            act_test_read_bt_addr_ext},

    {TESTID_READ_BTNAME,            act_test_read_bt_name_ext},

    {TESTID_BT_TEST,                act_test_bt_test_ext},

    {TESTID_LINEIN_TEST,            act_test_linein_play_test_ext},

    {TESTID_SDCARD_TEST,            act_test_sdcard_play_test_ext},

    {TESTID_UHOST_TEST,             act_test_uhost_play_test_ext},

    {TESTID_FM_TEST,                act_test_fm_play_test_ext},

    {TESTID_GPIO_TEST,              act_test_gpio_test_ext},

    {TESTID_LINEIN_CH_TEST,         act_test_linein_channel_test_ext},

    {TESTID_MIC_CH_TEST,            act_test_mic_channel_test_ext},
};


/*
    根据测试项id得到其代码所在的测试项bin文件id和bank_id
*/
int32 get_bank_id_of_test_id(uint16 test_id, uint8* bin_id, uint8* bank_id)
{
    int32 total_count = sizeof(test_id_to_bank_id_maps)/sizeof(test_id_to_bank_id_map_t);

    int i;
    for(i = 0; i < total_count; i++){
        if (test_id_to_bank_id_maps[i].test_id == test_id){
            *bin_id  = test_id_to_bank_id_maps[i].bin_id;
            *bank_id = test_id_to_bank_id_maps[i].bank_id;
            return 0;
        }
    }

    return -1;
}


test_func_ext_t get_op_entry_of_test_id(uint16 test_id)
{
    int32 total_count = sizeof(autotest_ops_ext)/sizeof(att_task_stru_ext_t);

    test_func_ext_t test_func = NULL;
    int i;
    for(i = 0; i < total_count; i++){
        if (autotest_ops_ext[i].test_id == test_id){
            test_func = autotest_ops_ext[i].test_func;
            break;
        }
    }

    return test_func;
}

