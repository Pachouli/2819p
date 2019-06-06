/********************************************************************************
 *                              USDK281A
 *                            Module: KERNEL
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>      <time>           <version >             <desc>
 *      wuyufan     2015-9-10 09:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     stub_interface.h
 * \brief    �ں˽ӿ��б�
 * \author   wuyufan
 * \version  1.0
 * \date  2015/9/10
 *******************************************************************************/


#ifndef _STUB_INTERFACE_H
#define _STUB_INTERFACE_H

#include "typeext.h"
#include "kernel_interface.h"
#include "stub_command.h"
#include "api.h"

#define OT_10MS_132MHZ                  (1200)
#define OT_500MS_72MHZ                  (300000)
#define OT_500MS_132MHZ                 (600000)

#define STUB_DELAY_100MS                (100)
#define STUB_DELAY_200MS                (100)
#define STUB_DELAY_500MS                (500)
#define STUB_DELAY_1000MS               (1000)

#define STUB_TRANS_EXT_CMD_HEADER_LEN   (8)

//stub��ӡģ�����
#define PRINT_MODULE_KERNEL             (1)
#define PRINT_MODULE_NOR                (2)
#define PRINT_MODULE_ANALOG             (3)
#define PRINT_MODULE_UDEVICE            (4)
#define PRINT_MODULE_UHOST              (5)
#define PRINT_MODULE_CARD               (6)
#define PRINT_MODULE_FAT                (7)
#define PRINT_MODULE_EXFAT              (8)

#define PRINT_MODULE_MP3_DEC            (30)
#define PRINT_MODULE_WMA_DEC            (31)
#define PRINT_MODULE_SBC_DEC            (32)
#define PRINT_MODULE_AAC_DEC            (33)
#define PRINT_MODULE_WAV_DEC            (34)
#define PRINT_MODULE_WAV_ENC            (35)
#define PRINT_MODULE_PEQ                (36)
#define PRINT_MODULE_AEC                (37)
#define PRINT_MODULE_HFP                (38)
#define PRINT_MODULE_MMM_MUSIC          (39)
#define PRINT_MODULE_MMM_BT             (40)
#define PRINT_MODULE_MMM_SPEECH         (41)
#define PRINT_MODULE_MMM_PCM            (42)


#define PRINT_MODULE_BT_CONTROLLER      (60)
#define PRINT_MODULE_MUSIC_ENGINE       (61)
#define PRINT_MODULE_BT_HOST            (62)
#define PRINT_MODULE_AP_MANAGER         (63)
#define PRINT_MODULE_AP_MUSIC           (64)
#define PRINT_MODULE_AP_CONFIG          (65)
#define PRINT_MODULE_AP_UDEVICE         (66)
#define PRINT_MODULE_AP_UPGRADE         (67)


//stub��ӡ�Ĵ���ȼ�
#define STUB_PRINT_ERRGRADE_0           (0)
#define STUB_PRINT_ERRGRADE_1           (1)
#define STUB_PRINT_ERRGRADE_2           (2)
#define STUB_PRINT_ERRGRADE_3           (3)





//trans command
typedef struct
{
    uint8 type;
    uint8 tools_type;
    uint8 sub_opcode;
    uint8 para;
    uint32 data_len;
} stub_trans_cmd_t;


typedef struct
{
    uint8 type;
    uint8 tools_type;
    uint8 sub_opcode;
    uint8 para;
    uint16 payload_len;
}stub_ext_cmd_t;


typedef struct
{
    uint8 *rw_buffer;
    uint16 read_len;
} stub_rw_param_t;


/*! \cond MMM_ID_API*/
/*!
 *  \brief
 *     �ṩ��Ӧ�ò��stub����
 */
typedef enum
{
    /*! ��stub */
    STUB_OP_OPEN = 0,
    /*! �ر�stub */
    STUB_OP_CLOSE = 1,
    /*! set param*/
    STUB_OP_IOCTL = 2,
    /*! read */
    STUB_OP_READ = 3,
    /*! write */
    STUB_OP_WRITE = 4,
    /*! read write ext */
    STUB_OP_EXT_RW = 5,
    /*! read write raw */
    STUB_OP_RAW_RW
} stub_cmd_t;


typedef enum
{
    STUB_USE_URAM0 = 0,
    STUB_USE_URAM1
} stub_uram_type_e;


typedef enum
{
    SET_TIMEOUT = 0,
    RESET_FIFO,
    SWITCH_URAM,
    SET_STATUS,
    GET_CONNECT_STATE,
    SET_LED_STATE,
    SET_READ_MODE,
    SET_DEBUG_MODE,
    SET_STUB_PRINT_MODE,
} stub_ioctl_t;


typedef struct
{
    uint16 opcode;
    uint16 payload_len;
    uint8 *rw_buffer;
} stub_ext_param_t;



