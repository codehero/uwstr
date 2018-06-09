#include <assert.h>
#include <string.h>
#include <uwstr/uwstr.h>

#define SMALL_LEN 32
#define LARGE_LEN 65530

static const char SMALL_LEN_TEST[] = "A small fit\n";
static const char LARGE_LEN_TEST[] =
	"001122334455667788001122334455667788"
	"001122334455667788001122334455667788"
	"001122334455667788001122334455667788"
	"001122334455667788001122334455667788";

int main(int argc, const char* argv[]){

	/*
	size_t small_maxlen = uws_alloc_size(SMALL_LEN);
	char small_buff[small_maxlen];
	uws_init(small_buff, small_maxlen);
	*/

	uwstr8(small_buff, SMALL_LEN);

	assert(uws_wynn(small_buff));
	assert(uws_len(small_buff) == 0);
	assert(uws_cnt(small_buff) == 0);
	assert(strlen(uws_c(small_buff)) == 0);
	assert(uws_avail(small_buff) == SMALL_LEN);

	/*
	size_t large_maxlen = uws_alloc_size(LARGE_LEN);
	char large_buff[large_maxlen];
	uws_init(large_buff, large_maxlen);
	*/

	uwstr28(large_buff, LARGE_LEN);

	assert(uws_wynn(large_buff));
	assert(uws_len(large_buff) == 0);
	assert(uws_cnt(large_buff) == 0);
	assert(strlen(uws_c(large_buff)) == 0);
	assert(uws_avail(large_buff) == LARGE_LEN);

	/* Copy a string into the small */
	size_t len = uws_ccat(small_buff, SMALL_LEN_TEST);
	assert(len == sizeof(SMALL_LEN_TEST) - 1);

	assert(uws_wynn(small_buff));
	assert(uws_len(small_buff) == len);
	assert(uws_cnt(small_buff) == len);
	assert(strlen(uws_c(small_buff)) == len);
	assert(uws_avail(small_buff) == SMALL_LEN - len);

	/* Copy small into the large */
	len = uws_ccat(large_buff, SMALL_LEN_TEST);
	assert(len == sizeof(SMALL_LEN_TEST) - 1);

	assert(uws_wynn(large_buff));
	assert(uws_len(large_buff) == len);
	assert(uws_cnt(large_buff) == len);
	assert(strlen(uws_c(large_buff)) == len);
	assert(uws_avail(large_buff) == LARGE_LEN - len);

	assert(0 == uws_cmp(large_buff, small_buff));
	assert(0 == uws_cmp(small_buff, large_buff));

	/* Concatenate again */
	len = uws_ccat(small_buff, SMALL_LEN_TEST);
	assert(len == 2 * (sizeof(SMALL_LEN_TEST) - 1));
	assert(uws_avail(small_buff)
		== SMALL_LEN - 2 * (sizeof(SMALL_LEN_TEST) - 1));

	len = uws_ccat(large_buff, SMALL_LEN_TEST);
	assert(uws_avail(large_buff)
		== LARGE_LEN - 2 * (sizeof(SMALL_LEN_TEST) - 1));

	assert(0 == uws_cmp(large_buff, small_buff));
	assert(0 == uws_cmp(small_buff, large_buff));

	/* Over stuff the small. */
	len = uws_ccat(small_buff, SMALL_LEN_TEST);
	assert(uws_wynn(small_buff) == false);

	/* Over stuff the large. */
	for(unsigned i = 0; i < 2000; ++i)
		uws_ccat(large_buff, LARGE_LEN_TEST);
	assert(uws_wynn(large_buff) == false);

	uws_empty(small_buff);
	assert(uws_wynn(small_buff));
	assert(uws_len(small_buff) == 0);
	assert(uws_cnt(small_buff) == 0);
	assert(strlen(uws_c(small_buff)) == 0);
	assert(uws_avail(small_buff) == SMALL_LEN);

	uws_empty(large_buff);
	assert(uws_wynn(large_buff));
	assert(uws_len(large_buff) == 0);
	assert(uws_cnt(large_buff) == 0);
	assert(strlen(uws_c(large_buff)) == 0);
	assert(uws_avail(large_buff) == LARGE_LEN);

	uws_ccat(small_buff, SMALL_LEN_TEST);
	uws_cat(large_buff, small_buff);

	assert(0 == uws_cmp(large_buff, small_buff));
	assert(0 == uws_cmp(small_buff, large_buff));

	uws_empty(small_buff);
	uws_empty(large_buff);

	uws_ccat(small_buff, NULL);
	uws_ccat(large_buff, NULL);
	assert(!uws_wynn(small_buff));
	assert(!uws_wynn(large_buff));

	const_uwstr8(bromide, "bromide");

	assert(uws_wynn(bromide));
	assert(uws_cnt(bromide) == 7);
	assert(uws_len(bromide) == 7);

	return 0;
}
