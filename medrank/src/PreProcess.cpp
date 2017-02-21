#include "PreProcess.h"
double **a;
unsigned short** dataObjects;

// -------------------------------------------------------------------
// 生成一个01分布随机数
// -------------------------------------------------------------------
double ZeroToOneUniformDistribution() {
    double num;
    do {
        num = rand() * 1.0 / RAND_MAX;
    } while (num <= 1e-6 || (1 - num) <= 1e-6);
    return num;
}

// -------------------------------------------------------------------
// 在d维空间生成m个随机向量
// -------------------------------------------------------------------
void GenerateRandomProjectionVectors(int m, int d) {
    a = (double **)malloc(sizeof(double *) * m);
    for (int i = 0; i < m; ++i) {
        a[i] = (double *)malloc(sizeof(double) * d);
    }
    double length = 0, sum = 0;
    int i, j;

    for (i = 0; i < m; ++i) {
        sum = 0;
        for (j = 0; j < d; ++j) {

            double u1 = ZeroToOneUniformDistribution();
            double u2 = ZeroToOneUniformDistribution();

            a[i][j] = sqrt(-2 * log(u1)) * sin(2 * PI * u2);

            sum += pow(a[i][j], 2);
        }
        length = sqrt(sum);

        for (j = 0; j < d; ++j) {
            a[i][j] /= length;
        }
    }
}

// -------------------------------------------------------------------
// 快排比较函数
// -------------------------------------------------------------------
int cmp(const void *a, const void *b) {
    if (((pair*)a)->key - ((pair*)b)->key > 1e-7)
        return 1;
    return -1;
}

// -------------------------------------------------------------------
// 将数据库中的点投影到投票者向量上，并调用函数将数据写入磁盘文件且
// 建立B+树索引
// -------------------------------------------------------------------
void Indexing(int N, int D, const char *DataSetFileName) {
    int i, j, k;

    FILE *DataSet = fopen(DataSetFileName, "r");
    if (DataSet == nullptr) {
        printf("Failede to open %s\n", DataSetFileName);
        exit(1);
    }

    GenerateRandomProjectionVectors(M, D);

    line *lines = (line *)malloc(sizeof(line) * M);
    for (i = 0; i < M; ++i) {
        lines[i].p = (pair *)malloc(sizeof(pair) * N);
    }

    dataObjects = (unsigned short **)malloc(sizeof(unsigned short*) * N);
    for (i = 0; i < N; ++i) {
        dataObjects[i] = (unsigned short *)malloc(sizeof(unsigned short) * D);
    }

    int objectID, info;

    for (i = 0; i < N; ++i) {
        info = fscanf(DataSet, "%d", &objectID);

        for (j = 0; j < D; j++) {
            info = fscanf(DataSet, "%hu", &dataObjects[i][j]);
        }

        for (j = 0; j < M; ++j) {
            pair temp;
            temp.key = 0;
            temp.value = objectID;

            for (k = 0; k < D; ++k) {
                temp.key += dataObjects[i][k] * a[j][k];
            }
            lines[j].p[i] = temp;
        }
    }

    B_Tree *b_tree;

    for (i = 0; i < M; i++) {
        qsort(lines[i].p, N, sizeof(lines[i].p[0]), cmp);
        b_tree = new B_Tree();

        char file_name[10];
        sprintf(file_name, "%d.bin", i + 1);
        b_tree->initial(file_name, 1024);
        b_tree->bulkload(lines[i].p, N);

        delete b_tree;
        b_tree = nullptr;
    }

    for (i = 0; i < M; ++i) {
        free(lines[i].p);
    }
    free(lines);
}

