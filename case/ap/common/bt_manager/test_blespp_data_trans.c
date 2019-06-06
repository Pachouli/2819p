/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：蓝牙BLE或SPP数传测试代码，使用炬芯自主开发的蓝牙助手进行测试。
 * 作者：cailizhen
 ********************************************************************************/

#include "common_bt_inner.h"

#define ENABLE_TEST_BLESPP_DATA_TRANS  0 //是否打开炬芯自主开发蓝牙调试工具数传测试模式

#if (ENABLE_TEST_BLESPP_DATA_TRANS == 0)

bool test_blespp_data_trans_init(void)
{
    return TRUE;
}

bool test_blespp_data_trans_exit(void)
{
    return TRUE;
}

bool test_blespp_data_trans_dispatch(bt_passth_link_type ltype, buffer_rw_t* buf)
{
    uint8 temp_buf[33];
    uint16 length, i;

    if (buf->length > 0)
    {
        length = (buf->length > 32) ? 32 : buf->length;
        buffer_rw_read(buf, temp_buf, length);

        temp_buf[length] = 0;

        for (i = 0; i < length; i++)
        {
            temp_buf[i]++;
        }

        bt_manager_blespp_data_trans_send_inner(ltype, temp_buf, length);
    }

    buffer_rw_flush(buf);
    return TRUE;
}

#else


//数据结构定义

#define TC_PRINTD_INFO_INT(str,data)    libc_printf("%s%x\n",str,data)
#define TC_PRINTD_INFO(str)             libc_printf("%s\n",str)
#define TC_PRINTD_DATA(buf,len)         libc_printhex(buf,len,0)

typedef enum
{
    PKG_DEAL_HEAD  = 0, //1B 0xfe
    PKG_DEAL_HEAD2 = 1, //1B 0x01
    PKG_DEAL_TYPE  = 2, //1B
    PKG_DEAL_LEN   = 3, //1B
    PKG_DEAL_CMD   = 4, //1B
    PKG_DEAL_PARA  = 5, //nB
    PKG_DEAL_SUM   = 6, //2B
} pkg_deal_sta_e;

typedef enum
{
    TEST_TYPE_DOWNLOAD,
    TEST_TYPE_UPLOAD,
    TEST_TYPE_LOOP,
} test_type_e;

#define TEST_CFG_ACK_SHIFT          (1<<0)
#define TEST_CFG_CHECK_SHIFT        (1<<1)
#define TEST_CFG_CHECKTYPE_SHIFT    (1<<2)

#define TEST_PACKET_TYPE_CMD        (0)
#define TEST_PACKET_TYPE_DATA       (0x55)

#define TEST_CMD_ACK                (0xe0)
#define TEST_CMD_START              (1)
#define TEST_CMD_STOP               (2)
#define TEST_CMD_GET_RESULT         (3)
#define TEST_CMD_SEND_RESULT        (4)

#define TEST_PACKET_HEAD            (0xfe01)

#define TEST_PACKET_CMD_HEAD_LEN    (4)
#define TEST_PACKET_DATA_HEAD_LEN   (8)
#define TEST_PACKET_SUMCRC_LEN      (2)

#define TEST_UPLOAD_DATA_LEN        (256)
#define SEND_UPLOAD_BUFFER_SIZE     (TEST_UPLOAD_DATA_LEN + 16)

#define SEND_UPLOAD_PACKET_LEN      (0x8000)

typedef struct
{
    uint32 all_len;
    uint32 lose_len;
    uint32 err_len;
    uint8 test_type;
    uint8 test_config;
    uint8 fixed_data;
} test_data_t;

typedef struct
{
    pkg_deal_sta_e test_pkg_deal_sta;
    uint8 test_pkg_need_len;
    uint8 test_pkg_type;
    uint8 test_pkg_cmd;
    uint8 test_pkg_len;
    uint8 test_pkg_index;

    test_data_t test_global_data;
    uint16 data_packet_send_sn;
    int8   wait_ack_cmd;// -1 is not ack
    uint8  check_data_sn;
    uint8 *send_upload_buffer;
    uint8 *temp_package_buffer;

    uint8 data_trans_mode; //数据传输模式，1表示下行，2表示上行
    uint8 data_trans_block; //数据传输块大小
} test_data_trans_t;

//全局变量定义
static test_data_trans_t *p_tdt;

uint16 add_sum_do(uint8 *data, uint32 len);
void cmd_send_packet(bt_passth_link_type ltype, uint8 cmd, uint8 context);
void cmd_send_result_packet(bt_passth_link_type ltype);


