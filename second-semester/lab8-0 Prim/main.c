#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

typedef struct pair {
    short first;
    short second;
} pair;

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

bool FindFrame(short n, int *graph, char used[n + 1], pair frame[5000]) {
    short ind = 0;
    int *minEdge = malloc(sizeof(int) * (n + 1));
    short *minWay = malloc(sizeof(short) * (n + 1));

    used[1] = true;
    for (short i = 0; i <= n; i++) {
        if (graph[1 * n + i]) {
            minEdge[i] = graph[1 * n + i];
            minWay[i] = 1;
        }
        else {
            minEdge[i] = 0;
            minWay[i] = 0;
        }
    }

    for (short i = 1; i < n; i++) {
        short next = 0;
        for (short j = 1; j <= n; j++) {
            if (!used[j] && (!next || !minEdge[next] || (minEdge[j] && minEdge[j] < minEdge[next])))
                next = j;
        }

        if (!minEdge[next]) {
            printf("no spanning tree");
            free(minEdge);
            free(minWay);
            return false;
        }

        frame[ind] = (pair) {next, minWay[next]};
        ind++;
        used[next] = true;
        for (short j = 1; j <= n; j++) {
            if (graph[next * n + j] && (!minEdge[j] || graph[next * n + j] < minEdge[j])) {
                minEdge[j] = graph[next * n + j];
                minWay[j] = next;
            }
        }
    }

    free(minEdge);
    free(minWay);
    return true;
}

int main() {

    short n = 0;
    int m = 0;
    if (scanf("%hd %d", &n, &m) < 2){
        printf("bad number of lines");
        return 0;
    }
    CheckInput(n ,m);

    char used[n + 1];
    int *graph = malloc(sizeof(int) * (n + 1) * (n + 1));
    for (int i = 1; i <= n; i++) {
        used[i] = false;
        for (int j = 1; j <= n; j++) {
            graph[i * n + j] = 0;
        }
    }

    for (int i = 0; i < m; i++) {
        short from, to;
        long long length;
        if (scanf("%hd %hd %lld", &from, &to, &length) < 3) {
            printf("bad number of lines");
            free(graph);
            return 0;
        }

        CheckVertex(from, n);
        CheckVertex(to, n);
        CheckLength(length);

        graph[from * n + to] = (int) length;
        graph[to * n + from] = (int) length;
    }

    pair frame[5000];
    if (FindFrame(n, graph, used, frame)) {
        for (int i = 0; i < n - 1; i++) {
            printf("%hd %hd\n", frame[i].first, frame[i].second);
        }
    }

    free(graph);
}
