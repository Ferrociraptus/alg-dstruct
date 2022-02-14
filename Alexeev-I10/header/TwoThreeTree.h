#ifndef GRAF_H
#define GRAF_H

typedef struct __TwoThreeTreeNode* TwoThreeTree;

void two_three_tree_insert(TwoThreeTree* tree_p, int value);
int two_three_tree_search(TwoThreeTree tree, int value);
void two_three_tree_remove(TwoThreeTree* tree_p, int value);
void two_three_tree_print(TwoThreeTree tree);

#endif
