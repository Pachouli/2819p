/********************************************************************************
 *        Copyright(c) 2014-2016 Actions (Zhuhai) Technology Co., Limited,
 *                            All Rights Reserved.
 *
 * 描述：AUX/FM/USB播歌中间件录音接口。
 * 作者：cailizhen
 ********************************************************************************/

#include "music_pcm_player_global.h"
#include "sysdef.h"

#if ((SUPPORT_RECORD_FUNC == 1) && (SUPPORT_MMM_PCM_RECORD_FUNC == 1))

#define DRV_NAME_SIZE 12
static const char rec_mod_name[DRV_NAME_SIZE] = "rec_mod.al";

uint8 __section__(".bss.encode_aout_buffer") encode_aout_buffer[ENCODE_OUT_BUFFER_SIZE];
uint8 __section__(".bss.encode_in_buffer") RECORD_FILE_ENCODE_IN_BUF_ADDR[ENCODE_IN_FRAME_SIZE*2];//可支持立体声
uint8 __section__(".bss.encode_out_buffer") RECORD_FILE_ENCODE_OUT_BUF_ADDR[RECORD_ENCODE_OUTPUT_CACHE_LEN];
uint8 __section__(".bss.encode_source_buffer") RECORD_FILE_TEMP_BUF_ADDR[RECORD_FILE_TEMP_BUF_LEN];

uint8 __section__(".stack.ap_rec_write_file_stack_ptos")rec_stk_ptos[1]; //动态数组
extern uint16 _REC_STK_SIZE;

int32 start_music_record(music_pcm_player_t *music_player)
{
    int32 ret = 0;
    record_module_buffer_set_t buffer_set;
    record_module_encode_detail_t encode_detail;

    //加载录音模块
    ret = (int32) sys_drv_install(REC_MOD_GROUP, 0, (void *) rec_mod_name);
    if (ret == -1)
    {
        while(1);
    }

    //初始化Encode Input Buffer管理器
    music_player->encode_input_buf_w.read_ptr = 0;
    music_player->encode_input_buf_w.write_ptr = 0;
    music_player->encode_input_buf_w.length = 0;
    music_player->encode_input_buf_w.capacity = RECORD_FILE_TEMP_BUF_LEN;
    music_player->encode_input_buf_w.raw_data = (uint8 *) (RECORD_FILE_TEMP_BUF_ADDR);
    //MR_DEBUG("# encode_input_buf_w addr : %x\n", music_player->encode_input_buf_w.raw_data);

    record_module_cmd(&(music_player->rec_mod_handle), RECORD_MODULE_OPEN, 0);
    record_module_cmd(music_player->rec_mod_handle, RECORD_MODULE_SET_FS, &(music_player->record_fs));
    record_module_cmd(music_player->rec_mod_handle, RECORD_MODULE_SET_ENCODE, &(music_player->encode_para));
    buffer_set.ptos = (rec_stk_ptos + (uint32)(&_REC_STK_SIZE));
    buffer_set.stk_size = (uint32)(&_REC_STK_SIZE);
    buffer_set.output_cache = RECORD_FILE_ENCODE_OUT_BUF_ADDR;
    buffer_set.output_cache_len = sizeof(RECORD_FILE_ENCODE_OUT_BUF_ADDR);
    buffer_set.output_buffer = encode_aout_buffer;

    record_module_cmd(music_player->rec_mod_handle, RECORD_MODULE_SET_BUFFER, &buffer_set);

    if (record_module_cmd(music_player->rec_mod_handle, RECORD_MODULE_START, 0) < 0)
    {
        music_player->music_status.status = MMM_PP_ENGINE_ERROR;
        libc_printf("creat_write_file_thread fail!!\n");
        ret = -1;
        while(1);
    }
    record_module_cmd(music_player->rec_mod_handle, RECORD_MODULE_GET_ENCODE_DETAIL, &encode_detail);
    music_player->encode_frame_len = encode_detail.encode_frame_len;

    libc_printf_info("encode_frame_len = %d\n", music_player->encode_frame_len);

    if (music_player->encode_frame_len > sizeof(RECORD_FILE_ENCODE_IN_BUF_ADDR))
    {
        libc_printf_error("encode frame in buffer not enough!\n");
        while (1);
    }

    music_player->mmm_record_sta = MMM_RECORD_RECORDING;

    return ret;
}


