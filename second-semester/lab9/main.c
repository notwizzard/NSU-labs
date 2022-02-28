#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_N 5000
#define ARRAY_SIZE 5001
#define NO_WAY UINT_MAX
#define INT_MAX_PLUS (NO_WAY - 1)

void CheckInput(short n, int m) {
    if (n < 0 || n > MAX_N) {
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

void CheckLength(long long length) {
    if (length < 0 || length > INT_MAX) {
        printf("bad length");
        exit(0);
    }
}

void FillStructures(short n, unsigned int *matrix, unsigned int shortestWayFromS[ARRAY_SIZE], char used[ARRAY_SIZE]) {
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            matrix[i * (n + 1) + j] = 0;
        }
        shortestWayFromS[i] = NO_WAY;
        used[i] = false;
    }
}

short NextShortestWay(short n, unsigned int *matrix, unsigned int shortestWayFromS[ARRAY_SIZE], char used[ARRAY_SIZE], short path[ARRAY_SIZE], short from) {
    int matrixShift = from * (n + 1);

    // count ways from "from" vertex
    for (short i = 1; i <= n; i++) {
        if (!used[i] && matrix[matrixShift + i]) {
            long long newWayLength = (long long) matrix[matrixShift + i] + shortestWayFromS[from];
            if (newWayLength <= shortestWayFromS[i]) {
                if (newWayLength > INT_MAX) {
                    shortestWayFromS[i] = INT_MAX_PLUS;
                }
                else {
                    shortestWayFromS[i] = newWayLength;
                }

                path[i] = from;
            }
        }
    }

    // find next shortest way
    unsigned int minWayLength = NO_WAY;
    short minWayVertex = 0;
    for (short i = 1; i <= n; i++) {
        if (!used[i] && i != from) {
            if (shortestWayFromS[i] < minWayLength) {
                minWayLength = shortestWayFromS[i];
                minWayVertex = i;
            }
        }
    }

    return minWayVertex;
}

void Dijkstra(short n, unsigned int *matrix, unsigned int shortestWayFromS[ARRAY_SIZE], short path[ARRAY_SIZE], char used[ARRAY_SIZE], short s) {
    shortestWayFromS[s] = 0;
    path[s] = s;

    short nextVertex = s;
    for (short i = 0; i < n; i++) {
        used[nextVertex] = true;
        nextVertex = NextShortestWay(n, matrix, shortestWayFromS, used, path, nextVertex);
    }
}

int main() {

    short n = 0;
    int m = 0;
    short s = 0;
    short f = 0;
    if (scanf("%hd %hd %hd %d", &n, &s, &f, &m) < 4){
        printf("bad number of lines");
        return 0;
    }
    CheckInput(n, m);
    CheckVertex(s, n);
    CheckVertex(f, n);

    int matrixSize = (n + 1) * (n + 1);
    unsigned int *matrix = malloc(matrixSize * sizeof(unsigned int));
    unsigned int shortestWayFromS[ARRAY_SIZE];
    short path[ARRAY_SIZE];
    int waysToFNumber = 0;
    char used[ARRAY_SIZE];

    FillStructures(n, matrix, shortestWayFromS, used);

    for (int i = 0; i < m; i++) {
        short from, to;
        long long length;
        if (scanf("%hd %hd %lld", &from, &to, &length) < 3) {
            printf("bad number of lines");
            free(matrix);
            return 0;
        }

        CheckVertex(from, n);
        CheckVertex(to, n);
        CheckLength(length);

        matrix[from * (n + 1) + to] = (unsigned int) length;
        matrix[to * (n + 1) + from] = (unsigned int) length;
    }

    Dijkstra(n, matrix, shortestWayFromS, path, used, s);

    for (int i = 1; i <= n; i++) {
        if (shortestWayFromS[i] == NO_WAY)
            printf("oo ");
        else if (shortestWayFromS[i] == INT_MAX_PLUS)
            printf("INT_MAX+ ");
        else
            printf("%u ", shortestWayFromS[i]);
    }
    printf("\n");

    int matrixShift = f * (n + 1);
    for (int i = 1; i <= n; i++) {
        if (shortestWayFromS[i] != NO_WAY && matrix[matrixShift + i])
            waysToFNumber++;
    }

    if (shortestWayFromS[f] == NO_WAY)
        printf("no path");
    else if (shortestWayFromS[f] == INT_MAX_PLUS && waysToFNumber > 1)
        printf("overflow");
    else {
        short v = f;
        while (path[v] != v) {
            printf("%d ", v);
            v = path[v];
        }
        printf("%d ", v);
    }

    free(matrix);
}
