#include <assert.h>
#include <endian.h>
#include <string.h>

#include "uwstr.h"

#if __UWSTR_CONFORMANCE_LEVEL > 0
#error "No support above 0 so far!"
#endif

/** @brief This bit indicates that the string resides in read only memory. */
#define RO_BIT  0x01

/** @brief This bit indicates that a partial inbuffer was made, but
 * the UTF-8 encoding was not complete. A call to uws_c() will result
 * in an invalid string. However, a subsequent buffer cat with the 
 * remaining UTF-8 bytes can clear this bit. */
#define GRAY_BIT  0x02

/** @brief This bit indicates that individual counts of each kind of
 * multibyte are maintained.
 * -Count of 1 byte encodings
 * -Count of 2 byte encodings
 * -Count of 3 byte encodings
 * -Count of 4 byte encodings
 * */
#define UTF8_COUNT_BIT 0x40

/** @brief This indicates that the string uses 2 bytes for string length.
 * Otherwise, if clear then only 1 byte is used. */
#define DOUBLE_BIT 0x20


#define SINGLE_BYTE 0xC0
#define DOUBLE_BYTE 0xE0

#define SIZE_MASK  0xE0

/* A continuation byte by itself is invalid in UTF-8.
 * wynnstr also expect both high bits to be set. */
#define INVALID_C_STRING "\x80"

#define single_str __internal_uws_single_str
#define double_str __internal_uws_double_str

static void uws_invalidate(char* uws){
	uint8_t* b = (uint8_t*)uws;
	if((b[0] & SIZE_MASK) == SINGLE_BYTE){
		__auto_type s = (struct single_str*)uws;
		s->length = s->capacity;
	}
	else if((b[0] & SIZE_MASK) == DOUBLE_BYTE){
		__auto_type d = (struct double_str*)uws;
		d->length = d->capacity;
	}
}

size_t uws_alloc_size(size_t capacity){
	/* Include the null terminator. */
	++capacity;

	capacity += 4;
	/* If less than 256, can use single byte. */
	if(capacity < 256)
		return capacity;

	/* Otherwise, the prefix */

	capacity += 4;
	return capacity;
}

void uws_init(char* buffer, size_t bufflen){
	assert(buffer);
	assert(bufflen > sizeof(struct single_str));

	if(bufflen < 260){
		/* Doing single byte */
		__auto_type s = (struct single_str*)buffer;
		s->hdr = SINGLE_BYTE;
		s->nullterm = '\0';
		s->length = 0;
		s->capacity = bufflen - 5;

		/* Set content to empty string. */
		s->data[0] = '\0';
		return;
	}

	__auto_type d = (struct double_str*)buffer;
	d->hdr = DOUBLE_BYTE;
	d->nullterm = '\0';
	d->length = 0;
	d->capacity = bufflen - sizeof(struct double_str) - 1;
	d->reserved = 0;

	d->data[0] = '\0';
}

void uws_empty(char* buffer){
	uint8_t* b = (uint8_t*)buffer;
	assert(b);
	assert(b[0] >= 0xC0);
	assert(!(b[0] & 1));
	assert(b[1] == 0);

	if((b[0] & SIZE_MASK) == SINGLE_BYTE){
		__auto_type s = (struct single_str*)buffer;
		s->length = 0;
		s->data[0] = '\0';
	}
	else if((b[0] & SIZE_MASK) == DOUBLE_BYTE){
		__auto_type d = (struct double_str*)buffer;
		d->length = 0;
		d->data[0] = '\0';
	}
	else
		assert(0);
}

bool uws_wynn(const char* buffer){
	assert(buffer);

	uint8_t* b = (uint8_t*)buffer;
	assert(b[0] >= 0xC0);
	assert(buffer[1] == 0);

	if((b[0] & SIZE_MASK) == SINGLE_BYTE){
		__auto_type s = (struct single_str*)buffer;
		if(s->length < s->capacity)
			return true;
	}
	else if((b[0] & SIZE_MASK) == DOUBLE_BYTE){
		__auto_type d = (struct double_str*)buffer;
		if(d->length < d->capacity)
			return true;
	}

	return false;
}

bool uws_ro(const char* uws){
	assert(buffer);

	uint8_t* b = (uint8_t*)buffer;
	assert(b[0] >= 0xC0);
	assert(buffer[1] == 0);

	return (b[0] & RO_BIT) ? true : false;
}

size_t uws_len(const char* uws){
	if(!uws)
		return sizeof(INVALID_C_STRING) - 1;

	uint8_t* b = (uint8_t*)uws;
	assert(b[0] >= 0xC0);
	assert(b[1] == 0);

	if(!uws_wynn(uws))
		return sizeof(INVALID_C_STRING) - 1;

	if((b[0] & SIZE_MASK) == SINGLE_BYTE){
		__auto_type s = (struct single_str*)uws;
		return s->length;
	}
	else if((b[0] & SIZE_MASK) == DOUBLE_BYTE){
		__auto_type d = (struct double_str*)uws;
		return d->length;
	}
	else
		return sizeof(INVALID_C_STRING) - 1;
}

