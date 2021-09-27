#include <stdlib.h>
#include <stdio.h>

void swap(int *x, int *y) {
    int a = *x;
    *x = *y;
    *y = a;
}

int Partition(int *array, int left, int right) {
    int pivot = array[(left + right) / 2];
    int l = left;
    int r = right;

    while (l < r) {
        while (array[l] < pivot)
            l++;
        while (array[r] > pivot)
            r--;
        if (l >= r)
            return r;
        swap(&array[l++], &array[r--]);
    }

    return r;
}

void QuickSort(int *array, int left, int right) {
    if (left < right) {
        int pivotIndex = Partition(array, left, right);
        QuickSort(array, left, pivotIndex);
        QuickSort(array, pivotIndex + 1, right);
    }
}

int main() {

    int length;
    if (!scanf("%d", &length)) return 0;

    int *array = malloc(sizeof(int) * length);

    for (int i = 0; i < length; i++) {
        if (!scanf("%d", &array[i])) return 0;
    }

    QuickSort(array, 0, length - 1);

    for (int i = 0; i < length; i++) {
        printf("%d ", array[i]);
    }

    free(array);

}
