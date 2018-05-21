#include <assert.h>
#include <endian.h>
#include <string.h>

#include "uwstr.h"

#if __UWSTR_CONFORMANCE_LEVEL > 0
#error "No support above 0 so far!"
#endif

#define SINGLE_BYTE 0xF5
#define DOUBLE_BYTE 0xF6

#define INVALID_C_STRING "\xFF"

struct single_str {
	uint8_t hdr;
	uint8_t nullterm;
	uint8_t length;
	uint8_t capacity;
	char data[];
};

struct double_str {
	uint8_t hdr;
	uint8_t nullterm;
	uint16_t length;
	uint16_t capacity;
	uint16_t reserved;
	char data[];
};


static void uws_invalidate(char* uws){
	if(uws[0] == SINGLE_BYTE){
		__auto_type s = (struct single_str*)uws;
		s->length = s->capacity;
	}
	else{
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
	assert(b[0] >= 0xF5 && b[0] <= 0xF6);
	assert(b[1] == 0);

	if(b[0] == SINGLE_BYTE){
		__auto_type s = (struct single_str*)buffer;
		s->length = 0;
		s->data[0] = '\0';
	}
	else{
		__auto_type d = (struct double_str*)buffer;
		d->length = 0;
		d->data[0] = '\0';
	}
}

bool uws_invalid(const char* buffer){
	uint8_t* b = (uint8_t*)buffer;
	assert(b[0] >= 0xF5 && b[0] <= 0xF6);
	assert(buffer[1] == 0);

	if(b[0] == SINGLE_BYTE){
		__auto_type s = (struct single_str*)buffer;
		if(s->length >= s->capacity)
			return true;
	}
	else{
		__auto_type d = (struct double_str*)buffer;
		if(d->length >= d->capacity)
			return true;
	}

	return false;
}

size_t uws_len(const char* uws){
	uint8_t* b = (uint8_t*)uws;
	assert(b[0] >= 0xF5 && b[0] <= 0xF6);
	assert(b[1] == 0);

	if(uws_invalid(uws))
		return sizeof(INVALID_C_STRING) - 1;

	if(b[0] == SINGLE_BYTE){
		__auto_type s = (struct single_str*)uws;
		return s->length;
	}
	else{
		__auto_type d = (struct double_str*)uws;
		return d->length;
	}
}

size_t uws_cnt(const char* uws){
	return uws_len(uws);
}

size_t uws_avail(const char* uws){
	uint8_t* b = (uint8_t*)uws;
	assert(b[0] >= 0xF5 && b[0] <= 0xF6);
	assert(uws[1] == 0);

	if(b[0] == SINGLE_BYTE){
		__auto_type s = (struct single_str*)uws;
		return s->capacity - s->length;
	}
	else{
		__auto_type d = (struct double_str*)uws;
		return d->capacity - d->length;
	}
}

size_t uws_cat(char* uws_dest, const char* uws_src){
	uint8_t* ds = (uint8_t*)uws_dest;

	if(uws_invalid(uws_src)){
		uws_invalidate(uws_dest);
		return 0;
	}

	size_t src_len = uws_len(uws_src);
	const char * src = uws_c(uws_src);

	if(ds[0] == SINGLE_BYTE){
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
	else{
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
}

int uws_cmp(const char* uws_a, const char* uws_b){

	if(uws_invalid(uws_a))
		return -1;

	if(uws_invalid(uws_b))
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

const char* uws_c(const char* uws){
	uint8_t* b = (uint8_t*)uws;
	assert(b[0] >= 0xF5 && b[0] <= 0xF6);
	assert(b[1] == 0);

	if(uws_invalid(uws))
		return INVALID_C_STRING;

	if(b[0] == SINGLE_BYTE){
		__auto_type s = (struct single_str*)uws;
		return s->data;
	}
	else{
		__auto_type d = (struct double_str*)uws;
		return d->data;
	}
}

size_t uws_ccat(char* uws_dest, const char* src){
	uint8_t* ds = (uint8_t*)uws_dest;
	size_t src_len = strlen(src);

	if(ds[0] == SINGLE_BYTE){
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
	else{
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

}
