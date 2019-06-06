#include <stdio.h>

//TBL必须是经过裁剪后的

const char MBtoUnicTabName[][13]=
    {"V936GBK.TBL",  /*简体中文codepage table*/
     "V950BIG.TBL",  /*繁体中文codepage table*/
     "V1252.TBL",      /*英文,西欧语系 codepage table*/
     "V932JIS.TBL",   /*日文 codepage table*/
     "V949KOR.TBL", /*韩文 codepage table*/
     "V1257.TBL",      /*波罗地海语系*/
     "V1251.TBL",      /*俄语语系*/
     "V1254.TBL",      /*土耳其语系*/
     "V1255.TBL",      /*希伯莱语*/
     "V874.TBL" ,       /*泰语*/
     "V1250.TBL",       /*北欧，东欧语系*/
     "V1256.TBL",      /*阿拉伯语系*/
     "V1253.TBL"      /*希腊文 codepage table*/
    };

unsigned short gbk_to_unicode(int gbk_code, FILE *in)
{
    unsigned short unicode_val;
    int diff;
    int index;

    if ((gbk_code&0xff) <= 0x7f)
    {
        unicode_val = gbk_code&0xff;
    }
    else
    {
        if ((gbk_code&0xff) == 0x80)
        {
            unicode_val = 0x20ac;
        }
        else if ((gbk_code&0xff) == 0xff)
        {
            unicode_val = 0xf8f5;
        }
        else
        {
            if (gbk_code < 0x100)
            {
                unicode_val = 0;
            }
            else if (gbk_code < 0x4080)
            {
                unicode_val = 0x3f;
            }
            else
            {
                diff = gbk_code - 0x4080;
                index = diff/0x100*0x80;
                index += (diff%0x80);
                fseek(in, index*2, 0);
                fread(&unicode_val, 2, 1, in);
            }
        }
    }

    return unicode_val;
}

unsigned short big5_to_unicode(int big5_code, FILE *in)
{
    unsigned short unicode_val;
    int diff;
    int index;

    if ((big5_code&0xff) <= 0x7f)
    {
        unicode_val = big5_code&0xff;
    }
    else
    {
        if ((big5_code&0xff) == 0x80)
        {
            unicode_val = 0x80;
        }
        else if ((big5_code&0xff) == 0xff)
        {
            unicode_val = 0xf8f8;
        }
        else
        {
            if (big5_code < 0x100)
            {
                unicode_val = 0;
            }
            else if ((big5_code < 0x4080) || ((big5_code >= 0x7f00)&&(big5_code < 0xa180)))
            {
                unicode_val = 0x3f;
            }
            else if (big5_code < 0x7f00)
            {
                diff = big5_code - 0x4080;
                index = diff/0x100*0x80;
                index += (diff%0x80);
                fseek(in, index*2, 0);
                fread(&unicode_val, 2, 1, in);
            }
            else
            {
                diff = big5_code - 0x6280;
                index = diff/0x100*0x80;
                index += (diff%0x80);
                fseek(in, index*2, 0);
                fread(&unicode_val, 2, 1, in);
            }
        }
    }

    return unicode_val;
}

unsigned short jis_to_unicode(int jis_code, FILE *in)
{
    unsigned short unicode_val;
    int diff;
    int index;

    if ((jis_code&0xff) <= 0x7f)
    {
        unicode_val = jis_code&0xff;
    }
    else
    {
        if ((jis_code&0xff) == 0x80)
        {
            unicode_val = 0x80;
        }
        else if ((jis_code&0xff) == 0xa0)
        {
            unicode_val = 0xf8f0;
        }
        else if (((jis_code&0xff) >= 0xa1) && ((jis_code&0xff) < 0xe0))
        {
            unicode_val = 0xff61 + ((jis_code&0xff)-0xa1);
        }
        else if ((jis_code&0xff) == 0xfd)
        {
            unicode_val = 0xf8f1;
        }
        else if ((jis_code&0xff) == 0xfe)
        {
            unicode_val = 0xf8f2;
        }
        else if ((jis_code&0xff) == 0xff)
        {
            unicode_val = 0xf8f3;
        }
        else if (jis_code < 0x100)
        {
            unicode_val = 0;
        }
        else if (jis_code < 0x4080)
        {
            unicode_val = 0x30fb;
        }
        else
        {
            diff = jis_code - 0x4080;
            index = diff/0x100*0x40;
            if ((diff%0x80) < 0x20)
            {
                index += (diff%0x80);
            }
            else
            {
                index += (diff%0x80-0x40);
            }
            fseek(in, index*2, 0);
            fread(&unicode_val, 2, 1, in);
        }
    }

    return unicode_val;
}

