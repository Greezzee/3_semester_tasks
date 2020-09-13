#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

void GenerateString(int n, char* string)
{
    char counter = 'a';

    for (int i = 0; i < n; i++)
    {
        int len = strlen(string);
        string[len] = counter;
        
        // лучше использовать готовую ф-ю strncpy вместо цикла ниже
        for (int j = 0; j < len; j++)
            string[len + j + 1] = string[j];
        counter++;
    }

}

int main()
{
    for (int i = 1; i < 10; i++) {
        char* str = (char*)calloc(pow(2, i) - 1, 1);
        GenerateString(i, str);
        printf("%s\n\n", str);
        free(str);
    }
    return 0;
}
