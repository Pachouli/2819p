/********************************************************************************
*                              USDK(1100)
*                            Module: REGDEF
*                 Copyright(c) 2003-2008 Actions Semiconductor,
*                            All Rights Reserved. 
*
* History:         
*      <author>    <time>           <version >             <desc>
*      wuyueqian     2011-9-10 15:00     1.0             build this file 
********************************************************************************/
/*!
* \file     regdef.h
* \brief    MIPS通用寄存器名定义
* \author   SD
* \version 1.0
* \date   2011-9-10
*******************************************************************************/
/*regdef.h : MIPS Rx000 symbolic register names for assembler*/
#ifndef __REGDEF_H__
#define __REGDEF_H__

#define zero    $0

#define AT	$1

#define v0 	$2
#define v1	$3

#define a0	$4
#define a1	$5
#define a2	$6
#define a3	$7

#if #abi(n32) || #abi(n64) || #abi(64) || #abi(eabi)
#define a4	$8
#define a5	$9
#define a6	$10
#define a7	$11
#define t0  	$12
#define t1  	$13
#define t2  	$14
#define t3  	$15
#define ta0	$8	/* alias for $a4 */
#define ta1	$9	/* alias for $a5 */
#define ta2	$10	/* alias for $a6 */
#define ta3	$11	/* alias for $a7 */
#else
#define t0  	$8
#define t1	        $9
#define t2	        $10
#define t3	        $11
#define t4  	$12
#define t5	        $13
#define t6	        $14
#define t7	        $15
#define ta0	$12	/* alias for $t4 */
#define ta1	$13	/* alias for $t5 */
#define ta2	$14	/* alias for $t6 */
#define ta3	$15	/* alias for $t7 */
#endif

#define s0	$16
#define s1	$17
#define s2	$18
#define s3	$19
#define s4	$20
#define s5	$21
#define s6	$22
#define s7	$23
#define s8	$30		/* == fp */

#define t8  	$24
#define t9  	$25
#define k0	$26
#define k1	$27

#define gp	$28

#define sp	$29
#define fp	        $30
#define ra	        $31

#endif
