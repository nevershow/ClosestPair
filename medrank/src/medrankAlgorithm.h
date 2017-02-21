#ifndef __MEDRANK_ALGORITHM_H
#define __MEDRANK_ALGORITHM_H

#include "B_Node.h"
#include "B_Tree.h"
#include "def.h"

void InitializeTwoPointers(int & h_pairindex, int & l_pairindex, int & h_blockindex, int & l_blockindex, B_Node* & h_block, B_Node* & l_block,
    double queryData, int treeIndex);

bool IsHCloser(const int h_pairindex, const int h_blockindex, const int l_pairindex, const int l_blockindex, double value, B_Node *h_block, B_Node *l_block);

void LeftShift(int & h_pairindex, int & h_blockindex, B_Node *&h_block, B_Tree*& b_Tree);

void RightShift(int & l_pairindex, int & l_blockindex, B_Node *&l_block, B_Tree *& b_Tree);

int medrank(unsigned short *queryObject, int D, int N);

double calcRatio(int D, int N, int result, unsigned short * queryObject, unsigned short ** dataObjects);

void TestMedrank(int D, int N, int qn, const char* QuerySetFileName);

void Object2Projection(unsigned short *queryObject, int D, double* queryProjections);

bool CheckWin(int num);

int getObjectID(int pairIndex, B_Node *block);
#endif
