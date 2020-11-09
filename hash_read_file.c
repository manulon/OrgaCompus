#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "hash_read_file.h"

int32_t
get_hash_(string_hash *sh, char *msg, size_t len, size_t stride)
{
	char *ptr = msg;
	size_t delta;
	size_t rem;

	string_hash_init(sh);
	for (rem = len; rem > 0; ) {
		if (rem >= stride)
			delta = stride;
		else
			delta = rem;
	      
		string_hash_more(sh, ptr, delta);
	
		rem -= delta;
		ptr += delta;
	}
	string_hash_done(sh);

	return string_hash_value(sh);
}

int
get_hash(char *msg)
{
	size_t len = strlen(msg);
	size_t stride;
	string_hash sh;
	int32_t h0;
	int32_t h;

	if (len > 1) {
		h0 = get_hash_(&sh, msg, len, len);

		for (stride = len; stride >= 1; stride--) {
			h = get_hash_(&sh, msg, len, stride);
			assert(h0 == h);
		}
	}
	return h0;
}

