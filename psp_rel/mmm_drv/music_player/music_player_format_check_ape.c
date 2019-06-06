/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：本地播歌中间件  APE文件 format check处理
 ********************************************************************************/

#include "music_player_global.h"

#if (SUPPORT_APE_TYPE == 1)

int32 apecheck(music_info_t *music_info)
{
    int32 ret = 0;
    int32 getbytes = 0, readlen = 0;
    uint8 *header;
    int32 iVersion;
    int32 nDescriptorBytes, nHeaderBytes, nSeekTableBytes, nHeaderDataBytes;
    int32 nCompressionLevel, nBlocksPerFrame, nFinalFrameBlocks, nTotalFrames;
    //    int32 i;
    //    char *seek_tbl_buff_pt;
    int32 seek_tbl_offset = 0;

    set_n_bytes(music_info, music_info->file_hdr_len);
    header = (uint8 *) music_info->data_buf_addr + 512;
    getbytes = read_n_bytes(music_info, 6, header);
    readlen += 6;
    iVersion = (int32) (((uint32) header[5] << 8) | (uint32) header[4]);

    if (iVersion >= 3980)
    {
        /* skip 2 bytes */
        skip_n_bytes(music_info, 2);
        readlen += 2;
        getbytes = read_n_bytes(music_info, 28, header);
        readlen += 28;
        nDescriptorBytes = (int32) (((uint32) header[3] << 24) | ((uint32) header[2] << 16) | ((uint32) header[1] << 8)
                | (uint32) header[0]);
        nHeaderBytes = (int32) (((uint32) header[7] << 24) | ((uint32) header[6] << 16) | ((uint32) header[5] << 8)
                | (uint32) header[4]);
        nSeekTableBytes = (int32) (((uint32) header[11] << 24) | ((uint32) header[10] << 16)
                | ((uint32) header[9] << 8) | (uint32) (header[8]));
        nHeaderDataBytes = (int32) ((((uint32) header[15] << 24) | ((uint32) header[14] << 16) | ((uint32) header[13]
                << 8) | (uint32) (header[12])));
        /* skip 16 bytes */
        skip_n_bytes(music_info, nDescriptorBytes - 8 - 28);
        readlen += nDescriptorBytes - 8 - 28;

        getbytes = read_n_bytes(music_info, nHeaderBytes, header);
        readlen += nHeaderBytes;
        nCompressionLevel = (int32) (((uint32) header[1] << 8) | (uint32) header[0]);
        nBlocksPerFrame = (int32) (((uint32) header[7] << 24) | ((uint32) header[6] << 16) | ((uint32) header[5] << 8)
                | (uint32) header[4]);
        nFinalFrameBlocks = (int32) (((uint32) header[11] << 24) | ((uint32) header[10] << 16) | ((uint32) header[9]
                << 8) | (uint32) header[8]);
        nTotalFrames = (int32) (((uint32) header[15] << 24) | ((uint32) header[14] << 16) | ((uint32) header[13] << 8)
                | (uint32) header[12]);
        music_info->bps = (int32) (((uint32) header[17] << 8) | (uint32) header[16]);
        music_info->channels = (int32) (((uint32) header[19] << 8) | (uint32) header[18]);
        music_info->sample_rate_hz = (int32) ((((uint32) header[23] << 24) | ((uint32) header[22] << 16)
                | ((uint32) header[21] << 8) | (uint32) header[20]));
        //music_info->sample_rate = music_info->sample_rate_hz / 1000;
        music_info->total_time = __divdi3(((int64) nBlocksPerFrame * (nTotalFrames - 1) + nFinalFrameBlocks) * 1000,
                music_info->sample_rate_hz);

        music_info->block_size = nBlocksPerFrame;
        music_info->frame_size = nFinalFrameBlocks;
        //        music_info->sub_format = nTotalFrames;

        seek_tbl_offset = readlen;

        /* skip seek table */
        skip_n_bytes(music_info, nSeekTableBytes);
        //#ifdef WIN32
        //       seek_tbl_buff_pt = (char *)ape_seek_table_array;
        //#else
        //        seek_tbl_buff_pt = (char *)APE_CHECK_SRAME_ADDR;
        //#endif

        //        for(i = 0; i < nTotalFrames; i++)
        //        {
        //            read_n_bytes(music_info, 4, seek_tbl_buff_pt + (i * 4));
        //        }
        readlen += nSeekTableBytes;

        /*F44_0db_a22s.ape，nSeekTableBytes达到29,需要特殊处理才能支持*/
        //if (nSeekTableBytes != (nTotalFrames * 4))
        //{
        //    ret = -1;
        //    goto apecheck_exit_0;
        //}

        /* skip wav header*/
        skip_n_bytes(music_info, nHeaderDataBytes);
        readlen += nHeaderDataBytes;
    }
    else if (iVersion > 3950)
    {
        getbytes = read_n_bytes(music_info, 26, header);
        readlen += 26;
        nCompressionLevel = (int32) (((uint32) header[1] << 8) | (uint32) header[0]);
        nBlocksPerFrame = 73728 * 4;
        nTotalFrames = (int32) (((uint32) header[21] << 24) | ((uint32) header[20] << 16) | ((uint32) header[19] << 8)
                | (uint32) header[18]);
        nFinalFrameBlocks = (int32) (((uint32) header[25] << 24) | ((uint32) header[24] << 16) | ((uint32) header[23]
                << 8) | (uint32) header[22]);
        nHeaderDataBytes = (int32) (((uint32) header[13] << 24) | ((uint32) header[12] << 16) | ((uint32) header[11]
                << 8) | (uint32) header[10]);

        music_info->bps = 16;
        music_info->channels = (int32) (((uint32) header[5] << 8) | (uint32) header[4]);
        music_info->sample_rate_hz = (int32) ((((uint32) header[9] << 24) | ((uint32) header[8] << 16)
                | ((uint32) header[7] << 8) | (uint32) header[6]));

        //music_info->sample_rate = music_info->sample_rate_hz / 1000;
        music_info->total_time = __divdi3(((int64) nBlocksPerFrame * (nTotalFrames - 1) + nFinalFrameBlocks) * 1000,
                music_info->sample_rate_hz);

        /* skip wav header*/
        skip_n_bytes(music_info, 8 + nHeaderDataBytes);
        readlen += 8 + nHeaderDataBytes;

        music_info->block_size = nBlocksPerFrame;
        music_info->frame_size = nFinalFrameBlocks;

        seek_tbl_offset = readlen;

        /* skip seek table */
        skip_n_bytes(music_info, nTotalFrames * 4);
        //#ifdef WIN32
        //        seek_tbl_buff_pt = (char *)ape_seek_table_array;
        //#else
        //        seek_tbl_buff_pt = (char *)APE_CHECK_SRAME_ADDR;
        //#endif
        //        for(i = 0; i < nTotalFrames; i++)
        //        {
        //            read_n_bytes(music_info, 4, seek_tbl_buff_pt + (i * 4));
        //       }
        readlen += nTotalFrames * 4;

        //while (1)
        //{
        //    getbytes = read_n_bytes(music_info, 4, header);
        //    readlen += 4;
        //    if ((header[0] & header[1] & header[2] & header[3]) != 0)
        //    {
        //        break;
        //    }
        //}

        /* empty data*/
        //while (1)
        //{
        //    getbytes = read_n_bytes(music_info, 4, header);
        //    if ((header[0] | header[1] | header[2] | header[3]) != 0)
        //    {
        //        break;
        //    }
        //    readlen += 4;
        // }
        /* fix seek table*/

        /*wav header?*/
    }
    else
    {
        ret = -1;
        goto apecheck_exit_0;

    }
    //music_info->file_hdr_len += readlen;
    music_info->avg_bitrate = __divdi3((int64) music_info->file_len * 8, music_info->total_time);

    music_info->data_buf_addr = (char *) seek_tbl_offset;
    //#ifdef WIN32
    //    music_info->data_buf_addr = (char *)ape_seek_table_array;
    //#else
    //    music_info->data_buf_addr = (char *)APE_CHECK_SRAME_ADDR;
    //#endif
    music_info->data_buf_len = nTotalFrames;//nSeekTableBytes/4?

    /*audio 不支持extra high 的ape歌曲*/
    if (nCompressionLevel >= 3000)
    {
        ret = -1;
    }

    apecheck_exit_0: return ret;
}

#endif
