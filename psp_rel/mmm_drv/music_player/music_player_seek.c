/********************************************************************************
 * 	   Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 * 						   All Rights Reserved.
 *
 * ���������ز����м��  ����seek����ʵ��
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
                    ret = AD_RET_DATAUNDERFLOW; //�����β
                }
            }
            else
            {
                music_player->music_decode_info.cur_time -= music_player->music_setting.ffb_step;
                if (music_player->music_decode_info.cur_time < 0)
                {
                    music_player->music_decode_info.cur_time = 0;
                    ret = AD_RET_DATAUNDERFLOW; //���˵�ͷ
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
                /*�ײ������û��seek��ͷ����β�Ļ��ᣬ����쳣���*/
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

        //��ͷ�͵�β�����⴦�����errror�Ĵ���
        delay_time_shiftor = music_player->music_ffrev_flag;
        if (delay_time_shiftor < 0)
        {
            delay_time_shiftor = -delay_time_shiftor;
        }
        *delay_time = 100 >> delay_time_shiftor; // 1->500ms, 2->250ms, 3->120ms, 4->60ms,5->30ms
        if (music_player->music_seek_flag != 0)
        {
            ret = audio_decoder_cmd(music_player->audiodec_handle, AD_CMD_CHUNK_RESET, (uint32) NULL);
            /*��100ms����*/
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

    //0���رտ�����ˣ�> 0 ������伶��< 0������˼��伶��ȡֵ��Χ[-5,+5]��
    if ((music_player->music_ffrev_flag != 0) && (args == 0))
    {
        /*ǿ��ֹͣseek*/
        music_player->music_seek_flag = music_player->music_ffrev_flag;
        //ret = audio_decoder_cmd(music_player->audiodec_handle, AD_CMD_CHUNK_RESET, (uint32)NULL);
        /*����80ms�������ο���˺󲥷�����*/
        music_player->music_mute_samples = music_player->music_info.sample_rate_hz / 100 * 8;

    }
    music_player->music_ffrev_flag = (int32) args;
    return ret;
}
