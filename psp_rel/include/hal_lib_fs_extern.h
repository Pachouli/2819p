#ifndef _HAL_LIB_FS_EXTERN_H
#define _HAL_LIB_FS_EXTERN_H
#include <attr.h>
#include <typeext.h>
#include <hal_spi1_extern.h>

#define SEEK_SET    0   // ���ļ������ļ�β��λ��offsetΪ����
#define SEEK_CUR    1   //�ӵ�ǰλ�����ļ�ͷ��β��λ��������ʾ���ļ�βseek��������ʾ���ļ�ͷseek
#define SEEK_END    2   // ���ļ�β���ļ��׶�λ��offsetΪ����

typedef struct
{
    unsigned char isvalid;
    unsigned char reserve[3];
    unsigned int startaddr;
    unsigned int endaddr;
    unsigned int rwpointer;
} lib_file_t;

typedef lib_file_t* lib_handle;

#endif
