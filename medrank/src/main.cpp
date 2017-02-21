#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "medrankAlgorithm.h"
#include "PreProcess.h"

extern int IOCost;
extern double cANNQueryTIme;
extern double calcRatioTime;

int main(int argc, char const *argv[]) {
    int N, D, qn;
    char dsFile[50], qFile[50];
    srand((unsigned int)time(NULL));
    // command line arg
    // -n 60000 -d 784 -qn 100 -ds ./data/Mnist.ds -qs ./data/Mnist.q
    if (argc != 11) {
        printf("%s\n", "Invalid command line arguments!");
        return -1;
    }
    else {
        for (int i = 1; i < 11; i += 2) {
            if (strncmp(argv[i], "-n", 5) == 0) N = atoi(argv[i + 1]);
            if (strncmp(argv[i], "-d", 5) == 0) D = atoi(argv[i + 1]);
            if (strncmp(argv[i], "-qn", 5) == 0) qn = atoi(argv[i + 1]);
            if (strncmp(argv[i], "-ds", 5) == 0) strncpy(dsFile, argv[i + 1], 50);
            if (strncmp(argv[i], "-qs", 5) == 0) strncpy(qFile, argv[i + 1], 50);
        }
    }

    time_t time0, time1;
    time0 = clock();
    Indexing(N, D, dsFile);
    time1 = clock();
    TestMedrank(D, N, qn, qFile);

    printf("Indexing time: %f s\n", (time1 - time0) * 1.0 / CLOCKS_PER_SEC);
    printf("c-ANN query time: %f s\n", cANNQueryTIme);
    printf("calculate ratio time: %f s\n", calcRatioTime);
    printf("Average I/O Cost = %f\n", IOCost * 1.0 / 100);

    return 0;
}
