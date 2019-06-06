#ifndef H_COM_ENGINE
#define H_COM_ENGINE

#ifndef NULL
#define NULL 0
#endif

#define IDSEEK_SET  0
#define IDSEEK_CUR  1
#define IDSEEK_END  2

typedef enum{
	EN_NOSUPPORT = -5,
	EN_DECERR,
	EN_FILEISEND,
	EN_FILESTARTPOS,
	EN_MEMERR,
	EN_NORMAL,
}rt_status_t;

typedef struct os_input
{
    int (*open)(struct os_input *disk_input, void *filename, int drvindex);
    int (*close)(struct os_input *disk_input);
    void *inputhandle;
}os_input_t;

typedef struct raw_input
{
	int (*read)(struct raw_input *raw, unsigned int buf, unsigned int len);
	int (*seek)(struct raw_input *raw, int offset, int original);
	int (*tell)(struct raw_input *raw);	
	int (*get_offset)(struct raw_input *raw);	//return valid stream offset of 512bytes
	int (*file_read)(unsigned char *buf, unsigned int len);	//call file system
	int  data_offset;
}raw_input_t;

void *en_malloc(int n);
void en_free(void *buf);
void en_memset(void *buf, int val, int len);
void en_memcpy(char *des,const char *src, int len);
void ie_file_input_open(struct os_input *file_input);
void ie_creat_disk_input(struct raw_input *input);
#endif
