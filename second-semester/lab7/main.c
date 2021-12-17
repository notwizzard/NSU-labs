#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define byte (sizeof(uint32_t) * 8)

void CheckInput(short n, int m) {
    if (n < 0 || n > 2000) {
        printf("bad number of vertices");
        exit(0);
    }

    if (m < 0 || m > (int) n * (n + 1) / 2) {
        printf("bad number of edges");
        exit(0);
    }
}

void CheckVertex(short v, short n) {
    if (v < 1 || v > n) {
        printf("bad vertex");
        exit(0);
    }
}

void FreeStructures(uint32_t *matrix, short *ans, char *color) {
    free(matrix);
    free(ans);
    free(color);
}

bool DFS(uint32_t *matrix, char *color, short v, short n, short *ans, short *ind) {
    color[v]++;
    for (short i = 1; i <= n; i++) {
        if (matrix[(v * n + i) / byte] & (1 << (byte - ((v * n + i) % byte)))) {
            if (color[i] == 1) {
                printf("impossible to sort");
                return false;
            }
            else if (!color[i])
                if (!DFS(matrix, color, i, n, ans, ind))
                    return false;
        }
    }
    color[v]++;

    ans[*ind] = v;
    (*ind)++;

    return true;
}

bool TopologicalSort(uint32_t *matrix, char *color, short *ans, short n) {
    short ind = 0;

    for (short i = 1; i <= n; i++) {
        if (!color[i])
            if (!DFS(matrix, color, i, n, ans, &ind))
                return false;
    }

    return true;
}

void FillStructures(uint32_t *matrix, int size, char *color, short n) {
    for (int i = 0; i < size; i++) {
        matrix[i] = 0;
    }
    for (int i = 1; i <= n; i++) {
        color[i] = 0;
    }
}

int main() {

    short n = 0;
    int m = 0;
    if (scanf("%hd %d", &n, &m) < 2){
        printf("bad number of lines");
        return 0;
    }
    CheckInput(n, m);

    int matrixSize = (n + 1) * (n + 1) / byte + 1;
    uint32_t *matrix = malloc(sizeof(uint32_t) * matrixSize);
    char *color = malloc(sizeof(char) * (n + 1));
    short *ans = malloc(sizeof(short) * n);
    FillStructures(matrix, matrixSize, color, n);

    for (int i = 0; i < m; i++) {
        short from, to;
        if (scanf("%hd %hd", &from, &to) < 2) {
            printf("bad number of lines");
            FreeStructures(matrix, ans, color);
            return 0;
        }

        CheckVertex(from, n);
        CheckVertex(to, n);

        int num = from * n + to;
        matrix[num / byte] |= 1 << (byte - (num % byte));
    }

    if (!TopologicalSort(matrix, color, ans, n)) {
        FreeStructures(matrix, ans, color);
        return 0;
    }

    for (int i = n - 1; i >= 0; i--) {
        printf("%d ", ans[i]);
    }

    FreeStructures(matrix, ans, color);
}
