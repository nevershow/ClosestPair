#include "pre_process.h"
#include "closest_pair.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[]) {
  int N, dim;
  char dataset_file[20];

  // ./cp -n 60000 -d 784 -f Mnist.ds
  if (argc != 7) {
    printf("%s\n", "Invalid arguments!");
    return -1;
  }

  for (int i = 1; i < 7; i += 2) {
    if (strncmp(argv[i], "-n", 5) == 0) {
      N = atoi(argv[i + 1]);
    } else if (strncmp(argv[i], "-d", 5) == 0) {
      dim = atoi(argv[i + 1]);
    } else if (strncmp(argv[i], "-f", 5) == 0) {
      strncpy(dataset_file, argv[i + 1], 20);
    }
  }

  FILE *dataset = fopen(dataset_file, "r");
  if (dataset == NULL) {
    printf("Failed to open %s\n", dataset_file);
    exit(1);
  }

  // 读入文件
  clock_t start = clock();
  unsigned short** data_objects = (unsigned short **)malloc(sizeof(unsigned short*) * N);
  for (int i = 0; i < N; ++i) {
    data_objects[i] = (unsigned short *)malloc(sizeof(unsigned short) * dim);
  }

  int object_id, info;
  for (int i = 0; i < N; ++i) {
    info = fscanf(dataset, "%d", &object_id);

    for (int j = 0; j < dim; ++j) {
      info = fscanf(dataset, "%hu", &data_objects[i][j]);
    }
  }
  clock_t end = clock();
  printf("The running time of read file: %lf seconds\n\n",
         (double)(end - start) / CLOCKS_PER_SEC);

  Line* lines1;
  // 预处理
  PreProcess(N, dim, data_objects, lines1);

  // 拷贝60000个点在100条线上的投影
  Line* lines2 = (Line *)malloc(sizeof(Line) * VECTORS_NUM);
  for (int i = 0; i < VECTORS_NUM; ++i) {
    lines2[i].points = (Point *)malloc(sizeof(Point) * N);
  }
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < VECTORS_NUM; ++j) {
      lines2[j].points[i].object_id = lines1[j].points[i].object_id;
      lines2[j].points[i].porjection = lines1[j].points[i].porjection;
    }
  }

  ClosestPair closestpair_median(N, dim, data_objects, lines1);
  closestpair_median.FindClosestPair(true);

  ClosestPair closestpair_pivot(N, dim, data_objects, lines2);
  closestpair_pivot.FindClosestPair(false);
  
  for (int i = 0; i < N; ++i) {
    free(data_objects[i]);
  }
  free(data_objects);

  return 0;
}
