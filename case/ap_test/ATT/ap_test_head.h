/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * �������Զ������ԣ�ͷ�ļ���
 * ���ߣ�zhouxl
 ********************************************************************************/

#ifndef __AP_TEST_HEAD_H
#define __AP_TEST_HEAD_H


#include <psp_includes.h>
#include "common_front_app_def.h"
#include "ap_autotest_mem_define.h"
#include "bluetooth_common.h"
#include "stub_att_command.h"

#define __PACKED__  __attribute__((packed))
//#define PRINTF(a,...) libc_printf("%s %d -> " a "\n", __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define PRINTF(...)

#define ENTRY_ID(a)                 _entry_id_##a

//ATT test item timeout value, unit:s
#define  ATT_ITEM_TEST_TIMEOUT      60

//ͨ�ò���ID,��Χ0x01-0xbf
#define TESTID_MODIFY_BTNAME        (0x01)
#define TESTID_MODIFY_BLENAME       (0x02)
#define TESTID_FM_TEST              (0x05)
#define TESTID_GPIO_TEST            (0x06)
#define TESTID_LINEIN_CH_TEST       (0x07)
#define TESTID_MIC_CH_TEST          (0x08)
#define TESTID_FM_CH_TEST           (0x09)
#define TESTID_SDCARD_TEST          (0x0a)
#define TESTID_UHOST_TEST           (0x0b)
#define TESTID_LINEIN_TEST          (0x0c)
#define TESTID_PREPARE_PRODUCT_TEST (0x0d)
#define TESTID_PRODUCT_TEST         (0x0e)
#define TESTID_FLASHDUMP_TEST       (0x0f)
#define TESTID_BURN_IC              (0x10)
#define TESTID_CHECK_IC             (0x11)
#define TESTID_FLASHDOWNLOAD_TEST   (0x12)
#define TESTID_MEM_UPLOAD_TEST      (0x13)
#define TESTID_MEM_DOWNLOAD_TEST    (0x14)
#define TESTID_GPIO_TEST_ATS2823    (0x15)
#define TESTID_READ_BTNAME          (0x16)
#define TESTID_MONITOR              (0x17)
#define TESTID_FTMODE               (0x18)
#define TESTID_BQBMODE              (0x19)

//�������ID,��Ҫ�͹��߰󶨣������������IDֵ ��Χ0xc0-0xfe
#define TESTID_MODIFY_BTADDR        (0xc0)
#define TESTID_BT_TEST              (0xc1)
#define TESTID_MP_TEST              (0xc2)
#define TESTID_MP_READ_TEST         (0xc3)
#define TESTID_READ_BTADDR          (0xc4)

//��ȡ�ò���ID,С����Ҫһֱ�ȴ�PC�ظ���Ч��TESTID
#define TESTID_ID_WAIT              (0xfffe)

//��ȡ�ò���ID,С�����Խ�������
#define TESTID_ID_QUIT              (0xffff)

#define TEST_BTNAME_MAX_LEN         56 /*18*3 + 2*/
#define TEST_BTBLENAME_MAX_LEN      30 /*29 + null*/

#define DEBUG_PRINT_ATT



#define _SIGN                       (1)  //�з���
#define _ZEROPAD                    (2)  //0ǰ׺
#define _LARGE                      (4)  //��д

#define _putc(_str, _end, _ch) \
do                             \
{                              \
    *_str++ = _ch;             \
}                              \
while (0)

/* Define __gnuc_va_list.  */
#ifndef __GNUC_VA_LIST
#define __GNUC_VA_LIST
typedef __builtin_va_list __gnuc_va_list;
#endif

typedef __gnuc_va_list va_list;

/* Note that the type used in va_arg is supposed to match the
   actual type **after default promotions**.
   Thus, va_arg (..., short) is not valid.
*/

#define va_start(v,l)   __builtin_va_start((v),l)
#define va_end      __builtin_va_end
#define va_arg      __builtin_va_arg


// ������������ڵ�bin����
typedef enum{
    TEST_ITEM_BIN_ID        = 0,    // ��Ӧbkitem.bin
    TEST_ITEM_BIN_ID_EXT,           // ��Ӧbkitex.bin
    TEST_ITEM_BIN_ID_MAX,           // ��Ч
}test_item_bin_index_t;

// base func overlay bank�����ص�bank id
typedef enum{
    BASE_FUNC_BANK_ID_ARGS_READ = 0,
    BASE_FUNC_BANK_ID_COMMON,  
    BASE_FUNC_BANK_ID_MAX_NULL,  
}base_func_bank_id_t;

