#include "block_file.h"

BlockFile::BlockFile(char* name, int b_length) {
    file_name = new char[strlen(name) + 1];
    strcpy(file_name, name);

    block_length = b_length;

    num_blocks = 0;

    if ((fp = fopen(name, "rb+")) != 0) {
        new_flag = false;
        block_length = fread_number();
        num_blocks = fread_number();
    }
    else {
        fp = fopen(file_name, "wb+");
        new_flag = true;

        // Header Node
        fwrite_number(block_length); // size of a node
        fwrite_number(0);            // number of nodes

        // Ê£ÏÂµÄ¿Õ¼äÌî³äÁã
        char* buffer = NULL;
        int len = block_length - (int)ftell(fp);
        buffer = new char[len];
        memset(buffer, 0, len);
        put_bytes(buffer, len);

        delete[] buffer;
        buffer = NULL;
    }
    // -------------------------------------------------------------------------
    //  Redirect file pointer to the start position of the file
    // -------------------------------------------------------------------------
    fseek(fp, 0, SEEK_SET);
    act_block = 0;
}

BlockFile::~BlockFile() {
    if (file_name) {
        delete[]file_name;
        file_name = nullptr;
    }
    if (fp) {
        fclose(fp);
    }
}

void BlockFile::fwrite_number(int value) {
    put_bytes((char *)&value, SIZEINT);
}

int BlockFile::fread_number() {
    char cache[SIZEINT];
    get_bytes(cache, SIZEINT);
    return *((int *)cache);
}

// -----------------------------------------------------------------------------
//  Note that this func does not read the header of blockfile. It fetches the
//  info in the first block excluding the header of blockfile.
// -----------------------------------------------------------------------------
void BlockFile::read_header(char* buffer) {
    // jump out of first 8 bytes
    fseek(fp, BFHEAD_LENGTH, SEEK_SET);
    // read remain bytes into <buffer>
    get_bytes(buffer, block_length - BFHEAD_LENGTH);

    if (num_blocks < 1) {          // no remain bytes
        fseek(fp, 0, SEEK_SET);    // fp return to beginning pos
        act_block = 0;             // no act block
    }
    else {
        act_block = 1;
    }
}

// -----------------------------------------------------------------------------
//  Note that this func does not write the header of blockfile. It writes the
//  info in the first block excluding the header of blockfile.
// -----------------------------------------------------------------------------
void BlockFile::set_header(char* header) {
    // jump out of first 8 bytes
    fseek(fp, BFHEAD_LENGTH, SEEK_SET);
    // write remain bytes into <buffer>
    put_bytes(header, block_length - BFHEAD_LENGTH);

    if (num_blocks < 1) {          // no remain bytes
        fseek(fp, 0, SEEK_SET);    // fp return to beginning pos
        act_block = 0;             // no act block
    }
    else {
        act_block = 1;
    }
}

bool BlockFile::read_block(Block block, int index) {
    index++;                        // extrnl block to intrnl block
    // move to the position
    if (index <= num_blocks && index > 0) {
        seek_block(index);
    }

    get_bytes(block, block_length);// read the block

    if (index + 1 > num_blocks) {  // <fp> reaches the end of file
        fseek(fp, 0, SEEK_SET);
        act_block = 0;             // <act_block> rewind to start pos
    }
    else {
        act_block = index + 1;     // <act_block> to next pos
    }
    return true;
}

// -----------------------------------------------------------------------------
//  Note that this function can only write to an already "allocated" block (in
//  the range of <num_blocks>).
//  If you allocate a new block, please use "append_block" instead.
// -----------------------------------------------------------------------------
bool BlockFile::write_block(Block block, int index) {
    index++;                        // extrnl block to intrnl block
    // move to the position
    if (index <= num_blocks && index > 0) {
        seek_block(index);
    }

    put_bytes(block, block_length);// write this block
    if (index + 1 > num_blocks) {  // update <act_block>
        fseek(fp, 0, SEEK_SET);
        act_block = 0;
    }
    else {
        act_block = index + 1;
    }
    return true;
}

// -----------------------------------------------------------------------------
//  Append a new block at the end of file (out of the range of <num_blocks>).
//  The file pointer is pointed to the new appended block and return its pos.
// -----------------------------------------------------------------------------
int BlockFile::append_block(Block block) {
    fseek(fp, 0, SEEK_END);        // <fp> point to the end of file
    put_bytes(block, block_length);// write a <block>
    num_blocks++;                  // add 1 to <num_blocks>

    fseek(fp, SIZEINT, SEEK_SET);  // <fp> point to pos of header
    fwrite_number(num_blocks);     // update <num_blocks>

    // -------------------------------------------------------------------------
    //  <fp> point to the pos of new added block.
    //  The equation <act_block> = <num_blocks> indicates the file pointer
    //  point to new added block.
    //  Return index of new added block
    // -------------------------------------------------------------------------
    fseek(fp, -block_length, SEEK_END);
    return (act_block = num_blocks) - 1;
}
