#ifndef __UWSTR_INTERNAL_H__
#error "Do not include this header from user code!"
#endif

struct __internal_uws_single_str {
	uint8_t hdr;
	uint8_t nullterm;
	uint8_t length;
	uint8_t capacity;
	char data[];
};

struct __internal_uws_double_str {
	uint8_t hdr;
	uint8_t nullterm;
	uint16_t reserved;
	uint16_t length;
	uint16_t capacity;
	char data[];
};

#define uwstr8(ATTR, VAR, CAPACITY) \
	ATTR struct { \
		struct __internal_uws_single_str u; \
		char buffer[CAPACITY]; \
	} __internal_uws_##VAR = { \
		.u.hdr = '\xC0', \
		.u.nullterm = '\0', \
		.u.length = 0, \
		.u.capacity = CAPACITY, \
		}; \
	ATTR char* VAR = (char*)&__internal_uws_##VAR; \
	_Static_assert(CAPACITY <= 256, "uwstr8 cannot hold more than 256 bytes")

#define const_uwstr8(ATTR, VAR, STR) \
	ATTR const union { \
		char uws[sizeof(STR) + 4]; \
		struct { \
			struct __internal_uws_single_str u; \
			char buffer[sizeof(STR)]; \
		} s;\
	} __internal_uws_const_##VAR = { \
		.s.u.hdr = '\xC1', \
		.s.u.nullterm = '\0', \
		.s.u.length = sizeof(STR) - 1, \
		.s.u.capacity = sizeof(STR), \
		.s.buffer = STR}; \
	ATTR const char* VAR = (const char*)&__internal_uws_const_##VAR; \
	_Static_assert(sizeof(STR) <= 256, "uwstr8 cannot hold more than 256 bytes")


#define uwstr28(ATTR, VAR, CAPACITY) \
	ATTR struct { \
		struct __internal_uws_double_str u; \
		char buffer[CAPACITY]; \
	} __internal_uws_##VAR = { \
		.u.hdr = '\xE0', \
		.u.nullterm = '\0', \
		.u.reserved = 0, \
		.u.length = 0, \
		.u.capacity = CAPACITY, \
		}; \
	ATTR char* VAR = (char*)&__internal_uws_##VAR; \
	_Static_assert(CAPACITY <= 65536, \
			"uwstr28 cannot hold more than 65536 bytes")

#define const_uwstr28(ATTR, VAR, STR) \
	ATTR const union { \
		char uws[sizeof(STR) + sizeof(__internal_uws_double_str)]; \
		const struct { \
			struct __internal_uws_double_str u; \
			char buffer[sizeof(STR)]; \
		} s; \
	}	__internal_uws_const_##VAR = { \
		.u.hdr = '\xE1', \
		.u.nullterm = '\0', \
		.u.reserved = 0, \
		.u.length = sizeof(STR) - 1, \
		.u.capacity = sizeof(STR), \
		.buffer = STR}; \
	ATTR const char* VAR = (const char*)&__internal_uws_const_##VAR; \
	_Static_assert(sizeof(STR) <= 65536,\
			"uwstr28 cannot hold more than 65536 bytes")
