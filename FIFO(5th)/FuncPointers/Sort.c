#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int comp(void* a, void* b) {
	int x = *(int*)a;
	int y = *(int*)b;
	return y - x;
}

void sort(void* arr, size_t elem_size, size_t n, int (*comp)(void*, void*)) {
	char* array = (char*)arr;

	char* buffer = (char*)malloc(elem_size);

	for (size_t i = 0; i < elem_size * n; i += elem_size)
		for (size_t j = 0; j < elem_size * n; j += elem_size) {
			int out = comp(arr + i, arr + j);
			if (out >= 0) {
				memcpy(buffer, array + i, elem_size);
				memcpy(array + i, array + j, elem_size);
				memcpy(array + j, buffer, elem_size);
			}
		}
	free(buffer);
}

int main() {
	int arr[5] = {5, 2, 6, 8, 5};
	sort(arr, sizeof(int), 5, comp);

	for (int i = 0; i < 5; i++)
		printf("%d\n", arr[i]);
	return 0;
}
