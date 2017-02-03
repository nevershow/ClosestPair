#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "medrankAlgorithm.h"
#include "B_Tree.h"

extern unsigned short** dataObjects;
extern double **a;

int IOCost = 0;
double cANNQueryTIme = 0;
double calcRatioTime = 0;

// -------------------------------------------------------------------
// �жϵ�ǰ�Ƿ��к�ѡ�˴ﵽ��֧�����ٽ�ֵ
// -------------------------------------------------------------------
bool CheckWin(int num) {
    return num >= M * MINFREQ;
}

// -------------------------------------------------------------------
// ����һ���������ݿ��еı��
// -------------------------------------------------------------------
int getObjectID(int pairIndex, B_Node *block) {
    return block->getPairByIndex(pairIndex).value;
}

// -------------------------------------------------------------------
// ��ʼ��50��ͶƱ�ߵ�h��lָ������ҳ��
// -------------------------------------------------------------------
void InitializeTwoPointers(int & h_pairindex, int & l_pairindex, int & h_blockindex, int & l_blockindex, B_Node* & h_block, B_Node* & l_block,
                           double queryData, int treeIndex) {
    B_Tree *b_Tree = new B_Tree();

    char temp[10];
    sprintf(temp, "%d.bin", treeIndex);

    b_Tree->restore(temp);
    // ���ҿ�ʹ�ÿ���key����Сֵ <= queryData < ��һ��Ŀ���key��Сֵ
    B_Node * node = b_Tree->searchNode(queryData);

    // queryDataС�����п���Сֵ�����
    if (node == nullptr) {
        h_blockindex = -1;
        h_pairindex = 0;
        h_block = nullptr;

        l_blockindex = 0;
        l_pairindex = 0;
        l_block = b_Tree->getBlockByIndex(0);
        IOCost++;
        return;
    }

    int i = 0;
    // l��h��ͬһ������
    for (i = 0; i < node->getEntriesNum() - 1; ++i) {
        if ((node->getPairByIndex(i)).key <= queryData && (node->getPairByIndex(i + 1)).key > queryData) {
            h_pairindex = i;
            l_pairindex = i + 1;
            h_blockindex = node->get_block();
            l_blockindex = h_blockindex;
            h_block = node;
            l_block = b_Tree->getBlockByIndex(node->get_block());
            IOCost++;

            delete b_Tree;
            b_Tree = nullptr;
            return;
        }
    }
    // l��h�ڲ�ͬ������
    h_pairindex = i;
    h_blockindex = node->get_block();
    h_block = node;
    l_pairindex = 0;
    l_blockindex = node->getRightSibling();
    if (l_blockindex == -1) {
        l_block = nullptr;
    }
    else {
        l_block = b_Tree->getBlockByIndex(l_blockindex);
        IOCost++;
    }
    delete b_Tree;
    b_Tree = nullptr;
}

// -------------------------------------------------------------------
// �ж�hָ��ָ���ֵ���Ƿ���ӽ���ѯ��
// -------------------------------------------------------------------
bool IsHCloser(const int h_pairindex, const int h_blockindex, const int l_pairindex,
               const int l_blockindex, double value, B_Node *h_block, B_Node *l_block) {
    if (h_blockindex == -1) {
        return false;
    }
    if (l_blockindex == -1) {
        return true;
    }

    double h_value = (h_block->getPairByIndex(h_pairindex)).key;
    double l_value = (l_block->getPairByIndex(l_pairindex)).key;
    return fabs(h_value - value) < fabs(l_value - value);
}

// -------------------------------------------------------------------
// hָ������
// -------------------------------------------------------------------
void LeftShift(int & h_pairindex, int & h_blockindex, B_Node *&h_block, B_Tree*& b_Tree) {
    // h�����ڿ������
    if (h_pairindex != 0) {
        --h_pairindex;
        return;
    }
    // h���ڿ������
    h_blockindex = h_block->getLeftSibling();
    if (h_blockindex == -1) {
        h_pairindex = 0;
        delete h_block;
        h_block = nullptr;
    }
    else {
        delete h_block;
        h_block = b_Tree->getBlockByIndex(h_blockindex);
        IOCost++;
        h_pairindex = h_block->getEntriesNum() - 1;
    }
}

// -------------------------------------------------------------------
// lָ������
// -------------------------------------------------------------------
void RightShift(int & l_pairindex, int & l_blockindex, B_Node *&l_block, B_Tree *& b_Tree) {
    // l�����ڿ����Ҷ�
    if (l_pairindex != l_block->getEntriesNum() - 1) {
        ++l_pairindex;
        return;
    }
    // l���ڿ����Ҷ�
    l_blockindex = l_block->getRightSibling();
    if (l_blockindex == -1) {
        l_pairindex = 0;
        delete l_block;
        l_block = nullptr;
    }
    else {
        delete l_block;
        l_block = b_Tree->getBlockByIndex(l_blockindex);
        IOCost++;
        l_pairindex = 0;
    }
}

// -------------------------------------------------------------------
// ����ѯ����50��ͶƱ����������ͶӰ
// -------------------------------------------------------------------
void Object2Projection(unsigned short *queryObject, int D, double* queryProjections) {
    memset(queryProjections, 0, sizeof(double) * M);
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < D; ++j) {
            queryProjections[i] += queryObject[j] * a[i][j];
        }
    }
}

