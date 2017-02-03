#ifndef _CLOSEST_PAIR_
#define _CLOSEST_PAIR_

#include "line.h"
#include "cp.h"

class ClosestPair {

public:
  ClosestPair(int N, int dim, unsigned short** data_objects, Line* lines);
  ~ClosestPair();
  float CalcDis(int id1, int id2);
  void PrintImage(int id1, int id2);
  void FindKMax(Point *S, int n, int K);
  CP ClosestPairLine(Point* S, int len, bool isMedian);
  void FindClosestPair(bool isMedian);

private:
  int N;
  int dim;
  unsigned short **data_objects;
  Line* lines;
};

#endif
