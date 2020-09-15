#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Заметил, что компилятор gcc не позволяет использовать const переменные как размер массива на стэке
//g++ при этом разрешает подобное
//Потому вместо const size_t пришлось применить define, чтобы работало на как на gcc, так и на g++
#define MaxSymbolsInNumber 33

void Translate(int input_number, int out_base, char* out)
{
    if (input_number == 0) {
        out[0] = '0';
        return;
    }
    
    char reversed_out[MaxSymbolsInNumber] = {};
    int i = 0;
    while (input_number != 0)
    {
        int d = input_number % out_base;
        input_number /= out_base;
        char c = d < 10 ? d + '0' : d - 10 + 'A';
        reversed_out[i] = c;
        i++;
    }

    int flag = 0;
    int cur_symbol = 0;
    for (int j = i - 1; j >= 0; j--) {
        if (flag || reversed_out[j] != '0') {
            out[cur_symbol] = reversed_out[j];
            cur_symbol++;
            flag = 1;
        }
    }
}

int main() {
    char start_base = 0, out_base = 0;
    unsigned long input_number = 0;

    char in[MaxSymbolsInNumber];
    char* end;
    printf("Base: ");
    scanf("%hhd", &start_base);
    printf("Number: ");
    scanf("%s", in);
    input_number = strtol(in, &end, start_base);
    printf("Out base: ");
    scanf("%hhd", &out_base);
    char out[MaxSymbolsInNumber] = {};
    Translate(input_number, out_base, out);
    
    printf("%s\n", out);

    return 0;
}
