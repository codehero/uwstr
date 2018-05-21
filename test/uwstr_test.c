#include <assert.h>
#include <string.h>
#include <uwstr/uwstr.h>

#define SMALL 32
#define LARGE 65530
int main(int argc, const char* argv[]){

	size_t small_maxlen = uws_alloc_size(SMALL);
	char small_buff[small_maxlen];
	uws_init(small_buff, small_maxlen);

	assert(uws_invalid(small_buff) == false);
	assert(uws_len(small_buff) == 0);
	assert(uws_cnt(small_buff) == 0);
	assert(strlen(uws_c(small_buff)) == 0);
	assert(uws_avail(small_buff) == SMALL);

	size_t large_maxlen = uws_alloc_size(LARGE);
	char large_buff[large_maxlen];
	uws_init(large_buff, large_maxlen);

	assert(uws_invalid(large_buff) == false);
	assert(uws_cnt(large_buff) == 0);
	assert(strlen(uws_c(large_buff)) == 0);
	assert(uws_avail(large_buff) == LARGE);

	/* Copy a string into the small */
	const char SMALL_TEST[] = "A small fit\n";
	size_t len = uws_ccat(small_buff, SMALL_TEST);
	assert(len == sizeof(SMALL_TEST) - 1);

	assert(uws_invalid(small_buff) == false);
	assert(uws_len(small_buff) == len);
	assert(uws_cnt(small_buff) == len);
	assert(strlen(uws_c(small_buff)) == len);
	assert(uws_avail(small_buff) == SMALL - len);

	/* Copy small into the large */
	len = uws_ccat(large_buff, SMALL_TEST);
	assert(len == sizeof(SMALL_TEST) - 1);

	assert(uws_invalid(large_buff) == false);
	assert(uws_len(large_buff) == len);
	assert(uws_cnt(large_buff) == len);
	assert(strlen(uws_c(large_buff)) == len);
	assert(uws_avail(large_buff) == LARGE - len);

	assert(0 == uws_cmp(large_buff, small_buff));
	assert(0 == uws_cmp(small_buff, large_buff));

	/* Concatenate again */
	len = uws_ccat(small_buff, SMALL_TEST);
	assert(len == 2 * (sizeof(SMALL_TEST) - 1));
	assert(uws_avail(small_buff) == SMALL - 2 * (sizeof(SMALL_TEST) - 1));

	len = uws_ccat(large_buff, SMALL_TEST);
	assert(uws_avail(large_buff) == LARGE - 2 * (sizeof(SMALL_TEST) - 1));

	assert(0 == uws_cmp(large_buff, small_buff));
	assert(0 == uws_cmp(small_buff, large_buff));

	/* Over stuff the small. */
	len = uws_ccat(small_buff, SMALL_TEST);
	assert(uws_invalid(small_buff));

	return 0;
}