uint32 get_uint32(uint8 *data)
{
    return (data[0] + (data[1] << 8) + (data[2] << 16) + (data[3] << 24));
}

//初始化
bool test_blespp_data_trans_init(void)
{
    if (p_tdt == NULL)
    {
        p_tdt = sys_malloc(sizeof(test_data_trans_t));
        libc_memset(p_tdt, 0x0, sizeof(test_data_trans_t));
    }

    if (p_tdt->send_upload_buffer == NULL)
    {
        p_tdt->send_upload_buffer = (uint8 *) sys_malloc(SEND_UPLOAD_BUFFER_SIZE);
    }
    //TODO 后续节省空间优化
    if (p_tdt->temp_package_buffer == NULL)
    {
        p_tdt->temp_package_buffer = (uint8 *) sys_malloc(SEND_UPLOAD_BUFFER_SIZE);
    }

    p_tdt->send_upload_buffer[0] = TEST_PACKET_HEAD%256;
    p_tdt->send_upload_buffer[1] = TEST_PACKET_HEAD/256;
    p_tdt->send_upload_buffer[2] = TEST_PACKET_TYPE_DATA;
    p_tdt->send_upload_buffer[3] = 0x00;

    p_tdt->test_global_data.test_type = 0xff;

    p_tdt->test_pkg_deal_sta = PKG_DEAL_HEAD;
    p_tdt->test_pkg_need_len = 1;
    p_tdt->test_pkg_index = 0;

    return TRUE;
}

bool test_blespp_data_trans_exit(void)
{
    if (p_tdt->send_upload_buffer != NULL)
    {
        sys_free(p_tdt->send_upload_buffer);
        p_tdt->send_upload_buffer = NULL;
    }
    //TODO 后续节省空间优化
    if (p_tdt->temp_package_buffer != NULL)
    {
        sys_free(p_tdt->temp_package_buffer);
        p_tdt->temp_package_buffer = NULL;
    }

    if (p_tdt != NULL)
    {
        sys_free(p_tdt);
        p_tdt = NULL;
    }

    return TRUE;
}

//命令包解析分支
void cmd_packet_pro(bt_passth_link_type ltype, uint8 *data, uint32 len)
{
    uint8 err_id = 0;
    uint16 tmp, tmp1;
    uint32 tmp32;

    //TC_PRINTD_DATA(data, len);

    tmp = data[3];

    if (tmp != len)
    {
        err_id = 1;//len is err
        goto cmd_end;
    }

    tmp1 = data[tmp - 2] + (data[tmp - 1] << 8);

    if (tmp1 != add_sum_do(data, tmp - 2))
    {
        err_id = 2;//sum is err
        goto cmd_end;
    }

    data += 4;

    TC_PRINTD_INFO_INT("cmd ", data[0]);
    // cmd_buf
    switch (data[0])
    {
        case TEST_CMD_ACK:
        if (data[1] == p_tdt->wait_ack_cmd)
        {
            p_tdt->wait_ack_cmd = -1;
        }
        else
        {
            err_id = 3; //err ack packet
        }
        break;

        case TEST_CMD_START:

        tmp32 = sys_local_irq_save();

        p_tdt->test_global_data.test_type = data[1];
        p_tdt->test_global_data.test_config = data[2];
        p_tdt->test_global_data.fixed_data = data[3];
        p_tdt->test_global_data.all_len = get_uint32(data + 4);
        p_tdt->test_global_data.lose_len = p_tdt->test_global_data.all_len;
        p_tdt->test_global_data.err_len = 0;
        p_tdt->data_packet_send_sn = 0xffff;
        p_tdt->wait_ack_cmd = -1;
        p_tdt->check_data_sn = 0;

        if (p_tdt->test_global_data.test_type == TEST_TYPE_UPLOAD)
        {
            //if((p_tdt->test_global_data.test_config & TEST_CFG_CHECK_SHIFT) != 0)
            {
                if ((p_tdt->test_global_data.test_config & TEST_CFG_CHECKTYPE_SHIFT) != 0)
                {
                    uint8 tmp_sn = 0;
                    for (tmp = 0; tmp < TEST_UPLOAD_DATA_LEN; tmp++)
                    {
                        p_tdt->send_upload_buffer[TEST_PACKET_DATA_HEAD_LEN + tmp] = tmp_sn;
                        tmp_sn++;
                    }
                }
                else
                {
                    libc_memset(&p_tdt->send_upload_buffer[TEST_PACKET_DATA_HEAD_LEN], (uint8) p_tdt->test_global_data.fixed_data,
                            TEST_UPLOAD_DATA_LEN);
                }
            }
        }
        else if (p_tdt->test_global_data.test_type == TEST_TYPE_LOOP)
        {
            p_tdt->data_trans_mode = 1;
            p_tdt->data_trans_block = 128;
        }

        cmd_send_packet(ltype, TEST_CMD_ACK, data[0]);

        sys_local_irq_restore(tmp32);
        TC_PRINTD_INFO_INT("test_len:",p_tdt->test_global_data.all_len);
        TC_PRINTD_INFO_INT("test_type:",p_tdt->test_global_data.test_type);

        break;

        case TEST_CMD_STOP:
        p_tdt->test_global_data.test_type = 0xff;
        p_tdt->data_trans_mode = 0;
        cmd_send_packet(ltype, TEST_CMD_ACK, data[0]);
        break;

        case TEST_CMD_GET_RESULT:
        cmd_send_result_packet(ltype);
        break;

        default:
        err_id = 0xff;
        break;
    }

    cmd_end:

    if (err_id != 0)
    {
        TC_PRINTD_INFO("test_cmd err!");
        TC_PRINTD_INFO_INT("err_id:", err_id);
    }
}

