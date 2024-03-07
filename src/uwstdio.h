#ifndef __UWSTR_STDIO_H__
#define __UWSTR_STDIO_H__

#include <stdio.h>

/* Standard IO adapters for uwstr */

/** @brief Append an entire line into a uws
 *  @param uws_dest Destination buffer
 *  @param stream
 *  @return Number of bytes read from the stream.
 *  uws_dest becomes a wynnstr under the following conditions:
 *  -The line does NOT fit into the wynnstr
 *  -The line contains invalid UTF-8 data
 *  -IO Error or stream is NULL
 *  Otherwise, all code unit data including the '\n' code unit are read
 *  into uws_dest. */
size_t uws_catline(char* uws_dest, FILE* stream);

/** @brief Append formatted data into a uws
 *  @param uws_dest
 *  @param fmt Usual printf format string
 *  @return Number of bytes written to uws_dest
 *  uws_dest becomes a wynnstr under the following conditions:
 *  -The output does not fit into uws_dest
 *  -The output contained invalid UTF-8 formatting */
size_t uws_catsprintf(char* uws_dest, const char* fmt, ...);

/** @brief Make a minimal (capacity == actual length) copy of src.
 *  calls malloc to allocate
 *  @param uws_src Source uws string
 *  @return NULL on malloc error 
 *  return value is a minimal wynnstr if uws_src is a wynnstr
 *  Use free() to free result pointer */
char* uws_mindup(const char* uws_src);

#endif
