/********************************************************************************
 *                              USDK(GL5110)
 *                            Module: KERNEL
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>      <time>           <version >             <desc>
 *      wuyueqian     2011-9-10 09:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     typestruct.h
 * \brief    �����ṹ�嶨��
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/
#ifndef _TYPESTRUCT_H
#define _TYPESTRUCT_H

#include <typeext.h>

typedef struct
{
    uint16 read_ptr;
    uint16 write_ptr;
    uint16 length;
    uint16 capacity;
    uint16 full_error : 1;  //�������������������ݶ����Ĵ��󣻻����������߶������ݺ�Ӧ�����flush������ʱҲҪ���
    uint16 filter_flag : 1; //���������˱�־���������ù������ݣ���ʱû�ã���һ��SDK���øñ�־ʹģ�黯����
    uint16 reserve;         //word�������
    uint8 *raw_data;
    uint8 *ext_data;
} buffer_rw_t;

typedef struct
{
    uint8  pkg_status;
    uint8  reserve;
    uint16 pkg_length;
} sco_err_report_t;

typedef struct
{
    uint8  read_ptr;
    uint8  write_ptr;
    uint8  length;
    uint8  count;
    sco_err_report_t *err_report_data;
} sco_err_report_info_t;

typedef enum
{
    AUD_OUTPUT_DUAL_CHAN        = 0,
    AUD_OUTPUT_MIX_LEFT_CHAN    = 1,
    AUD_OUTPUT_MIX_RIGHT_CHAN   = 2,
    AUD_OUTPUT_MIX_DAUL_CHAN    = 3,
    AUD_OUTPUT_SWITCH_DAUL_CHAN = 4,
    AUD_OUTPUT_LEFT_CHAN        = 5, //�����������
    AUD_OUTPUT_RIGHT_CHAN       = 6, //�����������
} aud_output_channel_e;


//�������� TWS �豸��ɫ����
typedef enum
{
    NORMAL_DEV,
    TWS_MASTER,
    TWS_SLAVE,
} dev_role_e;

//�������� TWS ��������λ�ö���
typedef enum
{
    TWS_SPK_STEREO,
    TWS_SPK_LEFT,
    TWS_SPK_RIGHT,
} dev_spk_pos_e;

//TWS SYNC CMD GROUP
#define TWS_SYNC_GROUP_PLAY         0x01 //���ڲ��ſ��ƣ������м������
#define TWS_SYNC_GROUP_EG           0x02 //����Э�����ţ������������
#define TWS_SYNC_GROUP_UI           0x03 //����UI���ƣ�����Ӧ�ý�������

typedef struct
{
    uint8 cmd_group; //��ʾ�����ĸ�ģ�飬�Ա�������ͬ�㼶�Ự
    uint8 cmd_id;
    uint8 cmd_para;
    uint8 cmd_len;
    uint8 cmd_data[16];
} tws_sync_cmd_t;

#define CMD_TYPE_HEAD_LEN 4
#define CMD_TYPE_DATA_LEN 20

//����ʱ���
typedef struct
{
    uint32 bt_clk; /*312.5usΪ��λ����2bitΪ0*/
    uint16 bt_intraslot; /*0~1249us*/
} bt_clock_t;

typedef struct
{
    uint32 head;
    uint32 total_size;
}btmempool_cfg_t;

typedef bool (*f_get_bt_clk)(bt_clock_t *p_bt_clock);
typedef bool (*f_set_bt_clk)(uint32 inform_clk);
typedef void (*f_bt_clk_sync_cbk)(void);
typedef void (*f_register_bt_clk_sync_cbk)(f_bt_clk_sync_cbk bt_clk_sync_cbk);
typedef bool (*f_has_avdtp_pkg_lost)(void);

typedef void (*f_send_frame)(uint8 *frame_data, uint16 data_len);

#endif  /* _TYPESTRUCT_H */
