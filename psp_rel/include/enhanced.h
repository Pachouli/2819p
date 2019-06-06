/*******************************************************************************
 *                              US281A
 *                            Module: ENHANCED
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/

#ifndef  _ENHACED_H
#define  _ENHACED_H

#include <typeext.h>
#include <api.h>
#include <driver_manager.h>
#include <kernel_interface.h>
#include <vfs_interface.h>
#include <libc_interface.h>
#include <res_fs.h>
#include <hal_interface.h>

//c宏定义

//是否支持shuffle循环模式 TODO shuffle功能未开发完善
//#define SUPPORT_SHUFFLE_MODE

//是否过滤record目录，music正常播放不扫描该目录文件
#define FILTER_RECORD_DIR

//是否通过前台music获取ID3, for SPP
#define ID3_SUPPORT

//是否通过music后台获取目录名
//#define DIR_NAME_SUPPORT

#define  ID3_ITEM_SIZE    28    //ID3信息长度
#define  DIRLAYER_MAX     9     //目录深度,root + 8级子目录
#define  EH_SECTOR_SIZE   512   //读写扇区大小
#define  USERPL_TITLE_MAX  (ID3_ITEM_SIZE * 2)  //收藏夹显示标题

//#ifdef ID3_SUPPORT

//id3文件名 buffer长度
#define Id3BufSIZE 92

//id3专辑/歌手buffer长度
#define ArtBufSIZE 52

#define PLIST_INFO_ADDR  0x9fc37000

#define PLIST_INDEX_ADDR 0x9fc38000
//end
//ID3获取支持类型
typedef enum
{
    ID3_TYPE_AA = 0,
    ID3_TYPE_AAC,
    ID3_TYPE_AAX,
    ID3_TYPE_APE,
    ID3_TYPE_FLAC,
    ID3_TYPE_MP3,
    ID3_TYPE_OGG,
    ID3_TYPE_WMA,
    ID3_TYPE_END
} id3_type_e;

//获取ID3的信息结构
typedef struct
{
    uint8 *tit2_buffer; //待存文件标题buffer
    uint8 *tpe1_buffer; //待存作者buffer
    uint8 *talb_buffer; //待存专辑buffer
    uint8 *genre_buffer; //待存流派buffer
    uint8 *drm_buffer; //待存drm的buffer
    uint8 *track_buffer; //待存音轨buffer
    uint8 tit2_length; //文件标题长度
    uint8 tpe1_length; //作者长度
    uint8 talb_length; //专辑长度
    uint8 genre_length; //流派长度
    uint8 drm_length; //drm长度
    uint8 track_length; //音轨长度
    uint8 apic_type; // 0-无图片，1-jpeg
    uint8 apic_flag; //获取图片偏移标记
    uint8 ldb_flag; //获取ldb歌词偏移标记
    uint8 reserve; //保留对齐
    uint16 track_num; //音轨号
    uint32 ldb_offset; //ldb歌词在文件中的字节偏移
    uint32 apic_offset; //apic图片在文件中的字节偏移
} id3_info_t;

/**********************************
 (6)id3信息获取
 目录：\id3
 **********************************/
extern bool fsel_get_id3_info(id3_info_t *id3_info, const char *filename, id3_type_e file_type)
__FAR__;

//#endif //ID3_SUPPORT

//文件选择器属性
typedef enum
{
    FSEL_MODULE_DIR = 0,
    FSEL_MODULE_PLIST,
} fsel_module_e;

//文件选择器选择类型
typedef enum
{
    FSEL_TYPE_COMMONDIR = 0, //普通目录播放
    FSEL_TYPE_LISTDIR, //目录列表形式
    FSEL_TYPE_FAVORITE, //收藏夹模式播放
    FSEL_TYPE_SDFILE, //sd区文件
    FSEL_TYPE_RESFILE,//res区文件
} fsel_type_e;

//播放模式
typedef enum
{
    FSEL_MODE_LOOPALL = 0, //循环所有
    FSEL_MODE_LOOPONE, //循环放当前首
    FSEL_MODE_LOOPONEDIR, //循环放当前目录
    FSEL_MODE_RANOM, //随机播放(shuffle + loop all)
    FSEL_MODE_NORMAL, //普通播放
    FSEL_MODE_DIRNOR, //顺序播放目录
    FSEL_MODE_INTRO, //浏览播放
} fsel_mode_e;

