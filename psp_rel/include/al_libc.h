
#ifndef __AL_LIBC_H__
#define __AL_LIBC_H__

#include <typeext.h>
#include <ucos/init.h>
#include <attr.h>
#include <api.h>
#include <act_stdio.h>

/*!
 * \brief
 *      数据类型申明
 */
#ifdef WIN32
/* WIN32 */
#define __FAR__
#define __section__(n)
#define INLINE      __inline
#define TRUE            1
#define FALSE           0
typedef unsigned char   uint8;
typedef signed char     int8;
typedef unsigned short  uint16;
typedef signed short    int16;
typedef unsigned int    uint32;
typedef signed int      int32;
typedef unsigned long   ulong32;
typedef signed long     long32;
typedef __int64         int64;
typedef unsigned __int64 uint64;
#else
/* MIPS */
#define INLINE      inline
/* 补充的数据类型定义 */
typedef long long   int64;
typedef unsigned long long  uint64;
#endif

/*!
 * \brief
 *      ALSP类型定义，方便源用旧版代码。
 */
typedef int8 int8_t;
typedef int16 int16_t;
typedef int32 int32_t;
typedef int64 int64_t;
typedef uint64 uint64_t;

typedef struct
{
    void* (*al_libc_memcpy)(void * dest, const void *src, uint32 count);
    void* (*al_libc_memset)(void * s, int c, uint32 count);
    int32 (*al_libc_memcmp)(const void * cs, const void * ct, uint32 count);
    void  (*al_libc_printf)(const uint8 *fmt, ...);
    void  (*al_libc_printh)(uint8 *memory, uint32 length, uint32 option);
    void  (*al_libc_dump)(const uint8 *file, uint8 *memory, uint32 length);
} al_libc_funcs_t;

extern al_libc_funcs_t g_al_libc_funcs;

#define actal_memcpy (g_al_libc_funcs.al_libc_memcpy)
#define actal_memset (g_al_libc_funcs.al_libc_memset)
#define actal_memcmp (g_al_libc_funcs.al_libc_memcmp)
#define actal_printf (g_al_libc_funcs.al_libc_printf)
#define actal_printh (g_al_libc_funcs.al_libc_printh)
#define actal_dump   (g_al_libc_funcs.al_libc_dump)

extern int64 __divdi3(int64 num, int64 den) __FAR__;
extern uint64 __udivdi3(uint64 num, int64 den) __FAR__;

extern int32 mix_process(int32 input1, int32 input2) __FAR__;

typedef struct
{
    char *out;
    int sampleout;
    int totalsample;
} rs_audiout_pcm_t;

typedef struct
{
    int sampleout;
    int obuflen;
    int frac;
    short last[2];
    int total;
    int cnt;
    int current_cnt;
    rs_audiout_pcm_t audiout;
} resample_handle_t;

extern void DAE_func_88(resample_handle_t *handle, int fs_in, int fs_out);
extern int DAE_func_116(resample_handle_t *handle, void* src_addr, int src_len, void* dst_src, int* dst_len, int channels);

#define resample       DAE_func_116
#define resample_reset DAE_func_88

#endif //__AL_LIBC_H__
