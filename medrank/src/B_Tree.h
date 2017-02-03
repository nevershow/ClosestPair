
#ifndef __B_TREE_H
#define __B_TREE_H
#include "block_file.h"

class B_Node;

// -------------------------------------------------------------------
// B_Tree类基本信息：
// 对应磁盘中的一个具体文件；本类的所有操作都是基于B+树数据类型的操作；本类支持
// 页面查询操作和文件建B+树的操作；本类成员函数不支持页面读写操作
// -------------------------------------------------------------------
class B_Tree {
public:
   B_Tree();

   ~B_Tree();

	// -------------------------------------------------------------------
    // 初始化函数，使一个B_Tree类型的对象指向某个具体文件
    // -------------------------------------------------------------------
   void initial(char* fname, int b_length) {
      // init <file>, b-tree store here
      file = new BlockFile(fname, b_length);
   }


	// -------------------------------------------------------------------
    // 返回指向文件的BlockFile指针
    // -------------------------------------------------------------------
   BlockFile* get_file() {
      return file;
   }

	// -------------------------------------------------------------------
    // 通过块编号定位文件中的一个块
    // -------------------------------------------------------------------
   B_Node* getBlockByIndex(int index) {
      B_Node *target_node = new B_Node();
      target_node->restore(this, index);
      return target_node;
   }

	// -------------------------------------------------------------------
    // 通过查询数据定位文件中的一个块
    // -------------------------------------------------------------------
   B_Node *searchNode(double queryData);
   
   void bulkload(pair *pairs, int n);
   void read_header(char* buf);
   void write_header(char* buf);
   void restore(char* fname);

private:
   BlockFile* file;  //B_Tree与文件对应
   B_Node* root;     //根节点
   int rootIndex;    //根节点（块)的编号
   int levels;       //树的高度
};

#endif
