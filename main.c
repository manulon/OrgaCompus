#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "file_reader.h"
#include "hash.h"


#define ERROR -1
#define SUCCESS 0

#define STD_FILE "-"

#define ENDLINE '\n'
#define NULLTER '\0'


#define MODO_HELP 1
#define MODO_VERSION 2
#define MODO_INPUT 3
#define O_MODE 4
#define MODO_INPUT_OUTPUT 5
#define MODO_OUTPUT_INPUT 6
#define MODO_STD 7

//COMPROBAR FLAGS
bool is_help_flag(const char* flag) {
    return (strcmp(flag, "-h") == 0 || strcmp(flag,"--help") == 0);
}

bool is_version_flag(const char* flag) {
    return (strcmp(flag, "-V") == 0 || strcmp(flag, "--version") == 0);
}

bool es_input(const char* flag) {
    return (strcmp(flag, "-i") == 0 || strcmp(flag, "--input") == 0);
}

bool es_output(const char* flag) {
    return (strcmp(flag, "-o") == 0 || strcmp(flag, "--output") == 0);
}


//Obtener modo de ejecicion
int modo_2_argumentos(char* const argv[]) {
    const char* flag = argv[1];

    if (is_help_flag(flag))
        return MODO_HELP;

    if (is_version_flag(flag))
        return MODO_VERSION;

    return ERROR;
}

int modo_3_argumentos(char* const argv[]) {
    const char* flag = argv[1];

    if (es_input(flag))
        return MODO_INPUT;

    if (es_output(flag))
        return O_MODE;

    return ERROR;
}

int modo_5_argumentos(char* const argv[]) {
    const char* flag_1 = argv[1];
    const char* flag_2 = argv[3];

    bool flag_1_input = es_input(flag_1);
    bool flag_2_input = es_input(flag_2);

    bool flag_1_output = es_output(flag_1);
    bool flag_2_output = es_output(flag_2);

    if (flag_1_input && flag_2_output)
        return MODO_INPUT_OUTPUT;

    if (flag_1_output && flag_2_input)
        return MODO_OUTPUT_INPUT;

    
    return ERROR;
}

int modo_ejecucion(int argc, char* const argv[]) {
    switch (argc) {
        case 1:
            return MODO_STD;
        case 2:
            return modo_2_argumentos(argv);
        case 3:
            return modo_3_argumentos(argv);
        case 5:
            return modo_5_argumentos(argv);
        default:
            return ERROR;
    }
}

//MODOS DE EJECUCION

int help() {
    printf("Usage:\n");
    printf("\ttp1 -h\n");
    printf("\ttp1 -V\n");
    printf("\ttp1 -i in_file -o out_file\n");
    printf("Options:\n");
    printf("\t-V, --version Print version and quit.\n");
    printf("\t-h, --help Print this information and quit.\n");
    printf("\t-i, --input Specify input stream/file, '-' for stdin\n");
    printf("\t-o, --output Specify output stream/file, '-' for stdout.\n");
    printf("Examples:\n");
    printf("\ttp1 < in.txt > out.txt\n");
    printf("\tcat in.txt | tp1 -i - > out.txt\n");
    return SUCCESS;
}

int version() {
    printf("Hash de texto version: 1.0.0\n");
    return SUCCESS;
}


//MAIN

int main(int argc, char* const argv[]) {
    const char* nombre_archivo_input = STD_FILE; 
    const char* nombre_archivo_output = "salida_default.txt"; 

    int mode = modo_ejecucion(argc, argv);
    switch (mode) {
        case MODO_VERSION:
            return version();
        case MODO_HELP:
            return help();
        case MODO_STD:
            break;
        case MODO_INPUT: {
            nombre_archivo_input = argv[2];
            break;
        }
        case O_MODE: {
            nombre_archivo_output = argv[2];
            break;
        }
        case MODO_INPUT_OUTPUT: {
            nombre_archivo_input = argv[2];
            nombre_archivo_output = argv[4];
            break;
        }
        case MODO_OUTPUT_INPUT: {
            nombre_archivo_input = argv[4];
            nombre_archivo_output = argv[2];
            break;
        }
        default: {
            fprintf(stderr,"unrecognized command line option\n");
            return ERROR;
        }
    }
    

    bool input_is_std = (strcmp(nombre_archivo_input, STD_FILE) == 0);
    bool output_is_std = (strcmp(nombre_archivo_output, STD_FILE) == 0);
   
    if (input_is_std) {
       	   if(!output_is_std) fopen(nombre_archivo_output, "w+"); 
           printf( "No se ingreso un archivo de entrada, para mas ayuda, usar comando -h\n");
           return ERROR;
        
    } 
   
   

    file_reader_t file_reader;
    file_init(&file_reader,nombre_archivo_input);
    read_hash_file(&file_reader, nombre_archivo_output);
    file_reader_uninit(&file_reader);

    return SUCCESS;
}
