/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：自动化测试：插卡自动化测试记录文件处理接口，记录文件保存当前蓝牙地址和当前测试数量
 *                  每完成一次测试，测试数量值会累加，蓝牙地址在"累加模式"也会累加
 * 作者：zhouxl
 ********************************************************************************/

#include "ap_test_head.h"
#include "ap_autotest_main_entry.h"

//#define BT_ADDR_RECORD_FILE_LEN   (512)
#define BT_ADDR_RECORD_MAGIC_L    (0x44434241)
#define BT_ADDR_RECORD_MAGIC_H    (0x48474645)


//att_reconrd.bin
const uint8 ATT_RECORD_FILENAME[] =
{
    0xff, 0xfe,
    'a',  0x00,
    't',  0x00,
    't',  0x00,
    '_',  0x00,
    'r',  0x00,
    'e',  0x00,
    'c',  0x00,
    'o',  0x00,
    'r',  0x00,
    'd',  0x00,
    '.',  0x00,
    'b',  0x00,
    'i',  0x00,
    'n',  0x00,
    0x00, 0x00
};


uint32 cal_att_file_checksum(btaddr_log_file_t *btaddr_log_file)
{
    uint32 checksum = 0;
    uint32 i;
    uint32 *pdata = (uint32 *)btaddr_log_file;

    for(i = 0; i < ((sizeof(btaddr_log_file_t) - 4) / 4); i++)
    {
        checksum += pdata[i];
    }

    return checksum;
}




int32 act_open_att_record_file(btaddr_log_file_t *btaddr_log)
{
    uint32 file_handle = 0;
    int32 file_valid;
    uint32 check_sum;
    
    //加载文件系统,打开磁盘
    uint32 file_sys_id = com_file_open(DISK_H, NULL, 0);
    //定位到根目录
    vfs_cd(file_sys_id, CD_ROOT, 0);

    file_handle = vfs_file_open(file_sys_id, (uint8*)ATT_RECORD_FILENAME, R_NORMAL_SEEK);
    //libc_memset(file_buffer, 0, BT_ADDR_RECORD_FILE_LEN);
    libc_memset(btaddr_log, 0, sizeof(btaddr_log_file_t));

    if(0 == file_handle)
    {
        file_valid = FALSE;
    }
    else
    {   
        //如果存在文件且检验合法，则读取record内容
        file_valid = TRUE;
        PRINTF("");

        if ( vfs_file_read(file_sys_id, (uint8*)btaddr_log, sizeof(btaddr_log_file_t), file_handle) != sizeof(btaddr_log_file_t))
        {
            file_valid = FALSE;
        }

        if((btaddr_log->magicl != BT_ADDR_RECORD_MAGIC_L) || (btaddr_log->magich != BT_ADDR_RECORD_MAGIC_H))
        {   
            PRINTF("");
            libc_printf("MAGIC ERR:%x", btaddr_log->magicl);
            file_valid = FALSE;
        }

        check_sum = cal_att_file_checksum(btaddr_log);
        if(check_sum != btaddr_log->checksum)
        {
            libc_printf("checksum err:%x", check_sum);
            file_valid = FALSE;
        }
    }


    if(file_valid == FALSE)
    {   
        //如果文件不存在或不合法，则重新创建该文件
        if( 0 == file_handle ){
            file_handle = vfs_file_create(file_sys_id, (uint8*)ATT_RECORD_FILENAME, 0);
            if(file_handle == 0)
            {
                libc_printf("create bt addr log file error!", 0, 0);
                while(1)
                {
                    //loop
                }
            }
        }
        
        vfs_file_seek(file_sys_id, 0, SEEK_SET, file_handle);

        btaddr_log->magicl = BT_ADDR_RECORD_MAGIC_L;
        btaddr_log->magich = BT_ADDR_RECORD_MAGIC_H;
        btaddr_log->record_cnt = 0;
        btaddr_log->checksum = cal_att_file_checksum(btaddr_log);

        vfs_file_write(file_sys_id, (uint8*)btaddr_log, sizeof(btaddr_log_file_t), file_handle);

        //vfs_file_close(file_sys_id, file_handle);
        com_file_close(file_handle);

        return FALSE;
    }

    //vfs_file_close(file_sys_id, file_handle);
    com_file_close(file_handle);
    return TRUE;
}



uint32 att_test_read_count_from_record_file(void)
{
    btaddr_log_file_t btaddr_log;

    act_open_att_record_file(&btaddr_log);

    return btaddr_log.record_cnt;
}


int32 act_test_read_bt_addr_from_record_file(uint8 *byte_buffer)
{
    btaddr_log_file_t btaddr_log;

    if(act_open_att_record_file(&btaddr_log) == TRUE)
    {
        //按照高位在前的顺序依次读入
        byte_buffer[2] = btaddr_log.addr[0];
        byte_buffer[1] = btaddr_log.addr[1];
        byte_buffer[0] = btaddr_log.addr[2];

        return TRUE;
    }

    return FALSE;
}



 int32 act_test_write_att_record_file(uint8 *byte_buffer, uint32 record_cnt, uint32 mode)
{
    uint32 i, ret = 0;
    int32 file_handle;
    
    btaddr_log_file_t btaddr_log;

    //加载文件系统,打开磁盘
    uint32 file_sys_id = com_file_open(DISK_H, NULL, 0);

    //定位到根目录
    vfs_cd(file_sys_id, CD_ROOT, 0);

    file_handle = vfs_file_open(file_sys_id, (uint8*)ATT_RECORD_FILENAME, R_NORMAL_SEEK);

    if(file_handle == 0)
    {
        libc_printf("open file error!\n", 0, 0);
        while(1)
        {
            //loop
        }
    }

    ret = vfs_file_read(file_sys_id, (uint8*)&btaddr_log, sizeof(btaddr_log_file_t), file_handle);
    if ( ret < sizeof(btaddr_log_file_t) )
    {
        libc_printf("read file error! read:%d bytes, expected:%d bytes\n", ret, sizeof(btaddr_log_file_t));
        while(1)
        {
            //loop
        }
    }

    if((btaddr_log.magicl != BT_ADDR_RECORD_MAGIC_L) || (btaddr_log.magich != BT_ADDR_RECORD_MAGIC_H))
    {
        libc_printf("file unvalid!\n", 0, 0);

        while(1);
    }

    if(TEST_RECORD_TESTCOUNT == mode)
    {
        btaddr_log.record_cnt = record_cnt;
    }
    else
    {
        if (TEST_RECORD_BTADDR_INCREASE == mode)
        {
            for(i = 0; i < 3; i++)
            {
                byte_buffer[i]++;

                if(byte_buffer[i] != 0)
                {
                    break;
                }
            }
        }

        //按照高位在前的顺序依次写入
        btaddr_log.addr[0] = byte_buffer[2];
        btaddr_log.addr[1] = byte_buffer[1];
        btaddr_log.addr[2] = byte_buffer[0];
    }

    btaddr_log.checksum = cal_att_file_checksum(&btaddr_log);

    vfs_file_seek(file_sys_id, 0, SEEK_SET, file_handle);

    vfs_file_write(file_sys_id, (uint8*)&btaddr_log, sizeof(btaddr_log_file_t), file_handle);

    //vfs_file_close(file_sys_id, file_handle);
    com_file_close(file_handle);

    return TRUE;
}


