/********************************************************************************
 * 	   Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 * 						   All Rights Reserved.
 *
 * 描述：本地播歌中间件  播放seek功能实现
 ********************************************************************************/

#include "music_player_global.h"

int32 seek_process(music_player_t *music_player, int32 *delay_time)
{
    int32 ret = 0;
    int32 delay_time_shiftor;
    int32 time_diff = 0;
    int32 time_acc;
    if ((music_player->music_ffrev_flag != 0) || (music_player->music_seek_flag != 0)/*ffrev decode frame count*/)
    {
        if (music_player->music_seek_flag != 0)
        {
            music_player->music_decode_info.cur_time = music_player->music_status.cur_time;
        }
        else
        {

            if (music_player->music_ffrev_flag > 0)
            {
                music_player->music_decode_info.cur_time += music_player->music_setting.ffb_step;
                if (music_player->music_decode_info.cur_time > music_player->music_info.total_time)
                {
                    music_player->music_decode_info.cur_time = music_player->music_info.total_time;
                    ret = AD_RET_DATAUNDERFLOW; //快进到尾
                }
            }
            else
            {
                music_player->music_decode_info.cur_time -= music_player->music_setting.ffb_step;
                if (music_player->music_decode_info.cur_time < 0)
                {
                    music_player->music_decode_info.cur_time = 0;
                    ret = AD_RET_DATAUNDERFLOW; //快退到头
                }
            }

            time_diff = music_player->music_decode_info.music_bp_info.bp_time_offset
                    - music_player->music_decode_info.cur_time;
            if (time_diff < 0)
            {
                time_diff = -time_diff;
            }
        }

        time_acc = 1000;
        if ((music_player->music_seek_flag != 0) || (time_diff > (music_player->music_setting.ffb_step * 4)/*4000*/)) //user defined time diff max
        {
            chunk_seek_proc: music_player->music_decode_info.music_bp_info.bp_time_offset
            = music_player->music_decode_info.cur_time;
            music_player->music_decode_info.music_bp_info.bp_file_offset = 0;
            //aax can not change info_ext
            //music_player->music_decode_info.music_bp_info.bp_info_ext = 0;
            if (ret == AD_RET_DATAUNDERFLOW)
            {
                /*底层解码库就没有seek到头或者尾的机会，规避异常情况*/
                //music_player->music_decode_info.music_bp_info.bp_time_offset
                //= music_player->music_decode_info.cur_time;
            }
            else
            {
                ret = audio_decoder_cmd(music_player->audiodec_handle, AD_CMD_CHUNK_SEEK, (uint32)
                        & music_player->music_decode_info);
            }
            /*for ape seek forward stop*/
            if ((music_player->music_seek_flag > 0) && (music_player->music_decode_info.cur_time
                    < music_player->music_status.cur_time))
            {
                music_player->music_decode_info.cur_time += time_acc;
                time_acc += 1000;
                goto chunk_seek_proc;
            }
        }

        //到头和到尾的特殊处理或者errror的处理
        delay_time_shiftor = music_player->music_ffrev_flag;
        if (delay_time_shiftor < 0)
        {
            delay_time_shiftor = -delay_time_shiftor;
        }
        *delay_time = 100 >> delay_time_shiftor; // 1->500ms, 2->250ms, 3->120ms, 4->60ms,5->30ms
        if (music_player->music_seek_flag != 0)
        {
            ret = audio_decoder_cmd(music_player->audiodec_handle, AD_CMD_CHUNK_RESET, (uint32) NULL);
            /*开100ms淡入*/
            *delay_time = 2;
            music_player->music_seek_flag = 0;
        }

    }
    return ret;
}

int32 update_ffrev_setting(music_player_t *music_player, uint32 args)
{
    int32 ret = 0;
    //dac mute
    //digital mute
    //wisht

    //0：关闭快进快退，> 0 快进及其级别，< 0，快进退及其级别，取值范围[-5,+5]。
    if ((music_player->music_ffrev_flag != 0) && (args == 0))
    {
        /*强制停止seek*/
        music_player->music_seek_flag = music_player->music_ffrev_flag;
        //ret = audio_decoder_cmd(music_player->audiodec_handle, AD_CMD_CHUNK_RESET, (uint32)NULL);
        /*静音80ms，以屏蔽快进退后播放杂音*/
        music_player->music_mute_samples = music_player->music_info.sample_rate_hz / 100 * 8;

    }
    music_player->music_ffrev_flag = (int32) args;
    return ret;
}
