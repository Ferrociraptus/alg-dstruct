#ifndef AATREE_H
#define AATREE_H

typedef struct _AABinaryTreeNode* AABinaryTree;

void aa_binary_tree_init(AABinaryTree* tree);
void aa_binary_tree_insert(AABinaryTree* tree_p, int value);
void aa_binary_tree_remove(AABinaryTree* tree, int del_value);
int aa_binary_tree_search(AABinaryTree tree, int value);
void aa_binary_tree_print(AABinaryTree tree);
void aa_binary_tree_clear(AABinaryTree* tree);

#endif