const uint8 stop_cmd_list[] = {0x01,0xfe,0x00,0x08,0x02,0x00,0x09,0x01};

//累加校验计算
uint16 add_sum_do(uint8 *data, uint32 len)
{
    uint16 add_sum = 0;

    while (len > 0)
    {
        add_sum += *data;
        data++;
        len--;
    }
    return add_sum;
}

//命令包发送打包
void cmd_send_packet(bt_passth_link_type ltype, uint8 cmd, uint8 context)
{
    uint8 send_buffer[32];
    uint16 add_sum;

    send_buffer[0] = TEST_PACKET_HEAD%256;
    send_buffer[1] = TEST_PACKET_HEAD/256;
    send_buffer[2] = TEST_PACKET_TYPE_CMD;
    send_buffer[3] = 2 + TEST_PACKET_CMD_HEAD_LEN + TEST_PACKET_SUMCRC_LEN;
    send_buffer[4] = cmd;
    send_buffer[5] = context;
    add_sum = add_sum_do(send_buffer, 2 + TEST_PACKET_CMD_HEAD_LEN);
    libc_memcpy(&send_buffer[2 + TEST_PACKET_CMD_HEAD_LEN], &add_sum, 2);
    if (cmd == TEST_CMD_ACK)
    {
        p_tdt->wait_ack_cmd = -1;
    }
    else
    {
        p_tdt->wait_ack_cmd = cmd;
    }

    libc_printf("send one package:\n");
    TC_PRINTD_DATA(send_buffer, (send_buffer[3] > 32) ? 32 : send_buffer[3]);

    bt_manager_blespp_data_trans_send_inner(ltype, send_buffer, send_buffer[3]);
}

//上报测试结果
void cmd_send_result_packet(bt_passth_link_type ltype)
{
    uint8 send_buffer[32];
    uint16 add_sum;

    send_buffer[0] = TEST_PACKET_HEAD%256;
    send_buffer[1] = TEST_PACKET_HEAD/256;
    send_buffer[2] = TEST_PACKET_TYPE_CMD;
    send_buffer[3] = 10 + TEST_PACKET_CMD_HEAD_LEN + TEST_PACKET_SUMCRC_LEN;
    send_buffer[4] = TEST_CMD_SEND_RESULT;
    send_buffer[5] = 0x00;
    libc_memcpy(&send_buffer[6], &p_tdt->test_global_data.lose_len, 4);
    libc_memcpy(&send_buffer[10], &p_tdt->test_global_data.err_len, 4);
    add_sum = add_sum_do(send_buffer, 10 + TEST_PACKET_CMD_HEAD_LEN);
    libc_memcpy(&send_buffer[10 + TEST_PACKET_CMD_HEAD_LEN], &add_sum, 2);
    p_tdt->wait_ack_cmd = TEST_CMD_SEND_RESULT;

    libc_printf("send one package:\n");
    TC_PRINTD_DATA(send_buffer, (send_buffer[3] > 32) ? 32 : send_buffer[3]);

    bt_manager_blespp_data_trans_send_inner(ltype, send_buffer, send_buffer[3]);
}

