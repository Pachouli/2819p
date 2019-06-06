#ifndef H_VP_ENGINE
#define H_VP_ENGINE

#ifdef __cplusplus
extern "C" {
#endif

#include "com_engine.h"

typedef struct
{
    char video[8];
    unsigned int frame_rate;
    unsigned int width;
    unsigned int height;
    unsigned int video_bitrate;
    unsigned int vtotal_time;
}ve_video_info_t;

typedef struct
{
	char audio[8];
    unsigned int sample_rate;
    unsigned int audio_bitrate;
    unsigned int channels;
    unsigned int atotal_time;
}ve_audio_info_t;

typedef enum{
	GET_CONT_INFO,
	FAST_FORWORD,
	FAST_BACK,
}seek_cmd_t;

typedef struct
{
	seek_cmd_t    seek_cmd;
	unsigned int  curpos;
    unsigned int  curframes;
    unsigned int  curtime; 
	unsigned char *searchbuf;
}seek_info_t;

typedef struct
{
	ve_video_info_t video_info;
	ve_audio_info_t audio_info;
	int index_flag;
}ve_media_info_t;

#define MAX_PACKET_LEN 2048

typedef struct{
	int w;
	int h;
}image_para_t;

typedef enum{
	mjpeg_amv = 0,	
	mjpeg_avi 	
}jpegdib_type_t;

typedef struct
{
	unsigned char *data;
	unsigned int data_len;
	void *stmread;
	void *iooffset;	
	void *fsread;
	jpegdib_type_t dibtype;
	image_para_t src_para;
	int offset_h;
	int valid_h;
	image_para_t out_para;	
}av_buf_t;

typedef enum{
	VIDEO_ES = 0,
	AUDIO_ES,
	UNKOWN_ES,	
}packet_type_t;

typedef struct
{
	packet_type_t packet_type;
	unsigned int pts;
	unsigned char *data;
	unsigned int data_len;	
}demuxer_packet_t;

typedef struct
{
	char file_extension[8];
	void *(*open)(raw_input_t *praw_io, void *file_info);
	int (*seek)(void *vp_handle, seek_info_t *seek_info);
	int (*getframe)(void *vp_handle, demuxer_packet_t *packet_buf);
	int (*dispose)(void *vp_handle);	
}demuxer_plugin_t;

typedef struct
{
	char file_extension[8];
	void *(*open)();
	int (*decode)(void *vp_handle, av_buf_t *av_buf);
	int (*dispose)(void *vp_handle);	
}dec_plugin_t;

#ifdef __cplusplus
}
#endif

#endif