typedef struct
{
/*
    uint16 module_id;       //ģ��ID��ȡֵ��Χ 0 ~ N����� print_info.ini ����
    uint8  err_grade;       //����ȼ���ȡֵ��Χ 0 ~ N����� print_info.ini ����
    uint8  reserved;
*/
    uint32 module_info;     //����μ�����ע��
    uint8 string[4];        //�ַ����������ԡ�\0����β�������ڲ���������
}print_string_t;

typedef struct
{
/*
    uint16 module_id;       //ģ��ID��ȡֵ��Χ 0 ~ N����� print_info.ini ����
    uint8  err_grade;       //����ȼ���ȡֵ��Χ 0 ~ N����� print_info.ini ����
    uint8  reserved;
*/
    uint32 module_info;     //����μ�����ע��

    union
    {
        uint32 data;        //Ҫ��ӡ������
        uint32 start_address;   //������ʼ��ַ��word��ַ����
    }data_address;
}print_info_head_t;


typedef struct
{
    print_info_head_t print_info;
    uint8 string[4];    //�ַ����������ԡ�\0����β�������ڲ���������
}print_string_data_t;


typedef struct
{
    print_info_head_t print_info;
    uint8 data[2];      //���ϵ�ַ��ʼ������,������word����,�����нӿ��ж���
}print_memory_t;





#define STUB_COMMAND_HEAD_SIZE          (8)
#define STUB_EXT_COMMAND_HEAD_SIZE      (6)




/*
 STUB ���ϲ��ṩ�������ָ��, ��������ʽ�������, ��������Щ����ָ���ַ
 */
#define STUB_INTERFACE_ENTRY_ADDR(x)    (STUB_ENTRY_VA_START + (4 * x))

#define STUB_OP_CMD_ENTRY               (*((uint32*)(STUB_INTERFACE_ENTRY_ADDR(0))))
#define STUB_PRINT_STRING_ENTRY         (*((uint32*)(STUB_INTERFACE_ENTRY_ADDR(1))))
#define STUB_PRINT_STRING_DATA_ENTRY    (*((uint32*)(STUB_INTERFACE_ENTRY_ADDR(2))))
#define STUB_PRINT_MEMORY_ENTRY         (*((uint32*)(STUB_INTERFACE_ENTRY_ADDR(3))))
#define STUB_UPLOAD_MEMORY_ENTRY        (*((uint32*)(STUB_INTERFACE_ENTRY_ADDR(4))))



#define STUB_OP_ENTRY(a, b, c, d)       ((int (*)(void* , void* , void* , int ))STUB_OP_CMD_ENTRY)((a), (b), (c), (d))


/*����stub,�豸��pc����*/
/*int stub_open(bool need_uninstall_led)*/
#define stub_open(a)                    (int)STUB_OP_ENTRY((void*)(a), (void*)(0), (void*)(0), STUB_OP_OPEN)

/*�ر�stub,�豸��pc�Ͽ�����*/
/*int stub_close(void)*/
#define stub_close()                    (int)STUB_OP_ENTRY((void*)(0), (void*)(0), (void*)(0), STUB_OP_CLOSE)

/*��ȡ����pc���͵�����*/
/*int stub_get_data(uint16 opcode, uint8 *data_buffer, uint32 data_len)*/
/*opcode��Э��������;data_buffer��������ݵ�bufferָ��;data_len�����ݳ���*/
#define stub_get_data(a, b, c)          (int)STUB_OP_ENTRY((void*)(a), (void*)(b), (void*)(c), STUB_OP_READ)

/*��pc��������*/
/*int stub_set_data(uint16 opcode, uint8 *data_buffer, uint32 data_len)*/
/*opcode��Э��������;data_buffer��������ݵ�bufferָ��;data_len�����ݳ���*/
#define stub_set_data(a, b, c)          (int)STUB_OP_ENTRY((void*)(a), (void*)(b), (void*)(c), STUB_OP_WRITE)

/*Ԥ����չ*/
/*int stub_ioctrl_set(uint16 op_code, void *param1, void *param2)*/
#define stub_ioctrl_set(a, b, c)        (int)STUB_OP_ENTRY((void*)(a), (void*)(b), (void*)(c), STUB_OP_IOCTL)

/*��ȡ����pc���͵�����*/
/*int stub_ext_read(stub_ext_param_t *ext_param)*/
/*ext_param: ������ַ*/
#define stub_ext_read(a)                (int)STUB_OP_ENTRY((void*)(a), (void*)(0), (void*)(0), STUB_OP_EXT_RW)

/*��pc��������*/
/*int stub_ext_write(stub_ext_param_t *ext_param)*/
/*ext_param: ������ַ */
#define stub_ext_write(a)               (int)STUB_OP_ENTRY((void*)(a), (void*)(1), (void*)(0), STUB_OP_EXT_RW)

/*��ȡ����pc���͵�����*/
/*int stub_raw_read(void *ext_cmd, uint32 cmd_len)*/
/*ext_cmd: ��չ�����buffer ��ַ; cmd_len: ��չ�����*/
#define stub_raw_read(a, b)             (int)STUB_OP_ENTRY((void*)(a), (void*)(b), (void*)(0), STUB_OP_RAW_RW)