// 检测是否需要校验
void check_data_crc_do(uint8 *data, uint32 len)
{
    if ((p_tdt->test_global_data.test_config & TEST_CFG_CHECK_SHIFT) == 0)
    {
        return;
    }

    if (0 == (p_tdt->test_global_data.test_config & TEST_CFG_CHECKTYPE_SHIFT))
    {
        //fix value
        while (len > 0)
        {
            if (*data != p_tdt->test_global_data.fixed_data)
            {
                p_tdt->test_global_data.err_len++;
            }
            len--;
            data++;
        }
    }
    else
    {
        //serial numb
        while (len > 0)
        {
            if (*data != p_tdt->check_data_sn)
            {
                p_tdt->test_global_data.err_len++;
                p_tdt->check_data_sn = *data;//更新序号
            }
            p_tdt->check_data_sn++;
            len--;
            data++;
        }
    }
}

//迂回数据处理
void send_loopback_data(bt_passth_link_type ltype, uint8 *data, uint32 len)
{
    if ((len == sizeof(stop_cmd_list)) && (0 == libc_memcmp(data,stop_cmd_list,sizeof(stop_cmd_list))))
    {
        cmd_send_packet(ltype, TEST_CMD_STOP, 0);
        TC_PRINTD_INFO("loop test stop!");
        p_tdt->test_global_data.test_type = 0xff;
        p_tdt->data_trans_mode = 0;
        return;
    }

    if (len > SEND_UPLOAD_BUFFER_SIZE)
    {
        TC_PRINTD_INFO_INT("len is over:", len);
        len = SEND_UPLOAD_BUFFER_SIZE;
    }

    bt_manager_blespp_data_trans_send_inner(ltype, data, len);
}

//数据分发函数
void recieve_blespp_testdata(bt_passth_link_type ltype, buffer_rw_t* buf)
{
    deal_loop:

    if (p_tdt->data_trans_mode != 0)
    {
        if (buf->length < p_tdt->data_trans_block)
        {
            return;
        }
        else
        {
            if (buffer_rw_read(buf, p_tdt->temp_package_buffer, p_tdt->data_trans_block) == FALSE)
            {
                libc_printf("buffer_rw_read fail!\n");
                return;
            }
            p_tdt->test_pkg_len = p_tdt->data_trans_block;
            goto load_package_ok;
        }
    }

    if (buf->length < p_tdt->test_pkg_need_len)
    {
        return;
    }

    if (buffer_rw_read(buf, p_tdt->temp_package_buffer + p_tdt->test_pkg_index, p_tdt->test_pkg_need_len) == FALSE)
    {
        libc_printf("buffer_rw_read fail!\n");
        return;
    }

    switch (p_tdt->test_pkg_deal_sta)
    {
    case PKG_DEAL_HEAD:
        if (p_tdt->temp_package_buffer[p_tdt->test_pkg_index] != TEST_PACKET_HEAD%256)
        {
            libc_printf("pkg head err!\n");
            p_tdt->test_pkg_deal_sta = PKG_DEAL_HEAD;
            p_tdt->test_pkg_need_len = 1;
            p_tdt->test_pkg_index = 0;
            goto deal_loop;
        }
        p_tdt->test_pkg_deal_sta = PKG_DEAL_HEAD2;
        p_tdt->test_pkg_need_len = 1;
        p_tdt->test_pkg_index = 1;
        goto deal_loop;

    case PKG_DEAL_HEAD2:
        if (p_tdt->temp_package_buffer[p_tdt->test_pkg_index] != TEST_PACKET_HEAD/256)
        {
            libc_printf("pkg head2 err!\n");
            p_tdt->test_pkg_deal_sta = PKG_DEAL_HEAD;
            p_tdt->test_pkg_need_len = 1;
            p_tdt->test_pkg_index = 0;
            goto deal_loop;
        }
        p_tdt->test_pkg_deal_sta = PKG_DEAL_TYPE;
        p_tdt->test_pkg_need_len = 1;
        p_tdt->test_pkg_index = 2;
        goto deal_loop;

    case PKG_DEAL_TYPE:
        if ((p_tdt->temp_package_buffer[p_tdt->test_pkg_index] != TEST_PACKET_TYPE_CMD)
            && (p_tdt->temp_package_buffer[p_tdt->test_pkg_index] != TEST_PACKET_TYPE_DATA))
        {
            libc_printf("pkg type err!\n");
            p_tdt->test_pkg_deal_sta = PKG_DEAL_HEAD;
            p_tdt->test_pkg_need_len = 1;
            p_tdt->test_pkg_index = 0;
            goto deal_loop;
        }
        p_tdt->test_pkg_type = p_tdt->temp_package_buffer[p_tdt->test_pkg_index];
        p_tdt->test_pkg_deal_sta = PKG_DEAL_LEN;
        p_tdt->test_pkg_need_len = 1;
        p_tdt->test_pkg_index = 3;
        goto deal_loop;

    case PKG_DEAL_LEN:
        p_tdt->test_pkg_len = p_tdt->temp_package_buffer[p_tdt->test_pkg_index];
        p_tdt->test_pkg_deal_sta = PKG_DEAL_CMD;
        p_tdt->test_pkg_need_len = 1;
        p_tdt->test_pkg_index = 4;
        goto deal_loop;

    case PKG_DEAL_CMD:
        p_tdt->test_pkg_cmd = p_tdt->temp_package_buffer[p_tdt->test_pkg_index];
        p_tdt->test_pkg_deal_sta = PKG_DEAL_PARA;
        p_tdt->test_pkg_need_len = p_tdt->test_pkg_len - 7; //4字节头，2字节校验，1字节命令号
        p_tdt->test_pkg_need_len += 2; //check sum
        p_tdt->test_pkg_index = 5;
        goto deal_loop;

    case PKG_DEAL_PARA:
    default:
        p_tdt->test_pkg_deal_sta = PKG_DEAL_HEAD;
        p_tdt->test_pkg_need_len = 1;
        p_tdt->test_pkg_index = 0;
        break;
    }

    load_package_ok:

    libc_printf("load one package:\n");
    libc_printhex(p_tdt->temp_package_buffer, (p_tdt->test_pkg_len > 32) ? 32 : p_tdt->test_pkg_len, 0);

    if (p_tdt->test_global_data.test_type == TEST_TYPE_LOOP)
    {
        send_loopback_data(ltype, p_tdt->temp_package_buffer, p_tdt->test_pkg_len);
        return;
    }

    if (p_tdt->test_pkg_type == TEST_PACKET_TYPE_CMD)
    {
        cmd_packet_pro(ltype, p_tdt->temp_package_buffer, p_tdt->test_pkg_len);
        return;
    }

    if(p_tdt->test_global_data.test_type == 0xff)
    {
        //未启动时的非法数据，丢弃
        return;
    }

    check_data_crc_do(p_tdt->temp_package_buffer, p_tdt->test_pkg_len);
    if(p_tdt->test_global_data.lose_len >= p_tdt->test_pkg_len)
    {
        p_tdt->test_global_data.lose_len -= p_tdt->test_pkg_len;
    }
    else
    {
        TC_PRINTD_INFO_INT("data_over:",p_tdt->test_pkg_len);
    }
}

