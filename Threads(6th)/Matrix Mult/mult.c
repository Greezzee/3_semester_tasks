#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <time.h>

struct Matrix_t
{
    size_t m, n;
    double* matrix;
};

void ReadMatrix(struct Matrix_t* matrix)
{
    scanf("%lu %lu", &matrix->n, &matrix->m);
    matrix->matrix = (double*)calloc((matrix->n) * (matrix->m), sizeof(double));
    for (size_t i = 0; i < matrix->n * matrix->m; i++) {
        scanf("%lg", matrix->matrix + i);
    }
}

void GenerateMatrix(struct Matrix_t* matrix)
{
    matrix->matrix = (double*)calloc((matrix->n) * (matrix->m), sizeof(double));
    for (size_t i = 0; i < matrix->n * matrix->m; i++) {
        *(matrix->matrix + i) = rand() % 10;
    }
}

void PrintMatrix(struct Matrix_t matrix)
{
    printf("Matrix: n = %lu; m = %lu\n", matrix.n, matrix.m);
    for (size_t row = 0; row < matrix.n; row++) {
        for (size_t col = 0; col < matrix.m; col++)
            printf("%lg ", *(matrix.matrix + col * matrix.n + row));
        printf("\n");
    }
}

struct Matrix_t MatrixMult(struct Matrix_t a, struct Matrix_t b)
{
    assert(a.m == b.n);
    struct Matrix_t result;
    result.n = a.n;
    result.m = b.m;

    result.matrix = (double*)calloc(result.n * result.m, sizeof(double));

    for (size_t row = 0; row < result.m; row++) 
        for (size_t col = 0; col < result.n; col++)
            for (size_t i = 0; i < a.m; i++)
                *(result.matrix + col * result.n + row) += *(a.matrix + a.n * i + row) * *(b.matrix + b.n * col + i);
    return result;
}

int main() {
    struct Matrix_t a, b, c;
    a.m = a.n = b.m = b.n = 1000;

    //ReadMatrix(&a);
    //ReadMatrix(&b);

    GenerateMatrix(&a);
    GenerateMatrix(&b);

    //ReadMatrix(&a);
    //ReadMatrix(&b);


    clock_t t = clock();
    c = MatrixMult(a, b);
    t = clock() - t;

    printf("Multiplication in %d clicks (%f mls)\n", (int)t, ((double)t)/ CLOCKS_PER_SEC * 1000.f);


    //PrintMatrix(a);
    //PrintMatrix(b);
    //PrintMatrix(c);
    return 0;
}