#ifndef __UWSTR_H__
#define __UWSTR_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* By default, don't do anything fancy. */
#ifndef __UWSTR_CONFORMANCE_LEVEL
#define __UWSTR_CONFORMANCE_LEVEL 0
#endif

#define __UWSTR_INTERNAL_H__
#include "internal.h"
#undef __UWSTR_INTERNAL_H__

/* There are the following types of uws strings
 * -Empty string: a string of length 0
 * -Invalidated string: A string result that does not contain valid data
 * */

/* Initialization. */

#ifdef _THIS_IS_JUST_FOR_DEMIOING_MACROS_
/* There are helper macros for eloquently instantiating uwstr
 *
 * Each macro takes one of 2 forms:
 *
 * uwstrBU(ATTR, VAR, N)
 * const_uwstrBU(ATTR, VAR, STR)
 *
 * where
 *
 *  B is the string length in bytes, epsilon, 2, 4 (epsilon signifies 1 byte)
 *
 *  U is the unicode encoding in bits, 8, 16, 32
 *
 *  ATTR is one of the following C attributes
 *   -auto (for local use in a function)
 *   -static (for static linkage in a file)
 *   -volatile (sure why not)
 *   -never const (special handling already done in const_ macro)
 *
 *  VAR is name of the variable
 *
 *  N is the length for
 *
 *  STR is the string literal
 *
 *
 * Note that if these macros are used outside of function scope,
 * their linkage will be global.
 *
 *
 * */

/* uwstr8(variable_name, capacity)
 *
 * Instantiates an empty uwstr able to store up to capacity bytes
 * (including null terminator)
 * The capacity may not exceed 256 bytes
 *
 *
 * */

 /* This example instantiates a variable, tmp, with type char* */
 uwstr8(auto, tmp, 128);

/* const_uwstr8(variable_name, c_string)
 *
 * Instantiates a read only uwstr iwth CONSTANT null terminated string as input
 * The string length may not exceed 256 including null terminator
 *
 *
 * */
 /* This example instantiates a variable, PREFIX, with type const char* */
 const_uwstr8(auto, PREFIX, "SomePrefix:");

/* uwstr28(variable_name, capacity)
 *
 * Instantiates an empty uwstr able to store up to capacity bytes
 * (including null terminator)
 * The capacity may not exceed 65536 bytes
 *
 *
 * */

 /* This example instantiates a variable, tmp, with type char* */
 uwstr28(auto, tmp, 4096);

/* const_uwstr28(variable_name, c_string)
 *
 * Instantiates a read only uwstr with CONSTANT null terminated string as input
 * The string length may not exceed 65536 including null terminator
 *
 *
 * */
 /* This example instantiates a variable, ERR_RESPONSE, with type const char* */
 const_uwstr28(auto, ERR_RESPONSE, "<html><head>.....</head></html>");
#endif


/** @brief Calculate header size given first byte of header
 *  @param hdr header byte
 *  @return 0 if invalid header byte; size otherwise. */
size_t uws_hdr_size(uint8_t header_byte);

/** @brief Calculate buffer size required to store X number content characters
 *  @param capacity Maximum number of content characters.
 *  @return total allocation size. */
size_t uws_alloc_size(size_t capacity);

/** @brief Initialize a raw buffer to an empty uws string.
 *  @param buffer pointer to raw data
 *  @param bufflen length of the buffer
 *  @return uws string initialized to the empty string. */
void uws_init(char* buffer, size_t bufflen);

/** @brief Set a string to the empty string.
 *  @param buffer Non-NULL pointer to character data. */
void uws_empty(char* uws);


/* Properties. */


/** @brief Whether uws string is a wynnstring.
 *  @return false if NULL == buffer
 *  or buffer does not contain a valid uws prefix.*/
bool uws_wynn(const char* buffer);

/** @brief Whether uws string is read only.
 *  @return true if read-only; false if not
 *  read-only strings cannot be passed to free() */
bool uws_ro(const char* uws);

/** @brief Get length of a uws string.
 *  @param uws Must point to string initialized by uws_init
 *  @return Number of bytes in the string. Invalid strings will return 1. */
size_t uws_len(const char* uws);

/** @brief Number of code points in a uws string.
 *  @param uws Must point to string initialized by uws_init
 *  @return 0 Number of bytes in the string. Invalid strings will return 1. */
size_t uws_cnt(const char* uws);

/** @brief How many bytes are still available in the buffer.
 *  @param uws Must point to string initialized by uws_init
 *  @return 0 if not a valid uws string. */
size_t uws_avail(const char* uws);



/* Manipulation. */


/** @brief Concatenate 2 uws strings.
 *  @param uws_dest Non-NULL pointer to uws data
 *  @param uws_src pointer to uws data
 *  @return Total length of new string.
 *  If enough space in uws, then uws_dest is the invalid string. */
size_t uws_cat(char* uws_dest, const char* uws_src);

/** @brief Append buffered data to the destination string.
 *  @param uws_dest where to store source data.
 *  @param src string data, does NOT need to be null terminated
 *  @param src_len length of source data
 *  @return number of continuation bytes needed for a valid UTF-8 string (0-3)
 *  < 0 if src contains invalid UTF-8 data, exceeds capacity
 *  or uws_dest is invalid.  */
int uws_inbuffer(const char* uws_dest, const uint8_t* src, size_t src_len);


/* Analysis. */


/** @brief Compare 2 uws strings.
 *  @return follows strcmp semantics.
 *  If either a or b is wynnstring, the return value is < 0. */
int uws_cmp(const char* uws_a, const char* uws_b);

/** @brief Finds a token in the uws_src string
 *  @param uws_src String in which to find tokens
 *  @param pos Pointer to a position in uws_src; if NULL then start is implied
 *  @param delimiters Set of characters defining token boundaries
 *  @return Length of token (0 if not found).
 *  pos is updated to point to a position in uws_src
 *  pos must be considered invalidated if any change to uws_src is made */
size_t uws_tok(const char* uws_src, const char** pos, const char* delimiters);


/* C style interfacing. */


/** @brief Get expected null terminated C string from uws string.
 *  @return pointer to invalid UTF-8 if string is not valid; valid ptr otherwise. */
const char* uws_c(const char* uws);

/** @brief Concatenate a VALIDATED UTF-8 C string.
 *  @return Total length of new string. If there was not enough space in uws
 *  then uws_dest is the invalid string. */
size_t uws_ccat(char* uws_dest, const char* c_src);

/** @brief Copy a C string to the destination, destroying old contents
 *  @return Total length of new string. If there was not enough space in uws
 *  then uws_dest is the invalid string. */
static inline size_t uws_ccpy(char* uws_dest, const char* c_src);


/* Implementation */

static inline size_t uws_ccpy(char* uws_dest, const char* c_src){
	uws_empty(uws_dest);
	return uws_ccat(uws_dest, c_src);
}

#endif
