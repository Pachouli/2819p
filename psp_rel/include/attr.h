/********************************************************************************
 *                              USDK(GL5110)
 *                            Module: KERNEL
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>      <time>           <version >             <desc>
 *      wuyueqian     2011-9-10 09:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     attr.h
 * \brief    ������Ϣ
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/
#ifndef _AP_BANK_DATA_ATTR_H
#define _AP_BANK_DATA_ATTR_H

/**
 ���ɵ�COMMON/DATA/BSS�ζ���GLOBAL�ģ�ֻ��_BANK_DATA_ATTR_�����ȫ�ֱ�����RODATA����text��BANK��
 ��Ϊstatic����bss�Σ�����ʵ������bank�ģ����Բ�֪�����ĺã�������ֻ����һ��bank�ã��ǾͲ�Ҫ��static
 δ��ʼ���ı�����ʱ������common�Σ���ʱ����bss�Σ�һ����common�Σ�����ȷ��������ȫ���鵽GLOBAL�ģ����������ļ�����д��
 */

#ifndef _ASSEMBLER_
#ifndef PC
//#define _BANK_DATA_ATTR_ __attribute__((section(".xdata")))
#define __section__(n) __attribute__((section (n)))
#define __FAR__ __attribute__( (far) )
#define _GLOBAL_DATA_ATTR_      //data
#define _GLOBAL_DATA_REUSED_ATTR_ __attribute__((section("reused_data")))     //˫������data
//#define static _BANK_DATA_ATTR_   //���Ƽ�ʹ��static
#else
#define _BANK_DATA_ATTR_
#define __section__(n)
#define __FAR__
#define _GLOBAL_DATA_ATTR_      //data
#define _GLOBAL_DATA_REUSED_ATTR_
#endif

#else

#define _BANK_DATA_ATTR_ .section bank_data
#define _GLOBAL_DATA_ATTR_ .data
#define _RDATA_ATTR_ .section .rodata //asm default is .rdata
#define _GLOBAL_DATA_REUSED_ATTR_ .section reused_data

#endif /*_ASSEMBLER_*/

#endif
