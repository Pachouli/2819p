/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：录音模块头文件。
 * 作者：cailizhen
 ********************************************************************************/

#include "audio_encoder_lib_dev.h"
#include "record_module_extern.h"

#define WRITE_SIZE_PER_TIME         (2048)     //一次性写入文件的大小

typedef struct
{
    record_module_status_t record_module_status;
    music_encode_info_t music_encode_info; /* 当前帧编码状态 */
    music_info_t music_info;
    record_module_fs_para_t     record_fs;
    record_module_encode_para_t encode_para;
    record_module_buffer_set_t  buffer_set;

    void *audioenc_handle; /*DAE AL句柄*/
    buffer_rw_t encode_output_buf_w;
    uint16 encode_frame_len;
    uint8 write_thread_terminate_flag;
    bool print_bytes_used;
    bool stop_encode_flag; /* 结束录音过程中，不再编码新数据 */
} record_module_t;

extern record_module_t g_record_module_global;

extern void *record_module_open(void *param) __FAR__;
extern int32 record_module_close(record_module_t *p_record_module, void *param) __FAR__;

extern int32 audio_record_module_cmd(void *handel, record_module_cmd_e cmd, uint32 args);

extern int32 start_record_module(record_module_t *p_record_module) __FAR__;
extern int32 stop_record_module(record_module_t *p_record_module) __FAR__;

extern void *write_loop(void *param);
extern int32 record_module_encode_deal(record_module_t *p_record_module, uint8 *encode_in_addr);

