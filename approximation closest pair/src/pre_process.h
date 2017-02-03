#ifndef _PRE_PROCESS_H_
#define _PRE_PROCESS_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "def.h"
#include "line.h"
#include "point.h"

float UniformDistribution();
void GenerateRandomProjectionVectors(int dim, float **vectors);
void PreProcess(int N, int D,
                unsigned short** & data_objects,
                Line* & lines);

#endif
