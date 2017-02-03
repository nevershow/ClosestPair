
#ifndef __B_TREE_H
#define __B_TREE_H
#include "block_file.h"

class B_Node;

// -------------------------------------------------------------------
// B_Tree�������Ϣ��
// ��Ӧ�����е�һ�������ļ�����������в������ǻ���B+���������͵Ĳ���������֧��
// ҳ���ѯ�������ļ���B+���Ĳ����������Ա������֧��ҳ���д����
// -------------------------------------------------------------------
class B_Tree {
public:
   B_Tree();

   ~B_Tree();

	// -------------------------------------------------------------------
    // ��ʼ��������ʹһ��B_Tree���͵Ķ���ָ��ĳ�������ļ�
    // -------------------------------------------------------------------
   void initial(char* fname, int b_length) {
      // init <file>, b-tree store here
      file = new BlockFile(fname, b_length);
   }


	// -------------------------------------------------------------------
    // ����ָ���ļ���BlockFileָ��
    // -------------------------------------------------------------------
   BlockFile* get_file() {
      return file;
   }

	// -------------------------------------------------------------------
    // ͨ�����Ŷ�λ�ļ��е�һ����
    // -------------------------------------------------------------------
   B_Node* getBlockByIndex(int index) {
      B_Node *target_node = new B_Node();
      target_node->restore(this, index);
      return target_node;
   }

	// -------------------------------------------------------------------
    // ͨ����ѯ���ݶ�λ�ļ��е�һ����
    // -------------------------------------------------------------------
   B_Node *searchNode(double queryData);
   
   void bulkload(pair *pairs, int n);
   void read_header(char* buf);
   void write_header(char* buf);
   void restore(char* fname);

private:
   BlockFile* file;  //B_Tree���ļ���Ӧ
   B_Node* root;     //���ڵ�
   int rootIndex;    //���ڵ㣨��)�ı��
   int levels;       //���ĸ߶�
};

#endif