// -------------------------------------------------------------------
// MedRank�㷨ģ��
// -------------------------------------------------------------------
int medrank(unsigned short *queryObject, int D, int N) {
    double *queryProjections = new double[M];

    Object2Projection(queryObject, D, queryProjections);

    // Initialize h and l for M lines
    int h_pairindex[M], l_pairindex[M];     // ���ڱ��
    int h_blockindex[M], l_blockindex[M];   // ����
    B_Node *h_block[M];           // h���ڿ�
    B_Node *l_block[M];           // l���ڿ�

    for (int i = 0; i < M; ++i) {
        InitializeTwoPointers(h_pairindex[i], l_pairindex[i], h_blockindex[i],
                              l_blockindex[i], h_block[i], l_block[i], queryProjections[i], i + 1);
    }

    // Initialize frequencies
    int *frequencies = new int[N + 1];
    memset(frequencies, 0, sizeof(int) * (N + 1));

    B_Tree *b_Tree[M];

    for (int i = 0; i < M; ++i) {
        b_Tree[i] = new B_Tree();

        char temp[10];
        sprintf(temp, "%d.bin", i + 1);

        b_Tree[i]->restore(temp);
    }

    // vote until an object win
    int flag = -1;
    while (true) {
        for (int i = 0; i < M; ++i) {
            if (IsHCloser(h_pairindex[i], h_blockindex[i], l_pairindex[i],
                          l_blockindex[i], queryProjections[i], h_block[i], l_block[i])) {
                flag = getObjectID(h_pairindex[i], h_block[i]);
                LeftShift(h_pairindex[i], h_blockindex[i], h_block[i], b_Tree[i]);
            }
            else {
                flag = getObjectID(l_pairindex[i], l_block[i]);
                RightShift(l_pairindex[i], l_blockindex[i], l_block[i], b_Tree[i]);
            }

            ++frequencies[flag];
            if (CheckWin(frequencies[flag])) {
                delete []queryProjections;
                delete []frequencies;

                for (int j = 0; j < M; j++) {
                    if (b_Tree[j])
                        delete b_Tree[j];
                    b_Tree[j] = nullptr;

                    if (h_block[j])
                        delete h_block[j];
                    h_block[j] = nullptr;

                    if (l_block[j])
                        delete l_block[j];
                    l_block[j] = nullptr;
                }
                return flag;
            }
        }
    }
}

// -------------------------------------------------------------------
// ����medrank�㷨Ѱ������ھӵľ�ȷ��
// -------------------------------------------------------------------
double calcRatio(int D, int N, int result, unsigned short * queryObject, unsigned short ** dataObjects) {
    double minDis = 0x3f3f3f3f, dis = 0, temp = 0;
    for (int i = 0; i < D; ++i)
        dis += pow(dataObjects[result - 1][i] - queryObject[i], 2);

    for (int i = 0; i < N; ++i) {
        temp = 0;
        for (int j = 0; j < D; ++j)
            temp += pow(dataObjects[i][j] - queryObject[j], 2);

        minDis = MIN(temp, minDis);
    }
    return sqrt(dis / minDis);
}

// -------------------------------------------------------------------
// ����medrank�㷨������
// -------------------------------------------------------------------
void TestMedrank(int D, int N, int qn, const char* QuerySetFileName) {
    unsigned short *queryObject = new unsigned short[D];
    FILE *QuerySet = fopen(QuerySetFileName, "r");
    FILE* output = fopen("output.txt", "w");
    if (QuerySet == nullptr) {
        printf("Failede to open %s\n", QuerySetFileName);
        exit(1);
    }

    double averageRatio = 0, minRatio = 1e10, maxRatio = 0;
    time_t time1, time2, time3;

    for (int i = 0; i < qn; ++i) {
        int temp, info;
        info = fscanf(QuerySet, "%d", &temp);
        for (int j = 0; j < D; j++) {
            info = fscanf(QuerySet, "%hu", &queryObject[j]);
        }
        time1 = clock();
        int result = medrank(queryObject, D, N);
        time2 = clock();
        // measure the accuracy
        fprintf(output, "approximate point ID: %d\n", result);
        double ratio = calcRatio(D, N, result, queryObject, dataObjects);
        minRatio = MIN(minRatio, ratio);
        maxRatio = MAX(maxRatio, ratio);
        averageRatio += ratio;
        fprintf(output, "Ratio = %lf\n", ratio);
        time3 = clock();

        cANNQueryTIme += (time2 - time1) * 1.0 / CLOCKS_PER_SEC;
        calcRatioTime += (time3 - time2) * 1.0 / CLOCKS_PER_SEC;
    }
    printf("Min Ratio = %lf\n", minRatio);
    printf("Max Ratio = %lf\n", maxRatio);
    printf("Average Ratio = %lf\n", averageRatio / 100);

    delete []queryObject;
    fclose(QuerySet);
    fclose(output);

    for (int i = 0; i < M; ++i) {
        free(a[i]);
    }
    free(a);

    for (int i = 0; i < N; ++i) {
        free(dataObjects[i]);
    }
    free(dataObjects);
}
