#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <time.h> // Certifica-se de incluir este cabeçalho

#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 1000000 // Define um valor padrão se necessário
#endif

int nNodes;
short int* graph;

void read();
void write(FILE *fl);
void floyd_warshall();

void read() {
    char line[50];
    char* token;
    int size = 50;
    int l, c;

    // Leitura inicial para determinar o tamanho do grafo
    fgets(line, size, stdin);
    while (!feof(stdin)) {
        token = strtok(line, " ");
        if (*token == 'p') {
            token = strtok(NULL, " "); // Skip 'sp'

            token = strtok(NULL, " "); // No. of vertices
            nNodes = atoi(token);

            token = strtok(NULL, " "); // No. of directed edges

            // Aloca a matriz do grafo
            graph = (short int*)malloc(nNodes * nNodes * sizeof(short int));
            if (graph == NULL) {
                printf("Error in graph allocation: NULL!\n");
                exit(EXIT_FAILURE);
            }

            // Inicialização da matriz com paralelismo
            #pragma omp parallel for schedule(static)
            for (int i = 0; i < nNodes; i++) {
                for (int j = 0; j < nNodes; j++) {
                    graph[i * nNodes + j] = (i == j) ? 0 : -1; // Inicializa valores
                }
            }
        } else if (*token == 'a') {
            token = strtok(NULL, " ");
            l = atoi(token) - 1;

            token = strtok(NULL, " ");
            c = atoi(token) - 1;

            token = strtok(NULL, " ");
            graph[l * nNodes + c] = 1; // Define a aresta
        }

        fgets(line, size, stdin);
    }
}


void write(FILE *fl) {
    for (int i = 0; i < nNodes; i++) {
        for (int j = 0; j < nNodes; j++) {
            fprintf(fl, "%d ", graph[i * nNodes + j]);
        }
        fprintf(fl, "\n");
    }
}

void floyd_warshall() {
    #pragma omp parallel
    {
        int* row_k = (int*)malloc(nNodes * sizeof(int));
        int thread_id = omp_get_thread_num();
        int n_threads = omp_get_num_threads();

        // Distribuição de linhas entre threads
        int rows_per_thread = (nNodes + n_threads - 1) / n_threads;
        int local_start = thread_id * rows_per_thread;
        int local_end = (local_start + rows_per_thread > nNodes) ? nNodes : local_start + rows_per_thread;

        for (int k = 0; k < nNodes; k++) {
            // Mestre faz o broadcast da linha k
            #pragma omp single
            {
                memcpy(row_k, &graph[k * nNodes], nNodes * sizeof(int));
            }

            // Sincroniza todas as threads antes de continuar
            #pragma omp barrier

            // Processa as linhas atribuídas a cada thread
            for (int i = local_start; i < local_end; i++) {
                for (int j = 0; j < nNodes; j++) {
                    if (graph[i * nNodes + k] != -1 && row_k[j] != -1) {
                        int new_path = graph[i * nNodes + k] + row_k[j];
                        if (graph[i * nNodes + j] == -1 || new_path < graph[i * nNodes + j]) {
                            graph[i * nNodes + j] = new_path;
                        }
                    }
                }
            }
        }

        free(row_k);
    }
}

int main(int argc, char* argv[]) {
    clock_t t;
    t = clock();
    printf("Calculating...\n");

    read();
    floyd_warshall();
    write(stdout);

    free(graph);
    t = clock() - t;
    printf("It took me %ld clicks (%f seconds).\n", t, ((float)t) / CLOCKS_PER_SEC);

    return 0;
}
