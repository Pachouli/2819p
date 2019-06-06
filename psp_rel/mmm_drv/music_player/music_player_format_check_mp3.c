/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：本地播歌中间件  MP3文件 format check处理
 ********************************************************************************/

#include "music_player_global.h"
//remove restrict for Copyright and Original for 20070616itou2.mp3 4-5 sec format error
#define MP3_HEADORA 0xFE0C03//(0xFE0C0F)
#define BS_PAGEMAX_ERRORNUM (600)
#define SYNC_HEAD_MAXCOUNT (48)
#define SYNC_HEAD_MAXSAMECOUNT (4)

#define FRAMES_FLAG     0x0001

/* 1: MPEG-1, 0: MPEG-2 LSF*/
const uint16 table_bitrate[2][3][15] =
{
    {
        { 0, 32, 48, 56, 64, 80, 96, 112, 128, 144, 160, 176, 192, 224, 256 },
        { 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160 },
        { 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160 }
    },
    {
        { 0, 32, 64, 96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448 },
        { 0, 32, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320, 384 },
        { 0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320 }
    }
};

const uint16 samplerate_table[3] =
{ 44100, 48000, 32000 };

uint16 mp3_getframesize(uint32 curframehead, int32 *pbitrate, int32 *psample_rate)
{
    uint32 curframesize, t;
    uint32 mpeg_lsf, mpeg_ver, mpeglayer, padding;
    uint32 bitrate, samplerate;
    mpeg_ver = (uint8)(curframehead >> 19) & 0x3;
    mpeg_lsf = (mpeg_ver & 0x1);
    if (mpeg_ver == 0)
    {
        mpeg_ver = 2;
    }
    else
    {
        mpeg_ver = 3 - mpeg_ver;
    }
    mpeglayer = 4 - ((uint8)(curframehead >> 17) & 0x3);
    t = (uint8)(curframehead >> 10) & 0x3;
    samplerate = samplerate_table[t];
    samplerate = samplerate >> mpeg_ver;
    t = (uint8)(curframehead >> 12) & 0xF;
    bitrate = table_bitrate[mpeg_lsf][mpeglayer - 1][t];
    padding = (uint8)(curframehead >> 9) & 0x1;
    if (mpeglayer == 3)
    {
        curframesize = (144000 * (uint32) bitrate) / (uint32) samplerate;
        if (mpeg_lsf == 0)
        {
            curframesize = curframesize >> 1;
        }
        curframesize = curframesize + padding;
    }
    else if (mpeglayer == 2)
    {
        curframesize = (144000 * (uint32) bitrate) / (uint32) samplerate;
        curframesize = curframesize + padding;
    }
    else //lay1
    {
        curframesize = (12000 * (uint32) bitrate) / (uint32) samplerate;
        curframesize = curframesize + padding;
        curframesize = curframesize << 2;
    }
    *pbitrate = (int32) bitrate;
    *psample_rate = (int32) samplerate;
    return curframesize;
}

