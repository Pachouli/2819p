
#ifndef UI30_STRUCT_H
#define UI30_STRUCT_H

#include "psp_includes.h"

//resource type
#define   RES_TYPE_PIC        1
#define   RES_TYPE_STRING     2
#define   RES_TYPE_MSTRING    3
#define   RES_TYPE_LOGO       4

/*��Դͷ�����ݽṹ*/
#define   RESHEADITEM     16 	//����entry���ȣ�ͳһΪ16uint8s
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
    uint16 counts;        //��Դ�ĸ���
}res_head_t;  //6 bytes

/*��Դ��������������ݽṹ*/
typedef struct
{
    uint32 dwOffset;     //��Դ�����������ƫ��  
    uint16 wCount;       //��Դ��������
    uint8  bItemType;    //'P'--PIC Table,'S'--String Table,'X' -- XML File
    uint8  type; 
}res_entry_t;

/*��Դ������Ϣ���������ݽṹ*/
typedef struct 
{
    uint32 dwOffset;         //ͼƬ���������ļ���ƫ��,4 bytes
    uint32 dwLength;         //��Դ����, ��� 4G��4 bytes
    uint8  bType;            //��Դ����,0x01--language string ,0x02--PIC  
    uint8  wWidth[2];        //����ͼƬ�������ͼƬ�������ַ����������ID����
    uint8  wHeight[2];       //����ͼƬ�������ͼƬ���������ַ��������������Ե�ID.
}res_infor_t;                //13 bytes

/*���������ԴID����������ݽṹ*/
typedef struct 
{
    uint16 dwOffset[2];       // �ַ�ID�Ŷ�Ӧ�ַ����������ļ��ڵ�ƫ��
    uint16 dwLength;          // �ַ�������.��unicode�����ַ������ֽ���
}res_langid_entry_t;          // 6 bytes  

#endif  /* UI30_STRUCT_H*/