typedef enum
{
    TEST_MODE_CARD = 0,
    TEST_MODE_USB,
    TEST_MODE_UART,
    TEST_MODE_EXTEND
}test_mode_e;


typedef enum
{
    TEST_PASS = 0,
    TEST_BT_FAIL,
    TEST_MIC_FAIL,
    TEST_LINEIN_FAIL,
    TEST_UHOST_FAIL,
    TEST_FM_FAIL,
    TEST_IR_FAIL,
    TEST_GPIO_SHORT,
    TEST_GPIO_SHORT_VCC,
    TEST_GPIO_SHORT_GND,
    TEST_GPIO_OPEN,
    TEST_GPIOSIO_SHORT,
    TEST_GPIOSIO_SHORT_VCC,
    TEST_GPIOSIO_SHORT_GND,
    TEST_GPIOSIO_OPEN,
    TEST_KEY_FAIL,
    TEST_SDCARD_FAIL,
    TEST_FW_ERROR,
}test_result_e;


typedef enum
{
    TEST_STATE_INIT = 0x0,
    TEST_STATE_START = 0x1,
    TEST_STATE_STOP = 0x2,
    TEST_STATE_PAUSE = 0x3,
}test_state_e;


typedef enum
{
    //DUTֱ�Ӻ�PC����
    DUT_CONNECT_MODE_DIRECT = 0,
    //DUTͨ��UDAת�Ӱ��PC����
    DUT_CONNECT_MODE_UDA,
}dut_connect_state_e;


typedef enum
{
    //DUT��������ģʽ,���Դ�������ֹͣ
    DUT_WORK_MODE_NORMAL = 0,
    //DUT���Դ���������һ�����
    DUT_WORK_MODE_SPECIAL = 1,
}dut_work_mode_e;


typedef enum
{
    TEST_RECORD_BTADDR_INCREASE = 0x0,
    TEST_RECORD_BTADDR_RANDOM,
    TEST_RECORD_BTADDR_FIX,
    TEST_RECORD_TESTCOUNT,
}test_record_info_e;


typedef struct
{
    stub_ext_cmd_t ext_cmd;
    uint16 offsetl;
    uint16 offseth;
    uint16 lengthl;
    uint16 lengthh;
}att_fread_arg_t;


typedef struct
{
    stub_ext_cmd_t ext_cmd;
    uint8 dut_connect_mode;
    uint8 dut_work_mode;
    uint8 timeout;
    uint8 reserved;
    uint8 bdaddr[6];
    uint8 reserved2[22];
}att_start_arg_t;


typedef struct
{
    stub_ext_cmd_t ext_cmd;
    uint8 reboot_timeout;
    uint8 reserved[31];
}att_swtich_fw_arg_t;


typedef struct
{
    uint16 last_test_id;
    uint8 reserved[30];
}pc_test_info_t;


typedef struct
{
    stub_ext_cmd_t ext_cmd;
    pc_test_info_t pc_test_info;
}att_pc_test_info_t;

#define ATF_MAX_SUB_FILENAME_LENGTH 11
typedef struct
{
    uint8 filename[ATF_MAX_SUB_FILENAME_LENGTH];
    uint8 reserved1[5];
    uint32 offset;
    uint32 length;
    uint8 reserved2[8];
} atf_dir_t;

#define ATF_MAX_SUB_FILE_NUM  15
typedef struct
{
    uint8 magic[8];
    uint8 sdk_version[4];
    uint8 reserved[20];
    atf_dir_t atf_dir[ATF_MAX_SUB_FILE_NUM];
} atf_head_t;

/*
    ���������������
����˵����
    test_mode:          ����ģʽ,usb��sd��ģʽ
    arg_buffer:         ������������Ĳ���
    log_buffer:         ���ڱ���log��Ϣ(usb���Ժ�sd������ͨ��)
    log_len:            ���ڷ���log��Ϣ����
*/
typedef test_result_e (*test_func_t)(void *arg_buffer);
typedef int32 (*test_func_ext_t)(void *arg_buffer, uint32 test_mode, uint8* log_buffer, uint32* log_len);

typedef int32 (*read_arg_func_t)(uint16 *line_buffer, uint8 test_mode, uint8 *arg_buffer, uint32 arg_len);

typedef struct
{
    uint16 test_id;
    test_func_t test_func;
}att_task_stru_t;

typedef struct
{
    uint16 test_id;
    test_func_ext_t test_func;
}att_task_stru_ext_t;

typedef struct
{
    uint16 test_id;
    read_arg_func_t read_arg_func;
}att_task_read_arg_t;

