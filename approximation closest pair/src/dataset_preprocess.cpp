#include <stdio.h>

// 把二进制文件转成文本
// SRC = "train-images.idx3-ubyte"
// DEST = "Mnist.ds"
void DataSetPreProcess(const char* SRC, const char* DEST) {
  FILE *src = fopen(SRC, "rb");
  FILE *dest = fopen(DEST, "w");

  int info[4], res;
  res = fread(info, sizeof(int), 4, src);

  unsigned char object[784];

  int i, j;
  for (i = 1; i <= 60000; ++i) {
    res = fread(object, 784, 1, src);
    fprintf(dest, "%d ", i);

    for (j = 1; j < 784; ++j) {
      fprintf(dest, "%u ", object[j]);
    }
    fprintf(dest, "%u\n", object[j]);
  }

  fclose(src);
  fclose(dest);
}
