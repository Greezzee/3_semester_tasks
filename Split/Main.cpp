#include <stdio.h>
#include "SplitFunc.h"

size_t GetFileSize(FILE* file)
{
    fseek(file, 0L, SEEK_END); 
    size_t res = ftell(file);
    fseek(file, 0, SEEK_SET);
    return res;
}

int main(int argc, char** argv) 
{
    if (argc != 3) {
        printf("Please, run with args:\n./split [filename] [delims]\n");
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("File not found\n");
        return -1;
    }

    size_t size = GetFileSize(file);

    char* input_string = (char*)calloc(size + 1, sizeof(char));
    char** out_strings = (char**)calloc(size, sizeof(char*));
    fread(input_string, sizeof(char), size, file);

    size_t str_num = Split(input_string, argv[2], out_strings);

    for (size_t i = 0; i < str_num; i++) {
        printf("[%u]: %s\n", i + 1, out_strings[i]);
    }

    return 0;

}