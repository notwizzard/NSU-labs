#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

typedef struct edge {
    short first;
    short second;
    int length;
} edge;

void FreeStructures(edge *edges, short *set) {
    free(edges);
    free(set);
}

void MakeSet(short *set, short v) {
    set[v] = v;
}

short FindSet(short *set, short v) {
    if (v == set[v])
        return v;
    return set[v] = FindSet(set, set[v]);
}

void UnionSets(short *set, short a, short b) {
    a = FindSet(set, a);
    b = FindSet(set, b);
    if (a != b)
        set[b] = a;
}

void CheckLength(long long length) {
    if (length < 0 || length > INT_MAX) {
        printf("bad length");
        exit(0);
    }
}

void CheckInput(int n, int m) {
    if (n < 0 || n > 5000) {
        printf("bad number of vertices");
        exit(0);
    }

    if (m < 0 || m > (int) n * (n - 1) / 2) {
        printf("bad number of edges");
        exit(0);
    }

    if (n == 0) {
        printf("no spanning tree");
        exit(0);
    }
}

void CheckVertex(short v, int n) {
    if (v < 1 || v > n) {
        printf("bad vertex");
        exit(0);
    }
}

void swap(edge *x, edge *y) {
    edge a = *x;
    *x = *y;
    *y = a;
}

int Partition(edge *array, int left, int right) {
    int pivot = array[(left + right) / 2].length;
    int l = left;
    int r = right;

    while (l <= r) {
        while (array[l].length < pivot)
            l++;
        while (array[r].length > pivot)
            r--;
        if (l >= r)
            return r;
        swap(&array[l++], &array[r--]);
    }

    return r;
}

void QuickSort(edge *array, int left, int right) {
    if (left < right) {
        int pivotIndex = Partition(array, left, right);
        QuickSort(array, left, pivotIndex);
        QuickSort(array, pivotIndex + 1, right);
    }
}

bool CheckConnectivity(edge *edges, short *set, int n, int m) {
    for (short i = 0; i < n; i++) {
        MakeSet(set, i);
    }

    short counter = 0;
    for (int i = 0; i < m; i++) {
        if (FindSet(set, edges[i].first) != FindSet(set, edges[i].second)) {
            UnionSets(set, edges[i].first, edges[i].second);
            counter++;
        }
    }

    if (counter < n - 1)
        return false;

    return true;
}


int main() {

    int n = 0;
    int m = 0;
    if (scanf("%d %d", &n, &m) < 2){
        printf("bad number of lines");
        return 0;
    }
    CheckInput(n ,m);

    edge *edges = malloc(sizeof(edge) * m);
    short *set = malloc(sizeof(short) * n);

    for (int i = 0; i < m; i++) {
        short from, to;
        long long length;
        if (scanf("%hd %hd %lld", &from, &to, &length) < 3) {
            printf("bad number of lines");
            FreeStructures(edges, set);
            return 0;
        }

        CheckVertex(from, n);
        CheckVertex(to, n);
        CheckLength(length);

        edges[i] = (edge) {--from, --to, (int) length};
    }

    QuickSort(edges, 0, m - 1);

    if (!CheckConnectivity(edges, set, n, m)) {
        printf("no spanning tree");
        FreeStructures(edges, set);
        return 0;
    }

    for (short i = 0; i < n; i++) {
        MakeSet(set, i);
    }

    for (int i = 0; i < m; i++) {
        if (FindSet(set, edges[i].first) != FindSet(set, edges[i].second)) {
            printf("%d %d\n", edges[i].first + 1, edges[i].second + 1);
            UnionSets(set, edges[i].first, edges[i].second);
        }
    }

    FreeStructures(edges, set);
}
