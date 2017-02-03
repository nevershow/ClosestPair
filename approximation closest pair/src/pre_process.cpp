#include "pre_process.h"
#include <stdio.h>
#include <time.h>

// 均匀分布
float UniformDistribution() {
  float num;
  do {
    num = rand() * 1.0 / RAND_MAX;
  } while (num <= 1e-4 || (1 - num) <= 1e-4);
  return num;
}

// 生成随机向量
void GenerateRandomProjectionVectors(int dim, float **vectors) {
  float length = 0, sum = 0;
  srand((unsigned int)time(NULL));

  for (int i = 0; i < VECTORS_NUM; ++i) {
    sum = 0;
    for (int j = 0; j < dim; ++j) {

      float u1 = UniformDistribution();
      float u2 = UniformDistribution();

      vectors[i][j] = sqrt(-2 * log(u1)) * sin(2 * PI * u2);

      sum += pow(vectors[i][j], 2);
    }
    length = sqrt(sum);

    for (int j = 0; j < dim; ++j) {
      vectors[i][j] /= length;
    }
  }
}

void PreProcess(int N, int dim, unsigned short** & data_objects, Line* & lines) {
  // 生成随机投影向量
  clock_t start = clock();
  float **vectors = (float **)malloc(sizeof(float *) * VECTORS_NUM);
  for (int i = 0; i < VECTORS_NUM; ++i) {
    vectors[i] = (float *)malloc(sizeof(float) * dim);
  }
  GenerateRandomProjectionVectors(dim, vectors);

  lines = (Line *)malloc(sizeof(Line) * VECTORS_NUM);
  for (int i = 0; i < VECTORS_NUM; ++i) {
    lines[i].points = (Point *)malloc(sizeof(Point) * N);
  }

  // 将N个点投影到向量上
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < VECTORS_NUM; ++j) {
      lines[j].points[i].object_id = i + 1;
      lines[j].points[i].porjection = 0;

      for (int k = 0; k < dim; ++k) {
        lines[j].points[i].porjection += data_objects[i][k] * vectors[j][k];
      }
    }
  }
  clock_t end = clock();
  printf("The running time of random porjection: %lf seconds\n\n",
         (double)(end - start) / CLOCKS_PER_SEC);
}