typedef struct
{
    uint8  test_state;
    uint8 reserved;
    uint16 test_id;
    uint16 arg_len;
}autotest_test_info_t;

typedef struct
{
    uint8 stub_head[6];
    /*�䳤����*/
    uint8 arg_data[0];
}att_pc_arg_data_t;

typedef struct
{
    uint8 bt_name[TEST_BTNAME_MAX_LEN];
}bt_name_arg_t;


typedef struct
{
    uint8 bt_ble_name[TEST_BTBLENAME_MAX_LEN];
}ble_name_arg_t;

typedef struct
{
    uint8 bt_addr[6];
    uint8 bt_addr_add_mode;
    uint8 bt_burn_mode;
    uint8 bt_print_flag;
}bt_addr_arg_t;

typedef struct
{
    uint8 bt_transmitter_addr[6];
    uint8 bt_test_mode;
}btplay_test_arg_t;

typedef struct
{
    uint32 freq;
    uint8  threshold;
}fmplay_test_arg_t;

/* gpio��������� */
typedef struct
{
    uint32 gpioA_value;
    uint32 gpioB_value;
    uint32 gpioSIO_value;
}gpio_test_arg_t;

/* channelͨ�����Բ��� */
typedef struct
{
    uint8 test_left_ch;
    uint8 test_right_ch;
    uint8 test_left_ch_SNR;
    uint8 test_right_ch_SNR;
    uint32 left_ch_power_threadshold;
    uint32 right_ch_power_threadshold;
    uint32 left_ch_SNR_threadshold;
    uint32 right_ch_SNR_threadshold;
    uint16 left_ch_max_sig_point;
    uint16 right_ch_max_sig_point;
}channel_test_arg_t;

typedef struct
{
    uint8 cfo_channel_low;          //���ŵ���
    uint8 cfo_channel_mid;          //���ŵ���
    uint8 cfo_channel_high;         //���ŵ���
    uint8 cfo_test;                 //�Ƿ����Ƶƫ����
    uint8 cfo_index_low;            //��¼��������Сֵ
    uint8 cfo_index_high;           //��¼���������ֵ
    uint8 cfo_index_changed;        //���Խ����Ƿ��޸�����
    int8 cfo_threshold_low;         //Ƶƫ��С��ֵ
    int8 cfo_threshold_high;        //Ƶƫ�����ֵ
    uint8 cfo_write_mode;           //Ƶƫ���д��ģʽ
    uint8 reserved[2];
    int32  cfo_upt_offset;          //UPT��ʼƵ��ƫ��
    uint8 pwr_test;
    int8 pwr_threshold_low;
    int8 pwr_threshold_high;
    uint8 ber_test;
    int8 ber_threshold_low;
    int8 ber_threshold_high;
}mp_test_arg_t;

typedef struct
{
    uint8 cmp_btname_flag;
    uint8 cmp_btname[TEST_BTNAME_MAX_LEN];
    uint8 cmp_blename_flag;
    uint8 cmp_blename[TEST_BTBLENAME_MAX_LEN];
}read_btname_test_arg_t;

typedef struct
{
    uint8  stub_head[6];
    uint16 test_id;
    uint8  test_result;
    uint8  timeout;
    /*�䳤����*/
    uint16  return_arg[0];
}return_result_t;

typedef struct
{
    uint8 front_ap_id;
    uint8 ap_switch_flag;
}test_share_info_t;

typedef struct
{
    uint32 magicl;
    uint32 magich;
    uint8 reserved;
    uint8 addr[3];
    uint32 record_cnt;
    uint32 checksum;
}btaddr_log_file_t;

typedef struct
{
    stub_ext_cmd_t  cmd;
    uint8 log_data[0];
}print_log_t;


typedef struct
{
    uint8 magic[4];         //"acbt"

}bt_test_packet_info_t;

/* 
    bkitem.bin��bkitex.bin���ǻ㼯������bin�ļ������bin�ļ����ֱ�������ص��Ĳ��������.
    bkitem.bin�е�ÿ������������ͬ��С����������bkitex.binҲ����ˣ�ֻ����Ҫ��Ĳ���������С��ǰ��(bkitem.bin)��ͬ.
    ͨ��������ű����֪���Ӳ����������������bin�ļ��е���ʼ��ַ���Ӷ����м���.

    test_id_to_bank_id_map_t�ṹ��������ÿ���Ӳ�������bin�ļ���bank������ŵ�ӳ���ϵ.
������

*/
typedef struct 
{
    uint16 test_id;
    uint8  bin_id;
    uint8  bank_id;
}test_id_to_bank_id_map_t;

