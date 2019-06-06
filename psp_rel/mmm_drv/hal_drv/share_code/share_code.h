/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：共享代码接口函数声明。
 * 作者：cailizhen
 ********************************************************************************/

#ifndef __SHARE_CODE_H__
#define __SHARE_CODE_H__

#include <psp_includes.h>
#include "post_process.h"

extern void update_PP_para_info_inner(post_process_detail_t *p_pp_detail);
extern bool buffer_rw_write_inner(buffer_rw_t *p_buf_rw, uint8 *w_buf, uint16 w_len);
extern bool buffer_rw_read_inner(buffer_rw_t *p_buf_rw, uint8 *r_buf, uint16 r_len);
extern bool buffer_rw_update_inner(buffer_rw_t *p_buf_rw, uint16 r_len);
extern bool buffer_rw_flush_inner(buffer_rw_t *p_buf_rw);
extern void energy_calc_inner(short *frame_buf, uint16 sample_cnt, bool is_stereo, short *p_cur_power_max, short *p_cur_power_rms);

#endif
