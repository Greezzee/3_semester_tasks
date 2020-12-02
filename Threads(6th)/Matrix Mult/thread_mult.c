#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <time.h>
#include <sys/sysinfo.h>
#include <pthread.h>

struct Matrix_t
{
    size_t m, n;
    double* matrix;
};

struct ThreadMultData
{
    struct Matrix_t *a, *b, *result;

    size_t m_start, m_end;

    pthread_t pthid;
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

void* CalcZone(void* data)
{
    struct ThreadMultData* dt = (struct ThreadMultData*)data;
    for (size_t row = dt->m_start; row < dt->m_end; row++) 
        for (size_t col = 0; col < dt->result->n; col++)
            for (size_t i = 0; i < dt->a->m; i++)
                dt->result->matrix[col * dt->result->n + row] += dt->a->matrix[dt->a->n * i + row] * dt->b->matrix[dt->b->n * col + i];

    return NULL;
}

struct Matrix_t MatrixMult(struct Matrix_t a, struct Matrix_t b)
{
    assert(a.m == b.n);
    struct Matrix_t result;
    result.n = a.n;
    result.m = b.m;

    result.matrix = (double*)calloc(result.n * result.m, sizeof(double));

    size_t cores = get_nprocs() * 2;
    //printf("cores: %d\n", cores);

    struct ThreadMultData* data = (struct ThreadMultData*)calloc(cores, sizeof(struct ThreadMultData));

    size_t base_len = result.m / cores;
    if (result.m % cores != 0)
        base_len++;
    size_t buf = result.m;

    for (size_t i = 0; i < cores; i++) {
        data[i].a = &a;
        data[i].b = &b;
        data[i].result = &result;

        data[i].m_end = buf;
        buf -= base_len;
        if (buf <= 0) {
            data->m_start = 0;
        }
        data[i].m_start = buf;

        pthread_create(&data[i].pthid, NULL, CalcZone, &data[i]);
        //printf("Thread %i: from %d to %d\n", i, data[i].m_start, data[i].m_end);
        if (buf <= 0)
            break;
    }

    for(size_t i = 0; i < cores; i++)
        pthread_join(data[i].pthid, NULL);

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
