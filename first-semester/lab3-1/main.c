#include <stdlib.h>
#include <stdio.h>

void swap(int *x, int *y) {
    int a = *x;
    *x = *y;
    *y = a;
}

void Heapify(int *array, int length, int index) {
    int left = index * 2;
    int right = index * 2 + 1;
    int localMaxIndex = index;

    if (left <= length && array[left] > array[localMaxIndex])
        localMaxIndex = left;

    if (right <= length && array[right] > array[localMaxIndex])
        localMaxIndex = right;

    if (localMaxIndex != index) {
        swap(&array[index], &array[localMaxIndex]);
        Heapify(array, length, localMaxIndex);
    }

}

void BuildMaxHeap(int *array, int length) {
    for (int i = length / 2; i >= 1; i--) {
        Heapify(array, length, i);
    }
}

void HeapSort(int *array, int length) {
    BuildMaxHeap(array, length);
    for (int i = length; i >= 1; i--) {
        swap(&array[1], &array[i]);
        length--;
        Heapify(array, length, 1);
    }
}

int main() {

    int length;
    if (!scanf("%d", &length)) return 0;

    int *array = malloc(sizeof(int) * (length + 1));

    for (int i = 1; i <= length; i++) {
        if (!scanf("%d", &array[i])) return 0;
    }

    HeapSort(array, length);

    for (int i = 1; i <= length; i++) {
        printf("%d ", array[i]);
    }

    free(array);

}
