#ifndef _HAL_LIB_FS_EXTERN_H
#define _HAL_LIB_FS_EXTERN_H
#include <attr.h>
#include <typeext.h>
#include <hal_spi1_extern.h>

#define SEEK_SET    0   // 从文件首往文件尾定位，offset为正数
#define SEEK_CUR    1   //从当前位置往文件头或尾定位；正数表示向文件尾seek，负数表示向文件头seek
#define SEEK_END    2   // 从文件尾往文件首定位，offset为负数

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