/*��pc����������,��������*/
/*int stub_raw_write(void *ext_cmd, uint32 cmd_len)*/
/*ext_cmd: ��չ�����buffer ��ַ; cmd_len: ��չ�����*/
#define stub_raw_write(a, b)            (int)STUB_OP_ENTRY((void*)(a), (void*)(b), (void*)(1), STUB_OP_RAW_RW)

/* ��ȡ����״̬; a: ������ӱ�־, 0: �����ϴ�����״̬, 1: ���¼������״̬;
   ����ֵ <= 0: δ����, > 0: ������ (���� UART_RX1: A21 �� A16). */
#define stub_get_connect_state(a)       (int)STUB_OP_ENTRY((void*)GET_CONNECT_STATE, (void*)(a), (void*)(0), STUB_OP_IOCTL)

/* ���� LED ָʾ��״̬ (ÿһλ��ʾһ��ָʾ��); a: LED ָʾ�Ʊ������, b: ״̬ . */
#define stub_set_led_state(a, b)        (int)STUB_OP_ENTRY((void*)SET_LED_STATE, (void*)(a), (void*)(b), STUB_OP_IOCTL)

/* ���õ���ģʽ; 1: ������Դ�ӡ, 0: ��ֹ���Դ�ӡ. */
#define stub_set_debug_mode(a)          (int)STUB_OP_ENTRY((void*)SET_DEBUG_MODE, (void*)(a), (void*)(0), STUB_OP_IOCTL)

/* �л�ϵͳ��ӡģʽ: 0:����ӡ��STUB�л�UART��ӡ; 1:����ӡ��UART�л�Ϊstub��ӡ . */
#define stub_switch_print_mode(a)       (int)STUB_OP_ENTRY((void*)SET_STUB_PRINT_MODE, (void*)(a), (void*)(0), STUB_OP_IOCTL)



//stub��ӡ�ӿ�
/*
int STUB_PRINT_STRING(UINT32 module_id, UINT8 *string, UINT32 str_length);
���ַ����뵱ǰģ��ID��Ϣ��ͨ��USB��ӡ��stub print������
���룺 module_id��ģ��ID�ʹ�ӡ������Ϣ����ͷ�ļ���ͨ������ͳһ����
            string: ����ӡ���ַ���ָ��
            str_length: ����ӡ���ַ�������,0:�ַ�������0����; ��0:ָ������
�������ӡ�Ƿ�ɹ���0��success��-1��fail
*/
#define STUB_PRINT_STRING(a, b, c)      ((int (*)(UINT32 module_id, UINT8 *string, UINT32 str_length))STUB_PRINT_STRING_ENTRY)((a), (b), (c))

/*
int STUB_PRINT_STRING_DATA(UINT32 module_id, UINT8 *string, UINT32 str_length, UINT32 data);
���ַ����������Լ���ǰģ��ID����Ϣ��ͨ��USB��ӡ��stub print������
���룺 module_id��ģ��ID�ʹ�ӡ������Ϣ����ͷ�ļ���ͨ������ͳһ����
            string: ����ӡ���ַ���ָ��
            str_length: ����ӡ���ַ�������,0:�ַ�������0����; ��0:ָ������
            data: ����ӡ������
�������ӡ�Ƿ�ɹ���0��success��-1��fail
*/
#define STUB_PRINT_STRING_DATA(a, b, c, d) \
    ((int (*)(UINT32 module_id, UINT8 *string, UINT32 str_length, UINT32 data))STUB_PRINT_STRING_DATA_ENTRY)((a), (b), (c), (d))

/*
int STUB_PRINT_MEMORY(UINT32 module_id, UINT32 address, UINT32 length);
���ڴ��I/O�������뵱ǰģ��ID����Ϣ��ͨ��USB��ӡ��stub print������
���룺 module_id��ģ��ID�ʹ�ӡ������Ϣ����ͷ�ļ���ͨ������ͳһ����
            address������ӡ������ʼ��ַ
            length������ӡ���ݳ���
�������ӡ�Ƿ�ɹ���0��success��-1��fail
*/
#define STUB_PRINT_MEMORY(a, b, c)      ((int (*)(UINT32 module_id, UINT32 address, UINT32 length))STUB_PRINT_MEMORY_ENTRY)((a), (b), (c))

/*
int STUB_UPLOAD_MEMORY(UINT32 module_id, UINT32 address, UINT32 length);
���ڴ��I/O�е����ݣ�ͨ��USB����PC�������浽��ID+address����Ķ������ļ���ȥ
���룺 module_id��ģ��ID�ʹ�ӡ������Ϣ����ͷ�ļ���ͨ������ͳһ����
            address��������������ʼ��ַ
            length�����������ݳ���
�������ӡ�Ƿ�ɹ���0��success��-1��fail
*/
#define STUB_UPLOAD_MEMORY(a, b, c)     ((int (*)(UINT32 module_id, UINT32 address, UINT32 length))STUB_UPLOAD_MEMORY_ENTRY)((a), (b), (c))
#endif

