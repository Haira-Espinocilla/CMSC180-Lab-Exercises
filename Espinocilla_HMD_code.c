#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

typedef struct {
    double *X;          //pointer to matrix
    int n;              // size of matrix
    int startColumn;    // first column that thread will process
    int endColumn;      // last column that thread will process
} threadInfo;

//function executed by each thread
void *zsnComputation(void *arg) {
    threadInfo *t = (threadInfo *)arg; //thread can access its assigned columns

    double *X = t->X;
    int n = t->n;
    int start = t->startColumn;
    int end = t->endColumn;

    for (int j = start; j < end; j++) {
        double mean = 0.0;

        // compute mean
        for (int i = 0; i < n; i++) {
            mean += X[i * n + j];
        }
        mean = mean/n;

        // compute standard deviation
        double var = 0.0;
        for (int i = 0; i < n; i++) {
            double diff = X[i * n + j] - mean;
            var += diff * diff;
        }
        double stddev = sqrt(var/n);

        // update column values
        if (stddev == 0.0) {
            for (int i = 0; i < n; i++) {
                X[i * n + j] = 0.0;
            }
        } else {
            double inv = 1.0 / stddev;
            for (int i = 0; i < n; i++) {
                X[i * n + j] = (X[i * n + j] - mean) * inv;
            }
        }
    }

    pthread_exit(NULL);
}

// n x n/t columns per thread
void zsn(double *X, int n, int countThread) {
    pthread_t threads[countThread]; //store thread IDs
    threadInfo args[countThread]; //store info for each thread

    int colThread = n/countThread;

    for (int t = 0; t < countThread; t++) {

        //thread access to matrix and size
        args[t].X = X;
        args[t].n = n;

        //compute starting column
        int start = t*colThread;

        //compute ending column
        int end;

        //if not divisible, last thread takes remaining columns
        if (t == countThread-1) {
            end = n;
        } else {
            end = start + colThread;
        }

        args[t].startColumn = start;
        args[t].endColumn = end;

        // create thread
        pthread_create(&threads[t], NULL, zsnComputation, &args[t]);
    }

    for (int t = 0; t < countThread; t++) {
        pthread_join(threads[t], NULL);
    }
}

// create nxn matrix
double *mallocMatrix(int n) {
    return (double *)malloc(n * n * sizeof(double));
}

// fill matrix values
void fillMatrix(double *X, int n) {
    for (int i = 0; i < n * n; i++) {
        X[i] = rand() % 9 + 1;
    }
}

int main() {
    int n, countThread;

    printf("Enter n: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid input!!\n");
        return 1;
    }

    printf("Enter number of threads: ");
    if (scanf("%d", &countThread) != 1 || countThread <= 0) {
        printf("Invalid input!!\n");
        return 1;
    }

    if (countThread > n) {
        countThread = n;
    }

    srand(time(NULL));
    double *X = mallocMatrix(n);
    fillMatrix(X, n);
    // printf("Original Matrix:\n");
    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < n; j++) {
    //         printf("%.2f ", X[i * n + j]);
    //     }
    //     printf("\n");
    // }

    clock_t start = clock();
    zsn(X, n, countThread);
    clock_t end = clock();

    // printf("\nZSN Normalized Matrix:\n");
    // for (int i = 0; i < n; i++) {
    //     for (int j = 0; j < n; j++) {
    //         printf("%.4f ", X[i * n + j]);
    //     }
    //     printf("\n");
    // }

    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nTime taken: %.4f seconds\n", elapsed);

    free(X);
    return 0;
}