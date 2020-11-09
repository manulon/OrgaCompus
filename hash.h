#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#define STRING_HASH_INIT 1
#define STRING_HASH_MORE 2
#define STRING_HASH_DONE 3

/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct {
	int8_t flag;
	int32_t hash;
	size_t size;
} string_hash;

/* *****************************************************************
 *                PRIMITIVAS DEL HASH
 * *****************************************************************/

void 
string_hash_init(string_hash *h);

void
string_hash_more(string_hash *sh, char *str, size_t len);

void
string_hash_done(string_hash *sh);

int32_t
string_hash_value(string_hash *sh);

void hash_line(char* buffer);

#endif