size_t uws_cnt(const char* uws){
	return uws_len(uws);
}

size_t uws_avail(const char* uws){
	if(!uws)
		return 0;

	uint8_t* b = (uint8_t*)uws;
	assert(b[0] >= 0xC0);
	assert(uws[1] == 0);

	if((b[0] & SIZE_MASK) == SINGLE_BYTE){
		__auto_type s = (struct single_str*)uws;
		return s->capacity - s->length;
	}
	else if((b[0] & SIZE_MASK) == DOUBLE_BYTE){
		__auto_type d = (struct double_str*)uws;
		return d->capacity - d->length;
	}
	else
		return 0;
}

size_t uws_cat(char* uws_dest, const char* uws_src){
	assert(uws_dest);

	uint8_t* ds = (uint8_t*)uws_dest;

	if(!uws_wynn(uws_src)){
		uws_invalidate(uws_dest);
		return 0;
	}

	size_t src_len = uws_len(uws_src);
	const char * src = uws_c(uws_src);

	if((ds[0] & SIZE_MASK) == SINGLE_BYTE){
		__auto_type s = (struct single_str*)uws_dest;
		if(s->length + src_len >= s->capacity){
			s->length = s->capacity;
			return 0;
		}

		memcpy(s->data + s->length, src, src_len);
		s->length += src_len;
		s->data[s->length] = '\0';

		return s->length;
	}
	else if((ds[0] & SIZE_MASK) == DOUBLE_BYTE){
		__auto_type d = (struct double_str*)uws_dest;

		if(d->length + src_len >= d->capacity){
			d->length = d->capacity;
			return 0;
		}

		memcpy(d->data + d->length, src, src_len);
		d->length += src_len;
		d->data[d->length] = '\0';

		return d->length;
	}
	else
		return 0;
}

int uws_inbuffer(const char* uws_dest, const uint8_t* src, size_t src_len){
	/* TODO */
	return -3;
}

int uws_cmp(const char* uws_a, const char* uws_b){
	assert(uws_a);
	assert(uws_b);

	if(!uws_wynn(uws_a))
		return -1;

	if(!uws_wynn(uws_b))
		return -2;

	const char * src_a = uws_c(uws_a);
	const char * src_b = uws_c(uws_b);

	size_t src_len_a = uws_len(uws_a);
	size_t src_len_b = uws_len(uws_b);

	if(src_len_a == src_len_b)
		return strncmp(src_a, src_b, src_len_b);

	size_t len = (src_len_a < src_len_b) ? src_len_a : src_len_b;

	int ret = strncmp(src_a, src_b, len);
	if(0 == ret)
		return (src_len_a < src_len_b) ? -1 : 1;
	return ret;
}

size_t uws_tok(const char* uws_src, const char** pos, const char* delimiters){
	/* TODO */
	return 0;
}

const char* uws_c(const char* uws){
	if(!uws)
		return INVALID_C_STRING;

	uint8_t* b = (uint8_t*)uws;
	assert(b[0] >= 0xC0);
	assert(b[1] == 0);

	if(!uws_wynn(uws))
		return INVALID_C_STRING;

	if((b[0] & SIZE_MASK) == SINGLE_BYTE){
		__auto_type s = (struct single_str*)uws;
		return s->data;
	}
	else if((b[0] & SIZE_MASK) == DOUBLE_BYTE){
		__auto_type d = (struct double_str*)uws;
		return d->data;
	}

	return INVALID_C_STRING;
}

size_t uws_ccat(char* uws_dest, const char* src){
	assert(uws_dest);

	if(!src){
		uws_invalidate(uws_dest);
		return 0;
	}

	uint8_t* ds = (uint8_t*)uws_dest;
	size_t src_len = strlen(src);

	if((ds[0] & SIZE_MASK) == SINGLE_BYTE){
		__auto_type s = (struct single_str*)uws_dest;
		if(s->length + src_len >= s->capacity){
			s->length = s->capacity;
			return 0;
		}

		memcpy(s->data + s->length, src, src_len);
		s->length += src_len;
		s->data[s->length] = '\0';

		return s->length;
	}
	else if((ds[0] & SIZE_MASK) == DOUBLE_BYTE){
		__auto_type d = (struct double_str*)uws_dest;

		if(d->length + src_len >= d->capacity){
			d->length = d->capacity;
			return 0;
		}

		memcpy(d->data + d->length, src, src_len);
		d->length += src_len;
		d->data[d->length] = '\0';

		return d->length;
	}
	else
		return 0;

}
