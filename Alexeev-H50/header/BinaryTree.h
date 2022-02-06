#ifndef GRAF_H
#define GRAF_H

typedef struct __BinaryTreeNode* BinaryTree;

void binary_tree_insert(BinaryTree* tree, int value);
int binary_tree_search(BinaryTree tree, int value);
void binary_tree_remove(BinaryTree* tree, int value);
void binary_tree_print(BinaryTree tree);
void binary_tree_clear(BinaryTree* tree);

#endif
