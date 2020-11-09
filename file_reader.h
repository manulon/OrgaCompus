#ifndef FILE_READER_H
#define FILE_READER_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <errno.h>
#define BUF_SIZE 4

#include "hash.h"

/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct {
    FILE* fp;
} file_reader_t;

/* *****************************************************************
 *                PRIMITIVAS DEL FILE READER
 * *****************************************************************/

//Abre un archivo
//Pre: recibe un nombre de archivo como parámetro
//Post devuelve 0 si el archivo fue abierto correctamente, 
//-1 en caso de error.
int file_init(file_reader_t* self, const char* input_file_name);

//Lee el archivo abierto en init.
//Pre: se inicializó un file_reader_t
int read_hash_file (file_reader_t* self,const char* output_file_name);

//Ciera el archivo abierto en init
//Pre: un file reader fue inicializado.
//Post devuelve 0 si el archivo fue cerrado correctamente, 
//-1 en caso de error.
int file_reader_uninit(file_reader_t* self);


#endif