int32 mp3check(music_info_t *music_info, bool only_mono)
{
    storage_io_t *storage_io = music_info->storage_io;
    uint8 *bs = music_info->data_buf_addr;
    uint8 *pbs = 0;
    uint32 mp3_readpagescount = 0;
    uint32 Sync_HeadCount = 0;
    uint32 tvalue = 0;
    int32 curframepos = 0, breakpos = 0;
    int32 bslen = 0, readbslen = 0;
    int32 i = 0, Sync_HeadFindFlag = 0;
    uint32 lastheadvalue = 0, curframehead = 0;
    int32 ret = 0, id3_len = music_info->file_hdr_len;
    int32 bitrate, sample_rate, mpeg_version;
    int32 first_frame_size = 0, frame_samples, is_vbr;
    uint32 head_flags = 0;

    uint32 *Sync_HeadValue;
    uint32 *FramePos;
    uint32 *FrameSize;
    uint32 *FrameBitrate;

    if (only_mono == TRUE)
    {
        music_info->channels = 1;//仅可支持单声道音频文件，内存资源消耗低
    }
    else
    {
        music_info->channels = 2;//可支持单声道和立体声的音频文件，内存资源消耗高
    }

    if (format_check_temp_buf != NULL)
    {
        Sync_HeadValue = (uint32 *) (format_check_temp_buf + FORMAT_CHECK_BUF_LEN);
        FramePos = Sync_HeadValue + SYNC_HEAD_MAXCOUNT;
        FrameSize = FramePos + SYNC_HEAD_MAXCOUNT;
        FrameBitrate = FrameSize + SYNC_HEAD_MAXCOUNT;
    }
    else
    {
        ret = -1;
        goto mp3check_exit_1;
    }
    for (i = 0; i < SYNC_HEAD_MAXCOUNT; i++)
    {
        Sync_HeadValue[i] = 0xFFFFFF;
        FramePos[i] = 0;
        FrameSize[i] = 0;
        FrameBitrate[i] = 0;
    }

    set_n_bytes(music_info, id3_len);
    pbs = (uint8*) ((int32) bs + (id3_len & 0X1FF));
    bslen = 512 - (id3_len & 0X1FF);
    breakpos = bslen;

    while (1)
    {
        //sync head
        while (bslen > 0)
        {
            if ((*pbs) != 0xFF)
            {
                pbs++;
                bslen--;
                continue;
            } //if 0xff head
            pbs++;
            bslen--;
            if (bslen < 3)
            {
                for (i = 0; i < bslen; i++)
                {
                    bs[4 - bslen + i] = *pbs;
                    pbs++;
                }
                pbs = bs + (4 - bslen);
                //warning小机上必须确保读的位置是4字节对齐，否则它会取4字节边界去读。
                readbslen = storage_io->read(bs + 4, 1, 512, storage_io);
                if (readbslen <= 0)
                {
                    ret = -1;
                    goto mp3check_exit_0;
                }
                breakpos += (int32) readbslen;
                bslen += readbslen;

                mp3_readpagescount++;
                if (mp3_readpagescount > BS_PAGEMAX_ERRORNUM)
                {
                    ret = -1;
                    goto mp3check_exit_0;
                }
            }
            curframehead = ((uint32) pbs[0] << 16) + ((uint32) pbs[1] << 8) + (uint32) pbs[2]; //24bit head

            if ((curframehead & 0xE00000) != 0xE00000) //sync
            {
                continue;
            }
            if ((curframehead & 0x180000) == 0x080000) //ver reserved
            {
                continue;
            }
            if ((curframehead & 0x060000) == 0x000000) //layer reserved
            {
                continue;
            }
            if ((curframehead & 0x00F000) == 0x00F000) //bitrate reserved
            {
                continue;
            }
            //if((curframehead&0x00F000) == 0x000000)//bitrate free
            //    continue;
            if ((curframehead & 0x000C00) == 0x000C00) //samplerate reserved
            {
                continue;
            }
            //if((curframehead&0x000003) == 0x000002)//emphasis reserved
            //    continue;
            tvalue = curframehead & MP3_HEADORA;
            curframepos = breakpos - (int32) bslen; //curframepos
            bitrate = 0;
            for (i = (Sync_HeadFindFlag - 1); i >= 0; i--)
            {
                if (tvalue == Sync_HeadValue[i])
                {
                    if (((uint32) curframepos - FramePos[i]) != FrameSize[i])
                    {
                        continue;
                    }
                    curframepos = (int32) FramePos[i];
                    bitrate += (int32) FrameBitrate[i];
                    Sync_HeadCount++;
                    if (Sync_HeadCount == SYNC_HEAD_MAXSAMECOUNT)
                    {
                        lastheadvalue = tvalue;
                        /*因此会丢失前面3帧，请注意*/
                        music_info->file_hdr_len = music_info->file_hdr_len + curframepos - 1;
                        first_frame_size = (int32)(FramePos[i + 1] - FramePos[i]);
                        bitrate = bitrate / SYNC_HEAD_MAXSAMECOUNT;
                        bitrate = (bitrate != 0) ? bitrate : 1;
                        music_info->avg_bitrate = bitrate;
                        music_info->total_time = __divdi3(
                                (int64)(music_info->file_len - music_info->file_hdr_len) * 8, bitrate);
                        music_info->sample_rate_hz = sample_rate;
                        //music_info->sample_rate = sample_rate / 1000;

                        /* channel, bps, frame size, block szie is not necessary*/
                        music_info->sub_format = (int32) tvalue;
                        goto mp3check_exit_0;
                    }
                }
            }
            //if(lastheadvalue == 0)
            {
                Sync_HeadCount = 0;
                if (Sync_HeadFindFlag < SYNC_HEAD_MAXCOUNT)
                {
                    Sync_HeadValue[Sync_HeadFindFlag] = tvalue;
                    FramePos[Sync_HeadFindFlag] = (uint32) breakpos - (uint32) bslen;
                    FrameSize[Sync_HeadFindFlag] = (uint16) mp3_getframesize(curframehead, &bitrate, &sample_rate);
                    FrameBitrate[Sync_HeadFindFlag] = (uint16) bitrate;
                    Sync_HeadFindFlag++;
                    continue;
                }
                else
                {
                    //lastheadvalue = Sync_HeadValue[0];
                    Sync_HeadFindFlag = 0;
                    continue;
                }
            }

        } //while sync head
        bslen = storage_io->read(bs, 1, 512, storage_io);
        if (bslen <= 0)
        {
            ret = -1;
            goto mp3check_exit_0;
        }
        breakpos += (int32) bslen;
        pbs = bs;
        mp3_readpagescount++;
        if (mp3_readpagescount > BS_PAGEMAX_ERRORNUM)
        {
            ret = -1;
            goto mp3check_exit_0;
        }
    } //while(1)

    mp3check_exit_0: pbs = bs;

    /*录音小文件的特殊处理*/
    if ((ret == -1) && (music_info->file_len - music_info->file_hdr_len <= (32 * 1024)))
    {
        if (Sync_HeadFindFlag > 0)
        {
            i = (Sync_HeadFindFlag - 1);
            tvalue = Sync_HeadValue[i];
            bitrate = (int32) FrameBitrate[i];

            /*该判断条件仍会导致不足1秒的8kbps录音歌曲误判为非法文件。但可以过滤非法小文件*/
            if ((bitrate != 0) && Sync_HeadFindFlag > 12)
            {
                music_info->avg_bitrate = bitrate;
                music_info->total_time = __divdi3((int64)(music_info->file_len - music_info->file_hdr_len) * 8,
                        bitrate);
                music_info->sample_rate_hz = sample_rate;

                /* channel, bps, frame size, block szie is not necessary*/
                music_info->sub_format = (int32) tvalue;
                ret = 0;
            }
        }

    }

    /*update total time and average bitrate if it is vbr case*/
    if (ret == 0)
    {
        readbslen = 0;
        storage_io->seek(storage_io, ((int32)((uint32) music_info->file_hdr_len & 0xfffffe00)), SEEK_SET);
        bslen = storage_io->read(pbs, 1, 512, storage_io);
        //if (bslen <= 0)
        //{
        //    ret = -1;
        //}
        readbslen = bslen - (music_info->file_hdr_len & 0X1FF);
        pbs += bslen;

        while (readbslen < (first_frame_size + 4))
        {
            bslen = storage_io->read(pbs, 1, 512, storage_io);
            if (bslen <= 0)
            {
                ret = -1;
                goto mp3check_exit_1;
            }
            readbslen += bslen;
            pbs += bslen;
        }
        pbs = bs + (music_info->file_hdr_len & 0X1FF);

        curframehead = ((uint32) pbs[1] << 16) + ((uint32) pbs[2] << 8) + (uint32) pbs[3]; //24bit head

        mpeg_version = (uint8)(curframehead >> 19) & 0x3;
        if (mpeg_version == 0)
        {
            mpeg_version = 2;
        }
        else
        {
            mpeg_version = 3 - mpeg_version;
        }
        sample_rate = samplerate_table[(curframehead >> 10) & 0x3];
        sample_rate = (int32)((uint32) sample_rate >> mpeg_version);
        music_info->sample_rate_hz = sample_rate;

        curframehead = 4 - ((uint8)(curframehead >> 17) & 0x3); //mpeglayer
        if (curframehead == 1)
        {
            frame_samples = 384;
        }
        if (curframehead == 1)
        {
            frame_samples = 384; //layer 1 -> 384
        }
        else if ((curframehead == 3) && (mpeg_version != 0))
        {
            frame_samples = 576; //mpeg2 or mpeg 2.5 layer3
        }
        else
        {
            frame_samples = 1152; //layer 2 & mpeg1layer3
        }

        is_vbr = 0;
        readbslen = 0;
        //9+4
        curframehead = ((uint32) pbs[13] << 24) + ((uint32) pbs[14] << 16) + ((uint32) pbs[15] << 8) + (uint32) pbs[16];
        //"Xing" 0x58696e67, "Info", 0x496e666f, "VBRI", 0x56425249
        if (curframehead == 0x58696e67)
        {
            is_vbr = 1;
            readbslen = 9 + 4 + 4;
        }
        //17+4
        curframehead = ((uint32) pbs[21] << 24) + ((uint32) pbs[22] << 16) + ((uint32) pbs[23] << 8) + (uint32) pbs[24];
        if ((curframehead == 0x58696e67) || (curframehead == 0x496e666f))
        {
            is_vbr = 1;
            readbslen = 17 + 4 + 4;
        }
        //32+4
        curframehead = ((uint32) pbs[36] << 24) + ((uint32) pbs[37] << 16) + ((uint32) pbs[38] << 8) + (uint32) pbs[39];
        if ((curframehead == 0x58696e67) || (curframehead == 0x496e666f))
        {
            is_vbr = 1;
            readbslen = 32 + 4 + 4;
        }
        if (curframehead == 0x56425249)
        {
            is_vbr = 2;
            readbslen = 36 + 10;
        }
        curframehead = 0;
        if (is_vbr == 1) //Xing
        {
            pbs += readbslen;
            head_flags = (uint32) pbs[3];
            if ((head_flags & FRAMES_FLAG) != 0)
            {
                curframehead = ((uint32) pbs[4] << 24) + ((uint32) pbs[5] << 16) + ((uint32) pbs[6] << 8)
                        + (uint32) pbs[7];
            }
            //if(head_flags & TOC_FLAG)
            //{
            //}
        }
        if (is_vbr == 2) //VBRI
        {
            pbs += readbslen;

            /* total bytes */

            /* total frames */
            curframehead = ((uint32) pbs[4] << 24) + ((uint32) pbs[5] << 16) + ((uint32) pbs[6] << 8) + (uint32) pbs[7];

            /* now, it's time to get the toc table */
        }
        if (curframehead != 0)
        {
            music_info->total_time = __divdi3((int64) curframehead * frame_samples * 1000, music_info->sample_rate_hz);
            music_info->avg_bitrate = __divdi3((int64)(music_info->file_len - music_info->file_hdr_len) * 8,
                    music_info->total_time);
        }
    }

    mp3check_exit_1:
    return ret;
}