//attЭ��: 'atf�ļ����ݶ�ȡ����'���ݰ��ṹ
typedef struct{
    stub_ext_cmd_t  cmd;
    uint32          offset;     //offset:  atf�ļ�λ��
    uint32          readLen;    //readLen: ��Ҫ��ȡ���ļ�����
}__PACKED__  atf_file_read_t;

//attЭ�飺 'atf�ļ����ݶ�ȡ����'���ݰ��ṹ
typedef struct{
    stub_ext_cmd_t  cmd;
    uint8           payload[0];
}__PACKED__ atf_file_read_ack_t;

//����buffer��Ϣ���������main ap��bank��֮��ĺ������ô���
typedef struct{
    uint8*  bank_base_func_buffer;
    uint32  bank_base_func_buffer_size;
    uint8*  bank_test_item_buffer[TEST_ITEM_BIN_ID_MAX];
    uint32  bank_test_item_buffer_size[TEST_ITEM_BIN_ID_MAX];
    uint8*  log_file_buffer;
    uint32  log_file_buffer_size;
    uint8*  cfg_file_buffer;
    uint32  cfg_file_length;  //ע����config.txt���ݵ�ʵ�ʳ���
}buffer_info_t;


/*
extern test_result_e act_test_linein_channel_test(void *arg_buffer);

extern test_result_e act_test_fm_channel_test(void *arg_buffer);

extern test_result_e act_test_mic_channel_test(void *arg_buffer);

extern test_result_e act_test_sdcard_play_test(void *arg_buffer);

extern test_result_e act_test_uhost_play_test(void *arg_buffer);

extern test_result_e act_test_linein_play_test(void *arg_buffer);

extern test_result_e act_test_fm_play_test(void *arg_buffer);

extern test_result_e att_mp_test(void *arg_buffer);

extern test_result_e att_mp_read_test(void *arg_buffer);

extern int32 act_test_read_test_info(uint8 read_line, uint16 *test_id, uint8 *arg_buffer, uint32 arg_len);

extern test_result_e act_test_modify_bt_addr(void *arg_buffer);

extern int32 act_test_read_bt_addr_from_record_file(uint8 *byte_buffer);

extern test_result_e act_test_modify_bt_name(void *arg_buffer);

extern test_result_e act_test_modify_bt_ble_name(void *arg_buffer);

extern void att_write_log_file(uint8 *test_info, bool test_result);

extern void att_write_log_file_data(uint8 *test_info, uint8* test_data, uint32 data_bytes);
*/

extern int32 act_test_stub_report_result(uint8 *write_data, uint16 payload_len);

extern uint32 att_write_data(uint16 cmd, uint32 payload_len, uint8* data_addr);

extern uint32 att_read_data(uint8* data_addr, uint32 payload_len);

extern int print_log(const char* format, ...);

extern int act_test_read_bt_addr(void *arg_buffer);
extern int act_test_read_bt_name(void *arg_buffer);
extern test_result_e act_test_bt_test(void *arg_buffer);

extern void byte_to_ascii(uint8 *byte_value, uint8 *ascii_buffer, uint32 byte_length);

extern int32 act_test_write_att_record_file(uint8 *byte_buffer, uint32 record_cnt, uint32 mode);

extern void write_log_and_file(int32 record_cnt);

extern bool create_app_engine(uint8 engine_id, uint8 para);
extern bool kill_current_app_engine(void);

extern void led_flash_ok(void);
extern void led_flash_fail(void);

extern bool act_test_get_btinfo(btengine_setinfo_t *p_btinfo);

extern void act_local_test_report_result_ext(uint8 *test_info, uint16 test_id, bool result, uint32 test_mode, uint8* log_buffer, uint32* cur_log_len);

extern void att_init_test_item_list_from_sd(uint8* cfg_buffer, uint32 max_buffer_len);

extern int read_atf_file(uint32 test_mode, const uint8* atf_file_name, uint8 * dstAddr, uint32 dst_buffer_len, uint32 atf_offset, uint32 total_len);
extern int atf_read_sub_file_attr(atf_head_t *p_atf_head, const uint8 * sub_file_name, uint32 * p_offset, uint32 * p_length);
extern int read_atf_sub_file_data(uint32 test_mode, uint8* dst_addr, uint32 dst_buffer_len, atf_head_t *p_atf_head, \
    const uint8* atf_file_name, const uint8 * sub_file_name, int32 offset_from_sub_file, int32 read_len);

extern void att_test_add_result_to_sd_log(uint8 *test_info, bool test_result, uint8 * log_buffer, uint32* cur_total_len);

extern void check_fatal_result(int ret, const uint8* print_err_info, ...);

#endif



