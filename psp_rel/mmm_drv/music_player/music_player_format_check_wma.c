/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：本地播歌中间件  WMA文件 format check处理
 ********************************************************************************/

#include "music_player_global.h"

#define WMA_OBJ_HDR_LEN (32)
const uint8 headerobject[16] =
{ 0x30, 0x26, 0xb2, 0x75, 0x8e, 0x66, 0xcf, 0x11, 0xa6, 0xd9, 0x00, 0xaa, 0x00, 0x62, 0xce, 0x6c };
const uint8 fileobject[16] =
{ 0xa1, 0xdc, 0xab, 0x8c, 0x47, 0xa9, 0xcf, 0x11, 0x8e, 0xe4, 0x00, 0xc0, 0x0c, 0x20, 0x53, 0x65 };
const uint8 streamobject[16] =
{ 0x91, 0x07, 0xdc, 0xb7, 0xb7, 0xa9, 0xcf, 0x11, 0x8e, 0xe6, 0x00, 0xc0, 0x0c, 0x20, 0x53, 0x65 };
const uint8 audioobject[16] =
{ 0x40, 0x9e, 0x69, 0xf8, 0x4d, 0x5b, 0xcf, 0x11, 0xa8, 0xfd, 0x00, 0x80, 0x5f, 0x5c, 0x44, 0x2b };
const uint8 encryptionobject[16] =
{ 0xfb, 0xb3, 0x11, 0x22, 0x23, 0xbd, 0xd2, 0x11, 0xb4, 0xb7, 0x00, 0xa0, 0xc9, 0x55, 0xfc, 0x6e };

AUDIOPARAM *piAudioparam_global;

