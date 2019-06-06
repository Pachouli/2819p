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
 * \brief    内核接口列表
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

//stub打印模块序号
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


//stub打印的错误等级
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
 *     提供给应用层的stub命令
 */
typedef enum
{
    /*! 打开stub */
    STUB_OP_OPEN = 0,
    /*! 关闭stub */
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
    uint16 module_id;       //模块ID，取值范围 0 ~ N，详见 print_info.ini 定义
    uint8  err_grade;       //错误等级，取值范围 0 ~ N，详见 print_info.ini 定义
    uint8  reserved;
*/
    uint32 module_info;     //定义参见上面注释
    uint8 string[4];        //字符串（必须以’\0’结尾），属于不定长数据
}print_string_t;

typedef struct
{
/*
    uint16 module_id;       //模块ID，取值范围 0 ~ N，详见 print_info.ini 定义
    uint8  err_grade;       //错误等级，取值范围 0 ~ N，详见 print_info.ini 定义
    uint8  reserved;
*/
    uint32 module_info;     //定义参见上面注释

    union
    {
        uint32 data;        //要打印的数字
        uint32 start_address;   //数据起始地址，word地址对齐
    }data_address;
}print_info_head_t;


typedef struct
{
    print_info_head_t print_info;
    uint8 string[4];    //字符串（必须以’\0’结尾），属于不定长数据
}print_string_data_t;


typedef struct
{
    print_info_head_t print_info;
    uint8 data[2];      //从上地址开始的数据,长度以word对齐,长度有接口中定义
}print_memory_t;





#define STUB_COMMAND_HEAD_SIZE          (8)
#define STUB_EXT_COMMAND_HEAD_SIZE      (6)




/*
 STUB 向上层提供多个函数指针, 以数组形式存放起来, 以下是这些函数指针地址
 */
#define STUB_INTERFACE_ENTRY_ADDR(x)    (STUB_ENTRY_VA_START + (4 * x))

#define STUB_OP_CMD_ENTRY               (*((uint32*)(STUB_INTERFACE_ENTRY_ADDR(0))))
#define STUB_PRINT_STRING_ENTRY         (*((uint32*)(STUB_INTERFACE_ENTRY_ADDR(1))))
#define STUB_PRINT_STRING_DATA_ENTRY    (*((uint32*)(STUB_INTERFACE_ENTRY_ADDR(2))))
#define STUB_PRINT_MEMORY_ENTRY         (*((uint32*)(STUB_INTERFACE_ENTRY_ADDR(3))))
#define STUB_UPLOAD_MEMORY_ENTRY        (*((uint32*)(STUB_INTERFACE_ENTRY_ADDR(4))))



#define STUB_OP_ENTRY(a, b, c, d)       ((int (*)(void* , void* , void* , int ))STUB_OP_CMD_ENTRY)((a), (b), (c), (d))


/*启动stub,设备与pc连接*/
/*int stub_open(bool need_uninstall_led)*/
#define stub_open(a)                    (int)STUB_OP_ENTRY((void*)(a), (void*)(0), (void*)(0), STUB_OP_OPEN)

/*关闭stub,设备与pc断开连接*/
/*int stub_close(void)*/
#define stub_close()                    (int)STUB_OP_ENTRY((void*)(0), (void*)(0), (void*)(0), STUB_OP_CLOSE)

/*读取数据pc发送的数据*/
/*int stub_get_data(uint16 opcode, uint8 *data_buffer, uint32 data_len)*/
/*opcode：协议命令字;data_buffer：存放数据的buffer指针;data_len：数据长度*/
#define stub_get_data(a, b, c)          (int)STUB_OP_ENTRY((void*)(a), (void*)(b), (void*)(c), STUB_OP_READ)

/*向pc发送数据*/
/*int stub_set_data(uint16 opcode, uint8 *data_buffer, uint32 data_len)*/
/*opcode：协议命令字;data_buffer：存放数据的buffer指针;data_len：数据长度*/
#define stub_set_data(a, b, c)          (int)STUB_OP_ENTRY((void*)(a), (void*)(b), (void*)(c), STUB_OP_WRITE)

/*预留扩展*/
/*int stub_ioctrl_set(uint16 op_code, void *param1, void *param2)*/
#define stub_ioctrl_set(a, b, c)        (int)STUB_OP_ENTRY((void*)(a), (void*)(b), (void*)(c), STUB_OP_IOCTL)

/*读取数据pc发送的数据*/
/*int stub_ext_read(stub_ext_param_t *ext_param)*/
/*ext_param: 参数地址*/
#define stub_ext_read(a)                (int)STUB_OP_ENTRY((void*)(a), (void*)(0), (void*)(0), STUB_OP_EXT_RW)

/*往pc发送数据*/
/*int stub_ext_write(stub_ext_param_t *ext_param)*/
/*ext_param: 参数地址 */
#define stub_ext_write(a)               (int)STUB_OP_ENTRY((void*)(a), (void*)(1), (void*)(0), STUB_OP_EXT_RW)

