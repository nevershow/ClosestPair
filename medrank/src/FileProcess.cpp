#include <stdio.h>

void FileProcess(const char* SRC, const char* DEST) {
    FILE *src = fopen(SRC, "rb");
    FILE *dest = fopen(DEST, "w");

    int info[4];
    fread(info, sizeof(int), 4, src);

    unsigned char object[784];
    int i, j;


    for (i = 1; i <= 60000; ++i) {
        fread(object, 784, 1, src);
        fprintf(dest, "%d ", i);

        for (j = 1; j < 784; ++j) {
            fprintf(dest, "%u ", object[j]);
        }
        fprintf(dest, "%u\n", object[j]);
    }

    fclose(src);
    fclose(dest);
}

int main() {
    const char *DataSet = "train-images.idx3-ubyte";
    const char *QuerySet = "t10k-images.idx3-ubyte";

    FileProcess(DataSet, "Mnist.ds");
    FileProcess(QuerySet, "Mnist.q");

    return 0;
}