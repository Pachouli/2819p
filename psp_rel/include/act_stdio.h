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
 * \file     act_stdio.h
 * \brief    ���Ͷ���
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/
#ifndef _ACT_STDIO_H
#define _ACT_STDIO_H

#include <ucos_types.h>

#ifndef _ASSEMBLER_

//�����ļ�SEEK�ķ�ʽ
#define SEEK_SET    0   // ���ļ������ļ�β��λ��offsetΪ����
#define SEEK_CUR    1   //�ӵ�ǰλ�����ļ�ͷ��β��λ��������ʾ���ļ�βseek��������ʾ���ļ�ͷseek
#define SEEK_END    2   // ���ļ�β���ļ��׶�λ��offsetΪ����

#endif/* _ASSEMBLER_ */

#endif  /* _ACT_STDIO_H*/
