#ifndef _HAL_LIB_FS_H
#define _HAL_LIB_FS_H

#include <attr.h>
#include <typeext.h>
#include "hal_lib_fs_extern.h"

#define MAX_LIB_HEANDLE_NUM                 (6)
#define LIB_MAP_SIZE                        (128)

typedef struct
{
    uint32 offset;
    uint32 length;
    uint8 name[12];
    uint32 reserved[3];
} lib_dir_info_t;

extern int32 hal_lib_read_data_inner(uint32 byte_addr, uint8 *buf, uint32 byte_num);
extern lib_file_t *hal_lib_fopen_inner(const char *filename);
extern void hal_lib_fs_init_inner(void);
extern int hal_lib_fclose_inner(lib_file_t *fp);
extern int hal_lib_fseek_inner(lib_file_t *fp, uint8 whence, int32 offset);
extern uint32 hal_lib_ftell_inner(lib_file_t *fp);
extern int32 hal_lib_fread_inner(lib_file_t *fp, void *buffer, uint32 len);

#endif
