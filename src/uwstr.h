#ifndef __UWSTR_H__
#define __UWSTR_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* By default, don't do anything fancy. */
#ifndef __UWSTR_CONFORMANCE_LEVEL
#define __UWSTR_CONFORMANCE_LEVEL 0
#endif

#include "internal.h"

/* There are the following types of uws strings
 * -Empty string: a string of length 0
 * -Invalidated string: A string result that does not contain valid data
 * */

/* Initialization. */


/** @brief Calculate buffer size required to store X number of content characters
 *  @param capacity Maximum number of content characters.
 *  @return total allocation size. */
size_t uws_alloc_size(size_t capacity);

/** @brief Initialize a raw buffer to an empty uws string.
 *  @param buffer pointer to raw data
 *  @param bufflen length of the buffer
 *  @return uws string initialized to the empty string. */
void uws_init(char* buffer, size_t bufflen);

/** @brief Set a string to the empty string. */
void uws_empty(char* buffer);


/* Properties. */


/** @brief Whether uws string is invalid. */
bool uws_invalid(const char* buffer);

/** @brief Get length of a uws string.
 *  @param uws Must point to string initialized by uws_init
 *  @return Number of bytes in the string. Invalid strings will return 1. */
size_t uws_len(const char* uws);

/** @brief Number of characters in a uws string.
 *  @param uws Must point to string initialized by uws_init
 *  @return 0 Number of bytes in the string. Invalid strings will return 1. */
size_t uws_cnt(const char* uws);

/** @brief How many bytes are still available in the buffer.
 *  @param uws Must point to string initialized by uws_init
 *  @return 0 if not a valid uws string. */
size_t uws_avail(const char* uws);



/* Manipulation. */


/** @brief Concatenate 2 uws strings.
 *  @return Total length of new string. If there was not enough space in uws, then
 *  then uws_dest is the invalid string. */
size_t uws_cat(char* uws_dest, const char* uws_src);


/* Comparison. */


/** @brief Compare 2 uws strings.
 *  @return follows strcmp semantics.
 *  If either a or b is undefined, the return value is undefined but never 0. */
int uws_cmp(const char* uws_a, const char* uws_b);


/* C style interfacing. */


/** @brief Get expected null terminated C string from uws string.
 *  @return pointer to invalid UTF-8 if string is not valid; valid ptr otherwise. */
const char* uws_c(const char* uws);

/** @brief Concatenate a VALIDATED UTF-8 C string.
 *  @return Total length of new string. If there was not enough space in uws
 *  then uws_dest is the invalid string. */
size_t uws_ccat(char* uws_dest, const char* c_src);


#endif
