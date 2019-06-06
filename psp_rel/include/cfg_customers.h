/********************************************************************************
 *
 * History:
 *	   <author> 	 <time> 		  <version >			 <desc>
 *	   chenjunhong	 	                         1.0			 build this file
 ********************************************************************************/
/*!
 * \file 	cfg_customers.h
 * \brief	cfg_customers.bin
 * \author	chenjunhong
 * \version	1.0
 * \date  2018/8/11
 *******************************************************************************/

#ifndef __CFG_CUSTOMERS_H__
#define __CFG_CUSTOMERS_H__

#include <typeext.h>
#include <ucos/init.h>

/******************************************************************Customer define in here*********************************************************/
#define CUSTOMER_EVB_ATS2819		0
#define CUSTOMER_EVB_ATS2819P		1

#define CUSTOMER_BOARD_TYPE CUSTOMER_EVB_ATS2819P

#define MODEL0 "Demo"
#define MODEL1 "Pre-ES"
#define MODEL2 "ES"
#define MODEL3 "EVT"
#define MODEL4 "DVT"
#define MODEL5 "PVT"
#define MODEL6 "MP"

#if (CUSTOMER_BOARD_TYPE == CUSTOMER_EVB_ATS2819P)
#define NAME "HY/ATS2819"
#define MODEL MODEL0
#define VER_MAJR 0
#define VER_MIN1 4
#define VER_MIN2 0
#elif (CUSTOMER_BOARD_TYPE == CUSTOMER_EVB_ATS2819P)
#define NAME "HY/ATS2819P"
#define MODEL MODEL0
#define VER_MAJR 0
#define VER_MIN1 4
#define VER_MIN2 0
#endif

#endif
