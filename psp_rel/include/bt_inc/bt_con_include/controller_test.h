/********************************************************************************
 *                              GL5118
 *                            Module: Bluetooth
 *                 Copyright(c) 2001-2016 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>            <version >             <desc>
 *      Johnny   2016-01-20 11:32     1.0                build this file
 ********************************************************************************/
#ifndef __CONTROLLER_API_H__
#define __CONTROLLER_API_H__



/************************* include file *********************/







/************************* macro define *********************/
//status result Error Messages used by TEST
typedef enum
{
    TEST_NO_ERROR = 0x00,                  /* No Error */
    TEST_ILLEGAL_COMMAND,                  /* Illegal Command */
    TEST_DISALLOWED,                        /* Command was disallowed */
}test_status_e;

//test control opcode
typedef enum
{
    TEST_CTL_SET_BT_ADDR = 0x00,
    
    TEST_CTL_READ_ADC_DATA,
    
    TEST_CTL_STOP,
    TEST_CTL_START,
    
    TEST_CTL_SET_RF_CHAN,
    TEST_CTL_SET_RF_GAIN,
    
    TEST_CTL_SET_PKT_TYPE,
    TEST_CTL_SET_PAYLOAD_TYPE,
    TEST_CTL_SET_PAYLOAD_LEN,
    TEST_CTL_SET_SEND_PKT_NUM,
    
    TEST_CTL_TEST_MODE,
    
    TEST_CTL_RX_REPORT,
    TEST_CTL_TX_REPORT,
}test_ctl_e;

//callback event type
//only two event now
typedef enum
{
    TEST_EVT_CMD_COMPLETE = 0x0e,              /* Command completed event */
    TEST_EVT_CMD_STATUS = 0x0F,                /* command status event */
}test_evt_e;

typedef enum
{
    TX_0_Pattern_test = 0x00, 
    TX_1_Pattern_test = 0xff, 
    TX_01_Pattern_test = 0x55,
    TX_10_Pattern_test = 0xaa,
    TX_0x0_to_0xf_Pattern_test = 0x88,  
    TX_00001111_Pattern_test = 0x0f,
    TX_11110000_Pattern_test = 0xf0,
    TX_Psuedo_Random_test = 0x99, 
} testTX_pattern_e;       

typedef enum 
{
    PACKET_TX = 0x00,
    PACKET_RX,
    
    CONTINUE_TX,
    CONTINUE_RX,
    
    SINGLE_TONE_TX,
    SINGLE_TONE_RX,
}ct_test_mode_e;

typedef enum
{
    RF_GAIN_LOW = 0x00,
    RF_GAIN_MIDDLE,
    RF_GAIN_HIGH,
}testRF_gain_e;




/************************* struct define *********************/
typedef struct
{
    test_ctl_e      opcode;
}ctlHdr_t;

typedef struct
{
    test_ctl_e      opcode;
    unsigned char   bytes[6];    
}testBD_addr_t;

typedef struct
{
    test_ctl_e      opcode;
    unsigned char   chan;           /* 有效值是0~78，0xff表示由controller自动跳频*/
}testRF_chan_t;

typedef struct
{
    test_ctl_e      opcode;
    unsigned char   pkt_type;       /* 后续使用时需要定义一个enum,将应用的包类型映射到controller对应的包类型*/
}testPKT_type_t;

typedef struct
{
    test_ctl_e      opcode;
    testTX_pattern_e payload_type;  /* 发送数据包的数据类型 */
}testTX_pattern_t;

typedef struct
{
    test_ctl_e      opcode;
    unsigned short  payload_len;    /* 发送数据包的大小 */
}testTX_len_t;

typedef struct
{
    test_ctl_e      opcode;
    testRF_gain_e   gain;           /* 发射功率，做了限制，只有三档 */
}testRF_gain_t;

typedef struct
{
    test_ctl_e      opcode;
    unsigned short  pkt_num;        /* 发送的数据包个数 */
}testPKT_num_t;

typedef struct
{
    test_ctl_e      opcode;
    ct_test_mode_e  test_mode;      /* 测试场景 */
}testCtl_mode_t;


//共用体，不同命令定义的参数可能不同
typedef union
{
    ctlHdr_t            hdr;        /* test control header */
    testBD_addr_t       bd_addr;    /* 本地蓝牙设备地址 */
    testRF_chan_t       rf_chan;    /* 选择发射或者接收的信道 */
    testPKT_type_t      pkt_type;   /* 数据包类型 */
    testTX_pattern_t    tx_pattern; /* 发送的数据格式 */
    testTX_len_t        tx_len;     /* 发送的数据长度 */
    testRF_gain_t       rf_gain;    /* 发射功率 */
    testPKT_num_t       tx_num;     /* 发送的包个数 */
    testCtl_mode_t      ctl_mode;   /* 选择测试模式 */
}testCtl_t;

typedef struct
{
    test_evt_e    event;
    test_status_e status;
    test_ctl_e    opcode;
}evtHdr_t;

typedef struct
{
    evtHdr_t        hdr;
    unsigned int    tx_pkt_num;         /* 已经发送的包个数 */
    unsigned int    tx_bit_num;         /* 已经发送的bit数 */
}testTX_report_t;

typedef struct
{
    evtHdr_t        hdr;
    char            rx_rssi;            /* 接收信号强度 */
    unsigned int    rx_bit_num;         /* 已经收到的bit数 */
    unsigned int    rx_err_bit;         /* 收到的错误bit数 */
    unsigned int    rx_pkt_num;         /* 已经收到的包的个数 */
    unsigned int    rx_err_pkt_num;     /* 已经收到的出错的包的个数 */
}testRX_report_t;

typedef struct
{
    evtHdr_t        hdr;
    unsigned int    adc_data_addr;
    unsigned int    adc_data_len;
}testADC_data_t;

//共用体，不同test control命令返回的事件参数可能不同
typedef union
{
    evtHdr_t            hdr;             /* event header */
    testTX_report_t     tx_report;      /* tx reporting */
    testRX_report_t     rx_report;      /* rx reporting */
    testADC_data_t      adc_data;       /* adc data */
}testEvt_t;

//上层应用回调函数，需要注册
typedef void (*Btest_EvtCback_t)(testEvt_t *pEvent);

//应用初始化controller test模式配置参数
typedef struct bt_con_test_init_s
{
    unsigned int log_mask;           /* 内部打印的使能mask */
    Btest_EvtCback_t evtCback;       /* 应用注册回调 */

    //todo:由上层提供adc数据存放位置???

}bt_con_test_init_t;




#endif

