
#ifndef UI30_STRUCT_H
#define UI30_STRUCT_H

#include "psp_includes.h"

//resource type
#define   RES_TYPE_PIC        1
#define   RES_TYPE_STRING     2
#define   RES_TYPE_MSTRING    3
#define   RES_TYPE_LOGO       4

/*资源头的数据结构*/
#define   RESHEADITEM     16 	//各个entry长度，统一为16uint8s
#define   GROUPDEFINE     4
#define   ROW_COUNT_DEF   4
#define   TYPE_DIR        0
#define   TYPE_FILE       1

/*  resource file chart:

 res_head * 1        (16 bytes)
 res_entry * n        (16 bytes)
 res_data *n            <--single res_data does not over bank

 */
typedef struct
{
    char   magic[4];      //'R', 'U', '2', 0x19      
    uint16 counts;        //资源的个数
}res_head_t;  //6 bytes

/*资源类型索引表的数据结构*/
typedef struct
{
    uint32 dwOffset;     //资源内容索引表的偏移  
    uint16 wCount;       //资源类型总数
    uint8  bItemType;    //'P'--PIC Table,'S'--String Table,'X' -- XML File
    uint8  type; 
}res_entry_t;

/*资源内容信息索引的数据结构*/
typedef struct 
{
    uint32 dwOffset;         //图片数据区在文件内偏移,4 bytes
    uint32 dwLength;         //资源长度, 最大 4G，4 bytes
    uint8  bType;            //资源类型,0x01--language string ,0x02--PIC  
    uint8  wWidth[2];        //若是图片，则代表图片宽，若是字符串，则代表ID总数
    uint8  wHeight[2];       //若是图片，则代表图片长，若是字符串，则代表该语言的ID.
}res_infor_t;                //13 bytes

/*多国语言资源ID索引表的数据结构*/
typedef struct 
{
    uint16 dwOffset[2];       // 字符ID号对应字符串编码在文件内的偏移
    uint16 dwLength;          // 字符串长度.即unicode编码字符串的字节数
}res_langid_entry_t;          // 6 bytes  

#endif  /* UI30_STRUCT_H*/