int32 stop_music_record(music_pcm_player_t *music_player)
{
    int32 ret = 0;

    music_player->mmm_record_sta = MMM_RECORD_STOP;

    //关闭录音模块
    sys_os_sched_unlock();
    record_module_cmd(music_player->rec_mod_handle, RECORD_MODULE_STOP, 0);
    sys_os_sched_lock();
    record_module_cmd(music_player->rec_mod_handle, RECORD_MODULE_CLOSE, 0);
    music_player->rec_mod_handle = 0;
    sys_drv_uninstall (REC_MOD_GROUP);

    return ret;
}


int32 player_encode_frame_deal(music_pcm_player_t *music_player)
{
    uint32 t1 = 0, t2;
    int32 ret = 0;
    uint8 *encode_in_buffer = RECORD_FILE_ENCODE_IN_BUF_ADDR; //存放一帧pcm数据，输入到编码库中

    while (music_player->encode_input_buf_w.length >= music_player->encode_frame_len)
    {
        //编码必须一帧一帧编码，帧长可由公式music_record->music_info.channels*music_record->music_info.block_size*sizeof(short)计算
        if (buffer_rw_read(&music_player->encode_input_buf_w, encode_in_buffer,
                music_player->encode_frame_len) == FALSE)
        {
            libc_printf("read encode_input_buffer err!\n");
        }

        if (music_player->need_print_time == TRUE)
        {
            t1 = sys_get_ab_timer_us();
        }

        //立体声变换为左一块右一块传数
        if (music_player->encode_para.channel == 2)
        {
            short *temp_swap_buffer = (short *) encode_aout_buffer;
            short *src = encode_in_buffer;
            int i;

            //右声道先存放到临时BUFFER，左声道左对齐存放到一块
            for (i = 0; i < (music_player->encode_frame_len/sizeof(short)/2); i++)
            {
                temp_swap_buffer[i] = src[2*i + 1];
                src[i] = src[2*i];
            }
            //临时BUFFER拷贝到右声道
            libc_memcpy(encode_in_buffer + (music_player->encode_frame_len/2), temp_swap_buffer, music_player->encode_frame_len/2);
        }

        ret = record_module_cmd(music_player->rec_mod_handle, RECORD_MODULE_FRAME_DEAL, encode_in_buffer);
        record_module_cmd(music_player->rec_mod_handle, RECORD_MODULE_GET_STATUS, &(music_player->record_status));

        if (music_player->need_print_time == TRUE)
        {
            t2 = sys_get_ab_timer_us();
            music_player->enc_play_time += (t2 - t1);
        }

        if (music_player->record_status.status == RECORD_MODULE_STA_ERROR)
        {
            //发生录音错误，暂停录音，等待应用层处理
            music_player->mmm_record_sta = MMM_RECORD_ERR_PAUSE;
            break;
        }

        if (ret < 0)
        {
            break;
        }
    }

    return ret;
}


//src 数据以左右左右间隔存放； sample_count 样本对数
int32 player_encode_source_deal(music_pcm_player_t *music_player, short *src, int32 samples_count)
{
    short *dest = (short *) RECORD_FILE_ENCODE_IN_BUF_ADDR; /*复用0x100的编码输入Buffer*/
    int32 w_len;
    int i;
    bool ret;

    if (music_player->encode_para.channel == 2)
    {
        dest = src;
        w_len = samples_count * sizeof(short) * 2;
    }
    else
    {
        if (music_player->encode_para.record_source == RECORD_SOURCE_LR_MIX)
        {
            int32 point_sum;
            for (i = 0; i < samples_count; i++)
            {
                point_sum = src[2*i] + src[2*i + 1];
                dest[i] = (int16)(point_sum / 2);
            }
            w_len = samples_count * sizeof(short);
        }
        else if (music_player->encode_para.record_source == RECORD_SOURCE_L_CH)
        {
            for (i = 0; i < samples_count; i++)
            {
                dest[i] = src[2*i];
            }
            w_len = samples_count * sizeof(short);
        }
        else
        {
            for (i = 0; i < samples_count; i++)
            {
                dest[i] = src[2*i + 1];
            }
            w_len = samples_count * sizeof(short);
        }
    }

    ret = buffer_rw_write(&(music_player->encode_input_buf_w), dest, w_len);

    if (ret == FALSE)
    {
        libc_printf_warning("write encode_input_buf_w full!\n");
    }

    return 0;
}

#endif
