#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char start_base = 0, out_base = 0;
    unsigned long input_number = 0;

    char in[33];
    char* end;
    printf("Base: ");
    scanf("%hhd", &start_base);
    printf("Number: ");
    scanf("%s", in);
    input_number = strtol(in, &end, start_base);

    if (input_number == 0)
        printf("0");
    
    printf("Out base: ");
    scanf("%hhd", &out_base);
    char out[33] = {};
    int i = 0;

    while (input_number != 0)
    {
        int d = input_number % out_base;
        input_number /= out_base;
        char c = d < 10 ? d + '0' : d - 10 + 'A';
        out[i] = c;
        i++;
    }
    
    int flag = 0;
    for (int j = i - 1; j >= 0; j--) {
        if (flag || out[j] != '0') {
            printf("%c", out[j]);
            flag = 1;
        }
    }
    printf("\n");
    return 0;
}