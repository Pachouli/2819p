/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：本地播歌中间件  WAV文件 format check处理
 ********************************************************************************/

#include "music_player_global.h"

enum DCASyncwords {
    DCA_SYNCWORD_CORE_BE        = 0x7FFE8001,
    DCA_SYNCWORD_CORE_LE        = 0xFE7F0180,
    DCA_SYNCWORD_CORE_14B_BE    = 0x1FFFE800,
    DCA_SYNCWORD_CORE_14B_LE    = 0xFF1F00E8,
    DCA_SYNCWORD_XCH            = 0x5A5A5A5A,
    DCA_SYNCWORD_XXCH           = 0x47004A03,
    DCA_SYNCWORD_X96            = 0x1D95F262,
    DCA_SYNCWORD_XBR            = 0x655E315E,
    DCA_SYNCWORD_LBR            = 0x0A801921,
    DCA_SYNCWORD_XLL            = 0x41A29547,
    DCA_SYNCWORD_SUBSTREAM      = 0x64582025,
    DCA_SYNCWORD_SUBSTREAM_CORE = 0x02B09261,
};
int32 wavcheck(music_info_t *music_info)
{
    int32 ret = 0;
    int32 getbytes = 0, readlen = 0;
    uint8 *header = 0;
    int32 nFormatTag = 0, nChannels = 0, nBitsPerSample = 0, chunk_size = 0;
    int32 nSamplesPerSec = 0, nAvgBytesPerSec = 0, nBlockAlign = 0, nExtension = 0;
    int32 tempval = 0;
    int32 *pInt32 = 0;
    uint32 state = 0;

    set_n_bytes(music_info, music_info->file_hdr_len);
    header = (uint8 *) music_info->data_buf_addr + 512;

    skip_n_bytes(music_info, 12);
    readlen = 12;

    do
    {
        getbytes = read_n_bytes(music_info, 8, header);
        readlen += 8;
        if (getbytes < 8)
        {
            ret = -1;
            goto wavcheck_exit_0;
        }
        chunk_size = (int32)(((uint32) header[7] << 24) | ((uint32) header[6] << 16) | ((uint32) header[5] << 8)
                | (uint32) header[4]);
        if ((header[0] == 0x64) && (header[1] == 0x61) && (header[2] == 0x74) && (header[3] == 0x61)) //data
        {
            music_info->file_len = chunk_size + readlen;//规避末尾杂音问题。
            break;
        }
        if (((chunk_size & 0xff000000) != 0) || (chunk_size > music_info->file_len))
        {
            /*fix bug: ADPCM_22050kHz.wav播放异常，chunk size太大*/
            ret = -1;
            goto wavcheck_exit_0;
        }

        else if ((header[0] == 0x66) && (header[1] == 0x6D) && (header[2] == 0x74) && (header[3] == 0x20)) //fmt
        {

            /*fix bug:ADPCM_MS_44100kHz_64kbps.MP3.wav 播放死机，实际blocksize为0x3f9*/
            header[38] = 0;
            header[39] = 0;
            getbytes = read_n_bytes(music_info, chunk_size, header + 20);
            nFormatTag = header[20];
            if(nFormatTag == 0xfe)//0xfffe subfomat = 0x1
            {   //wav extension
                nFormatTag = header[44];
            }
            nChannels = header[22];
            nSamplesPerSec = (int32)(
                    ((uint32) header[27] << 24) | ((uint32) header[26] << 16) | ((uint32) header[25] << 8)
                            | (uint32) header[24]);
            nAvgBytesPerSec = (int32)(
                    ((uint32) header[31] << 24) | ((uint32) header[30] << 16) | ((uint32) header[29] << 8)
                            | (uint32) header[28]);
            nBlockAlign = (int32)(((uint32) header[33] << 8) | (uint32) header[32]);
            nBitsPerSample = header[34];

            nExtension = 0;
            /*WAV_IMA_ADPCM WAV_MS_ADPCM WAV_ALAW WAV_ULAW*/
            if ((nFormatTag == 0x2) || (nFormatTag == 0x11) || (nFormatTag == 0x6) || (nFormatTag == 0x7))
            {
                nExtension = (int32)(((uint32) header[39] << 8) | (uint32) header[38]);
            }
        }
        else
        {
            skip_n_bytes(music_info, chunk_size);
        }
        readlen += chunk_size;
    } while (1);

    music_info->file_hdr_len += readlen;

    getbytes = read_n_bytes(music_info, 6, header);//for dts check
    readlen += 6;
    state = (((uint32) header[0] << 24) | ((uint32) header[1] << 16) | ((uint32) header[2] << 8)
                    | (uint32) header[3]);

    if(((state == DCA_SYNCWORD_CORE_14B_LE  && (header[4] & 0xF0) == 0xF0 &&  header[5]         == 0x07) || \
     (state == DCA_SYNCWORD_CORE_14B_BE &&  header[4]         == 0x07 && (header[5] & 0xF0) == 0xF0) || \
      state == DCA_SYNCWORD_CORE_LE || state == DCA_SYNCWORD_CORE_BE || state == DCA_SYNCWORD_SUBSTREAM))
    {
        //is_dts_flag = 1;
        ret = -1;
        goto wavcheck_exit_0;//Not support dts stream
    }
    /*WAV_IMA_ADPCM WAV_MS_ADPCM */
    if ((nFormatTag == 0x2) || (nFormatTag == 0x11))
    {
        music_info->avg_bitrate = (nAvgBytesPerSec << 3) / 1000;
        music_info->total_time = __divdi3((int64)((uint32) chunk_size) * 1000, nAvgBytesPerSec);
        music_info->block_size = nExtension;
        music_info->frame_size = nBlockAlign;
        if (music_info->block_size == 0)
        {
            /*fix bug: ADPCM_MS_44100kHz_64kbps.MP3.wav播放死机，现在报错退出*/
            ret = -1;
            goto wavcheck_exit_0;
        }
    }
    else
    {
        tempval = (int32)((uint32) nChannels * ((uint32) nBitsPerSample >> 3));
        music_info->total_time = __divdi3((int64)((uint32) chunk_size) * 1000, tempval * nSamplesPerSec);
        music_info->avg_bitrate = (nSamplesPerSec * tempval) / 125;
        music_info->block_size = nChannels;
        music_info->frame_size = nBlockAlign;
    }

    music_info->channels = nChannels;
    music_info->sample_rate_hz = nSamplesPerSec;
    //music_info->sample_rate = nSamplesPerSec / 1000;
    music_info->bps = nBitsPerSample;

    music_info->sub_format = nFormatTag;

    if (readlen < 256)
    {
        skip_n_bytes(music_info, 256 - readlen - 16);
        getbytes = read_n_bytes(music_info, 32, header);
        if (header[16] == 0x84)
        {
            pInt32 = (int32 *) header;
            tempval = pInt32[0] | pInt32[1] | pInt32[2] | pInt32[3] | pInt32[6] | pInt32[7];
            if (tempval == 0)
            {
                //act typ
                music_info->avg_bitrate = 8;
                tempval = (int32) header[19] + ((int32) header[20] * 60) + ((int32) header[21] * 3600);
                music_info->total_time = (uint32)(tempval * 1000);
                ret = 1;
            }
        }
    }

    wavcheck_exit_0: return ret;
}