int32 wmacheck(music_info_t *music_info)
{
    int32 ret = 0;
    int32 getbytes = 0, readlen = 0;
    uint8 *header;
    int32 headerobjectsize, objectsize, audio_object_exit = 0;
    int32 packetsize = 0, blockalign, packetsizeaddr = 0, blockalignaddr;
    int32 preroll;
    int64 send_duration, duration;

    set_n_bytes(music_info, music_info->file_hdr_len);
    header = (uint8 *) music_info->data_buf_addr + 512;
    getbytes = read_n_bytes(music_info, 24, header);
    readlen += 24;

    /* is header object? */
    if (libc_memcmp(header, headerobject, 16) != 0)
    {
        ret = -1;
        goto wmacheck_exit_0;

    }
    headerobjectsize = (int32)(((uint32) header[19] << 24) | ((uint32) header[18] << 16) | ((uint32) header[17] << 8)
            | (uint32) header[16]);

    skip_n_bytes(music_info, 6);
    readlen += 6;

    while (readlen < headerobjectsize)
    {
        getbytes = read_n_bytes(music_info, 24, header);
        readlen += 24;
        objectsize = (int32)(((uint32) header[19] << 24) | ((uint32) header[18] << 16) | ((uint32) header[17] << 8)
                | (uint32) header[16]);

        objectsize -= 24;

        if (objectsize <= 0)
        {
            /* error stream. */
            ret = -1;
            goto wmacheck_exit_0;
        }

        /* is Content Encryption Object? */
        if (libc_memcmp(header, encryptionobject, 16) == 0)
        {
            /* drm stream. */
            ret = -1;
            goto wmacheck_exit_0;
        }

        /* is file object? */
        if (libc_memcmp(header, fileobject, 16) == 0)
        {
            skip_n_bytes(music_info, 32);
            readlen += 32;
            objectsize -= 32;

            getbytes = read_n_bytes(music_info, 8, header);
            readlen += 8;
            objectsize -= 8;
            /*Total packets*/
            music_info->block_size = (int32) (((uint32) header[3] << 24) + ((uint32) header[2] << 16)
                    + ((uint32) header[1] << 8) + (uint32) header[0]);
            piAudioparam_global->cPackets = music_info->block_size;

            getbytes = read_n_bytes(music_info, 8, header);
            readlen += 8;
            objectsize -= 8;
            /* Play Duration */
            duration = (((uint32) header[4] << 24) + ((uint32) header[3] << 16) + ((uint32) header[2] << 8)
                    + (uint32) header[1]);

            getbytes = read_n_bytes(music_info, 8, header);
            readlen += 8;
            objectsize -= 8;
            /* Send Duration */
            send_duration = (((uint32) header[4] << 24) + ((uint32) header[3] << 16)
                    + ((uint32) header[2] << 8) + (uint32) header[1]);
            send_duration = send_duration * 256;
            music_info->total_time = __divdi3(send_duration, 10000);

            /* Preroll */
            getbytes = read_n_bytes(music_info, 12, header);
            readlen += 12;
            objectsize -= 12;

            /* preroll Duration */
            preroll = (((uint32) header[3] << 24) + ((uint32) header[2] << 16) + ((uint32) header[1] << 8)
                    + (uint32) header[0]);

            if(music_info->total_time < 1000)
            {
                send_duration = duration * 256;
                music_info->total_time = __divdi3(send_duration, 10000);
                music_info->total_time -= preroll;
            }
            piAudioparam_global->TotalTime = music_info->total_time;

            getbytes = read_n_bytes(music_info, 4, header);
            readlen += 4;
            objectsize -= 4;
            music_info->frame_size = (int32)(((uint32) header[3] << 24) | ((uint32) header[2] << 16)
                    | ((uint32) header[1] << 8) | (uint32) header[0]);
            /* Max Packet Size */
            packetsizeaddr = readlen - 4;
            packetsize = music_info->frame_size;
            piAudioparam_global->cbPacketSize = packetsize;

        }

        /* is streamobject object? */
        if (libc_memcmp(header, streamobject, 16) == 0)
        {
            getbytes = read_n_bytes(music_info, 24, header);
            readlen += 24;
            objectsize -= 24;

            /* is audio object? */
            if (libc_memcmp(header, audioobject, 16) == 0)
            {
                read_n_bytes(music_info, 30, header);
                readlen += 30;
                objectsize -= 30;

                //usually 1, for skip video payload
                piAudioparam_global->wAudioStreamId = header[24];
                getbytes = read_n_bytes(music_info, 24, header);
                readlen += 24;
                objectsize -= 24;

                music_info->sub_format = (int32)((((uint32) header[1] << 8) + (uint32) header[0]));
                if (music_info->sub_format == 0x161)
                {
                    piAudioparam_global->nVersion = 2;

                }
                else if (music_info->sub_format == 0x160)
                {
                    piAudioparam_global->nVersion = 1;
                }
                else
                {
                    piAudioparam_global->nVersion = 3;
                }
                music_info->channels = (int32) header[2];
                piAudioparam_global->dwChannelMask = 4;
                if (music_info->channels == 2)
                {
                    piAudioparam_global->dwChannelMask = 1 | 2;
                }
                piAudioparam_global->nChannels = music_info->channels;
                music_info->sample_rate_hz = (int32)((((uint32) header[5] << 8) + (uint32) header[4]));
                piAudioparam_global->nSamplesPerSec = music_info->sample_rate_hz;
                music_info->avg_bitrate = (int32)((((uint32) header[9] << 8) + (uint32) header[8])) * 8 / 1000;
                piAudioparam_global->nAvgBytesPerSec = ((((uint32) header[9] << 8) + (uint32) header[8]));
                music_info->bps = (int32)((((uint32) header[15] << 8) + (uint32) header[14]));
                piAudioparam_global->nBitsPerSample = music_info->bps;

                blockalignaddr = readlen - 4 - 8;
                blockalign = (int32)((((uint32) header[13] << 8) + (uint32) header[12]));
                piAudioparam_global->nBlockAlign = blockalign;

                piAudioparam_global->nSamplesPerBlock = (int32)((((uint32) header[19] << 8) + (uint32) header[18]));

                if (music_info->sub_format >= 0x161)
                {
                    piAudioparam_global->nEncodeOpt = (int32)((((uint32) header[23] << 8) + (uint32) header[22]));
                }
                audio_object_exit = 1;
            }
            else
            {
                /* Not audio stream. */
                ret = -1;
                goto wmacheck_exit_0;
            }
        }

        skip_n_bytes(music_info, objectsize);
        readlen += objectsize;
    }

    if (audio_object_exit == 0)
    {
        /* Not audio stream. */
        ret = -1;
        goto wmacheck_exit_0;
    }

    /* header object and data object header */
    /* data packet header 50 bytes*/
    music_info->file_hdr_len += headerobjectsize + 0x32;

    piAudioparam_global->cbHeader = music_info->file_hdr_len;

    music_info->data_buf_addr = piAudioparam_global;

    if ((music_info->sub_format != 0x161) && (music_info->sub_format != 0x160))
    {
        /* Professional or Lossless. */
        ret = -1;
        goto wmacheck_exit_0;
    }

    /*seek to second packet*/

    wmacheck_exit_0: return ret;
}