unsigned short kor_to_unicode(int kor_code, FILE *in)
{
    unsigned short unicode_val;
    int diff;
    int index;

    if ((kor_code&0xff) <= 0x7f)
    {
        unicode_val = kor_code&0xff;
    }
    else
    {
        if ((kor_code&0xff) == 0x80)
        {
            unicode_val = 0x80;
        }
        else if ((kor_code&0xff) == 0xff)
        {
            unicode_val = 0xf8f7;
        }
        else
        {
            if (kor_code < 0x100)
            {
                unicode_val = 0;
            }
            else if (kor_code < 0x4180)
            {
                unicode_val = 0x3f;
            }
            else
            {
                diff = kor_code - 0x4180;
                index = diff/0x100*0x80;
                index += (diff%0x80);
                fseek(in, index*2, 0);
                fread(&unicode_val, 2, 1, in);
            }
        }
    }

    return unicode_val;
}

unsigned short thai_to_unicode(int thai, FILE *in)
{
    unsigned short unicode_val;
    int diff;
    int index;

    if ((thai&0xff) <= 0x7f)
    {
        unicode_val = thai&0xff;
    }
    else
    {
        if ((thai&0xff) == 0xff)
        {
            unicode_val = 0xf8c8;
        }
        else if ((thai&0xff) == 0xfe)
        {
            unicode_val = 0xf8c7;
        }
        else if ((thai&0xff) == 0xfd)
        {
            unicode_val = 0xf8c6;
        }
        else if ((thai&0xff) == 0xfc)
        {
            unicode_val = 0xf8c5;
        }
        else if ((thai&0xff) == 0xde)
        {
            unicode_val = 0xf8c4;
        }
        else if ((thai&0xff) == 0xdd)
        {
            unicode_val = 0xf8c3;
        }
        else if ((thai&0xff) == 0xdc)
        {
            unicode_val = 0xf8c2;
        }
        else if ((thai&0xff) == 0xdb)
        {
            unicode_val = 0xf8c1;
        }
        else if ((thai&0xff) == 0xa0)
        {
            unicode_val = 0xa0;
        }
        else if (((thai&0xff) >= 0xa1) && ((thai&0xff) <= 0xda))
        {
            unicode_val = 0xe01 + ((thai&0xff)-0xa1);
        }
        else if ((thai&0xff) >= 0xdf)
        {
            unicode_val = 0xe3f + ((thai&0xff)-0xdf);
        }
        else
        {
            {
                diff = thai - 0x80;
                index = diff/0x100*0x20;
                index += (diff%0x80);
                fseek(in, index*2, 0);
                fread(&unicode_val, 2, 1, in);
            }
        }
    }

    return unicode_val;
}

char unicode_map[0x10000];

typedef struct
{
    unsigned short free_start;
    unsigned short free_count;
} unicode_free_t;

const unicode_free_t s_unicode_free_blocks[] =
{
    {0x0500, 0x90},
    {0x0700, 0x700},
    {0x0e80, 0x1180},
    {0x2700, 0x900},
    {0x3400, 0x19c0},
    {0xa000, 0xc00},
    {0xd800, 0x800},
    {0xfb00, 0x330}
};

