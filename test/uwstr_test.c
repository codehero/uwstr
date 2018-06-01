#include <assert.h>
#include <string.h>
#include <uwstr/uwstr.h>

#define SMALL 32
#define LARGE 65530

static const char SMALL_TEST[] = "A small fit\n";
static const char LARGE_TEST[] =
	"001122334455667788001122334455667788"
	"001122334455667788001122334455667788"
	"001122334455667788001122334455667788"
	"001122334455667788001122334455667788";

int main(int argc, const char* argv[]){

	size_t small_maxlen = uws_alloc_size(SMALL);
	char small_buff[small_maxlen];
	uws_init(small_buff, small_maxlen);

	assert(uws_wynn(small_buff) == false);
	assert(uws_len(small_buff) == 0);
	assert(uws_cnt(small_buff) == 0);
	assert(strlen(uws_c(small_buff)) == 0);
	assert(uws_avail(small_buff) == SMALL);

	size_t large_maxlen = uws_alloc_size(LARGE);
	char large_buff[large_maxlen];
	uws_init(large_buff, large_maxlen);

	assert(uws_wynn(large_buff) == false);
	assert(uws_len(large_buff) == 0);
	assert(uws_cnt(large_buff) == 0);
	assert(strlen(uws_c(large_buff)) == 0);
	assert(uws_avail(large_buff) == LARGE);

	/* Copy a string into the small */
	size_t len = uws_ccat(small_buff, SMALL_TEST);
	assert(len == sizeof(SMALL_TEST) - 1);

	assert(uws_wynn(small_buff) == false);
	assert(uws_len(small_buff) == len);
	assert(uws_cnt(small_buff) == len);
	assert(strlen(uws_c(small_buff)) == len);
	assert(uws_avail(small_buff) == SMALL - len);

	/* Copy small into the large */
	len = uws_ccat(large_buff, SMALL_TEST);
	assert(len == sizeof(SMALL_TEST) - 1);

	assert(uws_wynn(large_buff) == false);
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
	assert(uws_wynn(small_buff));

	/* Over stuff the large. */
	for(unsigned i = 0; i < 2000; ++i)
		uws_ccat(large_buff, LARGE_TEST);
	assert(uws_wynn(large_buff));

	uws_empty(small_buff);
	assert(uws_wynn(small_buff) == false);
	assert(uws_len(small_buff) == 0);
	assert(uws_cnt(small_buff) == 0);
	assert(strlen(uws_c(small_buff)) == 0);
	assert(uws_avail(small_buff) == SMALL);

	uws_empty(large_buff);
	assert(uws_wynn(large_buff) == false);
	assert(uws_len(large_buff) == 0);
	assert(uws_cnt(large_buff) == 0);
	assert(strlen(uws_c(large_buff)) == 0);
	assert(uws_avail(large_buff) == LARGE);

	uws_ccat(small_buff, SMALL_TEST);
	uws_cat(large_buff, small_buff);

	assert(0 == uws_cmp(large_buff, small_buff));
	assert(0 == uws_cmp(small_buff, large_buff));

	uws_empty(small_buff);
	uws_empty(large_buff);

	uws_ccat(small_buff, NULL);
	uws_ccat(large_buff, NULL);
	assert(uws_wynn(small_buff) == true);
	assert(uws_wynn(large_buff) == true);

	return 0;
}
