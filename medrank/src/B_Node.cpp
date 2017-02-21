#include "B_Node.h"
#include "B_Tree.h"
#include "block_file.h"

// -------------------------------------------------------------------
// ���ڵ��Ӧ���ļ��е�һ���飨ҳ�棩�����ݶ����ڴ�
// -------------------------------------------------------------------
void B_Node::restore(B_Tree *b_tree_, int block_) {
    b_tree = b_tree_;               // init <b_tree>
    block_index = block_;           // init <block_index>
    dirty = false;                  // init <dirty>

                                    // get block length
    int b_len = b_tree->get_file()->get_blocklength();

    // -------------------------------------------------------------------------
    //  Read the buffer <blk> to init <level_>, <num_entries_>, <left_sibling_>,
    //  <right_sibling_>, <key_> and <son_>.
    // -------------------------------------------------------------------------
    char* blk = new char[b_len];

    b_tree->get_file()->read_block(blk, block_index);
    read_from_buffer(blk);

    delete[] blk;
    blk = nullptr;
}

// -------------------------------------------------------------------
// ���캯������ʼ��<level_>, <num_entries>��<pairs>��
// -------------------------------------------------------------------
B_Node::B_Node(int level_, B_Tree * b_tree_) {
    level = level_;

    num_entries = 0;
    left_sibling = -1;
    right_sibling = -1;

    b_tree = b_tree_;
    dirty = true;

    pairs = new pair[capacity];
    memset(pairs, 0, sizeof(pair) * capacity);
}

// -------------------------------------------------------------------
// ��������
// -------------------------------------------------------------------
B_Node::~B_Node() {
    char* buffer = nullptr;

    if (dirty) {
        int block_length = b_tree->get_file()->get_blocklength();
        buffer = new char[block_length];

        write_to_buffer(buffer);
        b_tree->get_file()->write_block(buffer, block_index);

        delete[] buffer;
        buffer = nullptr;
    }

    if (pairs != nullptr) {
        delete[]pairs;
        pairs = nullptr;
    }
}

// -------------------------------------------------------------------
// ���ļ���Ҫ��д���һ���飨ҳ�棩��С����������һ����(ҳ��)
// -------------------------------------------------------------------
void B_Node::new_block() {
    int b_length = b_tree->get_file()->get_blocklength();

    char* blk = new char[b_length];
    memset(blk, 0, b_length);

    block_index = b_tree->get_file()->append_block(blk);

    delete[] blk;
    blk = nullptr;
}

// -------------------------------------------------------------------
// ���ַ����н�һ�����С�����ݶ����ڴ棨���ڵ�ǰ�ڵ��У�
// -------------------------------------------------------------------
void B_Node::read_from_buffer(char* buffer) {
    int i = 0;
    // -------------------------------------------------------------------------
    //  Read header: <level_> <num_entries_> <left_sibling_> <right_sibling_>
    // -------------------------------------------------------------------------
    memcpy(&level, &buffer[i], SIZEINT);

    i += SIZEINT;

    memcpy(&num_entries, &buffer[i], SIZEINT);
    i += SIZEINT;

    memcpy(&left_sibling, &buffer[i], SIZEINT);
    i += SIZEINT;

    memcpy(&right_sibling, &buffer[i], SIZEINT);
    i += SIZEINT;

    for (int j = 0; j < capacity; j++) {
        memcpy(&pairs[j].key, &buffer[i], SIZEDOUBLE);
        i += SIZEDOUBLE;
        memcpy(&pairs[j].value, &buffer[i], SIZEUNSIGNEDSHORT);
        i += SIZEUNSIGNEDSHORT;
    }
}

// -------------------------------------------------------------------
// ����ǰ�ڵ������д��һ���ַ�����
// -------------------------------------------------------------------
void B_Node::write_to_buffer(char* buffer) {
    int i = 0;
    // -------------------------------------------------------------------------
    //  Write header: <level_> <num_entries_> <left_sibling_> <right_sibling_>
    // -------------------------------------------------------------------------
    memcpy(&buffer[i], &level, SIZEINT);
    i += SIZEINT;

    memcpy(&buffer[i], &num_entries, SIZEINT);
    i += SIZEINT;

    memcpy(&buffer[i], &left_sibling, SIZEINT);
    i += SIZEINT;

    memcpy(&buffer[i], &right_sibling, SIZEINT);
    i += SIZEINT;

    for (int j = 0; j < capacity; j++) {
        memcpy(&buffer[i], &pairs[j].key, SIZEDOUBLE);
        i += SIZEDOUBLE;
        memcpy(&buffer[i], &pairs[j].value, SIZEUNSIGNEDSHORT);
        i += SIZEUNSIGNEDSHORT;
    }
}
