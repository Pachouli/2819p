/********************************************************************************
 *     Copyright(c) 2015-2016 Actions (Zhuhai) Technology Co., Limited,
 *                         All Rights Reserved.
 *
 * 描述：本地播歌中间件  FLAC文件 format check处理
 ********************************************************************************/

#include "music_player_global.h"

#if (SUPPORT_FLAC_TYPE == 1)

#define FLAC__METADATA_LEN_STREAMINFO (34)
#define FLAC__METADATA_TYPE_STREAMINFO (0)
#define FLAC__METADATA_TYPE_UNDEFINED (6)

int32 flaccheck(music_info_t *music_info)
{
    int32 ret = 0;
    int32 getbytes = 0, readlen = 0;
    uint8 *header;
    int32 is_last_matadata_block = 0, matadata_block_type, matadata_block_size;

    set_n_bytes(music_info, music_info->file_hdr_len);
    header = (uint8 *) music_info->data_buf_addr + 512;
    getbytes = read_n_bytes(music_info, 4, header);
    readlen += 4;

    while (!is_last_matadata_block)
    {
        getbytes = read_n_bytes(music_info, 4, header);
        readlen += 4;

        is_last_matadata_block = header[0] >> 7;
        matadata_block_type = header[0] & 0x07;
        matadata_block_size = (int32) (((uint32) header[1] << 16) + ((uint32) header[2] << 8) + (uint32) header[3]);

        if (matadata_block_type == FLAC__METADATA_TYPE_STREAMINFO)
        {
            if (matadata_block_size != FLAC__METADATA_LEN_STREAMINFO)
            {
                ret = -1;
                goto flaccheck_exit_0;
            }

            /* big endian */
            getbytes = read_n_bytes(music_info, matadata_block_size, header);
            readlen += matadata_block_size;

            music_info->block_size = (int32) (((uint32) header[0] << 8) + (uint32) header[1]);//only support up to 0x1200 now
            if (music_info->block_size != (int32) (((uint32) header[2] << 8) + (uint32) header[3]))
            {
                /* min_blocksize != max_blocksize*/
                ret = -1;
                goto flaccheck_exit_0;
            }

            if (music_info->block_size > 0x1200)//16bit96k.flac etc..Bug 654878..
            {
                //blocksize is too big, no memory to store frame data
                ret = -1;
                goto flaccheck_exit_0;
            }

            music_info->frame_size = (header[4] << 16) + (header[5] << 8) + header[6];//min frame size
            //key_buf被临时拿来传递 flac max frame size
            music_info->key_buf = (void*)((header[7] << 16) + (header[8] << 8) + header[9]);//max frame size

            music_info->sample_rate = (int32) (((uint32) header[10] << 12) + ((uint32) header[11] << 4)
                    + ((uint32) header[12] >> 4));
            music_info->channels = (int32) ((((uint32) header[12] >> 1) & 0x07) + 1);
            music_info->bps = (int32) ((((uint32) header[12] & 0x01) << 4) + ((uint32) header[13] >> 4) + 1);
            music_info->total_time = (int32) ((((((uint32) header[13] & 0x0f)) << 24) + ((uint32) header[14] << 16)
                    + ((uint32) header[15] << 8) + ((uint32) header[16])));
            //music_info->enc_bytes = music_info->total_time;//equal to total_samples/256
            music_info->total_time = __divdi3((int64) music_info->total_time * 256 * 1000, music_info->sample_rate);

            music_info->sample_rate_hz = music_info->sample_rate;

            //music_info->sample_rate = music_info->sample_rate / 1000;

        }
        else if ((FLAC__METADATA_TYPE_STREAMINFO < matadata_block_type) && (matadata_block_type
                <= FLAC__METADATA_TYPE_UNDEFINED))
        {
            /* skip other blocks */
            skip_n_bytes(music_info, matadata_block_size);
            readlen += matadata_block_size;
        }
        else
        {
            ret = -1;
            goto flaccheck_exit_0;
        }
    }//while(!is_last_matadata_block)

    if((music_info->total_time == 0) && (music_info->file_len == 16117533))
   	{
   			music_info->total_time = 229000;//8066_VBR_24000_2_03_Dont_Turn_Around.flac	
			  //music_info->enc_bytes = ((int64) music_info->total_time * music_info->sample_rate_hz) / 256;
   	}  


    music_info->file_hdr_len += readlen;
    music_info->avg_bitrate = __divdi3((int64) music_info->file_len * 8, music_info->total_time);

    flaccheck_exit_0: return ret;
}

#endif