int main(void)
{
    FILE *in, *out;
    int file_len;
    unsigned short unicode_val;
    int i,j;
    int code_tatal;
    int free_count;
    int free_start;
    int free_end;
    int free_index;
    int free_total;
    unsigned char char_point[33];
    int m,n;
    int is_free;

    for (i = 0;i<13;i++)
    {
        in = fopen(MBtoUnicTabName[i], "rb");

        if (in == NULL)
        {
            printf("Can't Open File %s！\n", MBtoUnicTabName[i]);
            return 0;
        }

        if (i == 0)
        {
            for (j = 0; j < 0x10000; j++)
            {
                unicode_val = gbk_to_unicode(j, in);
                unicode_map[unicode_val] = 1;
            }
        }
        else if (i == 1)
        {
            for (j = 0; j < 0x10000; j++)
            {
                unicode_val = big5_to_unicode(j, in);
                unicode_map[unicode_val] = 1;
            }
        }
        else if (i == 3)
        {
            for (j = 0; j < 0x10000; j++)
            {
                unicode_val = jis_to_unicode(j, in);
                unicode_map[unicode_val] = 1;
            }
        }
        else if (i == 4)
        {
            for (j = 0; j < 0x10000; j++)
            {
                unicode_val = kor_to_unicode(j, in);
                unicode_map[unicode_val] = 1;
            }
        }
        else if (i == 9)
        {
            for (j = 0; j < 0x10000; j++)
            {
                unicode_val = thai_to_unicode(j, in);
                unicode_map[unicode_val] = 1;
            }
        }
        else
        {
            fseek(in, 0, 2);
            file_len = ftell(in);
            fseek(in, 0, 0);
            printf("len:%d\n", file_len);

            for (j = 0; j < file_len/2; j++)
            {
                fread(&unicode_val, 2, 1, in);
                unicode_map[unicode_val] = 1;
            }
        }

        fclose(in);
    }

    code_tatal = 0;
    free_count = 0;
    free_index = 0;
    free_total = 0;
    for (i = 0; i < 0x10000; i++)
    {
        if (unicode_map[i] != 0)
        {
            code_tatal++;
            if (free_count >= 128)
            {
                free_end = i-1;
                printf("[%d]free[0x%x,0x%x=%d]\n", ++free_index, free_start, free_end, free_count);
                free_total += free_count;
            }
            free_count = 0;
        }
        else
        {
            if (free_count == 0)
            {
                free_start = i;
            }
            free_count++;
        }
    }

    if (free_count >= 128)
    {
        free_end = i-1;
        printf("[%d]free[0x%x,0x%x=%d]\n", ++free_index, free_start, free_end, free_count);
        free_total += free_count;
    }

    printf("code_tatal:%d\n", code_tatal);
    printf("free_total:%d\n", free_total);

    in = fopen("UNICODE.BIN", "rb");
    if (in == NULL)
    {
        printf("Can't Open File UNICODE.BIN！\n");
        return 0;
    }
    out = fopen("UNICODE_CUT.BIN", "wb");

    for (unicode_val = 0, n = 0; n < 0x10000; unicode_val++, n++)
    {
        fread(char_point, 33, 1, in);
        is_free = 0;
        for (m = 0; m < sizeof(s_unicode_free_blocks)/sizeof(unicode_free_t); m++)
        {
            if ((unicode_val >= s_unicode_free_blocks[m].free_start)
                && (unicode_val < (s_unicode_free_blocks[m].free_start + s_unicode_free_blocks[m].free_count)))
            {
                is_free = 1;
                break;
            }
        }

        if (is_free == 0)
        {
            fwrite(char_point, 33, 1, out);
        }
    }

    fclose(in);
    fclose(out);

    in = fopen("UNICODE_CUT.BIN", "rb");
    out = fopen("UNICODE_LARGE.BIN", "wb");

    for (unicode_val = 0, n = 0; n < 0x10000; unicode_val++, n++)
    {
        is_free = 0;
        for (m = 0; m < sizeof(s_unicode_free_blocks)/sizeof(unicode_free_t); m++)
        {
            if ((unicode_val >= s_unicode_free_blocks[m].free_start)
                && (unicode_val < (s_unicode_free_blocks[m].free_start + s_unicode_free_blocks[m].free_count)))
            {
                is_free = 1;
                break;
            }
        }

        if (is_free == 0)
        {
            fread(char_point, 33, 1, in);
        }
        else
        {
            memset(char_point, 0x5a, 33);
        }
        fwrite(char_point, 33, 1, out);
    }

    fclose(in);
    fclose(out);

    return 0;
}