//接口调试错误类型
typedef enum
{
    FSEL_NO_ERR = 0, //没有报错
    FSEL_ERR_OUTOF, //选择溢出
    FSEL_ERR_EMPTY, //目录空报错
    FSEL_ERR_SETDIR, //操作目录路径出错
    FSEL_ERR_SETFILE, //操作文件信息出错
    FSEL_ERR_EXCEPTION, //异常有误
    FSEL_ERR_FAIL, //接口操作失败
    FSEL_ERR_DISK, //盘符出错
    FSEL_ERR_EXIST, //文件已存在
    FSEL_ERR_NO_EXIST, //文件不存在
    FSEL_ERR_USING, //文件在使用中
    FSEL_ERR_STG, //安装驱动失败
    FSEL_ERR_FS, //文件系统接口报错
    FSEL_ERR_LIST_NOEXIST, //收藏夹或播放列表不存在
    FSEL_ERR_SHUFFLE, //shuffle机制出错
    FSEL_ERR_INPUT //输入参数有误
} fsel_error_e;

//ENHANCED 初始化参数
typedef struct
{
    uint8 fsel_type; //选择类型      fsel_type_e ；最高位表示是否播放录音目录，1表示不过滤录音目录，需要播放录音目录，0表示过滤录音目录
    uint8 fsel_mode; //播放模式      fsel_mode_e
    uint8 disk; //DISK_C;DISK_H;DISK_U
    uint8 mode;//不同的目录表示不同的模式
    uint32 file_type_bit; //文件后缀名bitmap
    uint8* dir1;
    uint8* dir2;
} fsel_param_t;

//目录下的文件信息
typedef struct
{
    uint8 disk; //DISK_C;DISK_H;DISK_U
    uint8 dir_layer; //当前目录层次，从0开始，0表示根目录
    uint16 resever; //保留对齐
    uint8 filename[4]; //文件的后缀名，全填0代表文件信息为空
    uint32 cluster_no; //文件的目录项所在的簇号
    uint32 dir_entry; //文件的目录项在所在簇号内的偏移
    pdir_layer_t dir_layer_info; //目录层次信息
    uint8 res_3byte[3]; //保留对齐
    uint16 file_total; //当前目录文件总数
    uint16 file_total_disk;//当前磁盘文件总数
    uint16 file_num; //当前文件在目录中的序号
} file_location_t;

//播放列表下的文件信息
typedef struct
{
    uint8 disk; //DISK_C;DISK_H;DISK_U
    uint8 list_layer; //列表层数
    uint16 list_type; //子表类型 plist_type_e
    uint8 filename[4]; //文件的后缀名，全填0代表文件信息为空
    uint32 cluster_no; //文件的目录项所在的簇号
    uint32 dir_entry; //文件的目录项在所在簇号内的偏移
    pdir_layer_t dir_layer_info; //目录层次信息
    uint8 res_3byte[3]; //保留对齐
    uint16 file_total; //当前文件总数 (在当前列表下文件层的总数)
    uint16 file_total_disk;//当前磁盘文件总数
    uint16 file_num; //当前文件序号 (在当前列表下文件层的序号)
    uint16 file_index; //当前文件索引号（在数据区的序号）
    uint16 reserved; //保留对齐
    uint16 list_layer_no[4]; //记录文件所在的每层列表中的位置，（分别记录在每层排序后的位置）
    uint16 layer_list_offset; //list偏移位置
    uint16 reserve; //保留对齐

} plist_location_t;

//收藏夹下的文件信息
typedef struct
{
    uint8 disk; //'DISK_C;DISK_H;DISK_U
    uint8 reserved; //保留对齐
    uint16 file_index; //no used
    uint8 filename[4]; //文件的后缀名，全填0代表文件信息为空
    uint32 cluster_no; //文件的目录项所在的簇号
    uint32 dir_entry; //文件的目录项在所在簇号内的偏移
    pdir_layer_t dir_layer_info; //目录层次信息
    uint8 res_3byte[3]; //保留对齐
    uint16 file_total; //当前收藏夹文件总数
    uint16 file_total_disk;//当前磁盘文件总数
    uint16 file_num; //当前文件在收藏夹中的序号
    uint8 title[USERPL_TITLE_MAX]; // 收藏夹显示的文件标题
} flist_location_t;

//文件信息联合体
typedef union
{
    file_location_t dirlocation; //目录模式下的文件信息
    plist_location_t plist_location; //播放列表模式下的文件信息
    flist_location_t flist_location; //收藏夹文模式下的文件信息
} musfile_location_u;

//文件信息
typedef struct
{
    uint8 file_source; //文件信息类型：目录、播放列表和收藏夹
    uint8 reserved[3]; //保留对齐
    musfile_location_u file_path; //文件信息结构
} file_path_info_t;


// 文件浏览时枚举输出目录项名称缓冲最大长度
#define MAX_DIRITEM_NAME_LEN    128

//输出目录项描述
typedef struct _DIRITEM
{
    uint8 attr;      //目录项属性, 参见vfs_interface.h中的"文件属性"定义
    uint8 reserved;  //保留使用，对齐
    uint16 name_len; //输出文件名长度，字节单位
    uint8 name_buf[MAX_DIRITEM_NAME_LEN]; //目录名称缓冲
} DIRITEM;

