#ifndef _CP_H_
#define _CP_H_

typedef struct CP {
  unsigned short id_1, id_2;
  float dis;

  CP(unsigned short id_1_ = 0, unsigned short id_2_ = 0, float dis_ = 0) {
    id_1 = id_1_;
    id_2 = id_2_;
    dis = dis_;
  }

  bool operator>(const CP& another) const {
    return this->dis > another.dis;
  }

  bool operator<(const CP& another) const {
    return this->dis < another.dis;
  }

  void operator=(const CP& another) {
    this->id_1 = another.id_1;
    this->id_2 = another.id_2;
    this->dis = another.dis;
  }

} CP;

#endif
