#ifndef H_IMAGE_ENGINE
#define H_IMAGE_ENGINE

#ifdef __cplusplus
extern "C" {
#endif

#include "com_engine.h"
#include "Simulator.h"

#define MAX_IMG_WIDTH 8192
#define MAX_IMG_HEIGHT 8192

#define MAXSIDELEN 110

#if defined AL_MMM_TEST || defined PC
unsigned char GIFFRAMEBUFFER[MAXSIDELEN*MAXSIDELEN*2];
#else
#define GIFFRAMEBUFFER ((void *)(0xBFC37000 - (MAXSIDELEN*MAXSIDELEN*2)))
#endif

typedef struct
{
	unsigned int width;
	unsigned int height;
	char   datetime[20];
   unsigned int autorota;
   unsigned short app_w;
   unsigned short app_h;
   unsigned short dec_w;
   unsigned short dec_h;
   char	album_flag;
   unsigned char delay;//1:jpeg ���������Ҫ��ʱ20ms 0:����Ҫ��ʱ
   // 0: �������¹����� --- �������Ĵ��ڳߴ��ԭͼ�ߴ���ȫһ��ʱ��ֱ�ӽ���ԭͼ��
   //    ���� ����Ƿ�������ͼ�� ���������ͼ�������Ƚ�����ͼ�� ������ͼ�����ԭͼ��
   // 1: �ϲ㴫��ָ���� ����ԭͼ�� ��������ͼ��
   unsigned char dec_non_thumbnail;
}img_file_info_t;

typedef struct
{
   unsigned int giffirstframe;
   unsigned int gifmultiframe;
   unsigned short x;
   unsigned short y;
   unsigned short app_w;
   unsigned short app_h;
   unsigned short dec_w;
   unsigned short dec_h;
   union{
   char *Y;
   char *RGB565;
   };
   char *U;   
   char *V;    
} img_dec_t;

typedef struct
{
	char file_extension[8];
	void *(*open)(raw_input_t *praw_io, void *file_info);
	int (*decode)(void *id_handle, img_dec_t *img_dec);
	int (*dispose)(void *id_handle);	
}imgdec_plugin_t;

#ifdef AL_MMM_TEST
#define MALLOCMEMORYSIZE 0x2000 
#else
#define MALLOCMEMORYSIZE 0x280 //640��С
#endif

#define SOFTDECSTART 0xBFC2C000	 //must more than ram5 + 0x2000 = 2C000
#ifndef PC
#define BMPOUTLINEBUFFER (0xBFC2E000 + 0x5000)//RAM6		//16k + 8k
#define GIFOUTLINEBUFFER 0xBFC34000 //RAM8      6k
#else
#define BMPOUTLINEBUFFER GET_REAL_ADDR(0xBFC2E000) //RAM6		//16k + 8k
#define GIFOUTLINEBUFFER GET_REAL_ADDR(0xBFC34000) //RAM8      6k
#endif

#ifdef _MAKE_ZERO_
#define BASEMMMADD 0xBFC2B600	//�ڴ����֮�󼴿�
#else
#define RAM5 0xBFC2A000
#define BASEMMMADD (0xBFC28000 - MALLOCMEMORYSIZE)	
#endif

#ifdef __cplusplus
}
#endif

#endif