//ENHANCED 初始化参数
typedef struct
{
    uint8 fsel_type; //选择类型      fsel_type_e ；最高位表示是否播放录音目录，1表示不过滤录音目录，需要播放录音目录，0表示过滤录音目录
    uint8 disk; //DISK_C;DISK_H;DISK_U
    uint32 file_type_bit; //文件后缀名bitmap
    uint32 dir_type_bit; //目录后缀名bitmap
} fsel_enum_param_t;

/********************************************************************************
 *            对外提供的接口列表
 *********************************************************************************/

/**********************************
 (1)模块系统接口
 目录：\COMMON
 **********************************/
extern fsel_error_e fsel_get_error(void)
__FAR__;
extern uint32 fsel_init(fsel_param_t *init_param, uint8 drv_mode)
__FAR__;
extern bool fsel_exit(void)
__FAR__;

/**********************************
 (3)磁盘/播放列表/收藏夹 文件定位播放接口
 目录：\fsel
 **********************************/
extern bool fsel_set_mode(uint8 mode)
__FAR__;

extern bool fsel_set_seltype(fsel_type_e type, uint16 cur_num)
__FAR__;

extern bool fsel_set_typebit(uint32 file_type_bit, bool reset_dir_flag)
__FAR__;

extern bool fsel_get_location(void *location)
__FAR__;

extern bool fsel_set_location(void *location)
__FAR__;

extern bool fsel_get_nextfile(char *strfile)
__FAR__;

extern bool fsel_get_prevfile(char *strfile)
__FAR__;

extern bool fsel_get_byno(char *strfile, uint16 num)
__FAR__;

extern uint16 fsel_get_total(void)
__FAR__;

extern uint16 fsel_get_disk_total(void)
__FAR__;

extern uint16 fsel_get_fileno(void)
__FAR__;

extern void fsel_set_fileno(uint16 set_num)
__FAR__;

extern uint16 fsel_total_file_cur(uint8* dir_info, uint8 check_flag)
__FAR__;

//以下为目录播放时，定位目录播放接口(预留)
extern bool fsel_get_nextdir (uint8 *strfile) __FAR__;

extern bool fsel_get_prevdir(uint8 *strfile) __FAR__;

extern bool fsel_enter_dir(uint8 *name_short, uint8* name_long, uint8 dir_direct)
__FAR__;

extern bool fsel_enter_favorite(uint8 *filename_short,uint8* filename_long) __FAR__;

extern HANDLE lyric_open(char * music_filename, uint32* file_len)
__FAR__;
//获取目录名（在应用中，定位到该文件之后，如果要获取该文件所在的目录的目录名）
//将该目录的location指针、存放目录名的buffer指针、buffer长度作为参数传递
extern bool fsel_get_dir_name(uint8 *strdir, uint8 name_len, file_location_t *location)
__FAR__;

//设置播放列表更新标志
extern bool fsel_set_plist_update(void)
__FAR__;

extern int32 get_disk_total_num(void)__FAR__;

extern bool get_current_file_name(uint8 index,uint8 *filename)__FAR__;

extern bool get_current_file_num(uint8*in_filename,uint8 *out_index)__FAR__;

extern bool get_next_file(uint8*filename)__FAR__;

extern bool get_prev_file(uint8*filename)__FAR__;

extern bool disk_c_fs_init(void)__FAR__;


// 文件遍历功能导出应用接口
extern int fsel_enum_dir_set(uint32 vfs_mount_hdl, uint8 *dir_name, uint16 path_len) __FAR__;
extern int fsel_enum_dir_first(uint32 vfs_mount_hdl, uint32 file_type_mask, DIRITEM *p_dir_item) __FAR__;
extern int fsel_enum_dir_last(uint32 vfs_mount_hdl, uint32 file_type_mask, DIRITEM *p_dir_item) __FAR__;
extern int fsel_enum_dir_prev(uint32 vfs_mount_hdl, uint32 file_type_mask, DIRITEM *p_dir_item) __FAR__;
extern int fsel_enum_dir_next(uint32 vfs_mount_hdl, uint32 file_type_mask, DIRITEM *p_dir_item) __FAR__;

extern int fsel_get_file_location(uint32 vfs_mount_hdl, file_location_t *p_file_loc) __FAR__;

extern uint32 fsel_vfs_init(fsel_enum_param_t *p_enum_para) __FAR__;
extern int fsel_vfs_deinit(uint32 vfs_mount_hdl) __FAR__;

extern uint16 extlib_get_disk_total_num(void);
extern bool extlib_get_current_file_name(uint16 num, uint8 *filename);
extern bool extlib_get_current_file_num(uint8*in_filename, uint16 *out_index);
extern bool extlib_get_next_file(uint8*filename);
extern bool extlib_get_prev_file(uint8*filename);
extern bool extlib_disk_c_fs_init(void);

#endif//#ifndef  _ENHACED_H
