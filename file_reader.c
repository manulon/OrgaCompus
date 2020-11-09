#include "file_reader.h"
#include "hash_read_file.h"
#include <stdbool.h>

int file_reader_init(file_reader_t* self, const char* input_file_name) {
    if (file_name == NULL) {
        self->fp = stdin;
    } else {
        self->fp = fopen(file_name, "rb");
    }
    return 0;
}

int read_hash_file(file_reader_t* self, const char* output_file_name) {
    char* buffer;
    size_t len = 0;
    int32_t hash;
    FILE* output_file = fopen(output_file_name, "w+");
    if (!output_file)
        printf("Error al abrir el archivo de escritura\n");
    while (true) {
	
        getline(&buffer, &len, self->fp);
	    if(feof(self->fp)) break;
	    len = strlen(buffer);
       	if(len > 1)
       		hash = get_hash(buffer);
        printf("0x%08x %s", hash, buffer);
        fprintf(output_file,"0x%08x %s\n", hash, buffer);
    }
    fclose(output_file);
    return 0;
}

int file_reader_uninit(file_reader_t* self) {
    if (self->fp != stdin) {
        if (fclose(self->fp) == EOF) {
//            printf(stderr, "file_reader_uninit-->close %s\n", 
//            strerror(errno));
            return -1;
        }
    }
    return 0;
}
