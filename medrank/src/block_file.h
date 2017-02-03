
#ifndef __BLOCK_FILE_H
#define __BLOCK_FILE_H
#include "def.h"
#include <cstdio>
#include <cstring>

// -------------------------------------------------------------------
// BlockFile类基本信息：
// 对应具体文件，本类成员函数支持文件中页面级别的读写操作
// -------------------------------------------------------------------
class BlockFile {
public:
    FILE* fp;                      // file pointer
    char* file_name;               // file name
    bool  new_flag;                // specifies if this is a new file

    int block_length;              // length of a block
    int act_block;                 // block num of fp position
    int num_blocks;                // total num of blocks

    BlockFile(char* name, int b_length);
    ~BlockFile();

    // write <bytes> of length <num>
    void put_bytes(char* bytes, int num) {
        fwrite(bytes, num, 1, fp);
    }

    // read <bytes> of length <num>
    void get_bytes(char* bytes, int num) {
        int info = fread(bytes, num, 1, fp);
    }

    // move <fp> to the right with <bnum>
    void seek_block(int bnum) {
        fseek(fp, (bnum - act_block) * block_length, SEEK_CUR);
    }

    // whether this block is modified?
    bool file_new() {
        return new_flag;
    }

    // get block length
    int get_blocklength() {
        return block_length;
    }
    // get number of blocks
    int get_num_of_blocks() {
        return num_blocks;
    }

    void fwrite_number(int num);
    int fread_number();

    void read_header(char* header);
    void set_header(char* header);

    bool read_block(Block block, int index);
    bool write_block(Block block, int index);

    int append_block(Block block);
};
#endif
