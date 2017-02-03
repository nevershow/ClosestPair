#include "closest_pair.h"
#include "def.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// 交换两个点
inline void swap(Point& x, Point& y) {
  Point tmp = x;
  x = y;
  y = tmp;
}

// 构造函数，进行初始化
ClosestPair::ClosestPair(int N, int dim, unsigned short** data_objects, Line* lines) {
  this->N = N;
  this->dim = dim;
  this->data_objects = data_objects;
  this->lines = lines;
}

// 输出两张图片
void ClosestPair::PrintImage(int id1, int id2) {
  printf("Object ID 1: %d\n", id1);
  printf("Object ID 2: %d\n", id2);
  for (int i = 0; i < 28; i++) {
    for (int j = 0; j < 28; j++) {
      if (data_objects[id1 - 1][i * 28 + j] == 0)
        printf("*");
      else printf(" ");
    }

    printf("          ");

    for (int j = 0; j < 28; j++) {
      if (data_objects[id2 - 1][i * 28 + j] == 0)
        printf("*");
      else printf(" ");
    }
    printf("\n");
  }
  printf("\n");
}

// 计算两个点的欧式距离的平方
float ClosestPair::CalcDis(int id1, int id2) {
  float dis = 0;
  for (int i = 0; i < dim; ++i)
    dis += pow(data_objects[id1 - 1][i] - data_objects[id2 - 1][i], 2);
  return dis;
}

// 寻找第K大的数
void ClosestPair::FindKMax(Point *S, int n, int K) {
  // 随机选取一个点x
  Point x = S[Random(n)];
  int i = 0, j = n - 1;
  // 将x左右两边的元素互换,使得x左边的数都比x小,右边的数都比x大
  do {
    while (S[i] < x) ++i;
    while (S[j] > x) --j;
    if (i <= j)
      swap(S[i++], S[j--]);
  } while (i <= j);

  // 找到第K大的数
  if (j + 1 < K && K <= i)
    return;

  // 第K大的数在x左边,递归左边
  if (j + 1 >= K)
    return FindKMax(S, j + 1, K);

  // 第K大的数在x右边,递归右边找第K-i大
  else
    return FindKMax(S + i, n - i, K - i);
}

// 在一条line上找最近对 [0, len)
// isMedian为true时每次找中位数，否则随机选一个点做pivot
CP ClosestPair::ClosestPairLine(Point* S, int len, bool isMedian) {
  // 没有点对
  if (len < 2) {
    return CP(-1, -1, oo);
  }

  // 刚好形成一个点对
  if (len == 2) {
    if (S[1] < S[0])
      swap(S[0], S[1]);
    return CP(S[0].object_id, S[1].object_id, fabsf(S[1].porjection - S[0].porjection));
  }

  // 找第m+1大的点，其下标刚好为m
  int m = isMedian ? len / 2 : Random(len);
  FindKMax(S, len, m + 1);

  CP min_cps[4];

  // S[m] 与左右两个点形成的点对对
  min_cps[0] = CP(S[m - 1].object_id, S[m].object_id,
                  fabsf(S[m - 1].porjection - S[m].porjection));
  min_cps[1] = CP(S[m].object_id, S[m + 1].object_id,
                  fabsf(S[m + 1].porjection - S[m].porjection));

  if (m == 1) {
    return MIN(min_cps[0], min_cps[1]);
  }

  // 递归S[m]左右两边的点对
  min_cps[2] = ClosestPairLine(S, m, isMedian);
  min_cps[3] = ClosestPairLine(S + m + 1, len - m - 1, isMedian);

  // 找出最小的点对
  float min_dis = oo;
  int min_index = -1;
  for (int k = 0; k < 4; ++k) {
    if (min_dis > min_cps[k].dis) {
      min_dis = min_cps[k].dis;
      min_index = k;
    }
  }

  return min_cps[min_index];
}

// 使用中位数的方法, 随机化的方法求解最近对
void ClosestPair::FindClosestPair(bool isMedian) {
  CP min_cp(-1, -1, oo);

  clock_t start = clock();
  for (int i = 0; i < VECTORS_NUM; i++) {
    CP cp = ClosestPairLine(lines[i].points, N, isMedian);
    cp.dis = CalcDis(cp.id_1, cp.id_2);
    min_cp = MIN(cp, min_cp);
  }
  clock_t end = clock();

  printf("The closest pair of Algorithm %s\n", isMedian ? "ClosestPairMedian" : "CLosestPairPivot");
  printf("The running time: %lf seconds\n",
         (double)(end - start) / CLOCKS_PER_SEC);
  PrintImage(min_cp.id_1, min_cp.id_2);
}

// 析构函数，释放资源
ClosestPair::~ClosestPair() {
  for (int i = 0; i < VECTORS_NUM; ++i) {
    free(lines[i].points);
  }
  free(lines);
}
