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
    unsigned char   chan;           /* ��Чֵ��0~78��0xff��ʾ��controller�Զ���Ƶ*/
}testRF_chan_t;

typedef struct
{
    test_ctl_e      opcode;
    unsigned char   pkt_type;       /* ����ʹ��ʱ��Ҫ����һ��enum,��Ӧ�õİ�����ӳ�䵽controller��Ӧ�İ�����*/
}testPKT_type_t;

typedef struct
{
    test_ctl_e      opcode;
    testTX_pattern_e payload_type;  /* �������ݰ����������� */
}testTX_pattern_t;

typedef struct
{
    test_ctl_e      opcode;
    unsigned short  payload_len;    /* �������ݰ��Ĵ�С */
}testTX_len_t;

typedef struct
{
    test_ctl_e      opcode;
    testRF_gain_e   gain;           /* ���书�ʣ��������ƣ�ֻ������ */
}testRF_gain_t;

typedef struct
{
    test_ctl_e      opcode;
    unsigned short  pkt_num;        /* ���͵����ݰ����� */
}testPKT_num_t;

typedef struct
{
    test_ctl_e      opcode;
    ct_test_mode_e  test_mode;      /* ���Գ��� */
}testCtl_mode_t;


//�����壬��ͬ�����Ĳ������ܲ�ͬ
typedef union
{
    ctlHdr_t            hdr;        /* test control header */
    testBD_addr_t       bd_addr;    /* ���������豸��ַ */
    testRF_chan_t       rf_chan;    /* ѡ������߽��յ��ŵ� */
    testPKT_type_t      pkt_type;   /* ���ݰ����� */
    testTX_pattern_t    tx_pattern; /* ���͵����ݸ�ʽ */
    testTX_len_t        tx_len;     /* ���͵����ݳ��� */
    testRF_gain_t       rf_gain;    /* ���书�� */
    testPKT_num_t       tx_num;     /* ���͵İ����� */
    testCtl_mode_t      ctl_mode;   /* ѡ�����ģʽ */
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
    unsigned int    tx_pkt_num;         /* �Ѿ����͵İ����� */
    unsigned int    tx_bit_num;         /* �Ѿ����͵�bit�� */
}testTX_report_t;

typedef struct
{
    evtHdr_t        hdr;
    char            rx_rssi;            /* �����ź�ǿ�� */
    unsigned int    rx_bit_num;         /* �Ѿ��յ���bit�� */
    unsigned int    rx_err_bit;         /* �յ��Ĵ���bit�� */
    unsigned int    rx_pkt_num;         /* �Ѿ��յ��İ��ĸ��� */
    unsigned int    rx_err_pkt_num;     /* �Ѿ��յ��ĳ���İ��ĸ��� */
}testRX_report_t;

typedef struct
{
    evtHdr_t        hdr;
    unsigned int    adc_data_addr;
    unsigned int    adc_data_len;
}testADC_data_t;

//�����壬��ͬtest control����ص��¼��������ܲ�ͬ
typedef union
{
    evtHdr_t            hdr;             /* event header */
    testTX_report_t     tx_report;      /* tx reporting */
    testRX_report_t     rx_report;      /* rx reporting */
    testADC_data_t      adc_data;       /* adc data */
}testEvt_t;

//�ϲ�Ӧ�ûص���������Ҫע��
typedef void (*Btest_EvtCback_t)(testEvt_t *pEvent);

//Ӧ�ó�ʼ��controller testģʽ���ò���
typedef struct bt_con_test_init_s
{
    unsigned int log_mask;           /* �ڲ���ӡ��ʹ��mask */
    Btest_EvtCback_t evtCback;       /* Ӧ��ע��ص� */

    //todo:���ϲ��ṩadc���ݴ��λ��???

}bt_con_test_init_t;




#endif