/*读取数据pc发送的数据*/
/*int stub_raw_read(void *ext_cmd, uint32 cmd_len)*/
/*ext_cmd: 扩展命令的buffer 地址; cmd_len: 扩展命令长度*/
#define stub_raw_read(a, b)             (int)STUB_OP_ENTRY((void*)(a), (void*)(b), (void*)(0), STUB_OP_RAW_RW)

/*往pc发送裸数据,不带命令*/
/*int stub_raw_write(void *ext_cmd, uint32 cmd_len)*/
/*ext_cmd: 扩展命令的buffer 地址; cmd_len: 扩展命令长度*/
#define stub_raw_write(a, b)            (int)STUB_OP_ENTRY((void*)(a), (void*)(b), (void*)(1), STUB_OP_RAW_RW)

/* 获取连接状态; a: 检测连接标志, 0: 返回上次连接状态, 1: 重新检测连接状态;
   返回值 <= 0: 未连接, > 0: 已连接 (返回 UART_RX1: A21 或 A16). */
#define stub_get_connect_state(a)       (int)STUB_OP_ENTRY((void*)GET_CONNECT_STATE, (void*)(a), (void*)(0), STUB_OP_IOCTL)

/* 设置 LED 指示灯状态 (每一位表示一个指示灯); a: LED 指示灯编号掩码, b: 状态 . */
#define stub_set_led_state(a, b)        (int)STUB_OP_ENTRY((void*)SET_LED_STATE, (void*)(a), (void*)(b), STUB_OP_IOCTL)

/* 设置调试模式; 1: 允许调试打印, 0: 禁止调试打印. */
#define stub_set_debug_mode(a)          (int)STUB_OP_ENTRY((void*)SET_DEBUG_MODE, (void*)(a), (void*)(0), STUB_OP_IOCTL)

/* 切换系统打印模式: 0:将打印由STUB切回UART打印; 1:将打印由UART切换为stub打印 . */
#define stub_switch_print_mode(a)       (int)STUB_OP_ENTRY((void*)SET_STUB_PRINT_MODE, (void*)(a), (void*)(0), STUB_OP_IOCTL)



//stub打印接口
/*
int STUB_PRINT_STRING(UINT32 module_id, UINT8 *string, UINT32 str_length);
将字符串与当前模块ID信息，通过USB打印到stub print工具上
输入： module_id：模块ID和打印配置信息，在头文件中通过宏来统一定义
            string: 待打印的字符串指针
            str_length: 待打印的字符串长度,0:字符串遇到0结束; 非0:指定长度
输出：打印是否成功：0：success；-1：fail
*/
#define STUB_PRINT_STRING(a, b, c)      ((int (*)(UINT32 module_id, UINT8 *string, UINT32 str_length))STUB_PRINT_STRING_ENTRY)((a), (b), (c))

/*
int STUB_PRINT_STRING_DATA(UINT32 module_id, UINT8 *string, UINT32 str_length, UINT32 data);
将字符串与数据以及当前模块ID等信息，通过USB打印到stub print工具上
输入： module_id：模块ID和打印配置信息，在头文件中通过宏来统一定义
            string: 待打印的字符串指针
            str_length: 待打印的字符串长度,0:字符串遇到0结束; 非0:指定长度
            data: 待打印的数据
输出：打印是否成功：0：success；-1：fail
*/
#define STUB_PRINT_STRING_DATA(a, b, c, d) \
    ((int (*)(UINT32 module_id, UINT8 *string, UINT32 str_length, UINT32 data))STUB_PRINT_STRING_DATA_ENTRY)((a), (b), (c), (d))

/*
int STUB_PRINT_MEMORY(UINT32 module_id, UINT32 address, UINT32 length);
将内存或I/O中数据与当前模块ID等信息，通过USB打印到stub print工具上
输入： module_id：模块ID和打印配置信息，在头文件中通过宏来统一定义
            address：待打印数据起始地址
            length：待打印数据长度
输出：打印是否成功：0：success；-1：fail
*/
#define STUB_PRINT_MEMORY(a, b, c)      ((int (*)(UINT32 module_id, UINT32 address, UINT32 length))STUB_PRINT_MEMORY_ENTRY)((a), (b), (c))

/*
int STUB_UPLOAD_MEMORY(UINT32 module_id, UINT32 address, UINT32 length);
将内存或I/O中的数据，通过USB传到PC，并保存到以ID+address命令的二进制文件中去
输入： module_id：模块ID和打印配置信息，在头文件中通过宏来统一定义
            address：待传输数据起始地址
            length：待传输数据长度
输出：打印是否成功：0：success；-1：fail
*/
#define STUB_UPLOAD_MEMORY(a, b, c)     ((int (*)(UINT32 module_id, UINT32 address, UINT32 length))STUB_UPLOAD_MEMORY_ENTRY)((a), (b), (c))
#endif