bool test_blespp_data_trans_dispatch(bt_passth_link_type ltype, buffer_rw_t* buf)
{
    recieve_blespp_testdata(ltype, buf);

    if ((p_tdt->test_global_data.test_type == TEST_TYPE_UPLOAD) && (p_tdt->wait_ack_cmd == -1))
    {
        uint8 *data_send_buf;
        uint16 data_send_len;

        if (p_tdt->test_global_data.lose_len == 0)
        {
            cmd_send_packet(ltype, TEST_CMD_STOP, 0);
            p_tdt->test_global_data.test_type = 0xff;
        }
        else
        {
            //发数据包
            if (p_tdt->test_global_data.lose_len > (TEST_UPLOAD_DATA_LEN >> 1))
            {
                data_send_len = (TEST_UPLOAD_DATA_LEN >> 1);
            }
            else
            {
                data_send_len = p_tdt->test_global_data.lose_len;
            }
            p_tdt->data_packet_send_sn++;
            if((p_tdt->data_packet_send_sn & 0x01) == 0)
            {
                data_send_buf = &p_tdt->send_upload_buffer[TEST_PACKET_DATA_HEAD_LEN];
            }
            else
            {
                data_send_buf = &p_tdt->send_upload_buffer[TEST_PACKET_DATA_HEAD_LEN + (TEST_UPLOAD_DATA_LEN >> 1)];
            }

            p_tdt->test_global_data.lose_len -= data_send_len;

            libc_printf("upload one package:\n");
            TC_PRINTD_DATA(data_send_buf, (data_send_len > 32) ? 32 : data_send_len);
            bt_manager_blespp_data_trans_send_inner(ltype, data_send_buf, data_send_len);
        }
    }

    return TRUE;
}


#endif
