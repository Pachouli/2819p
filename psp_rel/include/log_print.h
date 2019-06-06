/*!
 * \file  log_print.h
 * \brief ��־��ӡ���
 */

#ifndef _LOG_PRINT_H
#define _LOG_PRINT_H


#include "libc_interface.h"


/* �ּ���ӡ
 */
#define PRINT_LEVEL_ERROR    1
#define PRINT_LEVEL_WARNING  2
#define PRINT_LEVEL_INFO     3
#define PRINT_LEVEL_DEBUG    4


#ifndef PRINT_LEVEL
#define PRINT_LEVEL  PRINT_LEVEL_DEBUG
#endif


/* �ּ���ӡ: ����
 */
#if (PRINT_LEVEL >= PRINT_LEVEL_ERROR)

#define print_error(_x...)  libc_printf(_x)

#define log_error(_format, ...)                                    \
do                                                                 \
{                                                                  \
    const char*  _str = "%s%s%s"_format"\n";                       \
    const char*  _sep = (_str[6] == '\n') ? "" : ", ";             \
                                                                   \
    print_error(_str, "<E> ", __FUNCTION__, _sep, ##__VA_ARGS__);  \
}                                                                  \
while (0)

#else
    #define print_error(_x...)  do {} while (0)
    #define log_error(_x...)    do {} while (0)
#endif


/* �ּ���ӡ: ����
 */
#if (PRINT_LEVEL >= PRINT_LEVEL_WARNING)

#define print_warning(_x...)  libc_printf(_x)

#define log_warning(_format, ...)                                    \
do                                                                   \
{                                                                    \
    const char*  _str = "%s%s%s"_format"\n";                         \
    const char*  _sep = (_str[6] == '\n') ? "" : ", ";               \
                                                                     \
    print_warning(_str, "<W> ", __FUNCTION__, _sep, ##__VA_ARGS__);  \
}                                                                    \
while (0)

#else
    #define print_warning(_x...)  do {} while (0)
    #define log_warning(_x...)    do {} while (0)
#endif


/* �ּ���ӡ: ��Ϣ
 */
#if (PRINT_LEVEL >= PRINT_LEVEL_INFO)

#define print_info(_x...)  libc_printf(_x)

#define log_info(_format, ...)                                    \
do                                                                \
{                                                                 \
    const char*  _str = "%s%s%s"_format"\n";                      \
    const char*  _sep = (_str[6] == '\n') ? "" : ", ";            \
                                                                  \
    print_info(_str, "<I> ", __FUNCTION__, _sep, ##__VA_ARGS__);  \
}                                                                 \
while (0)

#else
    #define print_info(_x...)  do {} while (0)
    #define log_info(_x...)    do {} while (0)
#endif


/* �ּ���ӡ: ����
 */
#if (PRINT_LEVEL >= PRINT_LEVEL_DEBUG)

#define print_debug(_x...)  libc_printf(_x)

#define log_debug(_format, ...)                                    \
do                                                                 \
{                                                                  \
    const char*  _str = "%s%s%s"_format"\n";                       \
    const char*  _sep = (_str[6] == '\n') ? "" : ", ";             \
                                                                   \
    print_debug(_str, "<D> ", __FUNCTION__, _sep, ##__VA_ARGS__);  \
}                                                                  \
while (0)

#else
    #define print_debug(_x...)  do {} while (0)
    #define log_debug(_x...)    do {} while (0)
#endif


#endif  // _LOG_PRINT_H


