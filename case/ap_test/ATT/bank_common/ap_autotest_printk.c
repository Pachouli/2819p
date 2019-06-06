/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：自动化测试：stub打印提示接口实现。
 * 作者：zhouxl
 ********************************************************************************/

#include "ap_test_head.h"
#include "ap_autotest_main_entry.h"

const char digits[16] = "0123456789abcdef";

/*!
 * \brief 格式化输出到字符串 (指定大小和参数列表).
 */
int vsnprintf(char* buf, size_t size, const char* fmt, va_list args)
{
    char* str = buf;
    char* end = buf + size - 1;

    for (; *fmt != '\0'; fmt++)
    {
        uint32 flags;
        int width;

        uint32 number;
        uint32 base;

        char num_str[16];
        int num_len;
        int sign;

        if (*fmt != '%')
        {
            _putc(str, end, *fmt);
            continue;
        }

        fmt++;

        flags = 0, width = 0, base = 10;

        if (*fmt == '0')
        {
            flags |= _ZEROPAD;
            fmt++;
        }

        while (isdigit(*fmt))
        {
            width = (width * 10) + (*fmt - '0');
            fmt++;
        }

        switch (*fmt)
        {
            case 'c':
            {
                uint8 ch = (uint8)va_arg(args, int);

                _putc(str, end, ch);
                continue;
            }

            case 's':
            {
                char* s = va_arg(args, char*);

                while (*s != '\0')
                    _putc(str, end, *s++);

                continue;
            }

            //case 'X':
            //    flags |= _LARGE;

            case 'x':
        //  case 'p':
            base = 16;
            break;

            case 'd':
        //  case 'i':
            flags |= _SIGN;

        //  case 'u':
            break;

            default:
            continue;
        }

        number = va_arg(args, uint32);

        sign = 0, num_len = 0;

        if (flags & _SIGN)
        {
            if ((int) number < 0)
            {
                number = -(int) number;

                sign = '-';
                width -= 1;
            }
        }

        if (number == 0)
        {
            num_str[num_len++] = '0';
        }
        else
        {


            while (number != 0)
            {
                char ch = digits[number % base];

                num_str[num_len++] = ch;
                number /= base;
            }
        }

        width -= num_len;

        if (sign != 0)
            _putc(str, end, sign);

        if (flags & _ZEROPAD)
        {
            while (width-- > 0)
                _putc(str, end, '0');
        }

        while (num_len-- > 0)
            _putc(str, end, num_str[num_len]);
    }

    *str = '\0';

    return (str - buf);
}


int stub_cmd_print_log(const char* format, ...)
{
    uint8 * stub_rw_buffer = sys_malloc(STUB_ATT_RW_TEMP_BUFFER_LEN);
    if (NULL == stub_rw_buffer){
        libc_printf("stub_cmd_print_log: stub_rw_buffer null\n");
        while(1){}
    }

    int trans_bytes;

    print_log_t *print_log;

    va_list  args;

    va_start(args, format);

    print_log = (print_log_t *)stub_rw_buffer;

    trans_bytes = vsnprintf((uint8 *)&(print_log->log_data), STUB_ATT_RW_TEMP_BUFFER_LEN, format, args);

    //添加结束字符
    libc_memset(&print_log->log_data[trans_bytes], 0, 4);
    trans_bytes += 1;

    //保证四字节对齐
    trans_bytes = (((trans_bytes + 3) >> 2) << 2);

    ATT_WRITE_DATA(STUB_CMD_ATT_PRINT_LOG, trans_bytes, stub_rw_buffer);
    ATT_READ_DATA(stub_rw_buffer, 0);

    libc_printf((uint8 *)&(print_log->log_data), 0, 0);

    sys_free(stub_rw_buffer);
    
    return 0;
}



