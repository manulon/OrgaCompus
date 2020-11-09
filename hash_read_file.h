#include "hash.h"

typedef struct {
	int32_t hash;
	char *msg;
} regression;

int32_t get_hash_(string_hash *sh, char *msg, size_t len, size_t stride);

int get_hash(char *msg);
