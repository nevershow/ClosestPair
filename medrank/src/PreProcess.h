#ifndef __PRE_PROCESS_H
#define __PRE_PROCESS_H

#include "def.h"
#include "B_Node.h"
#include "B_Tree.h"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

typedef struct line {
    pair *p;
} line;

double ZeroToOneUniformDistribution();

void GenerateRandomProjectionVectors(int m, int d);

int cmp(const void *a, const void *b);

void Indexing(int N, int D, const char *DataSetFileName);

#endif
