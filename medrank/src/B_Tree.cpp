#include "B_Node.h"
#include "B_Tree.h"
#include "block_file.h"
#include <cstdio>

// -------------------------------------------------------------------
// 将文件的基本信息读入内存中一个B_Tree类型的对象中
// -------------------------------------------------------------------
void B_Tree::restore(char * fname) {
    // -------------------------------------------------------------------------
    //  It doesn't matter to initialize blocklength to 0.
    //  After reading file, <blocklength> will be reinitialized by file.
    // -------------------------------------------------------------------------

    // init <file>
    file = new BlockFile(fname, 0);
    root = nullptr;

    // -------------------------------------------------------------------------
    //  Read the content after first 8 bytes of first block into <header>
    // -------------------------------------------------------------------------
    char* header = new char[file->get_blocklength()];
    file->read_header(header);     // read remain bytes from header
    read_header(header);            // init <root> from <header>
    if (header != nullptr) {           // release space
        delete[] header;
        header = nullptr;
    }
}

// -------------------------------------------------------------------
// 构造函数
// -------------------------------------------------------------------
B_Tree::B_Tree() {
  root = nullptr;
  levels = -1;
  rootIndex = -1;
}

// -------------------------------------------------------------------
// 析构函数
// -------------------------------------------------------------------
B_Tree::~B_Tree() {
  char* header = new char[file->get_blocklength()];
  memset(header, 0, file->get_blocklength());
  write_header(header);           // write <root_> to <header>
  file->set_header(header);       // write back to disk

  if (root != nullptr) {
    delete root;
    root = nullptr;
  }
  if (file != nullptr) {
    delete file;
    file = nullptr;
  }
  if (header != nullptr) {
    delete [] header;
    header = nullptr;
  }
}

// -------------------------------------------------------------------
// 通过查询数据定位一个块
// -------------------------------------------------------------------
B_Node * B_Tree::searchNode(double queryData) {
  B_Node *now_node;
  now_node = this->getBlockByIndex(rootIndex);

  // 如果queryData小于树中所有键值对的key，返回一个空指针
  if (queryData < now_node->getPairByIndex(0).key) {
    delete now_node;
    now_node = nullptr;
    return now_node;
  }

  // 查找queryData对应的叶子结点
  B_Node *toDelete;
  int i;
  while (now_node->getLevel() != 0) {
    for (i = 0; i < now_node->getEntriesNum() - 1; i++)
      if (now_node->getPairByIndex(i).key <= queryData && queryData < now_node->getPairByIndex(i + 1).key)
        break;

    toDelete = now_node;
    now_node = this->getBlockByIndex(now_node->getPairByIndex(i).value);
    delete toDelete;
    toDelete = nullptr;
  }
  return now_node;
}

