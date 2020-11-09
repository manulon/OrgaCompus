#include "hash.h"

void
string_hash_init(string_hash *h)
{
	h->flag = STRING_HASH_INIT;
	h->hash = 0;
	h->size = 0;
}


void
string_hash_done(string_hash *sh)
{
	assert(sh->flag == STRING_HASH_INIT || sh->flag == STRING_HASH_MORE);

	if ((sh->hash ^= sh->size) == -1)
		sh->hash = -2;

	sh->flag = STRING_HASH_DONE;
}

int32_t
string_hash_value(string_hash *sh)
{
	return sh->hash;
}

void hash_line(char* buffer) {
	string_hash hash;
	char *msg = buffer;
	char *ptr;
	size_t len = strlen(msg);
	size_t delta;
	size_t stride;
	size_t rem;

	for (stride = len; stride >= 1; stride--) {
		string_hash_init(&hash);
		ptr = msg;
		rem = len;

		while (rem) {
			if (rem >= stride)
				delta = stride;
			else
				delta = rem;
	
			string_hash_more(&hash, ptr, delta);
			rem -= delta;
			ptr += delta;
		}

		string_hash_done(&hash);
	}
	printf("0x%04x %s\n", string_hash_value(&hash), buffer);
}

#if 0
static long string_hash(PyStringObject *a)
{
    register Py_ssize_t len;
    register unsigned char *p;
    register long x;

    if (a->ob_shash != -1)
        return a->ob_shash;
    len = Py_SIZE(a);
    p = (unsigned char *) a->ob_sval;
    x = *p << 7;
    while (--len >= 0)
        x = (1000003*x) ^ *p++;
    x ^= Py_SIZE(a);
    if (x == -1)
        x = -2;
    a->ob_shash = x;
    return x;
}
#endif

