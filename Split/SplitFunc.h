#include <stdlib.h>
#include <string.h>

//Return number of lexems
//put lexems into splitted_strings
//memory for splitted_strings should be allocated. It should be enough to store all lexems
size_t Split(char* input_string, char* delims, char** splitted_strings)
{
    char* lexem = strtok(input_string, delims);
    size_t i = 0;
    for (i = 0; lexem != NULL; i++) {
        splitted_strings[i] = lexem;
        lexem = strtok(NULL, delims);
    }
    return i;
}