// -------------------------------------------------------------------
// 对文件数据进行建树操作
// -------------------------------------------------------------------
void B_Tree::bulkload(pair *pairs, int n) {
  B_Node* index_child   = nullptr;
  B_Node* index_prev_nd = nullptr;
  B_Node* index_act_nd  = nullptr;

  B_Node* leaf_child   = nullptr;
  B_Node* leaf_prev_nd = nullptr;
  B_Node* leaf_act_nd  = nullptr;

  unsigned short value = 0;
  int            block = -1;
  double         key = 0;

  bool first_node  = false;       // determine relationship of sibling
  int  start_block = -1;          // position of first node
  int  end_block   = -1;          // position of last node

  start_block = 0;  // 记录每一层开始的块
  end_block   = 0;  // 记录每一层结束的块
  first_node  = true;

  for (int i = 0; i < n; i++) {
    key = pairs[i].key;
    value = pairs[i].value;
    if (leaf_act_nd == nullptr) {
      leaf_act_nd = new B_Node(0, this);
      leaf_act_nd->new_block();

      if (first_node) {
        first_node = false;
        start_block = leaf_act_nd->get_block();
      } else {
        leaf_act_nd->set_left_sibling(leaf_prev_nd->get_block());
        leaf_prev_nd->set_right_sibling(leaf_act_nd->get_block());

        delete leaf_prev_nd;
        leaf_prev_nd = nullptr;
      }
      end_block = leaf_act_nd->get_block();
    }
    leaf_act_nd->add_new_child(key, value);
    if (leaf_act_nd->isFull()) {
      leaf_prev_nd = leaf_act_nd;
      leaf_act_nd = nullptr;
    }
  }

  if (leaf_prev_nd != nullptr) {
    delete leaf_prev_nd;
    leaf_prev_nd = nullptr;
  }
  if (leaf_act_nd != nullptr) {
    delete leaf_act_nd;
    leaf_act_nd = nullptr;
  }
  // -------------------------------------------------------------------------
  //  Stop consition: lastEndBlock == lastStartBlock (only one node, as root)
  // -------------------------------------------------------------------------
  int current_level = 1;         // build the b-tree level by level
  int last_start_block = start_block;
  int last_end_block = end_block;

  while (last_end_block > last_start_block) {
    first_node = true;
    for (int i = last_start_block; i <= last_end_block; i++) {
      block = i;              // get <block>

      // 获取儿子的第一个Key
      if (current_level == 1) {
        leaf_child = new B_Node();
        leaf_child->restore(this, block);

        key = leaf_child->get_key_of_node();

        delete leaf_child;
        leaf_child = nullptr;
      }
      else {
        index_child = new B_Node();
        index_child->restore(this, block);

        key = index_child->get_key_of_node();

        delete index_child;
        index_child = nullptr;
      }

      if (!index_act_nd) {
        index_act_nd = new B_Node(current_level, this);
        index_act_nd->new_block();

        if (first_node) {
          first_node = false;
          start_block = index_act_nd->get_block();
        }
        else {
          index_act_nd->set_left_sibling(index_prev_nd->get_block());
          index_prev_nd->set_right_sibling(index_act_nd->get_block());

          delete index_prev_nd;
          index_prev_nd = nullptr;
        }
        end_block = index_act_nd->get_block();
      }                       // add new entry
      index_act_nd->add_new_child(key, block);

      if (index_act_nd->isFull()) {
        index_prev_nd = index_act_nd;
        index_act_nd = nullptr;
      }
    }

    if (index_prev_nd != nullptr) {// release the space
      delete index_prev_nd;
      index_prev_nd = nullptr;
    }

    if (index_act_nd != nullptr) {
      delete index_act_nd;
      index_act_nd = nullptr;
    }
    // update info
    last_start_block = start_block;
    last_end_block = end_block; // build b-tree of higher level
    current_level++;
  }

  rootIndex = last_start_block;       // update the <root>

  if (index_prev_nd != nullptr) {
    delete index_prev_nd;
    index_prev_nd = nullptr;
  }
  if (index_act_nd != nullptr) {
    delete index_act_nd;
    index_act_nd = nullptr;
  }
  if (index_child != nullptr) {
    delete index_child;
    index_child = nullptr;
  }
  if (leaf_prev_nd != nullptr) {
    delete leaf_prev_nd;
    leaf_prev_nd = nullptr;
  }
  if (leaf_act_nd != nullptr) {
    delete leaf_act_nd;
    leaf_act_nd = nullptr;
  }
  if (leaf_child != nullptr) {
    delete leaf_child;
    leaf_child = nullptr;
  }
}

// -------------------------------------------------------------------
// 从字符串中读入根节点的块编号
// -------------------------------------------------------------------
void B_Tree::read_header(char* buf) {
  memcpy(&rootIndex, buf, SIZEINT);
}

// -------------------------------------------------------------------
// 将根节点的块编号写入字符串
// -------------------------------------------------------------------
void B_Tree::write_header(char* buf) {
  memcpy(buf, &rootIndex, SIZEINT);
}
